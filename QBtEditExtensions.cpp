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
 * File         : QBtEditExtensions.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 2007.11.11
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtEditExtensions.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QtDebug>

/*------- local constants:
-------------------------------------------------------------------*/
const char* const QBtEditExtensions::CAPTION      = QT_TR_NOOP( "Extension editor" );
const char* const QBtEditExtensions::INFO_CAPTION = QT_TR_NOOP( "Additional information about call parameters" );
const char* const QBtEditExtensions::EXTENSION    = QT_TR_NOOP( "&Extension:" );
const char* const QBtEditExtensions::APPLICATION  = QT_TR_NOOP( "&Program path:" );
const char* const QBtEditExtensions::PARAMETERS   = QT_TR_NOOP( "&Call parameters:" );
const char* const QBtEditExtensions::ACCEPT       = QT_TR_NOOP( "&Accept" );
const char* const QBtEditExtensions::CANCEL       = QT_TR_NOOP( "&Cancel" );
const char* const QBtEditExtensions::HELP         = QT_TR_NOOP( 
   "<qt><i>When you type a call parameters,<br>"
   "you can use the following macros:</i><br><br>"
   "<b>$dir</b> - <i>directory of a selected file,</i><br>"
   "<b>$name</b> - <i>name of the selected file,</i><br>"
   "<b>$path</b> - <i>complete path of the selected file.</i></qt>" );
const char* const QBtEditExtensions::EXT_IS_EMPTY  = QT_TR_NOOP( "Extension input field can't be empty." );
const char* const QBtEditExtensions::PRG_IS_EMPTY  = QT_TR_NOOP( "Program input field can't be empty." );
const char* const QBtEditExtensions::PRG_NOT_FOUND = QT_TR_NOOP( "Program file not found.\n(%1)" );
const char* const QBtEditExtensions::PAR_IS_EMPTY  = QT_TR_NOOP( "Parameters input field can't be empty.\nAt last should be $path." );
const char* const QBtEditExtensions::IS_NOT_PRG    = QT_TR_NOOP( "Selected file is not executable.\n(%1)" );
const char* const QBtEditExtensions::SELECT        = QT_TR_NOOP( "Selected a program" );
const char* const QBtEditExtensions::FILTER        = QT_TR_NOOP( "All files (*)" );


//*******************************************************************
// QBtEditExtensions                                     CONSTRUCTOR
//*******************************************************************
QBtEditExtensions::QBtEditExtensions( QWidget* in_parent, const QString& in_ext, const QString& in_prg, const QString& in_par )
: QDialog( in_parent )
, ext_prompt_ ( new QLabel( tr( EXTENSION   ) ) )
, prg_prompt_ ( new QLabel( tr( APPLICATION ) ) )
, par_prompt_ ( new QLabel( tr( PARAMETERS  ) ) )
, ext_        ( new QLineEdit )
, prg_        ( new QLineEdit )
, par_        ( new QLineEdit )
, path_       ( new QPushButton( "..." ) )
, accept_     ( new QPushButton( tr( ACCEPT ) ) )
, cancel_     ( new QPushButton( tr( CANCEL ) ) )
, info_       ( new QGroupBox( tr( INFO_CAPTION ) ) )
, help_       ( new QLabel( tr( HELP ) ) )
{
   setWindowTitle( tr( CAPTION ) );
   ext_->setText( in_ext );
   prg_->setText( in_prg );
   par_->setText( in_par.isEmpty() ? "$path" : in_par );

   ext_prompt_->setBuddy( ext_ );
   prg_prompt_->setBuddy( prg_ );
   par_prompt_->setBuddy( par_ );

   const qint32 w = fontMetrics().width( "_..._" );
   path_->setMaximumWidth( w );
   path_->setMinimumWidth( w );

   QVBoxLayout* const browser_layout = new QVBoxLayout;
   browser_layout->addWidget( help_);
   info_->setLayout( browser_layout );
   
   QGridLayout* const grid = new QGridLayout;
   grid->addWidget( ext_prompt_, 0, 0 );
   grid->addWidget( prg_prompt_, 1, 0 );
   grid->addWidget( par_prompt_, 2, 0 );
   grid->addWidget( ext_       , 0, 1 );
   grid->addWidget( prg_       , 1, 1 );
   grid->addWidget( par_       , 2, 1 );
   grid->addWidget( path_      , 1, 2 );
   grid->addWidget( info_      , 3, 0, 1, 3 );
   
   QVBoxLayout* const btn_layout = new QVBoxLayout;
   btn_layout->addStretch( 100 );
   btn_layout->addWidget( accept_ );
   btn_layout->addWidget( cancel_ );
   
   QHBoxLayout* const main_layout = new QHBoxLayout;
   main_layout->addLayout( grid );
   main_layout->addLayout( btn_layout );
   setLayout( main_layout );

   connect( path_  , SIGNAL( clicked() ), this, SLOT( select() ) );
   connect( accept_, SIGNAL( clicked() ), this, SLOT( accept() ) );
   connect( cancel_, SIGNAL( clicked() ), this, SLOT( reject() ) );
}
// end of QBtEditExtensions

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtEditExtensions::showEvent( QShowEvent* const in_event )
{
   QDialog::showEvent( in_event );
   ext_->setFocus( Qt::OtherFocusReason );
}
// end of showEvent

//*******************************************************************
// select                                              PRIVATE slots
//-------------------------------------------------------------------
// Tutaj uzytkownik moze wskazac plik programu i jego lokalizacje
// uzywajac stosownego dialogu.
//*******************************************************************
void QBtEditExtensions::select()
{
   const QString fpath = QFileDialog::getOpenFileName( this, tr( SELECT ), QDir::homePath(), tr( FILTER ) );
   if( !fpath.isEmpty() ) {
      const QFileInfo fi( fpath );
      if( fi.isExecutable() ) prg_->setText( fpath );
      else QMessageBox::warning( this, tr( CAPTION ), tr( IS_NOT_PRG ).arg( fi.absoluteFilePath() ) );
   }
   prg_->setFocus( Qt::OtherFocusReason );
}
// end of select

//*******************************************************************
// accept                                              PRIVATE slots
//*******************************************************************
void QBtEditExtensions::accept()
{
   if( !is_ext_ok() ) {
      ext_->setFocus( Qt::OtherFocusReason );
      return;
   }
   if( !is_prg_ok() ) {
      prg_->setFocus( Qt::OtherFocusReason );
      return;
   }
   if( !is_par_ok() ) {
      par_->setFocus( Qt::OtherFocusReason );
      return;
   }
   QDialog::accept();
}
// end of accept

//*******************************************************************
// is_ext_ok                                                 PRIVATE
//*******************************************************************
bool QBtEditExtensions::is_ext_ok()
{
   if( ext_->text().trimmed().isEmpty() ) {
      QMessageBox::critical( this, tr( CAPTION ), tr( EXT_IS_EMPTY ) );
      return false;
   }
   return true;
}
// end of is_ext_ok

//*******************************************************************
// is_prg_ok                                                 PRIVATE
//*******************************************************************
bool QBtEditExtensions::is_prg_ok()
{
   if( prg_->text().trimmed().isEmpty() ) {
      QMessageBox::critical( this, tr( CAPTION ), tr( PRG_IS_EMPTY ) );
      return false;
   }

   const QFileInfo fi( prg_->text() );
   const QString fpath = fi.path();
   if( fpath != "." ) {    // podano sciezke - sprawdzamy czy istnieje
      if( !fi.exists() ) {
         QMessageBox::critical( this, tr( CAPTION ), tr( PRG_NOT_FOUND ).arg( fi.absoluteFilePath() ) );
         return false;
      }
   }

   return true;
}
// end of is_ext_ok

//*******************************************************************
// is_par_ok                                                 PRIVATE
//*******************************************************************
bool QBtEditExtensions::is_par_ok()
{
   if( par_->text().trimmed().isEmpty() ) {
      QMessageBox::critical( this, tr( CAPTION ), tr( PAR_IS_EMPTY ) );
      return false;
   }
   return true;
}
// end of is_par_ok

//*******************************************************************
// data                                                       PUBLIC
//*******************************************************************
void QBtEditExtensions::data( QString& out_ext, QString& out_prg, QString& out_par )
{
   out_ext = ext_->text().trimmed();
   out_prg = prg_->text().trimmed();
   out_par = par_->text().trimmed();
}
// end of data
