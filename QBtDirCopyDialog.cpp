/********************************************************************
 * Copyright (C) Piotr Pszczolkowski
 *-------------------------------------------------------------------
 * This file is part of Beesoft Commander.
 *
 * Beesoft Commander is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * Beesoft Commander is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Beesoft Commander; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *-------------------------------------------------------------------
 * Project      : Beesoft Commander
 * File         : QBtDirCopyDialog.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 02.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtDirCopyDialog.h"
#include "QBtCanOverwrite.h"
#include "QBtDirParser.h"
#include "QBtShared.h"
#include "QBtDirCopyWorker.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QMessageBox>
#include <QLineEdit>
#include <QtDebug>

#include <unistd.h>
#include <utime.h>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtDirCopyDialog::CAPTION = QT_TR_NOOP( "Copy processing" );
const QString     QBtDirCopyDialog::CHOWN   = "chown %1:%2 %3";


//*******************************************************************
// QBtDirCopyDialog                                         CONSTRUCTOR
//*******************************************************************
QBtDirCopyDialog::QBtDirCopyDialog( QWidget* const in_parent ) : QBtCopyDialog( in_parent )
{
    setWindowTitle( tr( CAPTION ) );

    worker_ = new QBtDirCopyWorker;
    worker_->moveToThread(&thread_);
    connect(&thread_, &QThread::finished, worker_, &QObject::deleteLater);
    connect(this, &QBtDirCopyDialog::copy, worker_, &QBtDirCopyWorker::copy);
    connect(this, &QBtDirCopyDialog::resume, worker_, &QBtDirCopyWorker::resume);
    connect(worker_, &QBtDirCopyWorker::ask_overwrite, this, &QBtDirCopyDialog::ask_overwrite);
    connect(worker_, &QBtDirCopyWorker::finished, this, &QBtDirCopyDialog::finish);
    connect(worker_, &QBtDirCopyWorker::reset_progress, this, &QBtDirCopyDialog::reset_progress_slot);
    connect(worker_, &QBtDirCopyWorker::progress, this, &QBtDirCopyDialog::progress_slot);
    connect(worker_, &QBtDirCopyWorker::paths, this, &QBtDirCopyDialog::paths_slot);
//    connect(worker_, &QBtDirListWorker::items_count, this, &QBtDirModel::items_count_slot);
//    connect(worker_, &QBtDirListWorker::item_info, this, &QBtDirModel::item_info_slot);
//    connect(worker_, &QBtDirListWorker::work_finished, this, &QBtDirModel::work_finished_slot);
    thread_.start();

}
// end of QBtDirCopyDialog


QBtDirCopyDialog::~QBtDirCopyDialog()
{
    thread_.quit();
    thread_.wait();
}

//*******************************************************************
// start                                      PRIVATE inherited slot
//*******************************************************************
void QBtDirCopyDialog::start()
{
   destpath_ = dst_path_->text();
   QFileInfo destInfo( destpath_ );
   if( sources_.size() == 1) {
       QFileInfo sourceInfo(*sources_.begin());
       if(!sourceInfo.isDir()) {
           if(destInfo.exists() && destInfo.isDir()) {
               destInfo = QFileInfo( destpath_, sourceInfo.fileName());
           }
           if(destInfo.exists() && !QBtCanOverwrite::canOverwrite(this)) {
               finished();
               return;
           }
           if( !QFileInfo(destInfo.path()).isWritable() ) {
              QMessageBox::critical( this, tr( CAPTION ), tr( DIR_NOT_WRITABLE ).arg( destpath_ ) );
              finished();
              return;
           }
           started();
           copy_file( sourceInfo.absoluteFilePath(), destInfo.absoluteFilePath() );
           finished();
           return;
       }
   }
   if( !destInfo.isWritable() ) {
      QMessageBox::critical( this, tr( CAPTION ), tr( DIR_NOT_WRITABLE ).arg( destpath_ ) );
      finished();
      return;
   }
   
   started();
   //--------------------------------------------
   QStringList sources_list;
   sources_list.reserve(sources_.size());
   for(auto source : sources_) {
       sources_list << source;
   }
   emit copy(sources_list, destInfo.absoluteFilePath());
//   SelectionsSet::const_iterator it = sources_.begin();
//   const SelectionsSet::const_iterator end = sources_.end();
//   while( !break_ && ( it != end ) ) {
//      QFileInfo src( *it );
//      copy_next( src.absoluteFilePath(), destInfo.absoluteFilePath() );
//      ++it;
//   }

   //--------------------------------------------
   //   finished();
}
// end of start


void QBtDirCopyDialog::finish()
{
    finished();
}

void QBtDirCopyDialog::reset_progress_slot(quint32 range)
{
    reset_progress(range);
}

void QBtDirCopyDialog::progress_slot(quint32 state)
{
    set_progress(state);
}

void QBtDirCopyDialog::paths_slot(const QString& src, const QString& dst)
{
    display_paths(src, dst);
}

void QBtDirCopyDialog::ask_overwrite(const QString &dst_path)
{
    auto answer = can_overwrite_->ask(dst_path);
    if(answer.action() == QBtOverwriteAnswer::CANCEL) {
        finished();
        return;
    }
    emit resume(answer);
}

//*******************************************************************
// copy_next                                                 PRIVATE
//*******************************************************************
void QBtDirCopyDialog::copy_next( const QString& in_src_path, const QString& in_dst_path )
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
      if( src.isReadable() ) {
         QDir dst_subdir( in_dst_path );
         dst_subdir.mkdir( src_name );
         if( dst_subdir.cd( src_name ) ) {
            copy_dir( src_path, dst_subdir.absolutePath() );
            if( do_remove() ) {
               remove_dir( src_path );
            }
            QFileInfo sfi( src_path );
            QFile dfi( dst_subdir.absolutePath() );
            if( do_permissions() ) {
               dfi.setPermissions( sfi.permissions() );
            }
            if( do_owner() ) {
                chown(dst_subdir.absolutePath().toUtf8(), sfi.ownerId(), sfi.groupId());
//               sc_.run( CHOWN.arg( sfi.owner() ).arg( sfi.group() ).arg( dst_subdir.absolutePath() ) );
            }
         }
         else {
            //  Nie mozna utworzyc podkatalogu.
            QMessageBox::critical( this, tr( CAPTION ), tr( MKDIR_ERROR ).arg( src_name ) );
         }
      }
      else {
         // Brak praw dostepu. Nic nierob. Wracamy.
         QMessageBox::warning( this, tr( CAPTION ), tr( DIR_NOT_READABLE ).arg( src_path ) );
      }
      return;
   }

   dst_path += "/" + src_name;
//   display_paths( in_src_path, dst_path );
   if( QFile::exists( dst_path ) ) {
       qint32 action = 0;//can_overwrite_->ask(dst_path);
       dst_path = can_overwrite_->newPath();
       switch( action ) {
       case QBtCanOverwrite::UPDATE_FILE:
           if( !can_update(src_path, dst_path) ) {
               return;
           }
           break;
       case QBtCanOverwrite::CANCEL_FILE:
           break_ = true;
           return;
           break;
       case QBtCanOverwrite::SKIP_FILE:
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
void QBtDirCopyDialog::copy_dir( const QString& in_src_dir, const QString& in_dst_dir )
{
//   QBtShared::idle();
   if( break_ ) return;

   QDir dir( in_src_dir );
   dir.setFilter( QDir::AllDirs | QDir::Files | QDir::System );
   dir.setSorting( QDir::Name | QDir::DirsFirst | QDir::IgnoreCase );

   const QFileInfoList data = dir.entryInfoList();
   QFileInfoList::const_iterator it = data.begin();
   const QFileInfoList::const_iterator end = data.end();
   while( !break_ && ( it != end ) ) {
      if( QBtShared::is_regular_file( it->fileName() ) ) {
         copy_next( it->absoluteFilePath(), in_dst_dir );
      }
      ++it;
   }
}
// end of copy_dir

//*******************************************************************
// copy_file                                       PRIVATE inherited
//*******************************************************************
void QBtDirCopyDialog::copy_file( const QString& in_src_path, const QString& dst_path )
{
   // Sprwadz czy czy plik jest do odczytu
   const QFileInfo fi( in_src_path );
   if( !fi.isReadable() ) {
      QMessageBox::critical( this, tr( CAPTION ), tr( FILE_NOT_READABLE ).arg( in_src_path ) );
      return;
   }
   
//   display_paths( in_src_path, dst_path );

  // Po uzyskaniu zgody na kopiowanie bierzemy sie do roboty.
  QFile in( in_src_path );
  QFile out( dst_path );
  if( in.open( QIODevice::ReadOnly ) ) {
     const quint32 nbytes = in.size(); // Liczba bajtow do przekopiowania.
     quint32 copied = 0;               // Licznik juz przekopiowanych bajtow.

     if( out.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
//        reset_progress( nbytes );
//        QBtShared::idle();
        // Petla kopiujaca.
        while( !in.atEnd() ) {
           const quint32 n = in.read( block_, BLOCK_SIZE );
           out.write( block_, n );
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
//        sc_.run( CHOWN.arg( fi.owner() ).arg( fi.group() ).arg( dst_path ) );
//        QBtShared::touch( in_src_path, dst_path );
     }
     else {
        QMessageBox::critical( this, tr( CAPTION ), tr( OPEN_WRITE_ERROR ).arg( dst_path ) );
     }
     in.close();
  }
  else {
     QMessageBox::critical( this, tr( CAPTION ), tr( OPEN_READ_ERROR ).arg( in_src_path ) );
  }
}
// end of copy_file

void QBtDirCopyDialog::copy_link( const QString& in_src_path, const QString& dst_path )
{
    const QString target = QBtSystemCall::sys("readlink \"" + in_src_path + "\"").trimmed();
    QFile::link(target, dst_path);
}

//*******************************************************************
// can_update                                      PRIVATE inherited
//*******************************************************************
bool QBtDirCopyDialog::can_update( const QString& in_src_path, const QString& in_dst_path ) const
{
   const QFileInfo src_fi( in_src_path );
   const QFileInfo dst_fi( in_dst_path );
   return ( src_fi.lastModified() > dst_fi.lastModified() );
}
// end of can_update

//*******************************************************************
// remove_dir                                      PRIVATE inherited
//*******************************************************************
void QBtDirCopyDialog::remove_dir( const QString& in_path )
{
   QDir dir( in_path );
   const QString dir_name = dir.dirName();
   if( dir.cdUp() ) {
      if( !dir.rmdir( dir_name ) ) {
         QMessageBox::critical( this, tr( CAPTION ), tr( CANT_DEL_DIR ).arg( in_path ) );
      }
   }
}
// end of remove_dir
