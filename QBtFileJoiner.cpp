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
 * File         : QBtFileJoiner.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 09.09.2008
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtFileJoiner.h"
#include "QBtShared.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QFileInfo>
#include <QTreeWidget>
#include <QHeaderView>
#include <QFile>
#include <QMessageBox>
#include <QByteArray>
#include <QProgressBar>
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtFileJoiner::Caption            = QT_TR_NOOP( "File joiner" );
const char* const QBtFileJoiner::SourceCaption      = QT_TR_NOOP( "Source files" );
const char* const QBtFileJoiner::DestinationCaption = QT_TR_NOOP( "Destination file" );
const char* const QBtFileJoiner::ProgressCaption    = QT_TR_NOOP( "Progress" );
const char* const QBtFileJoiner::Start              = QT_TR_NOOP( "&Start" );
const char* const QBtFileJoiner::Cancel             = QT_TR_NOOP( "&Cancel" );
const char* const QBtFileJoiner::Break              = QT_TR_NOOP( "&Break" );
const char* const QBtFileJoiner::Path               = QT_TR_NOOP( "Path: %1" );
const char* const QBtFileJoiner::Overwrite          = QT_TR_NOOP( "&Overwrite" );
const char* const QBtFileJoiner::Append             = QT_TR_NOOP( "&Append" );
const char* const QBtFileJoiner::AtTop              = QT_TR_NOOP( "insert at the &beginning" );
const char* const QBtFileJoiner::AtEnd              = QT_TR_NOOP( "append at the &end" );
const char* const QBtFileJoiner::Up                 = QT_TR_NOOP( "&Up" );
const char* const QBtFileJoiner::Down               = QT_TR_NOOP( "&Down" );
const char* const QBtFileJoiner::CantRemove         = QT_TR_NOOP( "Can't remove the file: %1" );
const char* const QBtFileJoiner::CantCreateTmp      = QT_TR_NOOP( "Can't create temporary file" );
const char* const QBtFileJoiner::ReadError          = QT_TR_NOOP( "Error on read a file: %1\n%2" );
const char* const QBtFileJoiner::WriteError         = QT_TR_NOOP( "Error on write to a file: %1\n%2" );

//*******************************************************************
// QBtFileJoiner                                         CONSTRUCTOR
//*******************************************************************
QBtFileJoiner::QBtFileJoiner( QWidget* const in_parent               ,
                                       const SelectionsSet& in_files ,
                                       const QString& in_dst_fpath   )
: QDialog        ( in_parent    )
, files_         ( in_files     )
, dst_fpath_     ( in_dst_fpath )
, dst_exists_    ( QFileInfo( in_dst_fpath ).exists() )
, start_btn_     ( new QPushButton ( tr( Start     ) ) )
, cancel_btn_    ( new QPushButton ( tr( Cancel    ) ) )
, overwrite_btn_ ( new QRadioButton( tr( Overwrite ) ) )
, append_btn_    ( new QRadioButton( tr( Append    ) ) )
, at_top_btn_    ( new QRadioButton( tr( AtTop     ) ) )
, at_end_btn_    ( new QRadioButton( tr( AtEnd     ) ) )
, list_          ( new QTreeWidget )
, up_btn_        ( new QPushButton( tr( Up   ) ) )
, down_btn_      ( new QPushButton( tr( Down ) ) )
, src_box_       ( new QGroupBox( tr( SourceCaption ) ) )
, dst_box_       ( new QGroupBox( tr( DestinationCaption ) ) )
, progress_box_  ( new QGroupBox( tr( ProgressCaption ) ) )
, progress_      ( new QProgressBar )
, bak_fpath_     ( QString() )
, runned_        ( false )
, break_         ( false )
{
   setWindowTitle( tr( Caption ) );
   create_list( in_files );
   start_btn_->setDefault( true );
   progress_box_->setEnabled( false );
   progress_->setRange( 0, 100 );
   progress_->setValue( 0 );

   //------- dst -------
   QButtonGroup* const btn_grp_1 = new QButtonGroup( this );
   btn_grp_1->addButton( overwrite_btn_, OverwriteId );
   btn_grp_1->addButton( append_btn_, AppendId );
   QButtonGroup* const btn_grp_2 = new QButtonGroup( this );
   btn_grp_2->addButton( at_top_btn_, AtTopId );
   btn_grp_2->addButton( at_end_btn_, AtEndId );
   
   QGridLayout* const where_layout = new QGridLayout;
   where_layout->addWidget( at_top_btn_, 0, 1 );
   where_layout->addWidget( at_end_btn_, 1, 1 );
   where_layout->setColumnStretch( 0, 10 );
   where_layout->setColumnStretch( 1, 90 );

   QVBoxLayout* const dst_layout = new QVBoxLayout;
   dst_layout->addWidget( new QLabel( tr( Path ).arg( in_dst_fpath ) ) );
   dst_layout->addWidget( overwrite_btn_ );
   dst_layout->addWidget( append_btn_ );
   dst_layout->addLayout( where_layout );
   dst_box_->setLayout( dst_layout );

   if( dst_exists_ ) {
      overwrite_btn_->setChecked( true );
      at_end_btn_   ->setChecked( true );
      mode_slot( OverwriteId );
   }
   else {
      overwrite_btn_->setVisible( false );
      append_btn_   ->setVisible( false );
      at_top_btn_   ->setVisible( false );
      at_end_btn_   ->setVisible( false );
   }

   //------- src -------
   QVBoxLayout* const move_layout = new QVBoxLayout;
   move_layout->addStretch();
   move_layout->addWidget( up_btn_ );
   move_layout->addWidget( down_btn_ );
   move_layout->addStretch();
   
   QHBoxLayout* const src_layout = new QHBoxLayout;
   src_layout->addWidget( list_ );
   src_layout->addLayout( move_layout );
   src_box_->setLayout( src_layout );

   //------- progress -------
   QHBoxLayout* const progress_layout = new QHBoxLayout;
   progress_layout->addWidget( progress_ );
   progress_box_->setLayout( progress_layout );
   
   QVBoxLayout* const lft_side = new QVBoxLayout;
   lft_side->addWidget( dst_box_ );
   lft_side->addWidget( src_box_ );
   lft_side->addWidget( progress_box_ );
   lft_side->addStretch();

   QVBoxLayout* const rgt_side = new QVBoxLayout;
   rgt_side->addStretch();
   rgt_side->addWidget( start_btn_ );
   rgt_side->addWidget( cancel_btn_ );
   
   QHBoxLayout* const main_layout = new QHBoxLayout;
   main_layout->addLayout( lft_side );
   main_layout->addLayout( rgt_side );
   main_layout->setStretchFactor( lft_side, 100 );
   setLayout( main_layout );

   connect( start_btn_ , SIGNAL( clicked      ()    ), this, SLOT( start_slot ()    ) );
   connect( cancel_btn_, SIGNAL( clicked      ()    ), this, SLOT( cancel_slot()    ) );
   connect( btn_grp_1  , SIGNAL( buttonClicked(int) ), this, SLOT( mode_slot  (int) ) );
   connect( up_btn_    , SIGNAL( clicked      ()    ), this, SLOT( up_slot    ()    ) );
   connect( down_btn_  , SIGNAL( clicked      ()    ), this, SLOT( down_slot  ()    ) );
}
// end of QBtFileJoiner

//*******************************************************************
// create_list                                               PRIVATE
//*******************************************************************
void QBtFileJoiner::create_list( const SelectionsSet& in_files )
{
   list_->header()->hide();
   list_->setColumnCount( 1 );
   list_->setRootIsDecorated( false );
   list_->setSortingEnabled( false );
   /*
   list_->setDragEnabled( true );
   list_->setAcceptDrops( true );
   list_->setDragDropOverwriteMode( false );
   list_->setDropIndicatorShown( true );
   list_->setDragDropMode( QTreeWidget::InternalMove );
   */

   QTreeWidgetItem* const root = list_->invisibleRootItem();
   SelectionsSet::const_iterator it = in_files.begin();
   const SelectionsSet::const_iterator end = in_files.end();
   while( it != end ) {
      QTreeWidgetItem* const item = new QTreeWidgetItem;
      item->setData( 0, FullPath, *it );
      item->setText( 0, QFileInfo( *it ).fileName() );
      root->addChild( item );
      ++it;
   }
}
// end of create_list

//*******************************************************************
// cancel_slot                                          PRIVATE slot
//*******************************************************************
void QBtFileJoiner::cancel_slot()
{
   if( runned_ ) {
      break_ = true;
   }
   else {
      reject();
   }
}
// end of cancel_slot

//*******************************************************************
// mode_slot                                            PRIVATE slot
//********************************************************************
void QBtFileJoiner::mode_slot( const int in_id )
{
   const bool append = ( in_id == AppendId );
   at_top_btn_->setEnabled( append );
   at_end_btn_->setEnabled( append );
}
// end of mode_slot

//###################################################################
//#                                                                 #
//#                          COPY OPERATIONS                        #
//#                                                                 #
//###################################################################

//*******************************************************************
// up_slot                                              PRIVATE slot
//*******************************************************************
void QBtFileJoiner::up_slot()
{
   QTreeWidgetItem* const current = list_->currentItem();
   if( current ) {
      QTreeWidgetItem* const root = list_->invisibleRootItem();
      const int idx = root->indexOfChild( current );
      if( idx > 0 ) {
         QTreeWidgetItem* const clon = current->clone();
         root->removeChild( current );
         root->insertChild( idx - 1, clon );
         list_->setCurrentItem( clon );
      }
   }
}
// end of up_slot

//*******************************************************************
// down_slot                                            PRIVATE slot
//*******************************************************************
void QBtFileJoiner::down_slot()
{
   QTreeWidgetItem* const current = list_->currentItem();
   if( current ) {
      QTreeWidgetItem* const root = list_->invisibleRootItem();
      const int idx = root->indexOfChild( current );
      const int edx = root->childCount() - 1;
      if( ( idx >= 0 ) && ( idx < edx ) ) {
         QTreeWidgetItem* const clon = current->clone();
         root->removeChild( current );
         root->insertChild( idx + 1, clon );
         list_->setCurrentItem( clon );
      }
   }
}
// end of down_slot

//*******************************************************************
// start_slot                                           PRIVATE slot
//*******************************************************************
void QBtFileJoiner::start_slot()
{
   src_box_->setEnabled( false );
   dst_box_->setEnabled( false );
   progress_box_->setEnabled( true );
   start_btn_->setEnabled( false );
   cancel_btn_->setText( tr( Break ) );
   break_ = runned_ = false;
   QBtShared::idle();
   
   QFile dstf( dst_fpath_ );
   if( init( dstf ) ) {
      runned_ = true;
      QTreeWidgetItem* const root = list_->invisibleRootItem();
      const int n = root->childCount();
      for( int idx = 0; !break_ && ( idx < n ); ++idx ) {
         const QTreeWidgetItem* const it = root->child( idx );
         break_ = !append( dstf, it->data( 0, FullPath ).toString() );
      }

      finish( dstf );
      dstf.close();
   }
   runned_ = false;
   QBtShared::idle();
   
   if( break_ ) reject();
   else         accept();
}
// end of start_slot

//*******************************************************************
// init                                                      PRIVATE
//*******************************************************************
bool QBtFileJoiner::init( QFile& in_dstf )
{
   bak_fpath_ = QString();
   
   if( in_dstf.exists() ) {
      // Wersja 1: plik docelowy istnieje i jest do usuniecia.
      if( overwrite_btn_->isChecked() ) {
         if( in_dstf.exists() && !in_dstf.remove() ) {
            QMessageBox::critical( this, tr( Caption ), tr( CantRemove ).arg( dst_fpath_ ) );
            return false;
         }
      }
      else {
         if( at_top_btn_->isChecked() ) {
            bak_fpath_ = dst_fpath_ + ".beediff.tmp";
            QFile::remove( bak_fpath_ );
            if( !QFile::rename( dst_fpath_, bak_fpath_ ) ) {
               QMessageBox::critical( this, tr( Caption ), tr( CantCreateTmp ) );
               return false;
            }
         }
      }
   }

   return in_dstf.open( QIODevice::Append );
}
// end of init

//*******************************************************************
// finish                                                    PRIVATE
//*******************************************************************
void QBtFileJoiner::finish( QFile& out_dstf )
{
   if( break_ ) {
      out_dstf.close();
      if( !bak_fpath_.isEmpty() ) {
         out_dstf.remove();
         QFile::rename( bak_fpath_, dst_fpath_ );
      }
   }
   else {
      if( append_btn_->isChecked() && at_top_btn_->isChecked() ) {
         append( out_dstf, bak_fpath_ );
         QFile::remove( bak_fpath_ );
      }
      out_dstf.close();
   }
   bak_fpath_ = QString();
}
// end of finish

//*******************************************************************
// append                                                    PRIVATE
//*******************************************************************
bool QBtFileJoiner::append( QFile& out_dstf, const QString& in_fpath )
{
   static const qint64 BytesToRead = 4096;

   QFile src( in_fpath );
   if( src.open( QIODevice::ReadOnly ) ) {
      qint64 b = qint64();
      qint64 n = src.size();
      progress_->setMaximum( n );
      progress_->setValue( b );
      QBtShared::idle();
      
      while( !break_ && ( n > 0 ) ) {
         const QByteArray data = src.read( BytesToRead );
         if( src.error() != QFile::NoError ) {
            QMessageBox::critical( this,
                                   tr( Caption ),
                                   tr( ReadError ).arg( in_fpath ).arg( src.errorString() ) );
            src.close();
            return false;
         }

         out_dstf.write( data );
         if( out_dstf.error() != QFile::NoError ) {
            QMessageBox::critical( this,
                                   tr( Caption ),
                                   tr( WriteError ).arg( in_fpath ).arg( out_dstf.errorString() ) );
            src.close();
            return false;
         }
         
         b += data.count();
         n -= data.count();
         progress_->setValue( b );
         QBtShared::idle();
      }
      src.close();
      return true;
   }
   
   return false;
}
// end of append
