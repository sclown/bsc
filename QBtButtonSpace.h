#ifndef INCLUDED_QBtButtonSpace_h
#define INCLUDED_QBtButtonSpace_h

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
 * File         : QBtButtonSpace.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QWidget>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QPushButton;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtButtonSpace : public QWidget
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtButtonSpace( QWidget* = 0 );
private:
   QBtButtonSpace( const QBtButtonSpace& );
   QBtButtonSpace& operator=( const QBtButtonSpace& );

//******* CONSTANTS *******
public:
   enum {
      ONLY_FN = 0,
      ONLY_TEXT,
      FULL_DESC,
      TWO_LINES
   };
private:
   enum {
      F1 = 0,
      F2, F3, F4, F5, F6, F7, F8, F9, F10,
      BUTTONS_COUNT,
   };
   static const char* const LABEL[ BUTTONS_COUNT ];
      
//******* MEMBERS *******
private:
   QPushButton* button_ [ BUTTONS_COUNT ];

//******* METHODS *******
public:
   void retranslate ();
private slots:
   void clicked     ( int );
};

#endif // INCLUDED_QBtButtonSpace_h
