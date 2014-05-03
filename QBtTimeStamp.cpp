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
 * File         : QBtTimeStamp.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 2008.09.18
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtTimeStamp.h"
#include "QBtShared.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QFileInfo>
#include <QDateEdit>
#include <QTimeEdit>
#include <QProcess>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtTimeStamp::Caption       = QT_TR_NOOP( "Date/time of last file modification" );
const char* const QBtTimeStamp::DataCaption   = QT_TR_NOOP( "Date and time" );
const char* const QBtTimeStamp::MusterCaption = QT_TR_NOOP( "Proposition" );
const char* const QBtTimeStamp::Now           = QT_TR_NOOP( "&Now" );
const char* const QBtTimeStamp::Accept        = QT_TR_NOOP( "&Accept" );
const char* const QBtTimeStamp::Cancel        = QT_TR_NOOP( "&Cancel" );
const char* const QBtTimeStamp::UseMuster     = QT_TR_NOOP( "&Use timestamp of %1" );
const QString     QBtTimeStamp::Date          = "dd-MM-yyyy";
const QString     QBtTimeStamp::Time          = "hh:mm:ss";
const QString     QBtTimeStamp::DateTime      = Date + " " + Time;


//*******************************************************************
// QBtTimeStamp                                          CONSTRUCTOR
//*******************************************************************
QBtTimeStamp::QBtTimeStamp( QWidget* const in_parent,
                            const SelectionsSet& in_data,
                            const QString& in_muster_fpath )
: QDialog       ( in_parent )
, data_         ( in_data )
, muster_fi_    ( in_muster_fpath )
, now_btn_      ( new QPushButton( tr( Now    ) ) )
, accept_btn_   ( new QPushButton( tr( Accept ) ) )
, cancel_btn_   ( new QPushButton( tr( Cancel ) ) )
, muster_       ( new QLabel( muster_fi_.lastModified().toString( DateTime ) ) )
, use_muster_   ( new QCheckBox( tr( UseMuster ).arg( muster_fi_.fileName() ) ) )
, date_edit_    ( new QDateEdit( QFileInfo( *data_.begin() ).lastModified().date() ) )
, time_edit_    ( new QTimeEdit( QFileInfo( *data_.begin() ).lastModified().time() ) )
, date_bak_     ( QDate() )
, time_bak_     ( QTime() )
{
   setWindowTitle( tr( Caption ) );
   
   date_edit_->setDisplayFormat( Date );
   time_edit_->setDisplayFormat( Time );
   muster_->setAlignment( Qt::AlignHCenter );

   //------- buttons -------
   QVBoxLayout* const btn_layout = new QVBoxLayout;
   btn_layout->addStretch();
   btn_layout->addWidget( now_btn_ );
   btn_layout->addWidget( accept_btn_ );
   btn_layout->addWidget( cancel_btn_ );

   //------- edit -------
   QHBoxLayout* const edit_layout = new QHBoxLayout;
   edit_layout->addWidget( date_edit_ );
   edit_layout->addWidget( time_edit_ );
   QGroupBox* edit_box = new QGroupBox( tr( DataCaption ) );
   edit_box->setLayout( edit_layout );

   //------- muster -------
   QHBoxLayout* const muster_layout = new QHBoxLayout;
   muster_layout->addWidget( muster_ );
   QGroupBox* muster_box = new QGroupBox( tr( MusterCaption ) );
   muster_box->setLayout( muster_layout );

   //------- left -------
   QVBoxLayout* left_layout = new QVBoxLayout;
   left_layout->addWidget( edit_box );
   left_layout->addWidget( use_muster_ );
   left_layout->addWidget( muster_box );

   //------- main -------
   QHBoxLayout* const main_layout = new QHBoxLayout;
   main_layout->addLayout( left_layout );
   main_layout->addLayout( btn_layout );
   setLayout( main_layout );

   connect( use_muster_, SIGNAL( toggled(bool) ), this, SLOT( muster(bool) ) );
   connect( now_btn_   , SIGNAL( clicked()     ), this, SLOT( now   ()     ) );
   connect( accept_btn_, SIGNAL( clicked()     ), this, SLOT( touch ()     ) );
   connect( cancel_btn_, SIGNAL( clicked()     ), this, SLOT( reject()     ) );
}
// end of QBtTimeStamp

//*******************************************************************
// now                                                  PRIVATE slot
//*******************************************************************
void QBtTimeStamp::now()
{
   const QDateTime now_time = QDateTime::currentDateTime();
   date_edit_->setDate( now_time.date() );
   time_edit_->setTime( now_time.time() );
}
// end of now

//*******************************************************************
// muster                                               PRIVATE slot
//*******************************************************************
void QBtTimeStamp::muster( const bool in_checked )
{
   if( in_checked ) {
      date_bak_ = date_edit_->date();
      time_bak_ = time_edit_->time();
      now_btn_->setEnabled( false );
      date_edit_->setDate( muster_fi_.lastModified().date() );
      time_edit_->setTime( muster_fi_.lastModified().time() );
   }
   else {
      date_edit_->setDate( date_bak_ );
      time_edit_->setTime( time_bak_ );
      now_btn_->setEnabled( true );
   }
}
// end of muster

//*******************************************************************
// touch                                                PRIVATE slot
//*******************************************************************
void QBtTimeStamp::touch()
{
   static const QString Touch ="touch -c -t %1 %2";
   static const QString Format ="yyyyMMddhhmm.ss";
   
   const QString dt = QDateTime( date_edit_->date(), time_edit_->time() ).toString( Format );

   SelectionsSet::const_iterator it = data_.begin();
   const SelectionsSet::const_iterator end = data_.end();
   while( it != end ) {
      const QString cmd = Touch.arg( dt ).arg( QBtShared::quoted_fpath( *it ) );
      QProcess process;
      process.startDetached( cmd );
      ++it;
   }

   accept();
}
// end of touch
