#include "QBtDirCopyWorker.h"
#include "QBtSystemCall.h"
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QMessageBox>
#include <QtDebug>

#include <unistd.h>
#include <utime.h>


QBtDirCopyWorker::QBtDirCopyWorker(QObject *parent) :
    break_(false),
    currentState_(nullptr),
    QObject(parent)
{

}

void QBtDirCopyWorker::copy_list()
{
    while( !break_ && list_.size() ) {
        if( currentState_->index < currentState_->items.size() ) {
            copy_next( currentState_->items[currentState_->index].absoluteFilePath(), currentState_->dest );
        }
        else {
            list_.pop_back();
            if( list_.size() ) {
                currentState_ = &(list_.last());
            }
            else {
                currentState_ = nullptr;
            }
        }
    }
    if(!break_) {
        emit finished();
    }

}

void QBtDirCopyWorker::copy_next( const QString& in_src_path, const QString& in_dst_path )
{
//   QBtShared::idle();
   if( break_ ) return;

   QFileInfo src( in_src_path );
   QString   src_path = src.absoluteFilePath();
   QString   src_name = src.fileName();

   QFileInfo dst( in_dst_path );
   QString   dst_path = dst.absoluteFilePath();
   QString   dst_name = dst.fileName();

   // KATALOG ---------------------------------------------
   if( src.isDir() ) {
      copy_dir( src_path, in_dst_path );
      return;
   }

   dst_path += "/" + src_name;
   emit paths( in_src_path, dst_path );
   if( QFile::exists( dst_path ) ) {
       if(!answer_.valid()) {
           emit ask_overwrite(dst_path);
           break_ = true;
           return;
       }
   }
   if(answer_.hasNewName()) {
       dst_path = answer_.newName();
   }
   if(answer_.valid()) {
       answer_.reset();
       switch( answer_.action() ) {
       case QBtOverwriteAnswer::UPDATE:
           if( !can_update(src_path, dst_path) ) {
               return;
           }
           break;
       case QBtOverwriteAnswer::SKIP:
           return;
           break;

       }

   }
   if( src.isSymLink() ) {
       copy_link(src_path, dst_path);
       return;
   }
   copy_file( src_path, dst_path );
}
// end of copy_next

//*******************************************************************
// copy_dir                                        PRIVATE inherited
//*******************************************************************
void QBtDirCopyWorker::copy_dir( const QString& in_src_dir, const QString& in_dst_dir )
{
   if( break_ ) return;
   QFileInfo src( in_src_dir );
   QString   src_name = src.fileName();
   if( !src.isReadable() ) {
       return;
   }
    QDir dst_subdir( in_dst_dir );
    dst_subdir.mkdir( src_name );
    if( !dst_subdir.cd( src_name ) ) {
        return;
    }
    //    if( do_remove() ) {
    //       remove_dir( src_path );
    //    }
        QFile dfi( dst_subdir.absolutePath() );
    //    if( do_permissions() ) {
        dfi.setPermissions( src.permissions() );
    //    }
    //    if( do_owner() ) {
        chown(dst_subdir.absolutePath().toUtf8(), src.ownerId(), src.groupId());
    //    }


   ++currentState_->index;

   QDir dir( in_src_dir );
   dir.setFilter( QDir::AllDirs | QDir::Files | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot );
   dir.setSorting( QDir::Name | QDir::DirsFirst | QDir::IgnoreCase );
   auto entries = dir.entryInfoList();
   if(!entries.empty()) {
       list_.push_back(QBtCopyState(dir.entryInfoList(), dst_subdir.absolutePath()));
       currentState_ = &(list_.last());
   }

}

void QBtDirCopyWorker::copy_file( const QString& in_src_path, const QString& dst_path )
{
   // Sprwadz czy czy plik jest do odczytu
   const QFileInfo fi( in_src_path );
   if( !fi.isReadable() ) {
//      QMessageBox::critical( this, tr( CAPTION ), tr( FILE_NOT_READABLE ).arg( in_src_path ) );
      return;
   }

  emit paths( in_src_path, dst_path );

  // Po uzyskaniu zgody na kopiowanie bierzemy sie do roboty.
  QFile in( in_src_path );
  QFile out( dst_path );
  if( in.open( QIODevice::ReadOnly ) ) {
     const quint32 nbytes = in.size(); // Liczba bajtow do przekopiowania.
     quint32 copied = 0;               // Licznik juz przekopiowanych bajtow.

     if( out.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
        emit reset_progress( nbytes );
//        reset_progress( nbytes );
//        QBtShared::idle();
        // Petla kopiujaca.
        while( !in.atEnd() ) {
           const quint32 n = in.read( block_, BLOCK_SIZE );
           out.write( block_, n );
           emit progress( nbytes );
//           set_progress( copied += n );
//           QBtShared::idle();
        }
        out.close();
        in.close();

       // czynnosci po kopiowaniu
//        if( do_remove() ) {
//           if( !in.remove() ) {
//              QMessageBox::warning( this, tr( CAPTION ), tr( CANT_DEL_FILE ).arg( in_src_path ) );
//           }
//        }
        utimbuf timebuf;
        timebuf.actime = fi.lastModified().toTime_t();
        timebuf.modtime = fi.lastModified().toTime_t();
        out.setPermissions( in.permissions() );
        chown(dst_path.toUtf8(), fi.ownerId(), fi.groupId());
        utime(dst_path.toUtf8(), &timebuf);
     }
     else {
//        QMessageBox::critical( this, tr( CAPTION ), tr( OPEN_WRITE_ERROR ).arg( dst_path ) );
     }
     in.close();
  }
  else {
//     QMessageBox::critical( this, tr( CAPTION ), tr( OPEN_READ_ERROR ).arg( in_src_path ) );
  }
  ++currentState_->index;

}

void QBtDirCopyWorker::copy_link( const QString& in_src_path, const QString& dst_path )
{
    const QString target = QBtSystemCall::sys("readlink \"" + in_src_path + "\"").trimmed();
    QFile::link(target, dst_path);
}

void QBtDirCopyWorker::copy(QStringList sources, QString dest)
{
    QFileInfoList list;
    for(auto source : sources) {
        list.push_back(QFileInfo(source));
    }
    list_.push_back(QBtCopyState(list,dest));
    currentState_ = &(list_.last());
    copy_list();
}

void QBtDirCopyWorker::resume(QBtOverwriteAnswer answer)
{
   answer_ = answer;
   break_ = false;
   copy_list();
}

bool QBtDirCopyWorker::can_update( const QString& in_src_path, const QString& in_dst_path ) const
{
   const QFileInfo src_fi( in_src_path );
   const QFileInfo dst_fi( in_dst_path );
   return ( src_fi.lastModified() > dst_fi.lastModified() );
}


