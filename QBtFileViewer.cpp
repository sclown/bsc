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
 * File         : QBtFileViewer.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 10.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtFileViewer.h"
#include "QBtShared.h"
#include "QBtSystemCall.h"
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextBrowser>
#include <QApplication>
#include <QKeyEvent>
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtFileViewer::CAPTION            = QT_TR_NOOP( "File viewer"    );
const char* const QBtFileViewer::CANCEL             = QT_TR_NOOP( "&Cancel" );
const char* const QBtFileViewer::SEARCH             = QT_TR_NOOP( "&Search" );


//*******************************************************************
// QBtFileViewer                                         CONSTRUCTOR
//*******************************************************************
QBtFileViewer::QBtFileViewer( QWidget* const in_parent, const QString& in_path ) : QDialog( in_parent )
, path_    ( in_path )
, browser_ ( new QTextBrowser )
, search_edit_ ( new QLineEdit )
, search_  ( new QPushButton( tr( SEARCH ) ) )
, cancel_  ( new QPushButton( tr( CANCEL ) ) )
, search_index_ ( 0 )
{
   setWindowTitle( tr( CAPTION ) );

   QHBoxLayout* const search_layout = new QHBoxLayout;
   search_layout->addSpacing( 8 );
   search_layout->addWidget( search_edit_ );
   search_layout->addWidget( search_ );
   search_layout->setContentsMargins( 5, 5, 5, 5 );

   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addLayout( search_layout );
   main_layout->addWidget( browser_ );
   main_layout->setContentsMargins( 0, 0, 0, 20 );
   main_layout->setSpacing( 0 );
   setLayout( main_layout );
   browser_->setFocus();

   connect( cancel_, SIGNAL( clicked() ), this, SLOT( accept() ) );
   connect( search_, SIGNAL( clicked() ), this, SLOT( search() ) );
   connect( search_edit_, SIGNAL( returnPressed() ), this, SLOT( search() ) );
}
// end of QBtFileViewer

void QBtFileViewer::keyPressEvent( QKeyEvent *event )
{
    if(event->matches(QKeySequence::Find))
    {
        search_edit_->setFocus();
    }
    QDialog::keyPressEvent(event);
}

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtFileViewer::showEvent( QShowEvent* const in_event )
{
   load_file();
   
   QBtShared::resize( this, 80, 70 );
   QDialog::showEvent( in_event );
}
// end of showEvent

//*******************************************************************
// load_file                                                 PRIVATE
//*******************************************************************
void QBtFileViewer::load_file()
{
   QString file_path = QFile::symLinkTarget( path_ );
   if( file_path.isEmpty() ) file_path = path_;

   static const QString cmd = "file -b -i \"%1\"";

   QApplication::setOverrideCursor( Qt::WaitCursor );      
   QBtSystemCall syscall;
   syscall.run( cmd.arg( file_path ) );
   const QString marker = syscall.result();

   if( file_path.endsWith( ".tar" ) || file_path.endsWith( ".tar.gz" ) || file_path.endsWith( ".tar.bz2" ) ) {
      tar_file( file_path );
   }
   else if( file_path.endsWith( ".zip" ) ) {
      zip_file( file_path );   
   }
   else if( marker.startsWith( "text" ) ) {
      ( marker.indexOf( "/html" ) != -1 ) ? html_file( file_path ) : text_file( file_path );
   }
   else if( marker.startsWith( "image" ) ) {
      image_file( file_path );
   }
   else {
      other_file( file_path );
   }
   QApplication::restoreOverrideCursor();
}
// end of load_file

//*******************************************************************
// tar_file                                                  PRIVATE
//*******************************************************************
void QBtFileViewer::tar_file( const QString& in_path )
{
   static const QString cmd = "tar -t -v -f \"%1\"";
   do_it( cmd.arg( in_path ) );
}
// end of tar_file

//*******************************************************************
// zip_file                                                  PRIVATE
//*******************************************************************
void QBtFileViewer::zip_file( const QString& in_path )
{
   static const QString cmd = "unzip -l \"%1\"";
   do_it( cmd.arg( in_path ) );
}
// end of tar_file


//*******************************************************************
// html_file                                                 PRIVATE
//*******************************************************************
void QBtFileViewer::html_file( const QString& in_path )
{
   QFile file( in_path );
   if( file.open( QIODevice::ReadOnly ) ) {
      QTextStream in( &file );
      browser_->insertHtml( in.readAll() );
      file.close();
   }
}
// end of html_file

//*******************************************************************
// text_file                                                 PRIVATE
//*******************************************************************
void QBtFileViewer::text_file( const QString& in_path )
{
    QFile file(in_path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }
    browser_->insertPlainText( QTextStream(&file).readAll() );
}
// end of text_file

//*******************************************************************
// other_file                                                PRIVATE
//*******************************************************************
void QBtFileViewer::other_file( const QString& in_path )
{
    QFile file(in_path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }
    browser_->insertPlainText( QTextStream(&file).readAll() );
}
// end of other_file

//*******************************************************************
// image_file                                                PRIVATE
//*******************************************************************
void QBtFileViewer::image_file( const QString& in_path )
{
   QImage image = QImage();
   if( image.load( in_path ) ) {
      static const QString PATH           = "File path: %1";
      static const QString SIZE           = "Size in pixels(width x hight): %1 x %2";
      static const QString BYTES_PER_LINE = "The number of bytes per image scanline: %1";
      static const QString DEPTH          = "The number of bits used to encode a pixel: %1";

      set_font();
      browser_->document()->addResource( QTextDocument::ImageResource, QUrl( "obrazek" ), image);
      QTextCursor cursor = browser_->textCursor();
      cursor.insertImage( "obrazek" );
      browser_->append( " " );
      browser_->append( PATH.arg( in_path ) );
      browser_->append( SIZE.arg( image.width() ).arg( image.height() ) );
      browser_->append( BYTES_PER_LINE.arg( image.bytesPerLine() ) );
      browser_->append( DEPTH.arg( image.depth() ) );
   }
}
// end of image_file

//*******************************************************************
// do_it                                                     PRIVATE
//*******************************************************************
void QBtFileViewer::do_it( const QString& in_cmd )
{
//   set_font();

   QBtSystemCall syscall;
   syscall.run( in_cmd );
   browser_->setLineWrapMode( QTextEdit::NoWrap );
   browser_->insertPlainText( syscall.result() );
}

QTextCharFormat QBtFileViewer::defaultCharFormat()
{
    return QTextCharFormat();
}

// end of do_it

//*******************************************************************
// set_font                                                  PRIVATE
//*******************************************************************
void QBtFileViewer::set_font()
{
   static const QString my_font = "Monospace,9,-1,5,50,0,0,0,0,0";
   QFont fnt;
   fnt.fromString( my_font );
   browser_->setFont( fnt );   
}
// end of set_font

void QBtFileViewer::highlightSearch(QTextDocument *document, QString searchString)
{
    QTextCursor highlightCursor(document);
    QTextCharFormat plainFormat( highlightCursor.charFormat() );
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setForeground( Qt::red );
    while( !highlightCursor.isNull() && !highlightCursor.atEnd() ) {
        highlightCursor = document->find( searchString, highlightCursor );
        if( !highlightCursor.isNull() ) {
            search_results_.append( highlightCursor.anchor() );
            highlightCursor.mergeCharFormat( colorFormat );
        }
    }
}

void QBtFileViewer::search()
{
    QString searchString = search_edit_->text();
    QTextDocument *document = browser_->document();
    QTextCursor cursor(document);

    cursor.beginEditBlock();


    if( searchString != search_text_ || searchString.isEmpty() || search_results_.isEmpty()) {
        search_results_.clear();
        search_index_ = 0;
        cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
        cursor.setCharFormat( defaultCharFormat() );
        if( !searchString.isEmpty() ) {
            highlightSearch(document, searchString);
        }
    }
    search_text_ = searchString;

    if( !search_results_.isEmpty() ) {
        cursor.setPosition( search_results_[search_index_] );
        cursor.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor, search_text_.length() );
        browser_->setTextCursor(cursor);
        ++search_index_;
        if(search_index_>=search_results_.size())
        {
            search_index_ = 0;
        }
    }

    cursor.endEditBlock();

}
