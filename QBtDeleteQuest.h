#ifndef INCLUDED_QBtDeleteQuest_h
#define INCLUDED_QBtDeleteQuest_h

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
 * File         : QBtDeleteQuest.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 04.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>
#include "BtTypes.h"

/*------- forward declarations:
-------------------------------------------------------------------*/
class QTreeWidget;
class QPushButton;
class QCheckBox;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtDeleteQuest : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtDeleteQuest( QWidget* = 0 );
private:
   QBtDeleteQuest( const QBtDeleteQuest& );
   QBtDeleteQuest& operator=( const QBtDeleteQuest& );

//******* CONSTANTS *******
private:
   static const char* const CAPTION;
   static const char* const WIPE;
   static const char* const ACCEPT;
   static const char* const CANCEL;
   static const char* const HEAD;

//******* MEMBERS *******
private:
   QTreeWidget* const view_;
   QCheckBox*   const wipe_;
   QPushButton* const accept_;
   QPushButton* const cancel_;

//******* METHODS *******
public:
   void set_files( const SelectionsSet& );
   bool to_wipe  () const;
private:
   void showEvent( QShowEvent* );
};

#endif // INCLUDED_QBtDeleteQuest_h
