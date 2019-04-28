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
 * File         : QBtCopyDialog.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 02.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "BtTypes.h"
#include "QBtOverwriteAnswer.h"
#include <QDialog>
#include <QFontMetrics>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QProgressBar;
class QCheckBox;
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
private:
   QBtCopyDialog( const QBtCopyDialog& );
   QBtCopyDialog& operator=( const QBtCopyDialog& );

//******* CONSTANTS *******
private:
   static const char* const SRC_DST_CAPTION;
   static const char* const PROGRESS_CAPTION;
   static const char* const REMOVE;
   static const char* const OWNER;
   static const char* const PERMISSIONS;
   static const char* const DATIME;
   static const char* const START;
   static const char* const EXIT;
   static const char* const BREAK;
   static const char* const SRC_LABEL;
   static const char* const DST_LABEL;
   static const char* const RENAME;
   static const char* const NEW_FILE_NAME;
   static const char* const SRC_ITEMS_COUNT_TEXT;
protected:
   static const char* const DIR_NOT_WRITABLE;
   static const char* const DIR_NOT_READABLE;
   static const char* const FILE_NOT_READABLE;
   static const char* const OPEN_READ_ERROR;
   static const char* const OPEN_WRITE_ERROR;
   static const char* const MKDIR_ERROR;
   static const char* const CANT_DEL_FILE;
   static const char* const CANT_DEL_DIR;
   

//******* MEMBERS *******
private:
   QFontMetrics  const font_metrics_;
   QProgressBar* const progress_;
   QPushButton*  const start_;
   QPushButton*  const exit_;
   bool                started_;
protected:
   bool                ask_again_;
   SelectionsSet       sources_;
   QString             destpath_;
   QBtInfoField* const src_path_;
   QLineEdit*    const dst_path_;
   QBtCanOverwrite*  const can_overwrite_;
   QBtOverwriteAnswer::Action command_;

//******* METHODS *******
public:
   int executeCopy( const SelectionsSet&, const QString&, const QBtOverwriteAnswer::Action);
private:
   void     showEvent           ( QShowEvent* );
   QString  sourceInitialText   ();
protected:
   virtual void stop() = 0;
   virtual bool can_update ( const QString&, const QString& ) const = 0;

   QString rename(const QString & );
   void started        ();
   void finished       ();
   void display_paths  ( const QString&, const QString& );
   void reset_progress (const qint64 = 0 );
   void set_progress   (const qint64 );

private slots:
   virtual void start  () = 0;
   void reject         ();
};

#endif // INCLUDED_QBtCopyDialog_h
