#ifndef INCLUDED_QBtDeleter_h
#define INCLUDED_QBtDeleter_h

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
 * File         : QBtDeleter.cpp
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
class QFrame;
class QPushButton;
class QBtInfoField;
class QBtDirParser;
class QBtWiper;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtDeleter : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtDeleter( QWidget* = 0 );
private:
   QBtDeleter( const QBtDeleter& );
   QBtDeleter& operator=( const QBtDeleter& );

//******* CONSTANTS *******
private:
   static const char* const CAPTION;
   static const char* const RUN;
   static const char* const CANCEL;
   static const char* const BREAK;
   static const char* const DIR_ERROR;
   static const char* const CANT_DEL_FILE;
   static const char* const CANT_DEL_DIR;

//******* MEMBERS *******
private:
   QBtInfoField*           const info_;
   QFrame*                 const line_;
   QPushButton*            const run_;
   QPushButton*            const cancel_;
   QBtDirParser*           const parser_;
   QBtWiper*                     wiper_;
   bool                          break_;
   bool                          runned_;
   bool                          wipe_;
   SelectionsSet                 data_;
   SelectionsSet::const_iterator it_;
   SelectionsSet::const_iterator end_;

//******* METHODS *******
public:
   void set_data( const SelectionsSet&, bool );
private:
   void showEvent         ( QShowEvent* );
   void keyPressEvent     ( QKeyEvent* );
private slots:
   void run               ();
   void cancel            ();
   void current_item      ( const QString& );
   void finished          ();
   void read_dir_error    ( const QString& );
   void remove_dir_error  ( const QString& );
   void remove_file_error ( const QString& );
   void progress          ( const QString& );
};

#endif // INCLUDED_QBtDeleter_h 
