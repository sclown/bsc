#include "QBtFileTimes.h"
#include <QFile>

#if defined(Q_OS_WIN)
#include <windows.h>
#else
#include <sys/stat.h>   // stat, lstat
#include <fcntl.h>      // AT_FDCWD
#include <unistd.h>     // setattrlist on macOS
#endif
#if defined(Q_OS_MACOS)
#include <sys/attr.h>   // setattrlist, ATTR_CMN_CRTIME
#endif

#if defined(Q_OS_WIN)

namespace {
// FILETIME counts 100 ns intervals since 1601-01-01.
const quint64 EPOCH_DIFF = 116444736000000000ULL;

QBtFileTimes::Stamp from_filetime( const FILETIME& ft )
{
   const quint64 t = ( quint64( ft.dwHighDateTime ) << 32 | ft.dwLowDateTime ) - EPOCH_DIFF;
   return { qint64( t / 10000000ULL ), qint64( t % 10000000ULL ) * 100 };
}

FILETIME to_filetime( const QBtFileTimes::Stamp& s )
{
   const quint64 t = quint64( s.seconds ) * 10000000ULL + quint64( s.nanoseconds ) / 100 + EPOCH_DIFF;
   FILETIME ft;
   ft.dwLowDateTime  = DWORD( t );
   ft.dwHighDateTime = DWORD( t >> 32 );
   return ft;
}
}

//*******************************************************************
// read                                                PUBLIC static
//*******************************************************************
QBtFileTimes QBtFileTimes::read( const QString& path, const bool is_link )
{
   QBtFileTimes t;
   if( is_link ) return t;   // QFile::link makes a shortcut on Windows
   WIN32_FILE_ATTRIBUTE_DATA a;
   if( !GetFileAttributesExW( reinterpret_cast<LPCWSTR>( path.utf16() ), GetFileExInfoStandard, &a ) ) {
      return t;
   }
   t.valid_        = true;
   t.has_birth_    = true;
   t.access_       = from_filetime( a.ftLastAccessTime );
   t.modification_ = from_filetime( a.ftLastWriteTime );
   t.birth_        = from_filetime( a.ftCreationTime );
   return t;
}
// end of read

//*******************************************************************
// apply                                                      PUBLIC
//*******************************************************************
void QBtFileTimes::apply( const QString& path, const bool ) const
{
   if( !valid_ ) return;
   // Backup semantics lets the handle refer to a directory.
   const HANDLE h = CreateFileW( reinterpret_cast<LPCWSTR>( path.utf16() ), FILE_WRITE_ATTRIBUTES,
                                 FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
                                 OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr );
   if( h == INVALID_HANDLE_VALUE ) return;
   const FILETIME creation = to_filetime( birth_ );
   const FILETIME access   = to_filetime( access_ );
   const FILETIME write    = to_filetime( modification_ );
   SetFileTime( h, &creation, &access, &write );
   CloseHandle( h );
}
// end of apply

#else

namespace {
QBtFileTimes::Stamp from_timespec( const timespec& ts )
{
   return { qint64( ts.tv_sec ), qint64( ts.tv_nsec ) };
}

timespec to_timespec( const QBtFileTimes::Stamp& s )
{
   timespec ts;
   ts.tv_sec  = time_t( s.seconds );
   ts.tv_nsec = long( s.nanoseconds );
   return ts;
}
}

//*******************************************************************
// read                                                PUBLIC static
//*******************************************************************
QBtFileTimes QBtFileTimes::read( const QString& path, const bool is_link )
{
   QBtFileTimes t;
   struct stat st;
   const QByteArray p = QFile::encodeName( path );
   if( ( is_link ? lstat( p.constData(), &st ) : stat( p.constData(), &st ) ) != 0 ) {
      return t;
   }
   t.valid_ = true;
#if defined(Q_OS_MACOS)
   t.access_       = from_timespec( st.st_atimespec );
   t.modification_ = from_timespec( st.st_mtimespec );
   t.birth_        = from_timespec( st.st_birthtimespec );
   t.has_birth_    = true;
#else
   t.access_       = from_timespec( st.st_atim );
   t.modification_ = from_timespec( st.st_mtim );
#endif
   return t;
}
// end of read

//*******************************************************************
// apply                                                      PUBLIC
//*******************************************************************
void QBtFileTimes::apply( const QString& path, const bool is_link ) const
{
   if( !valid_ ) return;
   const QByteArray p = QFile::encodeName( path );
   const timespec times[ 2 ] = { to_timespec( access_ ), to_timespec( modification_ ) };
   utimensat( AT_FDCWD, p.constData(), times, is_link ? AT_SYMLINK_NOFOLLOW : 0 );
#if defined(Q_OS_MACOS)
   // After the modification time: APFS pulls the birth time back to it
   // when it is older, which would undo a birth time set first.
   if( has_birth_ ) {
      attrlist al {};
      al.bitmapcount = ATTR_BIT_MAP_COUNT;
      al.commonattr  = ATTR_CMN_CRTIME;
      timespec birth = to_timespec( birth_ );
      setattrlist( p.constData(), &al, &birth, sizeof( birth ), is_link ? FSOPT_NOFOLLOW : 0 );
   }
#endif
}
// end of apply

#endif
