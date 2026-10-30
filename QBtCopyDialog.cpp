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
#include "QBtCopyDialog.h"
#include "QBtCanOverwrite.h"
#include "QBtInfoField.h"
#include "QBtShared.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QCompleter>
#include <QCoreApplication>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtCopyDialog::CAPTION          = QT_TR_NOOP( "Copy processing" );
const char* const QBtCopyDialog::MOVE_CAPTION     = QT_TR_NOOP( "Move processing" );
const char* const QBtCopyDialog::SRC_DST_CAPTION  = QT_TR_NOOP( "Source and destination" );
const char* const QBtCopyDialog::PROGRESS_CAPTION = QT_TR_NOOP( "Progress" );
const char* const QBtCopyDialog::START            = QT_TR_NOOP( "&Start" );
const char* const QBtCopyDialog::EXIT             = QT_TR_NOOP( "&Exit" );
const char* const QBtCopyDialog::BREAK            = QT_TR_NOOP( "&Break" );
const char* const QBtCopyDialog::SRC_LABEL        = QT_TR_NOOP( "Source:" );
const char* const QBtCopyDialog::DST_LABEL        = QT_TR_NOOP( "Destination:" );
const char* const QBtCopyDialog::SRC_ITEMS_COUNT_TEXT = QT_TR_NOOP( "%1 items" );
const int         QBtCopyDialog::REFRESH_MS       = 50;
const int         QBtCopyDialog::PROGRESS_STEPS   = 1000;


//*******************************************************************
// QBtCopyDialog                                         CONSTRUCTOR
//*******************************************************************
QBtCopyDialog::QBtCopyDialog( QWidget* const in_parent ) : QDialog( in_parent )
, progress_       ( new QProgressBar )
, start_          ( new QPushButton( tr( START ) ) )
, exit_           ( new QPushButton( tr( EXIT  ) ) )
, src_path_       ( new QBtInfoField )
, dst_path_       ( new QLineEdit )
, can_overwrite_  ( new QBtCanOverwrite( this ) )
, started_        ( false )
, sources_        ()
, destpath_       ()
, command_        ( QBtOverwriteAnswer::ASK )
, mode_           ( QBtCopyWorker::Mode::COPY )
{
   setWindowTitle( tr( CAPTION ) );

   QGroupBox* const src_dst_gbox = new QGroupBox( tr( SRC_DST_CAPTION ) );
   QGridLayout* const grid = new QGridLayout( src_dst_gbox );

   QCompleter* const completer = new QCompleter( this );
   QFileSystemModel* const completer_model = new QFileSystemModel( completer );
   completer_model->setRootPath( QString() );
   completer->setModel( completer_model );
   dst_path_->setCompleter( completer );

   QLabel* const src_label = new QLabel( tr( SRC_LABEL ) );
   QLabel* const dst_label = new QLabel( tr( DST_LABEL ) );
   src_label->setBuddy( src_path_ );
   dst_label->setBuddy( dst_path_ );
   grid->addWidget( src_label, 0, 0, 1, 1, Qt::AlignRight );
   grid->addWidget( src_path_, 0, 1 );
   grid->addWidget( dst_label, 1, 0, 1, 1, Qt::AlignRight );
   grid->addWidget( dst_path_, 1, 1 );

   // Progress
   QGroupBox* const progress_gbox = new QGroupBox( tr( PROGRESS_CAPTION ) );
   QHBoxLayout* const progress_layout = new QHBoxLayout( progress_gbox );
   progress_->setAlignment( Qt::AlignHCenter );
   progress_->setRange( 0, PROGRESS_STEPS );
   progress_->setValue( 0 );
   progress_layout->addWidget( progress_ );

   // Buttons
   QHBoxLayout* const in_layout = new QHBoxLayout;
   in_layout->addWidget( start_ );
   in_layout->addWidget( exit_ );

   // Main layout
   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addWidget( src_dst_gbox );
   main_layout->addWidget( progress_gbox );
   main_layout->addLayout( in_layout );
   setLayout( main_layout );

   src_path_->setFocusPolicy( Qt::StrongFocus );
   exit_->setFocusPolicy( Qt::StrongFocus );
   start_->setFocusPolicy( Qt::StrongFocus );
   setTabOrder( src_path_, dst_path_ );
   setTabOrder( dst_path_, start_ );
   setTabOrder( start_, exit_ );
   dst_path_->setFocus();

   connect( exit_ , &QPushButton::clicked, this, &QBtCopyDialog::reject );
   connect( start_, &QPushButton::clicked, this, &QBtCopyDialog::start  );

   // The worker reports every item; a tree of tens of thousands of
   // small files would flood the GUI thread if each report repainted.
   // The slots only record the latest state and refresh_ paints it.
   refresh_.setInterval( REFRESH_MS );
   connect( &refresh_, &QTimer::timeout, this, &QBtCopyDialog::refresh );

   connect( &worker_, &QBtCopyWorker::progress, this, &QBtCopyDialog::progress );
   connect( &watcher_, &QFutureWatcher<void>::finished, this, &QBtCopyDialog::finish );
}
// end of QBtCopyDialog

//*******************************************************************
// ~QBtCopyDialog                                         DESTRUCTOR
//-------------------------------------------------------------------
// The worker may be blocked on a question for this thread, so a
// plain waitForFinished could deadlock. Keep delivering events until
// it has unwound; a pending question sees the cancel and returns.
//*******************************************************************
QBtCopyDialog::~QBtCopyDialog()
{
   watcher_.cancel();
   while( watcher_.isRunning() ) {
      QCoreApplication::processEvents( QEventLoop::ExcludeUserInputEvents | QEventLoop::WaitForMoreEvents );
   }
}
// end of ~QBtCopyDialog

//*******************************************************************
// execute                                                    PUBLIC
//*******************************************************************
int QBtCopyDialog::execute( const QBtCopyWorker::Mode mode, const SelectionsSet& sources,
                            const QString& destination, const QBtOverwriteAnswer::Action command )
{
   mode_     = mode;
   sources_  = sources;
   destpath_ = destination;
   command_  = command;
   setWindowTitle( tr( QBtCopyWorker::Mode::MOVE == mode ? MOVE_CAPTION : CAPTION ) );
   return exec();
}
// end of execute

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtCopyDialog::showEvent( QShowEvent* const in_event )
{
   {
      QPalette p = QPalette();
      p.setColor( src_path_->backgroundRole(), palette().color( backgroundRole() ) );
      p.setColor( src_path_->foregroundRole(), palette().color( foregroundRole() ) );
      src_path_->setPalette( p );
   }
   {
      QPalette p = QPalette();
      p.setColor( dst_path_->backgroundRole(), palette().color( backgroundRole() ) );
      p.setColor( dst_path_->foregroundRole(), palette().color( foregroundRole() ) );
      dst_path_->setPalette( p );
   }
   if( sources_.size() == 1 ) {
      const QFileInfo src( *sources_.begin() );
      if( !src.isDir() ) {
         destpath_ += QDir::separator() + src.fileName();
      }
   }
   display_paths( sourceInitialText(), destpath_ );
   QBtShared::resize_width( this, 40 );
   start_->setDefault( true );
   QDialog::showEvent( in_event );
   if( QBtOverwriteAnswer::AUTORENAME == command_ ) {
      start();
   }
}
// end of showEvent

//*******************************************************************
// sourceInitialText                                         PRIVATE
//*******************************************************************
QString QBtCopyDialog::sourceInitialText() const
{
   if( sources_.size() == 1 ) {
      return *sources_.begin();
   }
   return tr( SRC_ITEMS_COUNT_TEXT ).arg( sources_.size() );
}
// end of sourceInitialText

//*******************************************************************
// display_paths                                             PRIVATE
//*******************************************************************
void QBtCopyDialog::display_paths( const QString& in_src_path, const QString& in_dst_path )
{
   src_path_->setText( in_src_path );
   dst_path_->setText( in_dst_path );
}
// end of display_paths

//*******************************************************************
// start                                                PRIVATE slot
//*******************************************************************
void QBtCopyDialog::start()
{
   if( started_ ) return;
   started_ = true;
   start_->setVisible( false );
   exit_->setText( tr( BREAK ) );
   destpath_ = dst_path_->text();
   refresh_.start();

   std::optional<QBtOverwriteAnswer> sticky;
   if( QBtOverwriteAnswer::AUTORENAME == command_ ) {
      sticky = QBtOverwriteAnswer( true, QBtOverwriteAnswer::AUTORENAME, QString() );
   }

   watcher_.setFuture( worker_.start(
      mode_,
      QStringList( sources_.begin(), sources_.end() ),
      QFileInfo( destpath_ ).absoluteFilePath(),
      sticky,
      [this]( const QString& path, const QBtOverwriteAnswer::ErrorType type ) {
         return ask_overwrite( path, type );
      } ) );
}
// end of start

//*******************************************************************
// reject                                      PRIVATE inherited slot
//-------------------------------------------------------------------
// Before Start this closes the dialog. Once running it breaks the
// copy; finish() then closes the dialog when the worker has stopped.
//*******************************************************************
void QBtCopyDialog::reject()
{
   if( started_ ) {
      watcher_.cancel();
   }
   else {
      QDialog::reject();
   }
}
// end of reject

//*******************************************************************
// ask_overwrite                                             PRIVATE
//-------------------------------------------------------------------
// Runs on the worker thread. The modal question is shown on this
// dialog's thread and the worker waits for the answer.
//*******************************************************************
QBtOverwriteAnswer QBtCopyDialog::ask_overwrite( const QString& path, const QBtOverwriteAnswer::ErrorType type )
{
   QBtOverwriteAnswer answer( false, QBtOverwriteAnswer::CANCEL, QString() );
   QMetaObject::invokeMethod( this, [&]() {
      if( !watcher_.isCanceled() ) {
         answer = can_overwrite_->ask( path, type );
      }
   }, Qt::BlockingQueuedConnection );
   return answer;
}
// end of ask_overwrite

//*******************************************************************
// finish                                               PRIVATE slot
//-------------------------------------------------------------------
// Items may have been copied even when the run was broken, so the
// caller always gets Accepted and refreshes both views.
//*******************************************************************
void QBtCopyDialog::finish()
{
   refresh_.stop();
   refresh();
   accept();
}
// end of finish

//*******************************************************************
// progress                                             PRIVATE slot
//*******************************************************************
void QBtCopyDialog::progress( const QBtCopyProgress& in_progress )
{
   status_update_ = in_progress;
}
// end of progress

//*******************************************************************
// refresh                                              PRIVATE slot
//*******************************************************************
void QBtCopyDialog::refresh()
{
   if( !status_update_ ) return;
   display_paths( status_update_->source, status_update_->destination );
   progress_->setValue( status_update_->size > 0
                        ? static_cast<int>( status_update_->done * PROGRESS_STEPS / status_update_->size )
                        : PROGRESS_STEPS );
   status_update_.reset();
}
// end of refresh
