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
 * File         : QBtHelp.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 10.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtHelp.h"
#include "QBtShared.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QMessageBox>
#include <QShowEvent>
#include <QApplication>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtHelp::CAPTION            = QT_TR_NOOP( "Help"    );
const char* const QBtHelp::CANCEL             = QT_TR_NOOP( "&Cancel" );
const char* const QBtHelp::NOT_FOUND_CAPTION  = QT_TR_NOOP( "File not found" );
const char* const QBtHelp::NOT_FOUND_MESSAGE  = QT_TR_NOOP( "The help file not found:\n%1" );

//*******************************************************************
// QBtHelp                                               CONSTRUCTOR
//*******************************************************************
QBtHelp::QBtHelp( QWidget* const in_parent ) : QDialog( in_parent )
, browser_ ( new QTextBrowser )
, cancel_  ( new QPushButton( tr( CANCEL ) ) )
{
   setWindowTitle( tr( CAPTION ) );

   QHBoxLayout* const btn_layout = new QHBoxLayout;
   btn_layout->addStretch();
   btn_layout->addWidget( cancel_ );

   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addWidget( browser_ );
   main_layout->addLayout( btn_layout );
   setLayout( main_layout );

   connect( cancel_, SIGNAL( clicked() ), this, SLOT( accept() ) );
}
// end of QBtHelp

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtHelp::showEvent( QShowEvent* const in_event )
{
   QString fpath = QApplication::applicationDirPath();
   if( !fpath.endsWith( QDir::separator() ) ) fpath += QDir::separator();
   fpath += "help.en.html";

   QFile file( fpath );
   if( file.open( QIODevice::ReadOnly ) ) {
      QTextStream in( &file );
      browser_->insertHtml( in.readAll() );
      file.close();
   }
   else {
      QMessageBox::critical( this, tr( NOT_FOUND_CAPTION ), tr( NOT_FOUND_MESSAGE ).arg( fpath ) );
   }

   QBtShared::resize( this, 50, 50 );
   QDialog::showEvent( in_event );
}
// end of showEvent
