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
#include "QBtPointsButton.h"
#include "QBtFileSearch.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QEvent>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontMetrics>
#include <QHeaderView>
#include <QRegularExpression>
#include <QtConcurrent>

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
const char* const QBtFinder::ASK_FOR_FROM = QT_TR_NOOP( "From where would you like to start a search process?" );
const char* const QBtFinder::FILE_NAME    = QT_TR_NOOP( "File name" );
const char* const QBtFinder::FILE_PATH    = QT_TR_NOOP( "File path" );
const char* const QBtFinder::BAD_PATTERN  = QT_TR_NOOP( "Invalid regular expression:\n%1" );
const int         QBtFinder::REFRESH_MS   = 50;


//*******************************************************************
// QBtFinder                                             CONSTRUCTOR
//*******************************************************************
QBtFinder::QBtFinder( QWidget* const in_parent, const QString &workDir ) : QDialog( in_parent )
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
, shown_count_       ( 0 )
{
   setWindowTitle( tr( CAPTION ) );

   QTreeWidgetItem* const header = new QTreeWidgetItem;
   header->setText( 0, tr( FILE_NAME ) );
   header->setText( 1, tr( FILE_PATH ) );
   result_->setHeaderItem( header );
   result_->setRootIsDecorated( false );
   result_->setUniformRowHeights( true );
   result_->setSortingEnabled( true );
   result_->sortByColumn( 0, Qt::AscendingOrder );
   result_->resizeColumnToContents( 0 );
   result_->setColumnWidth(0, 200);

   run_->setDefault( true );
   break_->setEnabled( false );
   goto_->setEnabled( false );
   file_->setText( "*" );
   from_->setText( workDir.length()? workDir : QDir::homePath() );
   for( QLineEdit* const edit : { file_, from_, what_ } ) {
      edit->setTextMargins( 4, 2, 4, 2 );
   }
   
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

   result_->installEventFilter( this );

   refresh_.setInterval( REFRESH_MS );
   connect( &refresh_, &QTimer::timeout, this, &QBtFinder::refresh );

   connect( &watcher_, &QFutureWatcher<QString>::progressTextChanged, this, &QBtFinder::search_progress );
   connect( &watcher_, &QFutureWatcher<QString>::finished           , this, &QBtFinder::search_finished );
}
// end of QBtFinder

//*******************************************************************
// ~QBtFinder                                             DESTRUCTOR
//*******************************************************************
QBtFinder::~QBtFinder()
{
   watcher_.cancel();
   watcher_.waitForFinished();
}
// end of ~QBtFinder

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtFinder::showEvent( QShowEvent* const in_event )
{
   QBtShared::resize_width( this, 50 );
   QDialog::showEvent( in_event );
   file_->setFocus( Qt::OtherFocusReason );
   file_->selectAll();
}
// end of showEvent

//*******************************************************************
// eventFilter                                     PRIVATE inherited
//*******************************************************************
bool QBtFinder::eventFilter( QObject* const in_object, QEvent* const in_event )
{
   if( in_object == result_ ) {
      if( in_event->type() == QEvent::FocusIn && goto_->isEnabled() ) {
         goto_->setDefault( true );
      }
      else if( in_event->type() == QEvent::FocusOut ) {
         run_->setDefault( true );
      }
   }
   return QDialog::eventFilter( in_object, in_event );
}
// end of eventFilter

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
      file_->setText( file_regex_->isChecked() ? ".*" : "*" );
   }

   if( from_->text().isEmpty() ) {
      QMessageBox::question( this, tr( CAPTION ), tr( ASK_FOR_FROM ) );
      from_->setFocus( Qt::OtherFocusReason );
      return;
   }

   BtSearch::Spec spec;
   spec.root             = from_->text();
   spec.name_pattern     = file_->text();
   spec.name_regex       = file_regex_->isChecked();
   spec.content          = what_->text();
   spec.content_regex    = what_regex_->isChecked();
   spec.case_insensitive = case_->isChecked();

   const QRegularExpression name_rx = BtSearch::name_matcher( spec );
   if( !name_rx.isValid() ) {
      QMessageBox::warning( this, tr( CAPTION ), tr( BAD_PATTERN ).arg( name_rx.errorString() ) );
      file_->setFocus( Qt::OtherFocusReason );
      return;
   }
   const QRegularExpression content_rx = BtSearch::content_matcher( spec );
   if( !content_rx.isValid() ) {
      QMessageBox::warning( this, tr( CAPTION ), tr( BAD_PATTERN ).arg( content_rx.errorString() ) );
      what_->setFocus( Qt::OtherFocusReason );
      return;
   }

   run_->setEnabled( false );
   break_->setEnabled( true );
   goto_->setEnabled( false );
   cancel_->setEnabled( false );
   settings_gbox_->setEnabled( false );
   result_->clear();

   root_dir_ = QDir( QDir( spec.root ).absolutePath() );
   shown_count_ = 0;
   progress_dir_.reset();
   watcher_.setFuture( QtConcurrent::run( BtSearch::run, spec ) );
   refresh_.start();
   result_->setFocus( Qt::OtherFocusReason );
}
// end of run_find

//*******************************************************************
// break_find                                           PRIVATE slot
//*******************************************************************
void QBtFinder::break_find()
{
   watcher_.cancel();
}
// end of break_find

//*******************************************************************
// goto_find                                            PRIVATE slot
//*******************************************************************
void QBtFinder::goto_find()
{
   const QTreeWidgetItem* const itm = result_->currentItem();
   if( itm ) {
      selected_file_path_ = itm->data( 1, Qt::UserRole ).toString();
      watcher_.cancel();
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
// search_progress                                      PRIVATE slot
//*******************************************************************
void QBtFinder::search_progress( const QString& in_dir )
{
   progress_dir_ = in_dir;
}
// end of search_progress

//*******************************************************************
// refresh                                              PRIVATE slot
//*******************************************************************
void QBtFinder::refresh()
{
   if( progress_dir_ ) {
      QString dir = root_dir_.relativeFilePath( *progress_dir_ );
      QBtShared::elide( font_metrics_, fpath_->width(), dir );
      fpath_->setText( dir );
      progress_dir_.reset();
   }

   const QFuture<QString> future = watcher_.future();
   const int count = future.resultCount();
   if( count <= shown_count_ ) return;

   QList<QTreeWidgetItem*> items;
   items.reserve( count - shown_count_ );
   for( int i = shown_count_; i < count; ++i ) {
      const QString path = future.resultAt( i );
      QTreeWidgetItem* const itm = new QTreeWidgetItem;
      itm->setText( 0, QFileInfo( path ).fileName() );
      itm->setText( 1, root_dir_.relativeFilePath( path ) );
      itm->setData( 1, Qt::UserRole, path );
      items.append( itm );
   }
   result_->addTopLevelItems( items );
   shown_count_ = count;

   const QHeaderView* const header = result_->header();
   result_->sortItems( header->sortIndicatorSection(), header->sortIndicatorOrder() );

   if( !goto_->isEnabled() ) {
      goto_->setEnabled( true );
      result_->setCurrentItem( result_->topLevelItem( 0 ) );
      if( result_->hasFocus() ) goto_->setDefault( true );
   }
   result_->scrollToItem( result_->currentItem(), QAbstractItemView::PositionAtCenter );
}
// end of refresh

//*******************************************************************
// search_finished                                      PRIVATE slot
//*******************************************************************
void QBtFinder::search_finished()
{
   refresh_.stop();
   refresh();   // whatever arrived since the last tick

   run_->setEnabled( true );
   break_->setEnabled( false );
   cancel_->setEnabled( true );
   settings_gbox_->setEnabled( true );
   fpath_->setText( QString() );


   const QTreeWidgetItem* const root = result_->invisibleRootItem();
   if( root->childCount() ) {
      if( !result_->currentItem() ) result_->setCurrentItem( root->child( 0 ) );
      result_->scrollToItem( result_->currentItem(), QAbstractItemView::PositionAtCenter );
      result_->setFocus( Qt::OtherFocusReason );
      goto_->setDefault( true );
   }
   else {
      file_->setFocus( Qt::OtherFocusReason );
   }
}
// end of search_finished
