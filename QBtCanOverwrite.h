#ifndef INCLUDED_QBtCanOverwrite_h
#define INCLUDED_QBtCanOverwrite_h

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
 * File         : QBtCopyDialog.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 02.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QLabel;
class QCheckBox;
class QPushButton;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtCanOverwrite : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtCanOverwrite( QWidget*, const QString& );
private:
   QBtCanOverwrite( const QBtCanOverwrite* );
   QBtCanOverwrite& operator=( const QBtCanOverwrite& );

//******* CONSTANTS *******
public:
   enum {
      CANCEL_FILE = -1,
      SKIP_FILE = 1,
      OVERWRITE_FILE,
      UPDATE_FILE,
      RENAME_FILE
   };
   static const char* const CAPTION;
   static const char* const MESSAGE;
   static const char* const SKIP;
   static const char* const OVERWRITE;
   static const char* const UPDATE;
   static const char* const RENAME;
   static const char* const DONT_ASK_AGAIN;
   static const char* const CANCEL;
   

//******* MEMBERS *******
private:
   QCheckBox*   const dont_ask_;
   QPushButton* const skip_;
   QPushButton* const overwrite_;
   QPushButton* const update_;
   QPushButton* const rename_;
   QPushButton* const cancel_;
   QLabel*      const path_;

//******* METHODS *******
public:
   bool ask_again() const;
private:
   void showEvent( QShowEvent* );
private slots:
   void skip     ();
   void overwrite();
   void update   ();
   void rename   ();
   void cancel   ();
};

#endif // INCLUDED_QBtCanOverwrite_h
