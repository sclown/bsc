#ifndef INCLUDED_QBtWorkspace_h
#define INCLUDED_QBtWorkspace_h

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
 * File         : QBtWorkspace.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "BtTypes.h"
#include <QWidget>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QSplitter;
class QBtPanel;
class QBtView;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtWorkspace : public QWidget
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtWorkspace( QWidget* = 0 );
   ~QBtWorkspace();
private:
   QBtWorkspace( const QBtWorkspace& );
   QBtWorkspace& operator=( const QBtWorkspace& );

//******* CONSTANTS *******
public:
   enum {
      LEFT_PANEL_ID = 0,
      RIGHT_PANEL_ID
   };
   static const char* const InputCaption;
   static const char* const InputPrompt;
   static const char* const CompareFiles;
   static const char* const NoFiles;
   static const char* const FilesJoining;
   static const char* const EnterFileName;
   static const char* const NoWritableDir;
   static const char* const CanOverwrite;
   static const char* const Md5;
   static const char* const SyncNoDirs;
   static const char* const CmpNoDirs;
   static const char* const UserError;
   static const QString     Md5Create;
   static const QString     Md5Check;

//******* MEMBERS *******
private:
   QSplitter* const splitter_;
   QBtPanel*        left_panel_;
   QBtPanel*        right_panel_;

//******* METHODS *******
private:
   void keyPressEvent   ( QKeyEvent* );
   void customEvent     ( QEvent* );
   void save            ();
   void restore         ();
   void switch_panels   ();
   bool src_and_dst_view( QBtView*&, QBtView*&      ) const;
   bool get_selections  ( QBtView* , SelectionsSet& ) const;
   void copy            ();
   void pack            ();
   void compare_files   ();
   void compare_dirs    ();
   void sync_dirs       ();
   void diff            ( const QString&, const QString& ) const;
   void join_files      ();
   void dir_tree        ();
   void md5_create      ();
   void md5_check       ();
   void date_time       ();
};

#endif // INCLUDED_QBtWorkspace_h
