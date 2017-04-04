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
 * File         : QBtDirModel.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 27.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtConfig.h"
#include "QBtDirModel.h"
#include "QBtShared.h"
#include "QBtEventsController.h"
#include "QBtViewItem.h"
#include "QBtViewStandardItem.h"
#include "QBtDirListWorker.h"
#include "QBtFileInfo.h"
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFileIconProvider>
#include <QMimeData>
#include <QUrl>
#include <QTimer>
#include <QPainter>


//*******************************************************************
// QBtDirModel                                           CONSTRUCTOR
//*******************************************************************
QBtDirModel::QBtDirModel( QObject* const in_parent ) : QBtViewModel( in_parent )
{

   worker_ = new QBtDirListWorker;
   worker_->moveToThread(&thread_);
   connect(&thread_, &QThread::finished, worker_, &QObject::deleteLater);
   connect(this, &QBtDirModel::list, worker_, &QBtDirListWorker::list);
   connect(this, &QBtDirModel::icon, worker_, &QBtDirListWorker::icon);
   connect(worker_, &QBtDirListWorker::work_started, this, &QBtDirModel::work_started_slot);
   connect(worker_, &QBtDirListWorker::items_count, this, &QBtDirModel::items_count_slot);
   connect(worker_, &QBtDirListWorker::item_info, this, &QBtDirModel::item_info_slot);
   connect(worker_, &QBtDirListWorker::item_icon, this, &QBtDirModel::item_icon_slot);
   connect(worker_, &QBtDirListWorker::work_finished, this, &QBtDirModel::work_finished_slot);
   thread_.start();


   connect( &watcher_, SIGNAL( directoryChanged      ( const QString & ) ),
            this    , SLOT  ( directory_changed_slot ( const QString ) ) );
}
// end of QBtDirModel

//*******************************************************************
// QBtDirModel                                           CONSTRUCTOR
//*******************************************************************
QBtDirModel::~QBtDirModel()
{
    thread_.quit();
    thread_.wait();
}
// end of ~QBtDirModel

void QBtDirModel::setup( const QString& in_path, int sortColumn, Qt::SortOrder sortOrder)
{
   sortIndex_ = sortColumn;
   sortOrder_ = sortOrder;
   update( in_path);
}


//*******************************************************************
// update                                          PUBLIC inherited
//*******************************************************************
void QBtDirModel::update( const QString& in_path)
{
   watcher_.removePath( current_path_ );
   emit list( in_path, sortIndex_, sortOrder_ );
}

void QBtDirModel::update( const QModelIndex& in_index )
{
   const QBtViewItem* const it = head_item( in_index );
   if( it ) {
      if( valid_dir_name( it->finfo().full_name() ) ) {
         emit list( it->finfo().path(), sortIndex_, sortOrder_ );
      }
   }
}
// end of update

//*******************************************************************
// refresh                                          PUBLIC inherited
//*******************************************************************
void QBtDirModel::refresh()
{
   update( current_path_ );
}
// end of refresh

//*******************************************************************
// is_dir                                                     PUBLIC
//*******************************************************************
bool QBtDirModel::is_dir( const QModelIndex& in_index )
{
   const QBtViewItem* const it = head_item( in_index );
   return ( it ) ? it->finfo().is_dir() : false;
}
// end of is_dir


bool QBtDirModel::is_executable(const QModelIndex& in_index)
{
    const QFileInfo fi( file_path( in_index ) );
    return !fi.isDir() || (fi.isBundle() && fi.suffix() == ".app");
}

//*******************************************************************
// is_readable                                                PUBLIC
//*******************************************************************
bool QBtDirModel::is_readable( const QModelIndex& in_index )
{
   const QBtViewItem* const it = head_item( in_index );
   return ( it ) ? it->finfo().is_readable() : false;
}
// end of is_dir

//*******************************************************************
// cdup                                                       PUBLIC
//*******************************************************************
void QBtDirModel::cdup()
{
   QDir dir( current_path_ );
   if( !dir.isRoot() && dir.cdUp() ) {
      update( dir.absolutePath() );
   }
}
// end of cdup

bool QBtDirModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return QBtViewModel::removeRows(row, count, parent);
}

bool QBtDirModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
    return QBtViewModel::setItemData(index, roles);
}

void QBtDirModel::sort(int column, Qt::SortOrder order)
{
    sortIndex_ = column;
    sortOrder_ = order;
    emit list( current_path_, sortIndex_, sortOrder_ );
}

//*******************************************************************
// append_row                                                PRIVATE
//*******************************************************************
void QBtDirModel::append_row( const qint32 in_row,
                              const QBtFileInfo& in_finfo,
                              const QStringList& in_data )
{
   const bool is_parent_dir = ( PARENT_DIR == in_data[ 0 ] );
   QStandardItem* const root_it = invisibleRootItem();

   {  // File name
       QBtViewItem* const it = new QBtViewItem;
       QFileIconProvider ip;

      if( is_parent_dir ) {
          it->setIcon(ip.icon(QFileIconProvider::Folder));
      }
      else {
         if( in_finfo.is_dir() ) {
             it->setIcon(ip.icon(QFileIconProvider::Folder));
            ++dir_count_;
         }
         else {
             it->setIcon(ip.icon(QFileIconProvider::File));
            ++file_count_;
         }
      }
      it->finfo( in_finfo );
      it->setText( in_data[ 0 ] );
      root_it->setChild( in_row, 0, it );
   }

   if( is_parent_dir ) return;
   int childIndex = 1;
//   {  // Extension
//      QBtViewStandardItem* const item = new QBtViewStandardItem;
//      item->setTextAlignment( Qt::AlignLeft );
//      item->setText( in_data[ childIndex ] );
//      root_it->setChild( in_row, childIndex, item );
//   }
//   ++childIndex;
   {  // Rozmiar pliku
      QBtViewStandardItem* const item = new QBtViewStandardItem;
      item->setTextAlignment( Qt::AlignRight );
      item->setText( in_data[ childIndex ] );
      root_it->setChild( in_row, childIndex, item );
   }
   ++childIndex;
   {  // Data
      QBtViewStandardItem* const item = new QBtViewStandardItem;
      item->setText( in_data[ childIndex ] );
      root_it->setChild( in_row, childIndex, item );
   }
   ++childIndex;
   {  // Czas
      QBtViewStandardItem* const item = new QBtViewStandardItem;
      item->setText( in_data[ childIndex ] );
      root_it->setChild( in_row, childIndex, item );
   }
   ++childIndex;
   {  // Prawa dostepu
      QBtViewStandardItem* const item = new QBtViewStandardItem;
      item->setTextAlignment( Qt::AlignLeft );
      item->setText( in_data[ childIndex ] );
      root_it->setChild( in_row, childIndex, item );
   }
   ++childIndex;
   {  // Nazwa wlasciciela
      QBtViewStandardItem* const item = new QBtViewStandardItem;
      item->setText( in_data[ childIndex ] );
      root_it->setChild( in_row, childIndex, item );
   }
   ++childIndex;
   {  // Nazwa grupy
      QBtViewStandardItem* const item = new QBtViewStandardItem;
      item->setText( in_data[ childIndex ] );
      root_it->setChild( in_row, childIndex, item );
   }
}
// end of append_row

QStringList QBtDirModel::mimeTypes() const
{
    return QStringList() << "text/uri-list";
}

Qt::ItemFlags QBtDirModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled |
            Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
}

Qt::DropActions QBtDirModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QMimeData *QBtDirModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QList<QUrl> urls;
    foreach (const QModelIndex &index, indexes) {
        if (index.isValid() && index.column()==0) {
            urls.append(QUrl("file://" + file_path(index)));
        }
    }
    mimeData->setUrls(urls);
    return mimeData;

}

bool QBtDirModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    QList<QUrl> urls = data->urls();
    QStringList files;
    foreach (QUrl url, urls) {
        if(url.scheme() == "file"){
            files += url.path();
        }
    }
    QMap<QString, QVariant> userInfo;
    userInfo["action"] = QVariant(action);
    userInfo["files"] = QVariant(files);
    userInfo["drop_target"] = current_path();
    dropInfo_ = userInfo;
    QTimer::singleShot( 100, this, SLOT( notify_drop() ) );
    return !files.empty();
}

//###################################################################
//#                                                                 #
//#                   P R I V A T E   S L O T S                     #
//#                                                                 #
//###################################################################

//*******************************************************************
// work_started_slot                                    PRIVATE slot
//*******************************************************************
void QBtDirModel::work_started_slot()
{
   QApplication::setOverrideCursor( Qt::WaitCursor );      
   busy_ = true;
   file_count_ = dir_count_ = qint32();
   selections_.clear();
   invisibleRootItem()->removeRows( 0, invisibleRootItem()->rowCount() );
}
// end of work_started_slot

//*******************************************************************
// items_count_slot                                     PRIVATE slot
//*******************************************************************
void QBtDirModel::items_count_slot( const qint32 in_row_count )
{
   invisibleRootItem()->setColumnCount( columnCount() );
   invisibleRootItem()->setRowCount( in_row_count );
}
// end of items_count_slot

//*******************************************************************
// item_info_slot                                       PRIVATE slot
//*******************************************************************
void QBtDirModel::item_info_slot( const qint32 in_row, QVariant in_fi, QStringList in_data )
{
   QBtFileInfo info = in_fi.value<QBtFileInfo>();
   append_row( in_row, info, in_data );
   emit icon(in_row, info.path());
}
// end of item_info_slot



void QBtDirModel::item_icon_slot(qint32 row, QIconPtr icon)
{
    auto item = head_item(row);
    if( item ) {
        item->setIcon(*icon.get());
    }
}


//*******************************************************************
// work_finished_slot                                   PRIVATE slot
//*******************************************************************
void QBtDirModel::work_finished_slot( const QString in_path )
{
   current_path_ = in_path;
   QApplication::restoreOverrideCursor();
   emit update_finished();
   watcher_.addPath( current_path_ );
   busy_ = false;
}
// end of work_finished_slot

void QBtDirModel::directory_changed_slot( const QString &in_path )
{
    QTimer::singleShot( 100, this, SLOT( delayed_refresh() ) );
}

void QBtDirModel::delayed_refresh()
{
    refresh();
}

void QBtDirModel::notify_drop()
{
    QBtEventsController::instance()->send_event( QBtEvent::DROP_FILES, dropInfo_ );
}
