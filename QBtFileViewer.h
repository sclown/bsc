#ifndef INCLUDED_QBtFileViewer_h
#define INCLUDED_QBtFileViewer_h

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
 * File         : QBtFileViewer.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 10.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>
#include <QTextCharFormat>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QTextBrowser;
class QPushButton;
class QLineEdit;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtFileViewer : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtFileViewer( QWidget*, const QString& );
private:
   QBtFileViewer( const QBtFileViewer& );
   QBtFileViewer& operator=( const QBtFileViewer& );

//******* CONSTANTS ********
private:
   static const char* const CAPTION;
   static const char* const CANCEL;
   static const char* const SEARCH;

//******* MEMBERS *******
private:
   const QString       path_;
   QTextBrowser* const browser_;
   QLineEdit*    const search_edit_;
   QPushButton*  const search_;
   QPushButton*  const cancel_;
   QList<int>          search_results_;
   int                 search_index_;
   QString             search_text_;

//******* METHODS *******
private:
   void keyPressEvent(QKeyEvent* );
   void showEvent  ( QShowEvent* );
   void tar_file   ( const QString& );
   void zip_file   ( const QString& );
   void html_file  ( const QString& );
   void text_file  ( const QString& );
   void image_file ( const QString& );
   void other_file ( const QString& );
   void set_font   ();
   void load_file  ();
   void do_it      ( const QString& );
   void highlightSearch(QTextDocument *document, QString searchString);
   QTextCharFormat defaultCharFormat();
private slots:
   void search();
};

#endif // INCLUDED_QBtFileViewer_h
