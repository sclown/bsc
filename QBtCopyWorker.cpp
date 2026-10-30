#include "QBtCopyWorker.h"
#include "QBtShared.h"
#include "QBtFileTimes.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtConcurrent>
#include <filesystem>
#include <system_error>

#if defined(Q_OS_UNIX)
#include <unistd.h>   // chown
#endif

namespace {
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
const Qt::CaseSensitivity PathCase = Qt::CaseInsensitive;
#else
const Qt::CaseSensitivity PathCase = Qt::CaseSensitive;
#endif
}

//*******************************************************************
// QBtCopyWorker                                      CONSTRUCTOR
//*******************************************************************
QBtCopyWorker::QBtCopyWorker( QObject* const parent ) : QObject( parent )
, promise_  ( nullptr )
, mode_     ( Mode::COPY )
, transfer_ ( &QBtCopyWorker::copy )
, stop_     ( false )
, block_   ( BLOCK_SIZE )
{
}
// end of QBtCopyWorker

//*******************************************************************
// start                                                      PUBLIC
//*******************************************************************
QFuture<void> QBtCopyWorker::start( const Mode mode, const QStringList& sources, const QString& destination,
                                       std::optional<QBtOverwriteAnswer> sticky, Ask ask )
{
   mode_   = mode;
   ask_    = std::move( ask );
   answer_ = std::move( sticky );
   stop_   = false;
   return QtConcurrent::run( [this, sources, destination]( QPromise<void>& promise ) {
      run( promise, sources, destination );
   } );
}
// end of start

//*******************************************************************
// run                                                       PRIVATE
//*******************************************************************
void QBtCopyWorker::run( QPromise<void>& promise, const QStringList& sources, const QString& destination )
{
   promise_  = &promise;
   transfer_ = ( mode_ == Mode::MOVE ) ? &QBtCopyWorker::move : &QBtCopyWorker::copy;

   if( renames( sources, destination, mode_ == Mode::MOVE) ) {
      transfer( sources.first(), destination );
   }
   else {
      for( const QString& source : sources ) {
         if( stopped() ) break;
         transfer_into( source, destination );
      }
   }

   promise_ = nullptr;
}
// end of run

//*******************************************************************
// stopped                                                   PRIVATE
//*******************************************************************
bool QBtCopyWorker::stopped() const
{
   return stop_ || promise_->isCanceled();
}
// end of stopped

//*******************************************************************
// transfer_into                                             PRIVATE
//*******************************************************************
bool QBtCopyWorker::transfer_into( const QString& source, const QString& dest_dir )
{
   return transfer( source, dest_dir + QDir::separator() + QFileInfo( source ).fileName() );
}
// end of transfer_into

//*******************************************************************
// transfer                                                  PRIVATE
//*******************************************************************
bool QBtCopyWorker::transfer( const QString& source, const QString& destination )
{
   if( stopped() ) return false;
   return ( this->*transfer_ )( source, destination );
}
// end of transfer

//*******************************************************************
// copy                                                      PRIVATE
//*******************************************************************
bool QBtCopyWorker::copy( const QString& source, const QString& destination )
{
   const QFileInfo src( source );
   if( src.isSymLink() ) {
      return copy_link( source, destination );
   } else if( src.isDir() ) {
      return copy_dir ( source, destination );
   } else {
      return copy_file( source, destination );
   }
}
// end of copy

//*******************************************************************
// move                                                      PRIVATE
//*******************************************************************
bool QBtCopyWorker::move( const QString& source, const QString& destination )
{
   const QFileInfo src( source );
   if( src.isSymLink() ) {
      return move_link( source, destination );
   }
   else if( src.isDir() ) {
      return move_dir ( source, destination );
   } else {
      return move_file( source, destination );
   }
}
// end of move

//===================================================================
// DIRECTORIES
//===================================================================

//*******************************************************************
// copy_dir                                                  PRIVATE
//*******************************************************************
bool QBtCopyWorker::copy_dir( const QString& source, const QString& in_destination )
{
   QString destination = QDir::cleanPath( in_destination );
   if( is_same( source, destination ) ) {
      destination = QBtShared::auto_rename( destination );
   }
   if( is_inside( destination, source ) ) {
      report( destination, QBtOverwriteAnswer::WRITE_ERROR );
      return false;
   }
   return write_dir( source, destination );
}
// end of copy_dir

//*******************************************************************
// move_dir                                                  PRIVATE
//*******************************************************************
bool QBtCopyWorker::move_dir( const QString& source, const QString& destination )
{
   if( is_same( source, destination ) ) return true;   // moved onto itself: nothing to do
   if( is_inside( destination, source ) ) {
      report( destination, QBtOverwriteAnswer::WRITE_ERROR );
      return false;
   }
   if( !QFileInfo::exists( destination ) ) {
      bool cross_device = false;
      if( rename_item( source, destination, &cross_device ) ) return true;
      if( !cross_device ) {
         report( destination, QBtOverwriteAnswer::WRITE_ERROR );
         return false;
      }
   }
   if( !write_dir( source, destination ) ) return false;
   QDir().rmdir( source );   // succeeds only when every entry has moved out
   return true;
}
// end of move_dir

//*******************************************************************
// write_dir                                                 PRIVATE
//*******************************************************************
bool QBtCopyWorker::write_dir( const QString& source, const QString& destination )
{
   const QFileInfo src( source );
   if( !src.isReadable() ) {
      report( source, QBtOverwriteAnswer::READ_ERROR );
      return false;
   }
   const QBtFileTimes times = QBtFileTimes::read( source, false );

   QDir dir( source );
   dir.setFilter( QDir::AllDirs | QDir::Files | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot );
   dir.setSorting( QDir::Name | QDir::DirsFirst | QDir::IgnoreCase );
   const QFileInfoList entries = dir.entryInfoList();

   const QFileInfo dst( destination );
   const bool created = !dst.exists();
   if( !created ) {
      if( !dst.isDir() ) {
         report( destination, QBtOverwriteAnswer::WRITE_ERROR );
         return false;
      }
   }
   else {
      if( !QDir().mkdir( destination ) ) {
         report( destination, QBtOverwriteAnswer::WRITE_ERROR );
         return false;
      }
      QFile::setPermissions( destination, src.permissions() );
#if defined(Q_OS_UNIX)
      chown( destination.toUtf8(), src.ownerId(), src.groupId() );
#endif
   }

   for( const QFileInfo& entry : entries ) {
      if( stopped() ) return false;
      transfer( entry.absoluteFilePath(), destination + QDir::separator() + entry.fileName() );
   }
   // Last: transferring the entries touched the directory's own times.
   if( created ) {
      times.apply( destination, false );
   }
   return true;
}
// end of write_dir

//===================================================================
// SYMBOLIC LINKS
//===================================================================

//*******************************************************************
// copy_link                                                 PRIVATE
//*******************************************************************
bool QBtCopyWorker::copy_link( const QString& source, const QString& in_destination )
{
   QString destination = in_destination;
   if( !resolve( source, destination ) ) return false;
   return write_link( source, destination );
}
// end of copy_link

//*******************************************************************
// move_link                                                 PRIVATE
//*******************************************************************
bool QBtCopyWorker::move_link( const QString& source, const QString& in_destination )
{
   QString destination = in_destination;
   if( is_same( source, destination ) ) return true;   // moved onto itself: nothing to do
   if( !resolve( source, destination ) ) return false;
   if( rename_item( source, destination ) ) return true;
   return write_link( source, destination ) && remove_source( source );
}
// end of move_link

//*******************************************************************
// write_link                                                PRIVATE
//*******************************************************************
bool QBtCopyWorker::write_link( const QString& source, const QString& destination )
{
   emit progress( QBtCopyProgress{ source, destination, 0, 0 } );

   const QString target = QFileInfo( source ).readSymLink();
   if( target.isEmpty() ) {
      report( source, QBtOverwriteAnswer::READ_ERROR );
      return false;
   }
   if( QFileInfo( destination ).isSymLink() || QFile::exists( destination ) ) {
      QFile::remove( destination );
   }
   if( !QFile::link( target, destination ) ) {
      report( destination, QBtOverwriteAnswer::WRITE_ERROR );
      return false;
   }
   QBtFileTimes::read( source, true ).apply( destination, true );
   return true;
}
// end of write_link

//===================================================================
// FILES
//===================================================================

//*******************************************************************
// copy_file                                                 PRIVATE
//*******************************************************************
bool QBtCopyWorker::copy_file( const QString& source, const QString& in_destination )
{
   QString destination = in_destination;
   if( !resolve( source, destination ) ) return false;
   return write_file( source, destination );
}
// end of copy_file

//*******************************************************************
// move_file                                                 PRIVATE
//*******************************************************************
bool QBtCopyWorker::move_file( const QString& source, const QString& in_destination )
{
   QString destination = in_destination;
   if( is_same( source, destination ) ) return true;   // moved onto itself: nothing to do
   if( !resolve( source, destination ) ) return false;
   if( rename_item( source, destination ) ) return true;
   return write_file( source, destination ) && remove_source( source );
}
// end of move_file

//*******************************************************************
// write_file                                                PRIVATE
//*******************************************************************
bool QBtCopyWorker::write_file( const QString& source, const QString& destination )
{
   const QFileInfo fi( source );
   const QBtFileTimes times = QBtFileTimes::read( source, false );
   QFile in( source );
   if( !fi.isReadable() || !in.open( QIODevice::ReadOnly ) ) {
      report( source, QBtOverwriteAnswer::READ_ERROR );
      return false;
   }
   QFile out( destination );
   if( !out.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
      report( destination, QBtOverwriteAnswer::WRITE_ERROR );
      return false;
   }

   const qint64 size = fi.size();
   qint64 done = 0;
   emit progress( QBtCopyProgress{ source, destination, done, size } );
   while( !in.atEnd() ) {
      if( stopped() ) {
         out.remove();
         return false;
      }
      const qint64 n = in.read( block_.data(), BLOCK_SIZE );
      if( n < 0 ) {
         out.remove();
         report( source, QBtOverwriteAnswer::READ_ERROR );
         return false;
      }
      if( out.write( block_.data(), n ) != n ) {
         out.remove();
         report( destination, QBtOverwriteAnswer::WRITE_ERROR );
         return false;
      }
      done += n;
      emit progress( QBtCopyProgress{ source, destination, done, size } );
   }

   out.close();
   out.setPermissions( fi.permissions() );
#if defined(Q_OS_UNIX)
   chown( destination.toUtf8(), fi.ownerId(), fi.groupId() );
#endif
   times.apply( destination, false );
   return true;
}
// end of write_file

//===================================================================
// SHARED HELPERS
//===================================================================

//*******************************************************************
// rename_item                                               PRIVATE
//*******************************************************************
bool QBtCopyWorker::rename_item( const QString& source, const QString& destination, bool* const cross_device )
{
   emit progress( QBtCopyProgress{ source, destination, 0, 0 } );

   std::error_code ec;
   std::filesystem::rename( std::filesystem::path( source.toStdU16String() ),
                            std::filesystem::path( destination.toStdU16String() ), ec );
   if( cross_device ) *cross_device = ( ec == std::errc::cross_device_link );
   return !ec;
}
// end of rename_item

//*******************************************************************
// remove_source                                             PRIVATE
//*******************************************************************
bool QBtCopyWorker::remove_source( const QString& source )
{
   if( QFile::remove( source ) ) return true;   // unlinks a symlink itself
   report( source, QBtOverwriteAnswer::WRITE_ERROR );
   return false;
}
// end of remove_source

//*******************************************************************
// resolve                                                   PRIVATE
//*******************************************************************
bool QBtCopyWorker::resolve( const QString& source, QString& destination )
{
   if( is_same( source, destination ) ) {
      destination = QBtShared::auto_rename( destination );
   }
   const QFileInfo dst( destination );
   if( !dst.exists() && !dst.isSymLink() ) return true;

   const QBtOverwriteAnswer answer = decide( destination, QBtOverwriteAnswer::EXIST );
   const QString new_name = answer.newName();

   switch( answer.action() ) {
      case QBtOverwriteAnswer::OVERWRITE:
         return true;
      case QBtOverwriteAnswer::UPDATE:
         return can_update( source, destination );
      case QBtOverwriteAnswer::RENAME:
         if( !new_name.isEmpty() && new_name != destination ) {
            destination = new_name;
            return true;
         }
         [[fallthrough]];
      case QBtOverwriteAnswer::AUTORENAME:
         destination = QBtShared::auto_rename( destination );
         return true;
      case QBtOverwriteAnswer::CANCEL:
         stop_ = true;
         return false;
      case QBtOverwriteAnswer::SKIP:
      case QBtOverwriteAnswer::ASK:
         return false;
   }
   return false;
}
// end of resolve

//*******************************************************************
// report                                                    PRIVATE
//*******************************************************************
void QBtCopyWorker::report( const QString& path, const QBtOverwriteAnswer::ErrorType type )
{
   if( stopped() ) return;
   if( QBtOverwriteAnswer::CANCEL == decide( path, type ).action() ) {
      stop_ = true;
   }
}
// end of report

//*******************************************************************
// decide                                                    PRIVATE
//*******************************************************************
QBtOverwriteAnswer QBtCopyWorker::decide( const QString& path, const QBtOverwriteAnswer::ErrorType type )
{
   if( answer_ ) return *answer_;
   const QBtOverwriteAnswer answer = ask_( path, type );
   if( answer.dontAsk() ) {
      answer_ = QBtOverwriteAnswer( true, answer.action(), QString() );
   }
   return answer;
}
// end of decide

//*******************************************************************
// is_same                                            PRIVATE static
//*******************************************************************
bool QBtCopyWorker::is_same( const QString& a, const QString& b )
{
   return QDir::cleanPath( a ).compare( QDir::cleanPath( b ), PathCase ) == 0;
}
// end of is_same

//*******************************************************************
// is_inside                                          PRIVATE static
//*******************************************************************
bool QBtCopyWorker::is_inside( const QString& path, const QString& dir )
{
   return is_same( path, dir )
       || QDir::cleanPath( path ).startsWith( QDir::cleanPath( dir ) + '/', PathCase );
}
// end of is_inside

//*******************************************************************
// can_update                                         PRIVATE static
//*******************************************************************
bool QBtCopyWorker::can_update( const QString& source, const QString& destination )
{
   return QFileInfo( source ).lastModified() > QFileInfo( destination ).lastModified();
}
// end of can_update

//*******************************************************************
// renames                                                   PRIVATE
//*******************************************************************
bool QBtCopyWorker::renames( 
   const QStringList& sources,
   const QString& destination,
   const bool move_mode
) {
   if( sources.size() != 1 ) return false;
   const QFileInfo src( sources.first() );
   if( src.isDir() && !src.isSymLink() && !move_mode ) return false;
   return !QFileInfo( destination ).isDir();
}
// end of renames
