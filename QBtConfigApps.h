#ifndef INCLUDED_QBtConfigApps_h
#define INCLUDED_QBtConfigApps_h

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
 * File         : QBtConfigApps.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QWidget>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QRadioButton;
class QLineEdit;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtConfigApps : public QWidget
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtConfigApps( QWidget* = 0 );
private:
   QBtConfigApps( const QBtConfigApps& );
   QBtConfigApps& operator=( const QBtConfigApps& );

//******* CONSTANTS ********
private:
   static const char* const CONSOLE;
   static const char* const DEFAULT_CONSOLE;
   static const char* const OTHER_CONSOLE;
   static const char* const DIFF;
   static const char* const DEFAULT_DIFF;
   static const char* const OTHER_DIFF;
      
//******* MEMBERS *******
private:
   QRadioButton* const console_default_;
   QRadioButton* const console_other_;
   QLineEdit*    const console_command_;
   QRadioButton* const diff_default_;
   QRadioButton* const diff_other_;
   QLineEdit*    const diff_command_;

//******* METHODS *******
public:
   void save          ();
private:
   void restore_state ();
   void save_state    ();
private slots:
   void other_console_selected( bool );
   void other_diff_selected   ( bool );
};

#endif // INCLUDED_QBtConfigApps_h
