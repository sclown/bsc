#ifndef INCLUDED_QBtFinder_h
#define INCLUDED_QBtFinder_h

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
 * File         : QBtFinder.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 2007.11.05
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>
#include <QFontMetrics>
#include "QBtDirParser.h"

/*------- forward declaration:
-------------------------------------------------------------------*/
class QPushButton;
class QTreeWidget;
class QLineEdit;
class QLabel;
class QCheckBox;
class QGroupBox;
class QBtPointsButton;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtFinder : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtFinder( QWidget* = 0, const QString &workDir="" );
private:
   QBtFinder( const QBtFinder& );
   QBtFinder& operator=( const QBtFinder& );

//******* CONSTANTS *******
private:
   static const char* const CAPTION;
   static const char* const SETTINGS;
   static const char* const RESULT;
   static const char* const FILE;
   static const char* const FROM;
   static const char* const WHAT;
   static const char* const RUN;
   static const char* const BREAK;
   static const char* const GOTO;
   static const char* const CANCEL;
   static const char* const CASE;
   static const char* const REGEX;
   static const char* const DIRSEL;
   static const char* const ASK_FOR_FILE;
   static const char* const ASK_FOR_FROM;
   static const char* const FILE_NAME;
   static const char* const FILE_PATH;

//******* MEMBERS *******
private:
   QGroupBox*       const settings_gbox_;
   QLabel*          const file_label_;
   QLabel*          const from_label_;
   QLabel*          const what_label_;
   QLineEdit*       const file_;
   QLineEdit*       const from_;
   QLineEdit*       const what_;
   QCheckBox*       const case_;
   QCheckBox*       const file_regex_;
   QBtPointsButton* const from_points_;
   QCheckBox*       const what_regex_;
   QPushButton*     const run_;
   QPushButton*     const break_;
   QPushButton*     const goto_;
   QPushButton*     const cancel_;
   QTreeWidget*     const result_;
   QLineEdit*       const fpath_;
   QBtDirParser           parser_;
   QFontMetrics           font_metrics_;
   QString                selected_file_path_;

//******* METHODS *******
public:
   void selected_file      ( QString& ) const;
private:
   void showEvent          ( QShowEvent* );
   bool search_what        ( const QString& ) const;
private slots:
   void select_dir         ();
   void run_find           ();
   void break_find         ();
   void goto_find          ();
   void parser_dir_changed ( const QString& );
   void parser_current_item( const QString& );
   void parser_finished    ();
};

 #endif // INCLUDED_QBtFinder_h
