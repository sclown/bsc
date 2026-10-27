#include "QBtCopyWorker.h"
#include "QBtShared.h"
#include "QBtFileTimes.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtConcurrent>

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
, promise_ ( nullptr )
, stop_    ( false )
, block_   ( BLOCK_SIZE )
{
}
// end of QBtCopyWorker

//*******************************************************************
// start                                                      PUBLIC
//*******************************************************************
QFuture<void> QBtCopyWorker::start( const QStringList& sources, const QString& destination,
                                       std::optional<QBtOverwriteAnswer> sticky, Ask ask )
{
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
//-------------------------------------------------------------------
// A single file source may be copied under a new name: the target is
// then a file path unless it names an existing directory. Everything
// else is copied into the target directory under its own name.
//*******************************************************************
void QBtCopyWorker::run( QPromise<void>& promise, const QStringList& sources, const QString& destination )
{
   promise_ = &promise;

   const QFileInfo single( sources.size() == 1 ? sources.first() : QString() );
   if( sources.size() == 1 && !single.isDir() ) {
      const QFileInfo dst( destination );
      if( dst.exists() && dst.isDir() ) {
         copy_into( sources.first(), destination );
      }
      else {
         copy_item( sources.first(), destination );
      }
   }
   else {
      for( const QString& source : sources ) {
         if( stopped() ) break;
         copy_into( source, destination );
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
// copy_into                                                 PRIVATE
//*******************************************************************
void QBtCopyWorker::copy_into( const QString& source, const QString& dest_dir )
{
   copy_item( source, dest_dir + QDir::separator() + QFileInfo( source ).fileName() );
}
// end of copy_into

//*******************************************************************
// copy_item                                                 PRIVATE
//-------------------------------------------------------------------
// Symlinks are recreated as links, even when they point at a
// directory, so a copy never follows them.
//*******************************************************************
void QBtCopyWorker::copy_item( const QString& source, const QString& destination )
{
   if( stopped() ) return;

   const QFileInfo src( source );
   if( src.isSymLink() ) {
      copy_link( source, destination );
   }
   else if( src.isDir() ) {
      copy_dir( source, destination );
   }
   else {
      copy_file( source, destination );
   }
}
// end of copy_item

//*******************************************************************
// copy_dir                                                  PRIVATE
//-------------------------------------------------------------------
// Existing target directories are merged into; the user is asked
// only about files. The entry list is taken before the target is
// created so a copy never descends into its own output.
//*******************************************************************
void QBtCopyWorker::copy_dir( const QString& source, const QString& in_destination )
{
   QString destination = QDir::cleanPath( in_destination );
   if( destination.compare( QDir::cleanPath( source ), PathCase ) == 0 ) {
      destination = QBtShared::auto_rename( destination );
   }
   if( is_inside( destination, source ) ) {
      report( destination, QBtOverwriteAnswer::WRITE_ERROR );
      return;
   }

   const QFileInfo src( source );
   if( !src.isReadable() ) {
      report( source, QBtOverwriteAnswer::READ_ERROR );
      return;
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
         return;
      }
   }
   else {
      if( !QDir().mkdir( destination ) ) {
         report( destination, QBtOverwriteAnswer::WRITE_ERROR );
         return;
      }
      QFile::setPermissions( destination, src.permissions() );
#if defined(Q_OS_UNIX)
      chown( destination.toUtf8(), src.ownerId(), src.groupId() );
#endif
   }

   for( const QFileInfo& entry : entries ) {
      if( stopped() ) return;
      copy_item( entry.absoluteFilePath(), destination + QDir::separator() + entry.fileName() );
   }
   // Last: copying the entries touched the directory's own times.
   if( created ) {
      times.apply( destination, false );
   }
}
// end of copy_dir

//*******************************************************************
// copy_link                                                 PRIVATE
//*******************************************************************
void QBtCopyWorker::copy_link( const QString& source, const QString& in_destination )
{
   QString destination = in_destination;
   if( !resolve( source, destination ) ) return;
   emit progress( QBtCopyProgress{ source, destination, 0, 0 } );

   // Raw link text (relative links stay relative), not the resolved path.
   const QString target = QFileInfo( source ).readSymLink();
   if( target.isEmpty() ) {
      report( source, QBtOverwriteAnswer::READ_ERROR );
      return;
   }
   if( QFileInfo( destination ).isSymLink() || QFile::exists( destination ) ) {
      QFile::remove( destination );
   }
   if( !QFile::link( target, destination ) ) {
      report( destination, QBtOverwriteAnswer::WRITE_ERROR );
      return;
   }
   QBtFileTimes::read( source, true ).apply( destination, true );
}
// end of copy_link

//*******************************************************************
// copy_file                                                 PRIVATE
//*******************************************************************
void QBtCopyWorker::copy_file( const QString& source, const QString& in_destination )
{
   QString destination = in_destination;
   if( !resolve( source, destination ) ) return;

   const QFileInfo fi( source );
   const QBtFileTimes times = QBtFileTimes::read( source, false );
   QFile in( source );
   if( !fi.isReadable() || !in.open( QIODevice::ReadOnly ) ) {
      report( source, QBtOverwriteAnswer::READ_ERROR );
      return;
   }
   QFile out( destination );
   if( !out.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
      report( destination, QBtOverwriteAnswer::WRITE_ERROR );
      return;
   }

   const qint64 size = fi.size();
   qint64 done = 0;
   emit progress( QBtCopyProgress{ source, destination, done, size } );
   while( !in.atEnd() ) {
      if( stopped() ) {
         out.remove();
         return;
      }
      const qint64 n = in.read( block_.data(), BLOCK_SIZE );
      if( n < 0 ) {
         out.remove();
         report( source, QBtOverwriteAnswer::READ_ERROR );
         return;
      }
      if( out.write( block_.data(), n ) != n ) {
         out.remove();
         report( destination, QBtOverwriteAnswer::WRITE_ERROR );
         return;
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
}
// end of copy_file

//*******************************************************************
// resolve                                                   PRIVATE
//-------------------------------------------------------------------
// Decides what to do about an existing target. Returns false when
// the item is to be left alone; destination may come back renamed.
//*******************************************************************
bool QBtCopyWorker::resolve( const QString& source, QString& destination )
{
   if( destination.compare( source, PathCase ) == 0 ) {
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
//-------------------------------------------------------------------
// Tells the user about an unreadable or unwritable item. The item
// is skipped whatever they answer, unless they cancel the whole run.
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
//-------------------------------------------------------------------
// A sticky answer settles every conflict; otherwise the user is
// asked. "Don't ask again" makes the action sticky, but a typed
// name applies to this one item only.
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
// is_inside                                          PRIVATE static
//*******************************************************************
bool QBtCopyWorker::is_inside( const QString& path, const QString& dir )
{
   const QString p = QDir::cleanPath( path );
   const QString d = QDir::cleanPath( dir );
   return p.compare( d, PathCase ) == 0 || p.startsWith( d + '/', PathCase );
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
