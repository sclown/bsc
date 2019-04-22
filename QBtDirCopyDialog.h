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
#include "QBtOverwriteAnswer.h"
#include <QThread>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QFileInfo;
class QBtDirParser;
class QBtDirCopyWorker;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtDirCopyDialog : public QBtCopyDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtDirCopyDialog( QWidget* = 0 );
   virtual ~QBtDirCopyDialog();
private:
   QBtDirCopyDialog( const QBtDirCopyDialog& );
   QBtDirCopyDialog& operator=( const QBtDirCopyDialog& );

//******* CONSTANTS *******
private:
   enum {
      BLOCK_SIZE = 8192000
   };
   static const char* const CAPTION;
   static const QString     CHOWN;

//******* MEMBERS *******
private:
   char block_ [ BLOCK_SIZE ];
   QBtSystemCall sc_;
   QThread thread_;
   QBtDirCopyWorker *worker_;

//******* METHODS *******
private:
   void stop();
   bool can_update ( const QString&, const QString& ) const;
private slots:
   void start();
   void finish();
   void progress_slot(qint64);
   void item_info_slot(const QString&, const QString&, const qint64);
   void ask_overwrite(const QString&, QBtOverwriteAnswer::ErrorType);

signals:
   void copy(QStringList, QString);
   void resume(QBtOverwriteAnswer);
};

#endif // INCLUDED_QBtDirCopyDialog_h
