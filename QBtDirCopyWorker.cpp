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
    QObject(parent),
    break_(false),
    currentState_(nullptr)
{

}

void QBtDirCopyWorker::stop()
{
   QMutexLocker locker(&mutex_);
   break_ = true;
}

bool QBtDirCopyWorker::isStopped()
{
    QMutexLocker locker(&mutex_);
    return break_;
}

void QBtDirCopyWorker::copy_list()
{
    qDebug() << "copy_list START";
    if(currentState_->items.size() == 1) {
        auto srcInfo = currentState_->items.front();
        if(!srcInfo.isDir()) {
            auto destInfo = QFileInfo(currentState_->dest);
            if(destInfo.exists() && destInfo.isDir()) {
                destInfo = QFileInfo( currentState_->dest, srcInfo.fileName());
            }
            if(destInfo.exists() && !answer_.valid()) {
                emit ask_overwrite(destInfo.absoluteFilePath(), QBtOverwriteAnswer::EXIST);
                stop();
                return;
            }
            emit item_info( srcInfo.absoluteFilePath(), destInfo.absoluteFilePath(), srcInfo.size() );
            if (!copy_file(srcInfo.absoluteFilePath(), destInfo.absoluteFilePath())){
                stop();
                return;
            }
        }
    }
    while( !isStopped() && list_.size() ) {
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
    if(!isStopped()) {
        emit finished();
        qDebug() << "copy_list finished";
    }
    qDebug() << "copy_list END";
}

void QBtDirCopyWorker::copy_next( const QString& in_src_path, const QString& in_dst_path )
{
//   QBtShared::idle();
   if( isStopped() ) return;

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
   emit item_info( in_src_path, dst_path, src.size() );
   if( QFile::exists( dst_path ) ) {
       if(!answer_.valid()) {
           emit ask_overwrite(dst_path, QBtOverwriteAnswer::EXIST);
           stop();
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
       default:
           break;
       }

   }
   if( src.isSymLink() ) {
       copy_link(src_path, dst_path);
       return;
   }
   if (!copy_file( src_path, dst_path )) {
       stop();
       return;
   }
}
// end of copy_next

//*******************************************************************
// copy_dir                                        PRIVATE inherited
//*******************************************************************
void QBtDirCopyWorker::copy_dir( const QString& in_src_dir, const QString& in_dst_dir )
{
   if( isStopped() ) return;
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
    QFile dfi( dst_subdir.absolutePath() );
    dfi.setPermissions( src.permissions() );
    chown(dst_subdir.absolutePath().toUtf8(), src.ownerId(), src.groupId());


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

bool QBtDirCopyWorker::copy_file( const QString& in_src_path, const QString& dst_path )
{
   const QFileInfo fi( in_src_path );
   if( !fi.isReadable() ) {
       emit ask_overwrite(dst_path, QBtOverwriteAnswer::READ_ERROR);
       return false;
   }

  QFile in( in_src_path );
  QFile out( dst_path );
  if( in.open( QIODevice::ReadOnly ) ) {
     if( out.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
        qint64 nbytes = 0;
        while( !in.atEnd() ) {
           const qint64 n = in.read( block_, BLOCK_SIZE );
           out.write( block_, n );
           nbytes += n;
           if (nbytes > BLOCK_SIZE) {
               emit progress( nbytes );
           }
        }
        utimbuf timebuf;
        timebuf.actime = fi.lastModified().toTime_t();
        timebuf.modtime = fi.lastModified().toTime_t();
        out.setPermissions( in.permissions() );
        chown(dst_path.toUtf8(), fi.ownerId(), fi.groupId());
        utime(dst_path.toUtf8(), &timebuf);
     }
     else {
         emit ask_overwrite(dst_path, QBtOverwriteAnswer::WRITE_ERROR);
         return false;
     }
  }
  else {
      emit ask_overwrite(dst_path, QBtOverwriteAnswer::READ_ERROR);
      return false;
  }
  ++currentState_->index;
  return true;

}

void QBtDirCopyWorker::copy_link( const QString& in_src_path, const QString& dst_path )
{
    QByteArray buf(1024, 0);
    uint size = static_cast<uint>(readlink(in_src_path.toUtf8(), buf.data(), static_cast<size_t>(buf.size())));
    buf[size] = 0;
    QString target = QString(buf);
    QFile::link(target, dst_path);
    ++currentState_->index;
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
   {
        QMutexLocker locker(&mutex_);
        break_ = false;
   }
   answer_ = answer;
   copy_list();
}

bool QBtDirCopyWorker::can_update( const QString& in_src_path, const QString& in_dst_path ) const
{
   const QFileInfo src_fi( in_src_path );
   const QFileInfo dst_fi( in_dst_path );
   return ( src_fi.lastModified() > dst_fi.lastModified() );
}


