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
#include "QBtDirCopyDialog.h"
#include "QBtCanOverwrite.h"
#include "QBtDirParser.h"
#include "QBtShared.h"
#include "QBtDirCopyWorker.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QMessageBox>
#include <QLineEdit>
#include <QtDebug>

#include <unistd.h>
#include <utime.h>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtDirCopyDialog::CAPTION = QT_TR_NOOP( "Copy processing" );
const QString     QBtDirCopyDialog::CHOWN   = "chown %1:%2 %3";


//*******************************************************************
// QBtDirCopyDialog                                         CONSTRUCTOR
//*******************************************************************
QBtDirCopyDialog::QBtDirCopyDialog( QWidget* const in_parent ) : QBtCopyDialog( in_parent )
{
    setWindowTitle( tr( CAPTION ) );

    worker_ = new QBtDirCopyWorker;
    worker_->moveToThread(&thread_);
    connect(&thread_, &QThread::finished, worker_, &QObject::deleteLater);
    connect(this, &QBtDirCopyDialog::copy, worker_, &QBtDirCopyWorker::copy);
    connect(this, &QBtDirCopyDialog::resume, worker_, &QBtDirCopyWorker::resume);
    connect(worker_, &QBtDirCopyWorker::ask_overwrite, this, &QBtDirCopyDialog::ask_overwrite);
    connect(worker_, &QBtDirCopyWorker::finished, this, &QBtDirCopyDialog::finish);
    connect(worker_, &QBtDirCopyWorker::progress, this, &QBtDirCopyDialog::progress_slot);
    connect(worker_, &QBtDirCopyWorker::item_info, this, &QBtDirCopyDialog::item_info_slot);
    thread_.start();

}
// end of QBtDirCopyDialog


QBtDirCopyDialog::~QBtDirCopyDialog()
{
    thread_.quit();
    thread_.wait();
}

void QBtDirCopyDialog::stop()
{
    worker_->stop();
}

//*******************************************************************
// start                                      PRIVATE inherited slot
//*******************************************************************
void QBtDirCopyDialog::start()
{
   destpath_ = dst_path_->text();
   QFileInfo destInfo( destpath_ );
   started();
   QStringList sources_list;
   sources_list.reserve(sources_.size());
   for(auto source : sources_) {
       sources_list << source;
   }
   emit copy(sources_list, destInfo.absoluteFilePath());
}
// end of start


void QBtDirCopyDialog::finish()
{
    finished();
}

void QBtDirCopyDialog::progress_slot(qint64 state)
{
    set_progress(state/1000000);
}

void QBtDirCopyDialog::item_info_slot(const QString& src, const QString& dst, const qint64 file_size)
{
    display_paths(src, dst);
    reset_progress(file_size/1000000);
}

void QBtDirCopyDialog::ask_overwrite(const QString &dst_path, QBtOverwriteAnswer::ErrorType error_type)
{
    auto answer = can_overwrite_->ask(dst_path, error_type);
    if(answer.action() == QBtOverwriteAnswer::CANCEL) {
        finished();
        return;
    }
    emit resume(answer);
}

//*******************************************************************
// can_update                                      PRIVATE inherited
//*******************************************************************
bool QBtDirCopyDialog::can_update( const QString& in_src_path, const QString& in_dst_path ) const
{
   const QFileInfo src_fi( in_src_path );
   const QFileInfo dst_fi( in_dst_path );
   return ( src_fi.lastModified() > dst_fi.lastModified() );
}
// end of can_update
