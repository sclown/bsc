#ifndef INCLUDED_QBtConfigExtensions_h
#define INCLUDED_QBtConfigExtensions_h

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
 * File         : QBtConfigExtensions.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QWidget>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QTreeWidget;
class QPushButton;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtConfigExtensions : public QWidget
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtConfigExtensions( QWidget* = 0 );
private:
   QBtConfigExtensions( const QBtConfigExtensions& );
   QBtConfigExtensions& operator=( const QBtConfigExtensions& );

//******* CONSTANTS ********
private:
   static const char* const REMOVE;
   static const char* const ADD;
   static const char* const EDIT;
   static const char* const EXT;
   static const char* const PRG;
   static const char* const PAR;
   
//******* MEMBERS *******
private:
   QTreeWidget* const table_;
   QPushButton* const remove_;
   QPushButton* const add_;
   QPushButton* const edt_;

//******* METHODS *******
private:
   void add_item( const QString&, const QString&, const QString& );
   void restore ();
private slots:
   void remove  ();
   void add     ();
   void edit    ();
public:
   void save    ();
};

#endif // INCLUDED_QBtConfigExtensions_h
