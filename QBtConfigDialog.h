#ifndef INCLUDED_QBtConfigDialog_h
#define INCLUDED_QBtConfigDialog_h

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
 * File         : QBtConfigDialog.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QTabBar;
class QStackedWidget;
class QPushButton;
class QBtConfigPanels;
class QBtConfigButtons;
class QBtConfigApps;
class QBtConfigExtensions;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtConfigDialog : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtConfigDialog( QWidget* = 0 );
private:
   QBtConfigDialog( const QBtConfigDialog& );
   QBtConfigDialog& operator=( const QBtConfigDialog& );

//******* CONSTANTS ********
private:
   enum {
      PANELS_IDX = 0,
      BUTTONS_IDX,
      CONSOLE_IDX,
      EXTENSIONS_IDX
   };
   static const char* const CAPTION;
   static const char* const PANELS;
   static const char* const BUTTONS;
   static const char* const CONSOLE;
   static const char* const EXTENSIONS;
   static const char* const APPLY;
   static const char* const EXIT;

//******* MEMBERS *******
private:
   QTabBar*             const tbar_;
   QStackedWidget*      const wstack_;
   QPushButton*         const apply_;
   QPushButton*         const exit_;
   QBtConfigPanels*     const panels_;
   QBtConfigButtons*    const buttons_;
   QBtConfigApps*       const apps_;
   QBtConfigExtensions* const extensions_;

//******* METHODS *******
private slots:
   void tab_changed( int );
   void apply      ();
};

#endif // INCLUDED_QBtConfigDialog_h
