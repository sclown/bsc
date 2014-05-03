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
 * File         : QBtFileEditor.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 11.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtFileEditor.h"
#include "QBtShared.h"
#include "QBtSystemCall.h"
#include <QFile>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>


/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtFileEditor::CAPTION     = QT_TR_NOOP( "Text editor"                    );
const char* const QBtFileEditor::RELOAD      = QT_TR_NOOP( "&Reload"                        );
const char* const QBtFileEditor::SAVE        = QT_TR_NOOP( "&Save"                          );
const char* const QBtFileEditor::CANCEL      = QT_TR_NOOP( "&Cancel"                        );
const char* const QBtFileEditor::NOTEXT_FILE = QT_TR_NOOP( "This is not the text file:\n%1" );
const char* const QBtFileEditor::READ_ERROR  = QT_TR_NOOP( "Can't read the file:\n%1\n(%2)" );
const char* const QBtFileEditor::SAVE_ERROR  = QT_TR_NOOP( "Can't save the file:\n%1\n(%2)" );


//*******************************************************************
// QBtFileEditor                                         CONSTRUCTOR
//*******************************************************************
QBtFileEditor::QBtFileEditor( QWidget* const in_parent, const QString& in_path ) : QDialog( in_parent )
, path_    ( QString() )
, editor_  ( new QTextEdit )
, reload_  ( new QPushButton( tr( RELOAD ) ) )
, save_    ( new QPushButton( tr( SAVE   ) ) )
, cancel_  ( new QPushButton( tr( CANCEL ) ) )
{
   setWindowTitle( tr( CAPTION ) );

   path_ = QFile::symLinkTarget( in_path );
   if( path_.isEmpty() ) path_ = in_path;
   
   static const QString my_font = "Monospace,9,-1,5,50,0,0,0,0,0";
   QFont fnt;
   fnt.fromString( my_font );
   editor_->setFont( fnt );   

   QHBoxLayout* const btn_layout = new QHBoxLayout;
   btn_layout->addStretch();
   btn_layout->addWidget( reload_ );
   btn_layout->addWidget( save_   );
   btn_layout->addWidget( cancel_ );

   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addWidget( editor_ );
   main_layout->addLayout( btn_layout );
   setLayout( main_layout );

   connect( reload_, SIGNAL( clicked() ), this, SLOT( reload_file() ) );
   connect( save_  , SIGNAL( clicked() ), this, SLOT( save_file  () ) );
   connect( cancel_, SIGNAL( clicked() ), this, SLOT( accept     () ) );

   cancel_->setDefault( true );
}
// end of QBtFileEditor

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtFileEditor::showEvent( QShowEvent* const in_event )
{
   load_file();
   
   QBtShared::resize( this, 60, 50 );
   QDialog::showEvent( in_event );
}
// end of showEvent

//*******************************************************************
// load_file                                                 PRIVATE
//*******************************************************************
void QBtFileEditor::load_file()
{
   static const QString cmd = "file -b -i \"%1\"";

   QApplication::setOverrideCursor( Qt::WaitCursor );      
   QBtSystemCall syscall;
   syscall.run( cmd.arg( path_ ) );
   const QString marker = syscall.result();

   const bool is_text = marker.startsWith( "text" ) ||
                        marker.contains( "script" ) ||
                        marker.contains( "perl" );
   
   if( is_text ) reload_file();
   QApplication::restoreOverrideCursor();

   if( !is_text ) {
      QMessageBox::information( this, tr( CAPTION ), tr( NOTEXT_FILE ).arg( path_ ) );
   }
}
// end of load_file

//*******************************************************************
// reload_file                                               PRIVATE
//*******************************************************************
void QBtFileEditor::reload_file()
{
   editor_->clear();
   
   QFile file( path_ );
   if( file.open( QIODevice::ReadOnly ) ) {
      QTextStream in( &file );
      editor_->insertPlainText( in.readAll() );
      file.close();
   }
   else {
      QMessageBox::critical( this,
         tr( CAPTION ),
         tr( READ_ERROR ).arg( path_ ).arg( file.errorString() ) );
   }
   
   QTextCursor cursor = editor_->textCursor();
   cursor.setPosition( 0 );
   editor_->setTextCursor( cursor );
   editor_->setFocus( Qt::OtherFocusReason );
}
// end of reload_file

//*******************************************************************
// save_file                                                 PRIVATE
//*******************************************************************
void QBtFileEditor::save_file()
{
   QFile file( path_ );

   if( file.open( QIODevice::WriteOnly ) ) {
      QTextStream out( &file );
      QApplication::setOverrideCursor( Qt::WaitCursor );
      out << editor_->toPlainText();
      QApplication::restoreOverrideCursor();
      file.close();
   }
   else {
      QMessageBox::critical( this,
         tr( CAPTION ),
         tr( SAVE_ERROR ).arg( path_ ).arg( file.errorString() ) );
   }
   
   editor_->setFocus( Qt::OtherFocusReason );
}
// end of save_file
