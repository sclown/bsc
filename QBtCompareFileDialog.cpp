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
#include "QBtCompareFileDialog.h"
#include "QBtShared.h"
#include "QBtInfoField.h"
#include "QBtFileComparator.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QMessageBox>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtCompareFileDialog::CAPTION  = QT_TR_NOOP( "Files comparing" );
const char* const QBtCompareFileDialog::FILES    = QT_TR_NOOP( "Files" );
const char* const QBtCompareFileDialog::PROGRESS = QT_TR_NOOP( "Progress" );
const char* const QBtCompareFileDialog::FILE_NR1 = QT_TR_NOOP( "First file:" );
const char* const QBtCompareFileDialog::FILE_NR2 = QT_TR_NOOP( "Second file:" );
const char* const QBtCompareFileDialog::CANCEL   = QT_TR_NOOP( "&Cancel" );
const char* const QBtCompareFileDialog::START    = QT_TR_NOOP( "&Start" );
const char* const QBtCompareFileDialog::BREAK    = QT_TR_NOOP( "&Break" );
const char* const QBtCompareFileDialog::EQUAL    = QT_TR_NOOP( "Files are identical." );
const char* const QBtCompareFileDialog::NOT_EQUAL= QT_TR_NOOP( "Files are not identical.\n"
                                                               "Would you like to see the diferences?" );
const char* const QBtCompareFileDialog::IO_ERROR = QT_TR_NOOP( "File reading error" );


//*******************************************************************
// QBtCompareFileDialog                                  CONSTRUCTOR
//*******************************************************************
QBtCompareFileDialog::QBtCompareFileDialog( QWidget* const in_parent,
                                            const QString& in_fpath1,
                                            const QString& in_fpath2 )
: QDialog      ( in_parent )
, fpath1_      ( in_fpath1 )
, fpath2_      ( in_fpath2 )
, ifield1_     ( new QBtInfoField )
, ifield2_     ( new QBtInfoField )
, progress_bar_( new QProgressBar )
, cancel_      ( new QPushButton( tr( CANCEL ) ) )
, start_       ( new QPushButton( tr( START ) ) )
, comparator_  ( new QBtFileComparator( this ) )
, is_started_  ( false )
, is_canceled_ ( false )
{
   setWindowTitle( tr( CAPTION ) );

   QGroupBox* const files_gbox = new QGroupBox( tr( FILES ) );
   QGridLayout* files_layout = new QGridLayout( files_gbox );
   QLabel* const prompt1 = new QLabel( tr( FILE_NR1 ) );
   QLabel* const prompt2 = new QLabel( tr( FILE_NR2 ) );
   prompt1->setAlignment( Qt::AlignRight );
   prompt2->setAlignment( Qt::AlignRight );
   files_layout->addWidget( prompt1 , 0, 0 );
   files_layout->addWidget( prompt2 , 1, 0 );
   files_layout->addWidget( ifield1_, 0, 1 );
   files_layout->addWidget( ifield2_, 1, 1 );
   
   QGroupBox* const progress_gbox = new QGroupBox( tr( PROGRESS ) );
   QHBoxLayout* const progress_layout = new QHBoxLayout( progress_gbox );
   progress_layout->addWidget( progress_bar_ );

   QHBoxLayout* const btn_layout = new QHBoxLayout;
   btn_layout->addStretch( 100 );
   btn_layout->addWidget( cancel_ );
   btn_layout->addWidget( start_ );

   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addWidget( files_gbox );
   main_layout->addWidget( progress_gbox );
   main_layout->addLayout( btn_layout );
   setLayout( main_layout );

   connect( cancel_    , SIGNAL( clicked() ),
            this       , SLOT  ( cancel() ) );
   connect( start_     , SIGNAL( clicked() ),
            this       , SLOT  ( start() ) );
   connect( comparator_, SIGNAL( init_progress( qint64 ) ),
            this       , SLOT  ( init_progress( qint64 ) ) );
   connect( comparator_, SIGNAL( progress( qint64 ) ),
            this       , SLOT  ( progress( qint64 ) ) );
}
// end of QBtCompareFileDialog

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtCompareFileDialog::showEvent( QShowEvent* const in_event )
{
   QBtShared::resize_width( this, 40 );
   QDialog::showEvent( in_event );
   
   const qint32       MAXLEN = ifield1_->width();
   const QFontMetrics fm     = ifield1_->fontMetrics();

   QString src_path = fpath1_;
   if( fm.width( src_path ) > MAXLEN ) {
      QBtShared::elide( fm, MAXLEN, src_path );
   }
   ifield1_->setText( src_path );
   
   QString dst_path = fpath2_;
   if( fm.width( dst_path ) > MAXLEN ) {
      QBtShared::elide( fm, MAXLEN, dst_path );
   }
   ifield2_->setText( dst_path );
}
// end of showEvent

//*******************************************************************
// cancel                                               PRIVATE slot
//*******************************************************************
void QBtCompareFileDialog::cancel()
{
   if( is_started_ ) {
      is_canceled_ = true;
      comparator_->stop();
      QBtShared::idle();
   }
   else {
      reject();
   }
}
// end of cancel

//*******************************************************************
// start                                                PRIVATE slot
//*******************************************************************
void QBtCompareFileDialog::start()
{
   cancel_->setText( tr( BREAK ) );
   start_->setVisible( false );
   is_started_ = true;

   const qint32 retval = comparator_->compare( fpath1_, fpath2_ );
   switch( retval ) {
      case QBtFileComparator::EQUAL:
         QMessageBox::information( this, tr( CAPTION ), tr( EQUAL ) );
         reject();
         break;
      case QBtFileComparator::NOT_EQUAL:
         if( is_canceled_ ) reject();
         check_files() ? accept() : reject();
         break;
      case QBtFileComparator::IO_ERROR:
         QMessageBox::critical( this, tr( CAPTION ), tr( IO_ERROR ) );
         reject();
         break;
   }
}
// end of start

//*******************************************************************
// check_files                                               PRIVATE
//*******************************************************************
bool QBtCompareFileDialog::check_files()
{
   const QMessageBox::StandardButton retval =
      QMessageBox::question( this, tr( CAPTION ), tr( NOT_EQUAL ),
                             QMessageBox::No | QMessageBox::Yes );
   return ( QMessageBox::Yes == retval );
}
// end of check_files

//*******************************************************************
// init_progress                                        PRIVATE slot
//*******************************************************************
void QBtCompareFileDialog::init_progress( const qint64 in_range )
{
   progress_bar_->setRange( 0, in_range );
   QBtShared::idle();
}
// end of init_progress

//*******************************************************************
// progress                                             PRIVATE slot
//*******************************************************************
void QBtCompareFileDialog::progress( const qint64 in_value )
{
   progress_bar_->setValue( in_value );
   QBtShared::idle();
}
// end of init_progress
   
