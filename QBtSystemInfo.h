#ifndef INCLUDED_QBtSystemInfo_h
#define INCLUDED_QBtSystemInfo_h

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
#include <QDialog>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QStackedWidget;
class QTabBar;
class QTreeWidget;
class QPushButton;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtSystemInfo : public QDialog
{
   Q_OBJECT

// ******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtSystemInfo( QWidget* = 0 );
private:
   QBtSystemInfo( const QBtSystemInfo& );
   QBtSystemInfo& operator=( const QBtSystemInfo& );

// ******* CONSTANTS *******
private:
   enum {
      DISK_IDX = 0,
      MEMORY_IDX,
      
      DSK_FS_IDX = 0,
      DSK_TYPE_IDX,
      DSK_SIZE_IDX,
      DSK_USED_IDX,
      DSK_AVAIL_IDX,
      DSK_USE_IDX,
      DSK_MOUNT_IDX,
      DSK_COL_COUNT,

      MEM_EMPTY_IDX = 0,
      MEM_TOTAL_IDX,
      MEM_USED_IDX,
      MEM_FREE_IDX,
      MEM_SHARED_IDX,
      MEM_BUFFERS_IDX,
      MEM_CACHED_IDX,
      MEM_COL_COUNT,
   };
   static const char* const CAPTION;
   static const char* const DISKS;
   static const char* const MEMORY;
   static const char* const CLOSE;
   static const char* const DSK_FS;
   static const char* const DSK_TYPE;
   static const char* const DSK_SIZE;
   static const char* const DSK_USED;
   static const char* const DSK_AVAIL;
   static const char* const DSK_USE;
   static const char* const DSK_MOUNT;
   static const char* const MEM_EMPTY;
   static const char* const MEM_TOTAL;
   static const char* const MEM_USED;
   static const char* const MEM_FREE;
   static const char* const MEM_SHARED;
   static const char* const MEM_BUFFERS;
   static const char* const MEM_CACHED;
   
// ******* MEMBERS *******
private:
   QStackedWidget* const wstack_;
   QTabBar*        const tbar_;
   QTreeWidget*    const disk_view_;
   QTreeWidget*    const memory_view_;
   QPushButton*    const close_;

// ******* METHODS *******
private:
   void showEvent     ( QShowEvent* );
   void read_dsk_data ();
   void read_mem_data ();
   void add_dsk_item  ( const QStringList& );
   void add_mem_item  ( const QStringList& );
private slots:
   void tab_changed   ( int );
};

#endif // INCLUDED_QBtSystemInfo_h
