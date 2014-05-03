#ifndef INCLUDED_QBtDirCopyDialog_h
#define INCLUDED_QBtDirCopyDialog_h

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
 * File         : QBtDirCopyDialog.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 02.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtCopyDialog.h"
#include "QBtSystemCall.h"

/*------- forward declarations:
-------------------------------------------------------------------*/
class QFileInfo;
class QBtDirParser;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtDirCopyDialog : public QBtCopyDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtDirCopyDialog( QWidget* = 0 );
private:
   QBtDirCopyDialog( const QBtDirCopyDialog& );
   QBtDirCopyDialog& operator=( const QBtDirCopyDialog& );

//******* CONSTANTS *******
private:
   enum {
      BLOCK_SIZE = 8192
   };
   static const char* const CAPTION;
   static const QString     CHOWN;

//******* MEMBERS *******
private:
   char block_ [ BLOCK_SIZE ];
   QBtSystemCall sc_;

//******* METHODS *******
private:
   void copy_next  ( const QString&, const QString& );
   void copy_file  ( const QString&, const QString& );
   void copy_dir   ( const QString&, const QString& );
   bool can_update ( const QString&, const QString& ) const;
   void remove_dir ( const QString& );
private slots:
   void start();
};

#endif // INCLUDED_QBtDirCopyDialog_h
