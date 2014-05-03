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
 * File         : QBtSystemInfo.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 25.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtSystemInfo.h"
#include "QBtShared.h"
#include <QStackedWidget>
#include <QTabBar>
#include <QTreeWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QProcess>
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtSystemInfo::CAPTION        = QT_TR_NOOP( "System information" );
const char* const QBtSystemInfo::DISKS          = QT_TR_NOOP( "&Disks"             );
const char* const QBtSystemInfo::MEMORY         = QT_TR_NOOP( "&Memory (Mb)"       );
const char* const QBtSystemInfo::CLOSE          = QT_TR_NOOP( "&Close"             );
const char* const QBtSystemInfo::DSK_FS         = QT_TR_NOOP( "File system"        );
const char* const QBtSystemInfo::DSK_TYPE       = QT_TR_NOOP( "Type"               );
const char* const QBtSystemInfo::DSK_SIZE       = QT_TR_NOOP( "Size"               );
const char* const QBtSystemInfo::DSK_USED       = QT_TR_NOOP( "Used"               );
const char* const QBtSystemInfo::DSK_AVAIL      = QT_TR_NOOP( "Available"          );
const char* const QBtSystemInfo::DSK_USE        = QT_TR_NOOP( "Use"                );
const char* const QBtSystemInfo::DSK_MOUNT      = QT_TR_NOOP( "Mounted on"         );
const char* const QBtSystemInfo::MEM_EMPTY      = QT_TR_NOOP( " "                  );
const char* const QBtSystemInfo::MEM_TOTAL      = QT_TR_NOOP( "Total"              );
const char* const QBtSystemInfo::MEM_USED       = QT_TR_NOOP( "Used"               );
const char* const QBtSystemInfo::MEM_FREE       = QT_TR_NOOP( "Free"               );
const char* const QBtSystemInfo::MEM_SHARED     = QT_TR_NOOP( "Shared"             );
const char* const QBtSystemInfo::MEM_BUFFERS    = QT_TR_NOOP( "Buffers"            );
const char* const QBtSystemInfo::MEM_CACHED     = QT_TR_NOOP( "Cached"             );


//*******************************************************************
// QBtSystemInfo                                         CONSTRUCTOR
//*******************************************************************
QBtSystemInfo::QBtSystemInfo( QWidget* const in_parent ) : QDialog( in_parent )
, wstack_      ( new QStackedWidget             )
, tbar_        ( new QTabBar                    )
, disk_view_   ( new QTreeWidget                )
, memory_view_ ( new QTreeWidget                )
, close_       ( new QPushButton( tr( CLOSE ) ) )
{
   setWindowTitle( tr( CAPTION ) );

   QFontMetrics fm( font() );
   const qint32 h = fm.height() + 2;

   disk_view_->setRootIsDecorated( false );
   disk_view_->setAlternatingRowColors( true );
   disk_view_->setColumnCount( DSK_COL_COUNT );
   disk_view_->header()->setMaximumHeight( h );
   QTreeWidgetItem* const dsk_hdr = new QTreeWidgetItem;
   dsk_hdr->setText( DSK_FS_IDX   , tr( DSK_FS    ) );
   dsk_hdr->setText( DSK_TYPE_IDX , tr( DSK_TYPE  ) );
   dsk_hdr->setText( DSK_SIZE_IDX , tr( DSK_SIZE  ) );
   dsk_hdr->setText( DSK_USED_IDX , tr( DSK_USED  ) );
   dsk_hdr->setText( DSK_AVAIL_IDX, tr( DSK_AVAIL ) );
   dsk_hdr->setText( DSK_USE_IDX  , tr( DSK_USE   ) );
   dsk_hdr->setText( DSK_MOUNT_IDX, tr( DSK_MOUNT ) );
   disk_view_->setHeaderItem( dsk_hdr );

   memory_view_->setRootIsDecorated( false );
   memory_view_->setAlternatingRowColors( true );
   memory_view_->setColumnCount( MEM_COL_COUNT );
   memory_view_->header()->setMaximumHeight( h );
   QTreeWidgetItem* const mem_hdr = new QTreeWidgetItem;
   mem_hdr->setText( MEM_EMPTY_IDX  , tr( MEM_EMPTY   ) );
   mem_hdr->setText( MEM_TOTAL_IDX  , tr( MEM_TOTAL   ) );
   mem_hdr->setText( MEM_USED_IDX   , tr( MEM_USED    ) );
   mem_hdr->setText( MEM_FREE_IDX   , tr( MEM_FREE    ) );
   mem_hdr->setText( MEM_SHARED_IDX , tr( MEM_SHARED  ) );
   mem_hdr->setText( MEM_BUFFERS_IDX, tr( MEM_BUFFERS ) );
   mem_hdr->setText( MEM_CACHED_IDX , tr( MEM_CACHED  ) );
   memory_view_->setHeaderItem( mem_hdr );

   tbar_->insertTab( DISK_IDX  , tr( DISKS  ) );
   tbar_->insertTab( MEMORY_IDX, tr( MEMORY ) );
   wstack_->insertWidget( DISK_IDX  , disk_view_   );
   wstack_->insertWidget( MEMORY_IDX, memory_view_ );

   QHBoxLayout* const hbl = new QHBoxLayout;
   hbl->addStretch( 100 );
   hbl->addWidget( close_ );

   QVBoxLayout* const vbl = new QVBoxLayout( this );
   vbl->addWidget( tbar_ );
   vbl->addWidget( wstack_ );
   vbl->addLayout( hbl );

   connect( close_, SIGNAL( clicked()           ), this, SLOT( accept()         ) );
   connect( tbar_ , SIGNAL( currentChanged(int) ), this, SLOT( tab_changed(int) ) );
}
// end of QBtSystemInfo

//*******************************************************************
// tab_changed                                          PRIVATE slot
//*******************************************************************
void QBtSystemInfo::tab_changed( const int in_idx )
{
   wstack_->setCurrentIndex( in_idx );
}
// end of tab_changed

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtSystemInfo::showEvent( QShowEvent* const in_event )
{
   read_dsk_data();
   read_mem_data();
   
   QBtShared::resize( this, 40, 30 );
   QDialog::showEvent( in_event );
}
// end of showEvent

//*******************************************************************
// read_dsk_data                                             PRIVATE
//*******************************************************************
void QBtSystemInfo::read_dsk_data()
{
   QStringList args;
   args << "-h" << "-T";
   
   QProcess sc;
   sc.start( "df", args );

   QByteArray data;
   while( sc.waitForReadyRead() ) {
      data += sc.readAll();
   }

   const QStringList text = QString::fromLocal8Bit( data ).split( '\n', QString::SkipEmptyParts );
   for( qint32 i = 1; i < text.size(); ++i ) {
      add_dsk_item( text[ i ].split( ' ', QString::SkipEmptyParts ) );
   }
   for( qint32 i = 0; i < DSK_COL_COUNT; ++i ) {
      disk_view_->resizeColumnToContents( i );
   }
}
// end of read_dsk_data

//*******************************************************************
// read_mem_data                                             PRIVATE
//*******************************************************************
void QBtSystemInfo::read_mem_data()
{
   QStringList args;
   args << "-t" << "-o";
   
   QProcess sc;
   sc.start( "free", args );

   QByteArray data;
   while( sc.waitForReadyRead() ) {
      data += sc.readAll();
   }

   const QStringList text = QString::fromLocal8Bit( data ).split( '\n', QString::SkipEmptyParts );
   for( qint32 i = 1; i < text.size(); ++i ) {
      add_mem_item( text[ i ].split( ' ', QString::SkipEmptyParts ) );
   }
   for( qint32 i = 0; i < MEM_COL_COUNT; ++i ) {
      memory_view_->resizeColumnToContents( i );
   }
}
// end of read_mem_data

//*******************************************************************
// add_dsk_item                                              PRIVATE
//*******************************************************************
void QBtSystemInfo::add_dsk_item( const QStringList& in_info )
{
   QTreeWidgetItem* const twi = new QTreeWidgetItem( disk_view_ );

   twi->setTextAlignment( DSK_SIZE_IDX , Qt::AlignRight   );
   twi->setTextAlignment( DSK_USED_IDX , Qt::AlignRight   );
   twi->setTextAlignment( DSK_AVAIL_IDX, Qt::AlignRight   );
   twi->setTextAlignment( DSK_USE_IDX  , Qt::AlignHCenter );

   twi->setText( DSK_FS_IDX   , in_info[ DSK_FS_IDX    ] );
   twi->setText( DSK_TYPE_IDX , in_info[ DSK_TYPE_IDX  ] );
   twi->setText( DSK_SIZE_IDX , in_info[ DSK_SIZE_IDX  ] );
   twi->setText( DSK_USED_IDX , in_info[ DSK_USED_IDX  ] );
   twi->setText( DSK_AVAIL_IDX, in_info[ DSK_AVAIL_IDX ] );
   twi->setText( DSK_USE_IDX  , in_info[ DSK_USE_IDX   ] );
   twi->setText( DSK_MOUNT_IDX, in_info[ DSK_MOUNT_IDX ] );
}
// end of add_dsk_item

//*******************************************************************
// add_mem_item                                              PRIVATE
//*******************************************************************
void QBtSystemInfo::add_mem_item( const QStringList& in_info )
{
   QTreeWidgetItem* const twi = new QTreeWidgetItem( memory_view_ );

   twi->setTextAlignment( MEM_TOTAL_IDX  , Qt::AlignRight );
   twi->setTextAlignment( MEM_USED_IDX   , Qt::AlignRight );
   twi->setTextAlignment( MEM_FREE_IDX   , Qt::AlignRight );
   twi->setTextAlignment( MEM_SHARED_IDX , Qt::AlignRight );
   twi->setTextAlignment( MEM_BUFFERS_IDX, Qt::AlignRight );
   twi->setTextAlignment( MEM_CACHED_IDX , Qt::AlignRight );

   const qint32 n = in_info.size();
   if( MEM_EMPTY_IDX < n ) {
      twi->setText( MEM_EMPTY_IDX, in_info[ MEM_EMPTY_IDX ] );
   }
   if( MEM_TOTAL_IDX < n ) {
      twi->setText( MEM_TOTAL_IDX, in_info[ MEM_TOTAL_IDX ] );
   }
   if( MEM_USED_IDX < n ) {
      twi->setText( MEM_USED_IDX, in_info[ MEM_USED_IDX ] );
   }
   if( MEM_FREE_IDX < n ) {
      twi->setText( MEM_FREE_IDX, in_info[ MEM_FREE_IDX ] );
   }
   if( MEM_SHARED_IDX < n ) {
      twi->setText( MEM_SHARED_IDX, in_info[ MEM_SHARED_IDX ] );
   }
   if( MEM_BUFFERS_IDX < n ) {
      twi->setText( MEM_BUFFERS_IDX, in_info[ MEM_BUFFERS_IDX ] );
   }
   if( MEM_CACHED_IDX < n ) {
      twi->setText( MEM_CACHED_IDX, in_info[ MEM_CACHED_IDX ] );
   }
}
// end of add_mem_item
