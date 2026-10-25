#ifndef INCLUDED_QBtMainWindow_h
#define INCLUDED_QBtMainWindow_h

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
 * File         : QBtMainWindow.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 24.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QMainWindow>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QMenu;
class QAction;
class QActionGroup;
class QToolBar;
class QBtWorkspace;
class QBtButtonSpace;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtMainWindow : public QMainWindow
{
   Q_OBJECT

// ******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtMainWindow();
   ~QBtMainWindow();
private:
   QBtMainWindow( const QBtMainWindow& );
   QBtMainWindow& operator=( const QBtMainWindow& );

// ******* CONSTANTS *******
private:
   static const QString IMG_LOC;
   //------- Menu -------
   static const char* const MENU_SYSTEM;
   static const char* const MENU_OPERATIONS;
   static const char* const MENU_HELP;
   //------- System -------
   static const char* const REFRESH_ICON;
   static const char* const EXIT_ICON;
   static const char* const REFRESH;
   static const char* const EXIT;
   static const char* const EXIT_ACCEL;
   //------- Operations -------
   static const char* const NEW_TAB_ICON;
   static const char* const DEL_TAB_ICON;
   static const char* const SWITCH_TABS_ICON;
   static const char* const SELECT_MASK_ICON;
   static const char* const UNSELECT_MASK_ICON;
   static const char* const SELECT_REV_ICON;
   static const char* const SYSTEM_INFO_ICON;
   static const char* const FIND_ICON;
   static const char* const NEW_TAB;
   static const char* const DEL_TAB;
   static const char* const SWITCH_TABS;
   static const char* const SELECT_MASK;
   static const char* const UNSELECT_MASK;
   static const char* const SELECT_REV;
   static const char* const SYSTEM_INFO;
   static const char* const FIND;
   static const char* const NEW_TAB_ACCEL;
   static const char* const DEL_TAB_ACCEL;
   static const char* const SWITCH_TABS_ACCEL;
   static const char* const FIND_ACCEL;
   //------- Help -------
   static const char* const ABOUT_QT_ICON;
   static const char* const ABOUT_BSC_ICON;
   static const char* const ABOUT_QT;
   static const char* const ABOUT_BSC;

// ******* MEMBERS *******
private:
   //------- Menu -------
   QMenu*          const menu_system_;
   QMenu*          const menu_operations_;
   QMenu*          const menu_help_;
   //------- System -------
   QAction*        const refresh_;
   QAction*        const exit_;
   //------- Operations -------
   QAction*        const new_tab_;
   QAction*        const del_tab_;
   QAction*        const switch_tabs_;
   QAction*        const select_mask_;
   QAction*        const unselect_mask_;
   QAction*        const select_rev_;
   QAction*        const system_info_;
   QAction*        const find_;
   //------- Help -------
   QAction*        const about_qt_;
   QAction*        const about_bsc_;
   //------- Pozostale -------
   QBtWorkspace*   const workspace_;
   QBtButtonSpace* const buttons_;
   
// ******* METHODS *******
private:
   void save_settings    () const;
   void restore_settings ();
   void showEvent        ( QShowEvent* );
   void keyPressEvent    ( QKeyEvent* );
   bool event            ( QEvent *event );
   bool eventFilter      ( QObject*, QEvent* ) override;
   void customEvent      ( QEvent* );
   void create_menu      ();
   void update_action    ();
   void retranslate      ();
   void f1_key           ();
   void f10_key          ();
private slots:
   //------- System -------
   void refresh          ();
   //------- Operations -------
   void new_tab          ();
   void del_tab          ();
   void switch_tabs      ();
   void select_mask      ();
   void unselect_mask    ();
   void select_rev       ();
   void system_info      ();
   void find             ();
   //------- Help -------
   void about_qt         ();
   void about_bsc        ();
};

#endif // INCLUDED_QBtMainWindow_h
