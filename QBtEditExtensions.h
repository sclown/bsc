#ifndef INCLUDED_QBtEditExtensions_h
#define INCLUDED_QBtEditExtensions_h

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
 * File         : QBtEditExtensions
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 2007.11.11
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QLineEdit;
class QLabel;
class QPushButton;
class QGroupBox;
class QTextBrowser;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtEditExtensions : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtEditExtensions( QWidget*, const QString& = QString(), const QString& = QString(), const QString& = QString() );
private:
   QBtEditExtensions( const QBtEditExtensions& );
   QBtEditExtensions& operator=( const QBtEditExtensions& );   

//******* CONSTANTS *******
private:
   static const char* const CAPTION;
   static const char* const INFO_CAPTION;
   static const char* const EXTENSION;
   static const char* const APPLICATION;
   static const char* const PARAMETERS;
   static const char* const ACCEPT;
   static const char* const CANCEL;
   static const char* const HELP;
   static const char* const EXT_IS_EMPTY;
   static const char* const PRG_IS_EMPTY;
   static const char* const PRG_NOT_FOUND;
   static const char* const PAR_IS_EMPTY;
   static const char* const IS_NOT_PRG;
   static const char* const SELECT;
   static const char* const FILTER;

//******* MEMBERS *******
private:
   QLabel*       const ext_prompt_;
   QLabel*       const prg_prompt_;
   QLabel*       const par_prompt_;
   QLineEdit*    const ext_;
   QLineEdit*    const prg_;
   QLineEdit*    const par_;
   QPushButton*  const path_;
   QPushButton*  const accept_;
   QPushButton*  const cancel_;
   QGroupBox*    const info_;
   QLabel*       const help_;

//******* METHODS *******
public:
   void data      ( QString&, QString&, QString& );
private:
   void showEvent ( QShowEvent* );
   bool is_ext_ok ();
   bool is_prg_ok ();
   bool is_par_ok ();
private slots:
   void accept    ();
   void select    ();
};

#endif // INCLUDED_QBtEditExtensions_h
