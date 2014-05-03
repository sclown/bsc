#ifndef INCLUDED_QBtHelp_h
#define INCLUDED_QBtHelp_h

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
 * File         : QBtHelp.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 10.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QTextBrowser;
class QPushButton;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtHelp : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtHelp( QWidget* = 0 );
private:
   QBtHelp( const QBtHelp& );
   QBtHelp& operator=( const QBtHelp& );

//******* CONSTANTS ********
private:
   static const char* const CAPTION;
   static const char* const CANCEL;
   static const char* const NOT_FOUND_CAPTION;
   static const char* const NOT_FOUND_MESSAGE;

//******* MEMBERS *******
private:
   QTextBrowser* const browser_;
   QPushButton*  const cancel_;

//******* METHODS *******
private:
   void showEvent( QShowEvent* );
};

#endif // INCLUDED_QBtHelp_h
