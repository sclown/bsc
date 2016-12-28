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
 * File         : QBtViewModel.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 28.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtViewModel.h"
#include "QBtViewItem.h"
#include "QBtEventsController.h"
#include <QInputDialog>
#include <QRegExp>
#include <QDir>
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const QString     QBtViewModel::THIS_DIR               = ".";
const QString     QBtViewModel::PARENT_DIR             = "..";
const char* const QBtViewModel::HDR_NAME               = QT_TR_NOOP( "Name"   );
const char* const QBtViewModel::HDR_EXT                = QT_TR_NOOP( "Ext"    );
const char* const QBtViewModel::HDR_ACCESS             = QT_TR_NOOP( "Access" );
const char* const QBtViewModel::HDR_SIZE               = QT_TR_NOOP( "Size"   );
const char* const QBtViewModel::HDR_DATE               = QT_TR_NOOP( "Date"   );
const char* const QBtViewModel::HDR_TIME               = QT_TR_NOOP( "Time"   );
const char* const QBtViewModel::HDR_OWNER              = QT_TR_NOOP( "Owner"  );
const char* const QBtViewModel::HDR_GROUP              = QT_TR_NOOP( "Group"  );
const char* const QBtViewModel::MASK_SELECTION_CAPTION = QT_TR_NOOP( "File selection" );
const char* const QBtViewModel::MASK_SELECTION_PROMPT  = QT_TR_NOOP( "Enter selection mask:" );
const char* const QBtViewModel::MASK_DESELECTION_PROMPT= QT_TR_NOOP( "Enter deselection mask" );
const char* const QBtViewModel::DIR_ICON               = ":/img/folder.png";
const char* const QBtViewModel::NOREADABLE_ICON        = ":/img/noreadable.png";


//*******************************************************************
// QBtViewModel                                           CONSTRUCTOR
//*******************************************************************
QBtViewModel::QBtViewModel( QObject* const in_parent ) : QStandardItemModel( in_parent )
, current_path_ ( QString()       )
, file_count_   ( qint32()        )
, dir_count_    ( qint32()        )
, busy_         ( true            )
, thread_       ( 0               )
, selections_   ( SelectionsSet() )
, sortIndex_    ( 0               )
, sortOrder_    ( Qt::AscendingOrder)
{
   QStringList hdrs;
   hdrs  << HDR_NAME
//         << HDR_EXT
         << HDR_SIZE
         << HDR_DATE
         << HDR_TIME
         << HDR_ACCESS
         << HDR_OWNER
         << HDR_GROUP;
   setHorizontalHeaderLabels ( hdrs );
}
// end of QBtViewModel

//*******************************************************************
// current_path                                               PUBLIC
//*******************************************************************
const QString& QBtViewModel::current_path() const
{
   return current_path_;
}
// end of path

//*******************************************************************
// file_path                                                  PUBLIC
//*******************************************************************
QString QBtViewModel::file_path( const QModelIndex& in_index ) const
{
   const QBtViewItem* const it = head_item( in_index );
   if( it ) {
      return it->finfo().path();
   }
   return QString();
}
// end of file_path

//*******************************************************************
// file_full_name                                             PUBLIC
//*******************************************************************
QString QBtViewModel::file_full_name( const QModelIndex& in_index ) const
{
   const QBtViewItem* const it = head_item( in_index );
   return ( ( it ) ? it->finfo().full_name() : QString() );
}
// end of file_full_name

//*******************************************************************
// search_file                                                PUBLIC
//*******************************************************************
QModelIndex QBtViewModel::search_file( const QString& in_fname )
{
   const qint32 n = rowCount();
   for( qint32 i = 0; i < n; ++i ) {
      const QBtViewItem* const it = head_item( i );
      if( it ) {
         if( it->finfo().full_name() == in_fname ) {
            return indexFromItem( it );
         }
      }
   }
   return QModelIndex();
}
// end of search_file

//*******************************************************************
// head_iteam                                                PRIVATE
//*******************************************************************
QBtViewItem* QBtViewModel::head_item( const QModelIndex& in_index ) const
{
   if( in_index.isValid() ) {
      return dynamic_cast<QBtViewItem*>( invisibleRootItem()->child( in_index.row(), 0 ) );
   }
   return 0;
}
QBtViewItem* QBtViewModel::head_item( const qint32 in_row ) const

{
   return dynamic_cast<QBtViewItem*>( invisibleRootItem()->child( in_row, 0 ) );
}
// end of head_item

QModelIndexList QBtViewModel::row_indexes(const qint32 in_row) const
{
    QModelIndexList result;
    const qint32 n = invisibleRootItem()->columnCount();
    for( qint32 i = 0; i < n; ++i ) {
        result.append( indexFromItem( invisibleRootItem()->child( in_row, i ) ) );
    }
    return result;
}

//*******************************************************************
// head_item_index                                            PUBLIC
//*******************************************************************
QModelIndex QBtViewModel::head_item_index( const qint32 in_row ) const
{
   return indexFromItem( head_item( in_row ) );
}
// end of head_item_index

//*******************************************************************
// valid_dir_name                                            PRIVATE
//*******************************************************************
bool QBtViewModel::valid_dir_name( const QString& in_path )
{
   if( QDir::rootPath() == current_path_ ) {
      if( PARENT_DIR == in_path ) {
         return false;
      }
   }
   return true;
}
// end of valid_dir_name
