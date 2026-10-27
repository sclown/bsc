#ifndef INCLUDED_QBtCopyDialog_h
#define INCLUDED_QBtCopyDialog_h

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
#include "BtTypes.h"
#include "QBtOverwriteAnswer.h"
#include "QBtCopyWorker.h"
#include <QDialog>
#include <QFutureWatcher>
#include <QTimer>
#include <optional>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QProgressBar;
class QPushButton;
class QLineEdit;
class QBtInfoField;
class QBtCanOverwrite;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtCopyDialog : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtCopyDialog( QWidget* = 0 );
   virtual ~QBtCopyDialog();
private:
   QBtCopyDialog( const QBtCopyDialog& );
   QBtCopyDialog& operator=( const QBtCopyDialog& );

//******* CONSTANTS *******
private:
   static const char* const CAPTION;
   static const char* const SRC_DST_CAPTION;
   static const char* const PROGRESS_CAPTION;
   static const char* const START;
   static const char* const EXIT;
   static const char* const BREAK;
   static const char* const SRC_LABEL;
   static const char* const DST_LABEL;
   static const char* const SRC_ITEMS_COUNT_TEXT;
   static const int         REFRESH_MS;
   static const int         PROGRESS_STEPS;

//******* MEMBERS *******
private:
   QProgressBar*    const progress_;
   QPushButton*     const start_;
   QPushButton*     const exit_;
   QBtInfoField*    const src_path_;
   QLineEdit*       const dst_path_;
   QBtCanOverwrite* const can_overwrite_;
   bool                   started_;
   SelectionsSet          sources_;
   QString                destpath_;
   QBtOverwriteAnswer::Action command_;

   QBtCopyWorker       worker_;
   QFutureWatcher<void>   watcher_;
   // Latest unpainted update from the worker; refresh_ paints and clears it.
   QTimer                 refresh_;
   std::optional<QBtCopyProgress> status_update_;

//******* METHODS *******
public:
   int executeCopy( const SelectionsSet&, const QString&, QBtOverwriteAnswer::Action );
private:
   void     showEvent        ( QShowEvent* );
   QString  sourceInitialText() const;
   void     display_paths    ( const QString&, const QString& );
   QBtOverwriteAnswer ask_overwrite( const QString&, QBtOverwriteAnswer::ErrorType );
private slots:
   void start         ();
   void reject        ();
   void finish        ();
   void progress      ( const QBtCopyProgress& );
   void refresh       ();
};

#endif // INCLUDED_QBtCopyDialog_h
