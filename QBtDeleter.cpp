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
#include "QBtDirParser.h"
#include "QBtWiper.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QFrame>
#include <QMessageBox>
#include <QApplication>
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtDeleter::CAPTION       = QT_TR_NOOP( "Delete a file" );
const char* const QBtDeleter::RUN           = QT_TR_NOOP( "&Run" );
const char* const QBtDeleter::CANCEL        = QT_TR_NOOP( "&Cancel" );
const char* const QBtDeleter::BREAK         = QT_TR_NOOP( "&Break" );
const char* const QBtDeleter::DIR_ERROR     = QT_TR_NOOP( "You don't have rights to this directory (%1).\nCheck permission access." );
const char* const QBtDeleter::CANT_DEL_FILE = QT_TR_NOOP( "Can't remove a file:\n%1" );
const char* const QBtDeleter::CANT_DEL_DIR  = QT_TR_NOOP( "Can't remove a directory:\n%1" );

//*******************************************************************
// QBtDeleter                                            CONSTRUCTOR
//*******************************************************************
QBtDeleter::QBtDeleter( QWidget* const in_parent ) : QDialog( in_parent )
, info_   ( new QBtInfoField )
, line_   ( new QFrame )
, run_    ( new QPushButton( tr( RUN    ) ) )
, cancel_ ( new QPushButton( tr( CANCEL ) ) )
, parser_ ( new QBtDirParser( true, this ) )
, wiper_  ( 0 )
, break_  ( false )
, runned_ ( false )
, wipe_   ( false )
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

   connect( run_   , SIGNAL( clicked          ()                 ),
            this   , SLOT  ( run              ()                 ) );
   connect( cancel_, SIGNAL( clicked          ()                 ),
            this   , SLOT  ( cancel           ()                 ) );
   connect( parser_, SIGNAL( current_item     ( const QString& ) ),
            this   , SLOT  ( current_item     ( const QString& ) ) );
   connect( parser_, SIGNAL( finished         ()                 ),
            this   , SLOT  ( finished         ()                 ) );
   connect( parser_, SIGNAL( cant_read_dir    ( const QString& ) ),
            this   , SLOT  ( read_dir_error   ( const QString& ) ) );
   connect( parser_, SIGNAL( cant_remove_dir  ( const QString& ) ),
            this   , SLOT  ( remove_dir_error ( const QString& ) ) );
   connect( parser_, SIGNAL( cant_remove_file ( const QString& ) ),
            this   , SLOT  ( remove_file_error( const QString& ) ) );
}
// end of QBtDeleter

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
}
// end of showEvent

//*******************************************************************
// keyPressEvent                                   PRIVATE inherited
//-------------------------------------------------------------------
// Wprowadzamy wlasna obsluge klawisza Esc.
//*******************************************************************
void QBtDeleter::keyPressEvent( QKeyEvent* in_event )
{
   if( Qt::Key_Escape != in_event->key() ) {
      break_ = true;
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
   if( !runned_ ) {
      reject();
   }
   else {
      break_ = true;
      parser_->cancel();
   }
}
// end of cancel

//*******************************************************************
// set_data                                                   PUBLIC
//*******************************************************************
void QBtDeleter::set_data( const SelectionsSet& in_data, const bool in_wipe )
{
   data_ = in_data;
   wipe_ = in_wipe;

   if( !data_.empty() ) {
      info_->setText( *data_.begin() );
   }

   if( wipe_ ) {
      if( wiper_ ) delete wiper_;
      wiper_ = new QBtWiper( this );
      connect( wiper_, SIGNAL( wipe_progress( const QString& ) ),
               this  , SLOT  ( progress     ( const QString& ) ) );
   }
}
// end of set_data

//*******************************************************************
// run                                                  PRIVATE slot
//*******************************************************************
void QBtDeleter::run()
{
   run_->setVisible( false );
   cancel_->setText( tr( BREAK ) );
   
   if( data_.empty() ) accept();

   QApplication::setOverrideCursor( Qt::WaitCursor );
   it_  = data_.begin();
   end_ = data_.end();
   runned_ = true;
   parser_->run( *it_ );
}
// end of run

//*******************************************************************
// finished                                             PRIVATE slot
//-------------------------------------------------------------------
// Funkcja wywolywana jest gdy 'obrobiono' juz caly katalog
// (wraz z podkatalogami i plikami).
//*******************************************************************
void QBtDeleter::finished()
{
   ++it_;
   if( !break_ && ( it_ != end_ ) ) {
      parser_->run( *it_ );
   }
   delete wiper_;
   QApplication::restoreOverrideCursor();
   accept();
}
// end of finished

//*******************************************************************
// current_item                                         PRIVATE slot
//*******************************************************************
void QBtDeleter::current_item ( const QString& in_path )
{
   if( wipe_ ) {
      wiper_->wipe( in_path );
   }
   else {
      progress( in_path );
   }
}
// end of current_item

//******************************************************************
// progress                                            PRIVATE slot
//******************************************************************
void QBtDeleter::progress( const QString& in_info )
{
   QString info = in_info;
   QBtShared::elide( info_->fontMetrics(), info_->width(), info );
   info_->setText( info );  
   QBtShared::idle();
}
// end of progress

//*******************************************************************
// read_dir_error                                       PRIVATE slot
//*******************************************************************
void QBtDeleter::read_dir_error( const QString& in_path)
{
   QMessageBox::critical( this, tr( CAPTION ), tr( DIR_ERROR ).arg( in_path ) );
}
// end of read_dir_error

//*******************************************************************
// remove_dir_error                                     PRIVATE slot
//*******************************************************************
void QBtDeleter::remove_dir_error( const QString& in_path )
{
   QMessageBox::critical( this, tr( CAPTION ), tr( CANT_DEL_DIR ).arg( in_path ) );
}
// end of remove_dir_error

//*******************************************************************
// remove_file_error                                    PRIVATE slot
//*******************************************************************
void QBtDeleter::remove_file_error( const QString& in_path )
{
   QMessageBox::critical( this, tr( CAPTION ), tr( CANT_DEL_FILE ).arg( in_path ) );
}
// end of remove_file_error
