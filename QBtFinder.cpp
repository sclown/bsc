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
 * File         : QBtFinder.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 2007.11.05
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtFinder.h"
#include "QBtShared.h"
#include "QBtConfig.h"
#include "QBtPointsButton.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontMetrics>
#include <QHeaderView>
#include <QRegExp>
#include <QApplication>

/*------- local constants:
-------------------------------------------------------------------*/
const char* const QBtFinder::CAPTION      = QT_TR_NOOP( "Finder" );
const char* const QBtFinder::SETTINGS     = QT_TR_NOOP( "Settings" );
const char* const QBtFinder::RESULT       = QT_TR_NOOP( "Result" );
const char* const QBtFinder::FILE         = QT_TR_NOOP( "&File(s):" );
const char* const QBtFinder::FROM         = QT_TR_NOOP( "&Where:" );
const char* const QBtFinder::WHAT         = QT_TR_NOOP( "Wha&t:" );
const char* const QBtFinder::RUN          = QT_TR_NOOP( "&Run" );
const char* const QBtFinder::BREAK        = QT_TR_NOOP( "&Break" );
const char* const QBtFinder::GOTO         = QT_TR_NOOP( "&Goto" );
const char* const QBtFinder::CANCEL       = QT_TR_NOOP( "&Cancel" );
const char* const QBtFinder::REGEX        = QT_TR_NOOP( "as reg.expr." );
const char* const QBtFinder::CASE         = QT_TR_NOOP( "case insensitive" );
const char* const QBtFinder::DIRSEL       = QT_TR_NOOP( "Select a directory" );
const char* const QBtFinder::ASK_FOR_FILE = QT_TR_NOOP( "What file would you like to search?" );
const char* const QBtFinder::ASK_FOR_FROM = QT_TR_NOOP( "From where would you like to start a search process?" );
const char* const QBtFinder::FILE_NAME    = QT_TR_NOOP( "File name" );
const char* const QBtFinder::FILE_PATH    = QT_TR_NOOP( "File path" );


//*******************************************************************
// QBtFinder                                             CONSTRUCTOR
//*******************************************************************
QBtFinder::QBtFinder( QWidget* const in_parent ) : QDialog( in_parent )
, settings_gbox_     ( new QGroupBox( tr( SETTINGS ) ) )
, file_label_        ( new QLabel( tr( FILE ) ) )
, from_label_        ( new QLabel( tr( FROM ) ) )
, what_label_        ( new QLabel( tr( WHAT ) ) )
, file_              ( new QLineEdit )
, from_              ( new QLineEdit )
, what_              ( new QLineEdit )
, case_              ( new QCheckBox( tr( CASE ) ) )
, file_regex_        ( new QCheckBox( tr( REGEX ) ) )
, from_points_       ( new QBtPointsButton )
, what_regex_        ( new QCheckBox( tr( REGEX ) ) )
, run_               ( new QPushButton( tr( RUN    ) ) )
, break_             ( new QPushButton( tr( BREAK  ) ) )
, goto_              ( new QPushButton( tr( GOTO   ) ) )
, cancel_            ( new QPushButton( tr( CANCEL ) ) )
, result_            ( new QTreeWidget )
, fpath_             ( new QLineEdit )
, font_metrics_      ( fontMetrics() )
, selected_file_path_( QString() )
{
   setWindowTitle( tr( CAPTION ) );

   QTreeWidgetItem* const header = new QTreeWidgetItem;
   header->setText( 0, tr( FILE_NAME ) );
   header->setText( 1, tr( FILE_PATH ) );
   result_->setHeaderItem( header );
   result_->setRootIsDecorated( false );
   result_->setSortingEnabled( true );
   result_->sortByColumn( 0, Qt::AscendingOrder );

   run_->setDefault( true );
   break_->setEnabled( false );
   goto_->setEnabled( false );
   from_->setText( QDir::homePath() );
   
   fpath_->setFrame( false );
   fpath_->setReadOnly( true );
   file_label_->setBuddy( file_ );
   from_label_->setBuddy( from_ );
   what_label_->setBuddy( what_ );

   QGridLayout* const input_layout = new QGridLayout( settings_gbox_ );
   input_layout->addWidget( file_label_ , 0, 0 );
   input_layout->addWidget( from_label_ , 1, 0 );
   input_layout->addWidget( what_label_ , 2, 0 );
   input_layout->addWidget( file_       , 0, 1 );
   input_layout->addWidget( from_       , 1, 1 );
   input_layout->addWidget( what_       , 2, 1 );
   input_layout->addWidget( file_regex_ , 0, 2 );
   input_layout->addWidget( from_points_, 1, 2 );
   input_layout->addWidget( case_       , 2, 2 );
   input_layout->addWidget( what_regex_ , 3, 2 );
   
   QVBoxLayout* const btn_layout = new QVBoxLayout;
   btn_layout->addWidget( run_ );
   btn_layout->addWidget( break_ );
   btn_layout->addWidget( goto_ );
   btn_layout->addWidget( cancel_ );
   btn_layout->addStretch( 100 );
   
   QHBoxLayout* const settings_layout = new QHBoxLayout;
   settings_layout->addWidget( settings_gbox_ );
   settings_layout->addLayout( btn_layout );

   QGroupBox* const result_gbox = new QGroupBox( tr( RESULT ) );
   QHBoxLayout* const result_layout = new QHBoxLayout( result_gbox );
   result_layout->addWidget( result_ );
   
   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addLayout( settings_layout );
   main_layout->addWidget( result_gbox );
   main_layout->addWidget( fpath_ );
   setLayout( main_layout );
   main_layout->setStretchFactor( result_gbox, 100 );

   connect( from_points_, SIGNAL( clicked() ), this, SLOT( select_dir() ) );
   connect( cancel_     , SIGNAL( clicked() ), this, SLOT( reject    () ) );
   connect( run_        , SIGNAL( clicked() ), this, SLOT( run_find  () ) );
   connect( break_      , SIGNAL( clicked() ), this, SLOT( break_find() ) );
   connect( goto_       , SIGNAL( clicked() ), this, SLOT( goto_find () ) );

   connect( &parser_, SIGNAL( change_dir  ( const QString& ) ), this, SLOT( parser_dir_changed ( const QString& ) ) );
   connect( &parser_, SIGNAL( current_item( const QString& ) ), this, SLOT( parser_current_item( const QString& ) ) );
   connect( &parser_, SIGNAL( finished    ()                 ), this, SLOT( parser_finished    () ) );
}
// end of QBtFinder

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtFinder::showEvent( QShowEvent* const in_event )
{
   QBtShared::resize_width( this, 50 );
   QDialog::showEvent( in_event );
}
// end of showEvent

//*******************************************************************
// select_dir                                           PRIVATE slot
//*******************************************************************
void QBtFinder::select_dir()
{
   const QString dir = QFileDialog::getExistingDirectory( this,
                           tr( DIRSEL ),
                           QDir::homePath(),
                           QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks ).trimmed();

   if( !dir.isEmpty() ) {
      from_->setText( dir );
   }
}
// end of select_dir

//*******************************************************************
// run_find                                             PRIVATE slot
//*******************************************************************
void QBtFinder::run_find()
{
   file_->setText( file_->text().trimmed() );
   from_->setText( from_->text().trimmed() );
   what_->setText( what_->text().trimmed() );
   
   if( file_->text().isEmpty() ) {
      QMessageBox::question( this, tr( CAPTION ), tr( ASK_FOR_FILE ) );
      file_->setFocus( Qt::OtherFocusReason );
      return;
   }

   if( from_->text().isEmpty() ) {
      QMessageBox::question( this, tr( CAPTION ), tr( ASK_FOR_FROM ) );
      from_->setFocus( Qt::OtherFocusReason );
      return;
   }

   run_->setEnabled( false );
   break_->setEnabled( true );
   goto_->setEnabled( false );
   cancel_->setEnabled( false );
   settings_gbox_->setEnabled( false );
   result_->clear();

   const QString muster = file_regex_->isChecked() ? QString() : file_->text();
   const bool is_filter = QBtConfig::instance()->filter();
   
   qApp->setOverrideCursor( Qt::WaitCursor );
   parser_.run( from_->text(), muster, is_filter );
}
// end of run_find

//*******************************************************************
// break_find                                           PRIVATE slot
//*******************************************************************
void QBtFinder::break_find()
{
   parser_.cancel();
}
// end of break_find

//*******************************************************************
// goto_find                                            PRIVATE slot
//*******************************************************************
void QBtFinder::goto_find()
{
   const QTreeWidgetItem* const itm = result_->currentItem();
   if( itm ) {
      selected_file_path_ = itm->text( 1 );
      accept();
   }
}
// end of goto_find

//*******************************************************************
// selected_file                                              PUBLIC
//*******************************************************************
void QBtFinder::selected_file( QString& out_file_path ) const
{
   out_file_path = selected_file_path_;
}
// end of selected_file

//*******************************************************************
// parser_dir_changed                                   PRIVATE slot
//*******************************************************************
void QBtFinder::parser_dir_changed( const QString& in_dir )
{
   static qint32 field_width = 0;
   if( !field_width ) field_width = fpath_->width();

   QString dir = in_dir;
   QBtShared::elide( font_metrics_, field_width, dir );
   fpath_->setText( dir );
}
// end of parser_dir_changed

//*******************************************************************
// parser_current_item                                  PRIVATE slot
//*******************************************************************
void QBtFinder::parser_current_item( const QString& in_fpath )
{
   const QFileInfo fi( in_fpath );

   // W przypadku uzywania wyrazen regularnych w odniesieniu do
   // nazw plikow, sprawdzamy czy nazwa pliku jest zgodna ze wzorcem.
   if( file_regex_->isChecked() ) {
      const QRegExp regex( file_->text() );
      if( -1 == regex.indexIn( fi.fileName() ) ) return;
   }

   // W przypadku wyszukiwania wzorca zawartosci, sprawdzamy czy
   // plik zawiera taki wzorzec.
   if( !what_->text().isEmpty() ) {
      if( !search_what( fi.filePath() ) ) return;
   }
   
   
   QTreeWidgetItem* const root = result_->invisibleRootItem();
   QTreeWidgetItem* const itm = new QTreeWidgetItem();
   itm->setText( 0, fi.fileName() );
   itm->setText( 1, fi.filePath() );
   root->addChild( itm );
   result_->resizeColumnToContents( 0 );
   result_->resizeColumnToContents( 1 );
   QBtShared::idle();
}
// end of parser_current_item

//*******************************************************************
// parser_finished                                      PRIVATE slot
//*******************************************************************
void QBtFinder::parser_finished()
{
   qApp->restoreOverrideCursor();
   run_->setEnabled( true );
   break_->setEnabled( false );
   cancel_->setEnabled( true );
   settings_gbox_->setEnabled( true );
   fpath_->setText( QString() );

   const QTreeWidgetItem* const root = result_->invisibleRootItem();
   if( root->childCount() ) {
      goto_->setEnabled( true );
      result_->setCurrentItem( root->child( 0 ) );
      result_->setFocus( Qt::OtherFocusReason );
   }
   else {
      file_->setFocus( Qt::OtherFocusReason );
   }
}
// end of parser_finished

//*******************************************************************
// search_what                                               PRIVATE
//*******************************************************************
bool QBtFinder::search_what( const QString& in_path ) const
{
   QFile file( in_path );
   if( file.open( QIODevice::ReadOnly ) ) {
      QTextStream in( &file );
      QString text = in.readAll();
      file.close();

      QString what_to_find = what_->text();
      if( case_->isChecked() ) {
         text = text.toLower();
         what_to_find = what_to_find.toLower();
      }
               
      if( what_regex_->isChecked() ) {
         QRegExp regex( what_to_find );
         if( regex.indexIn( text ) != -1 ) {
            return true;
         }
      }
      else {
         if( text.indexOf( what_to_find ) != -1 ) {
            return true;
         }
      }   
   }

   return false;
}
// end of search_what
