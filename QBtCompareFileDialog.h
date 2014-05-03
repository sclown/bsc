#ifndef INCLUDED_QBtCompareFileDialog_h
#define INCLUDED_QBtCompareFileDialog_h

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
 * File         : QBtCompareFileDialog.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 2008-01-20
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QProgressBar;
class QPushButton;
class QBtInfoField;
class QBtFileComparator;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtCompareFileDialog : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtCompareFileDialog( QWidget*, const QString&, const QString& );
private:
   QBtCompareFileDialog( const QBtCompareFileDialog& );
   QBtCompareFileDialog& operator=( const QBtCompareFileDialog& );

//******* CONSTANTS *******
private:
   static const char* const CAPTION;
   static const char* const FILES;
   static const char* const PROGRESS;
   static const char* const FILE_NR1;
   static const char* const FILE_NR2;
   static const char* const CANCEL;
   static const char* const START;
   static const char* const BREAK;
   static const char* const EQUAL;
   static const char* const NOT_EQUAL;
   static const char* const IO_ERROR;

//******* MEMBERS *******
private:
   const QString            fpath1_;
   const QString            fpath2_;
   QBtInfoField*      const ifield1_;
   QBtInfoField*      const ifield2_;
   QProgressBar*      const progress_bar_;
   QPushButton*       const cancel_;
   QPushButton*       const start_;
   QBtFileComparator* const comparator_;
   bool                     is_started_;
   bool                     is_canceled_;

//******* METHODS *******
private:
   void showEvent    ( QShowEvent* );
   bool check_files  ();
private slots:
   void init_progress( qint64 );
   void progress     ( qint64 );
   void cancel       ();
   void start        ();
};

#endif // INCLUDED_QBtCompareFileDialog_h
