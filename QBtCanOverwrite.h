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
#include "QBtOverwriteAnswer.h"

/*------- forward declarations:
-------------------------------------------------------------------*/
class QLabel;
class QCheckBox;
class QPushButton;
class QBtInfoField;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtCanOverwrite : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtCanOverwrite( QWidget* );
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
   static const char* const NEW_FILE_NAME;
   static const char* const RENAME_TITLE;

public:
   static bool canOverwrite(QWidget * const in_parent);


//******* MEMBERS *******
private:
   QCheckBox*   const dont_ask_;
   QPushButton* const skip_;
   QPushButton* const overwrite_;
   QPushButton* const update_;
   QPushButton* const rename_;
   QPushButton* const cancel_;
   QBtInfoField*      const path_label_;
   QBtOverwriteAnswer::Action result_;
   QString            path_;

//******* METHODS *******
public:
   QBtOverwriteAnswer ask( const QString& path );
   bool ask_again() const;
   QString newPath() const;
private:
   void showEvent( QShowEvent* );
   QString inputNewName();
private slots:
   void skip_slot     ();
   void overwrite_slot();
   void update_slot   ();
   void rename_slot   ();
   void cancel_slot   ();
};

#endif // INCLUDED_QBtCanOverwrite_h
