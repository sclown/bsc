#ifndef INCLUDED_QBtPanel_h
#define INCLUDED_QBtPanel_h

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
 * File         : QBtPanel.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 27.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QWidget>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QBtTabBar;
class QStackedWidget;
class QLabel;
class QComboBox;
class QBtView;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtPanel : public QWidget
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtPanel( qint32, QWidget* = 0 );
   ~QBtPanel();
private:
   QBtPanel( const QBtPanel& );
   QBtPanel& operator=( const QBtPanel& );

//******* CONSTANTS *******
private:
   static const char* const DIRS;
   static const char* const FILES;
   static const char* const SELECTED;

//******* MEMBERS *******
private:
   qint32                idx_;
   QComboBox*      const path_;
   QComboBox*      const fstab_;
   QBtTabBar*      const tbar_;
   QStackedWidget* const wstack_;
   QLabel*         const dirs_;
   QLabel*         const files_;
   QLabel*         const selected_;

//******* METHODS *******
public:
   void           show_fstab            ();
   void           set_tab_id            ( qint32 );
   QBtView* const current_view          () const;
private:
   void           keyPressEvent         ( QKeyEvent* );
   void           customEvent           ( QEvent* );
   void           new_tab_request       ();
   void           del_tab_request       ();
   qint32         new_tab               ( const QString& );
   void           update_fstab          ();
   void           save                  ();
   void           restore               ();
private slots:
   void           tab_mouse_event       ();
   void           tab_bar_changed       ( int );
   void           current_path          ( const QString& );
   void           update_dirs_count     ( qint32 );
   void           update_files_count    ( qint32 );
   void           update_selected_count ( qint32 );
   void           path_changed          ( const QString& );
   void           fstab_changed         ( const QString& );
   void           edit_finished         ();
};

#endif // INCLUDED_QBtPanel_h
