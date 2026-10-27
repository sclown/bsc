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
 * File         : QBtDeleter.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 04.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtDeleter.h"
#include "QBtInfoField.h"
#include "QBtShared.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QFrame>
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QTimer>
#include <QPromise>
#include <QtConcurrent>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtDeleter::CAPTION    = QT_TR_NOOP( "Delete a file" );
const char* const QBtDeleter::RUN        = QT_TR_NOOP( "&Run" );
const char* const QBtDeleter::CANCEL     = QT_TR_NOOP( "&Cancel" );
const char* const QBtDeleter::BREAK      = QT_TR_NOOP( "&Break" );
const char* const QBtDeleter::CANT_TRASH = QT_TR_NOOP( "Can't move to trash:\n%1" );

namespace {
// Worker: trashes every item in turn, reporting the ones that failed
// as results. Stops at the next item once cancelled.
void trash_all( QPromise<QString>& promise, const QStringList items )
{
   int n = 0;
   for( const QString& path : items ) {
      if( promise.isCanceled() ) return;
      promise.setProgressValueAndText( ++n, path );
      if( !QFile::moveToTrash( path ) ) promise.addResult( path );
   }
}
}

//*******************************************************************
// QBtDeleter                                            CONSTRUCTOR
//*******************************************************************
QBtDeleter::QBtDeleter( QWidget* const in_parent ) : QDialog( in_parent )
, info_    ( new QBtInfoField )
, line_    ( new QFrame )
, run_     ( new QPushButton( tr( RUN    ) ) )
, cancel_  ( new QPushButton( tr( CANCEL ) ) )
, running_ ( false )
{
   setWindowTitle( tr( CAPTION ) );

   line_->setFrameShape( QFrame::HLine );
   line_->setFrameShadow( QFrame::Sunken );
   
   QHBoxLayout* const btn_layout = new QHBoxLayout;
   btn_layout->addStretch( 100 );
   btn_layout->addWidget( cancel_ );
   btn_layout->addWidget( run_ );

   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addWidget( info_ );
   main_layout->addWidget( line_ );
   main_layout->addLayout( btn_layout );
   setLayout( main_layout );

   connect( run_   , &QPushButton::clicked, this, &QBtDeleter::run    );
   connect( cancel_, &QPushButton::clicked, this, &QBtDeleter::cancel );
   connect( &watcher_, &QFutureWatcher<QString>::progressTextChanged, this, &QBtDeleter::progress     );
   connect( &watcher_, &QFutureWatcher<QString>::resultReadyAt      , this, &QBtDeleter::result_ready );
   connect( &watcher_, &QFutureWatcher<QString>::finished           , this, &QBtDeleter::finished     );
}
// end of QBtDeleter

//*******************************************************************
// ~QBtDeleter                                            DESTRUCTOR
//*******************************************************************
QBtDeleter::~QBtDeleter()
{
   watcher_.cancel();
   watcher_.waitForFinished();
}
// end of ~QBtDeleter

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtDeleter::showEvent( QShowEvent* const in_event )
{
   QPalette p = QPalette();
   p.setColor( info_->backgroundRole(), palette().color( backgroundRole() ) );
   p.setColor( info_->foregroundRole(), palette().color( foregroundRole() ) );
   info_->setPalette( p );

   QBtShared::resize_width( this, 40 );
   QDialog::showEvent( in_event );
   run_->setDefault( true );
   QTimer::singleShot( 100, this, &QBtDeleter::run );
}
// end of showEvent

//*******************************************************************
// keyPressEvent                                   PRIVATE inherited
//-------------------------------------------------------------------
// Esc breaks a running operation instead of closing the dialog.
//*******************************************************************
void QBtDeleter::keyPressEvent( QKeyEvent* in_event )
{
   if( Qt::Key_Escape == in_event->key() ) {
      cancel();
   }
   else {
      QDialog::keyPressEvent( in_event );
   }
}
// end of keyPressEvent

//*******************************************************************
// cancel                                               PRIVATE slot
//*******************************************************************
void QBtDeleter::cancel()
{
   if( running_ ) {
      watcher_.cancel();
   }
   else {
      reject();
   }
}
// end of cancel

//*******************************************************************
// set_data                                                   PUBLIC
//*******************************************************************
void QBtDeleter::set_data( const SelectionsSet& in_data )
{
   data_ = in_data;
   if( !data_.empty() ) {
      info_->setText( *data_.begin() );
   }
}
// end of set_data

//*******************************************************************
// run                                                  PRIVATE slot
//*******************************************************************
void QBtDeleter::run()
{
   if( running_ ) return;
   if( data_.empty() ) {
      accept();
      return;
   }

   run_->setVisible( false );
   cancel_->setText( tr( BREAK ) );
   running_ = true;
   failed_.clear();

   QApplication::setOverrideCursor( Qt::BusyCursor );
   watcher_.setFuture( QtConcurrent::run( trash_all, QStringList( data_.begin(), data_.end() ) ) );
}
// end of run

//*******************************************************************
// progress                                             PRIVATE slot
//*******************************************************************
void QBtDeleter::progress( const QString& in_path )
{
   QString info = in_path;
   QBtShared::elide( info_->fontMetrics(), info_->width(), info );
   info_->setText( info );
}
// end of progress

//*******************************************************************
// result_ready                                         PRIVATE slot
//*******************************************************************
void QBtDeleter::result_ready( const int in_index )
{
   failed_ << watcher_.resultAt( in_index );
}
// end of result_ready

//*******************************************************************
// finished                                             PRIVATE slot
//-------------------------------------------------------------------
// Items may have been trashed even when the run was broken or some
// failed, so the caller always gets Accepted and refreshes the view.
//*******************************************************************
void QBtDeleter::finished()
{
   running_ = false;
   QApplication::restoreOverrideCursor();
   if( !failed_.isEmpty() ) {
      QMessageBox::warning( this, tr( CAPTION ), tr( CANT_TRASH ).arg( failed_.join( '\n' ) ) );
   }
   accept();
}
// end of finished
