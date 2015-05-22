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
   static const char* const MENU_LANGS;
   static const char* const MENU_HELP;
   //------- System -------
   static const char* const REFRESH_ICON;
   static const char* const FILTER_ICON;
   static const char* const CONFIGURE_ICON;
   static const char* const EXIT_ICON;
   static const char* const REFRESH;
   static const char* const FILTER;
   static const char* const CONFIGURE;
   static const char* const EXIT;
   static const char* const FILTER_ACCEL;
   static const char* const EXIT_ACCEL;
   //------- Operations -------
   static const char* const NEW_TAB_ICON;
   static const char* const DEL_TAB_ICON;
   static const char* const SWITCH_TABS_ICON;
   static const char* const FILE_JOIN_ICON;
   static const char* const DIR_TREE_ICON;
   static const char* const CMP_FILES_ICON;
   static const char* const CMP_DIRS_ICON;
   static const char* const DIR_SYNC_ICON;
   static const char* const DATE_TIME_ICON;
   static const char* const SELECT_MASK_ICON;
   static const char* const UNSELECT_MASK_ICON;
   static const char* const SELECT_REV_ICON;
   static const char* const FTP_CONNECT_ICON;
   static const char* const FTP_DISCONNECT_ICON;
   static const char* const SYSTEM_INFO_ICON;
   static const char* const FIND_ICON;
   static const char* const NEW_TAB;
   static const char* const DEL_TAB;
   static const char* const SWITCH_TABS;
   static const char* const FILE_JOIN;
   static const char* const DIR_TREE;
   static const char* const CMP_FILES;
   static const char* const CMP_DIRS;
   static const char* const DIR_SYNC;
   static const char* const DATE_TIME;
   static const char* const SELECT_MASK;
   static const char* const UNSELECT_MASK;
   static const char* const SELECT_REV;
   static const char* const MD5_CREATE;
   static const char* const MD5_CHECK;
   static const char* const FTP_CONNECT;
   static const char* const FTP_DISCONNECT;
   static const char* const SYSTEM_INFO;
   static const char* const FIND;
   static const char* const NEW_TAB_ACCEL;
   static const char* const DEL_TAB_ACCEL;
   static const char* const SWITCH_TABS_ACCEL;
   static const char* const FILE_JOIN_ACCEL;
   static const char* const MD5_CREATE_ACCEL;
   static const char* const MD5_CHECK_ACCEL;
   static const char* const FIND_ACCEL;
   static const char* const SHELL_ICON;
   static const char* const TERMINAL_ICON;
   //------- Languages -------
   static const char* const CS_LANG;
   static const char* const DE_LANG;
   static const char* const EN_LANG;
   static const char* const PL_LANG;
   static const char* const RU_LANG;
   static const char* const ES_LANG;
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
   QMenu*          const menu_langs_;
   QMenu*          const menu_help_;
   //------- Toolbars -------
   QToolBar*       const system_tbar_;
   QToolBar*       const operations_tbar_;
   //------- System -------
   QAction*        const refresh_;
   QAction*        const filter_;
   QAction*        const configure_;
   QAction*        const exit_;
   //------- Operations -------
   QAction*        const new_tab_;
   QAction*        const del_tab_;
   QAction*        const switch_tabs_;
   QAction*        const file_join_;
   QAction*        const dir_tree_;
   QAction*        const cmp_files_;
   QAction*        const cmp_dirs_;
   QAction*        const dir_sync_;
   QAction*        const date_time_;
   QAction*        const select_mask_;
   QAction*        const unselect_mask_;
   QAction*        const select_rev_;
   QAction*        const md5_create_;
   QAction*        const md5_check_;
   QAction*        const ftp_connect_;
   QAction*        const ftp_disconnect_;
   QAction*        const system_info_;
   QAction*        const find_;
   QAction*        const open_shell_;
   QAction*        const open_terminal_;
   //------- Languages -------
   QAction*        const cs_lang_;
   QAction*        const de_lang_;
   QAction*        const en_lang_;
   QAction*        const pl_lang_;
   QAction*        const ru_lang_;
   QAction*        const es_lang_;
   QActionGroup*   const lang_;
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
   void customEvent      ( QEvent* );
   void create_menu      ();
   void create_tool_bars ();
   void update_action    ();
   void retranslate      ();
   void f1_key           ();
   void f10_key          ();
private slots:
   //------- System -------
   void refresh          ();
   void filter           ();
   void configure        ();
   //------- Operations -------
   void new_tab          ();
   void del_tab          ();
   void switch_tabs      ();
   void file_join        ();
   void dir_tree         ();
   void cmp_files        ();
   void cmp_dirs         ();
   void sync_dirs        ();
   void date_time        ();
   void select_mask      ();
   void unselect_mask    ();
   void select_rev       ();
   void md5_create       ();
   void md5_check        ();
   void ftp_connect      ();
   void ftp_disconnect   ();
   void system_info      ();
   void find             ();
   void open_shell       ();
   void open_terminal    ();
   //------- Languages -------
   void lang             ( QAction* );
   //------- Help -------
   void about_qt         ();
   void about_bsc        ();
};

#endif // INCLUDED_QBtMainWindow_h
