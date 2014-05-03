#ifndef INCLUDED_QBtFileEditor_h
#define INCLUDED_QBtFileEditor_h

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
 * File         : QBtFileEditor.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 11.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QTextEdit;
class QPushButton;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtFileEditor : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtFileEditor( QWidget*, const QString& );
private:
   QBtFileEditor( const QBtFileEditor& );
   QBtFileEditor& operator=( const QBtFileEditor& );

//******* CONSTANTS ********
private:
   static const char* const CAPTION;
   static const char* const RELOAD;
   static const char* const SAVE;
   static const char* const CANCEL;
   static const char* const NOTEXT_FILE;
   static const char* const READ_ERROR;
   static const char* const SAVE_ERROR;

//******* MEMBERS *******
private:
   QString             path_;
   QTextEdit*    const editor_;
   QPushButton*  const reload_;
   QPushButton*  const save_;
   QPushButton*  const cancel_;

//******* METHODS *******
private:
   void showEvent  ( QShowEvent* );
   void load_file  ();
private slots:
   void reload_file();
   void save_file  ();
};

#endif // INCLUDED_QBtFileEditor_h
