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
 * File         : QBtCompareDirsDialog.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.at
 * Creation date: 2008.09.16
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtCompareDirsDialog.h"
#include "QBtInfoLabel.h"
#include "QBtShared.h"
#include "QBtSettings.h"
#include "QBtConfig.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QTreeWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QLabel>
#include <QDir>
#include <QByteArray>
#include <QMessageBox>
#include <QProcess>
#include <QtDebug>
using namespace std;

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtCompareDirsDialog::Caption       = QT_TR_NOOP( "Directories comparing" );
const char* const QBtCompareDirsDialog::DirsCaption   = QT_TR_NOOP( "Compared directories" );
const char* const QBtCompareDirsDialog::ResultCaption = QT_TR_NOOP( "Result" );
const char* const QBtCompareDirsDialog::LftPanel      = QT_TR_NOOP( "File from left panel" );
const char* const QBtCompareDirsDialog::RgtPanel      = QT_TR_NOOP( "File from right panel" );
const char* const QBtCompareDirsDialog::Run           = QT_TR_NOOP( "&Run" );
const char* const QBtCompareDirsDialog::Compare       = QT_TR_NOOP( "Co&mpare" );
const char* const QBtCompareDirsDialog::Close         = QT_TR_NOOP( "&Close" );
const char* const QBtCompareDirsDialog::Break         = QT_TR_NOOP( "&Break" );
const char* const QBtCompareDirsDialog::LeftDir       = QT_TR_NOOP( "Left panel directory:" );
const char* const QBtCompareDirsDialog::RightDir      = QT_TR_NOOP( "Right panel directory:" );
const char* const QBtCompareDirsDialog::NotPresent    = QT_TR_NOOP( "not present" );
const char* const QBtCompareDirsDialog::TheSame       = QT_TR_NOOP( "Both files are the same." );
const char* const QBtCompareDirsDialog::CantOpen      = QT_TR_NOOP( "Can't open the file: %1\n%2" );
const char* const QBtCompareDirsDialog::ReadError     = QT_TR_NOOP( "Error on read a file: %1\n%2" );
const QString     QBtCompareDirsDialog::Dir           = "<DIR>";
const QString     QBtCompareDirsDialog::Equal         = "=";
const QString     QBtCompareDirsDialog::NotEqual      = "< >";
const QString     QBtCompareDirsDialog::Error         = "x";


//*******************************************************************
//  QBtCompareDirsDialog                                 CONSTRUCTOR
//*******************************************************************
QBtCompareDirsDialog::QBtCompareDirsDialog( QWidget* const in_parent )
: QDialog        ( in_parent )
, view_          ( new QTreeWidget )
, run_btn_       ( new QPushButton( tr( Run ) ) )
, close_btn_     ( new QPushButton( tr( Close ) ) )
, left_dirinfo_  ( new QBtInfoLabel )
, right_dirinfo_ ( new QBtInfoLabel )
, left_path_     ( QString()  )
, right_path_    ( QString()  )
, runned_        ( false )
, continue_      ( false )
, ready_         ( false )
{
   setWindowTitle( tr( Caption ) );
   run_btn_->setEnabled( false );

   QStringList hdr;
   hdr << tr( LftPanel ) << tr( ResultCaption ) << tr( RgtPanel );
   view_->setHeaderLabels( hdr );
   view_->setColumnCount( 3 );
   view_->setRootIsDecorated( false );
   view_->setSortingEnabled( false );
   view_->resizeColumnToContents( 0 );
   view_->resizeColumnToContents( 1 );
   view_->resizeColumnToContents( 2 );
      
   //------- data --------
   QGridLayout* const dirs_layout = new QGridLayout;
   dirs_layout->addWidget( new QLabel( tr( LeftDir ) ), 0, 0, Qt::AlignRight );
   dirs_layout->addWidget( new QLabel( tr( RightDir ) ), 1, 0, Qt::AlignRight );
   dirs_layout->addWidget( left_dirinfo_               , 0, 1, Qt::AlignLeft );
   dirs_layout->addWidget( right_dirinfo_              , 1, 1, Qt::AlignLeft );
   dirs_layout->setColumnStretch( 1, 100 );
   QGroupBox* const dirs_box = new QGroupBox( tr( DirsCaption ) );
   dirs_box->setLayout( dirs_layout );

   QVBoxLayout* const view_layout = new QVBoxLayout;
   view_layout->addWidget( view_ );
   QGroupBox* const view_box = new QGroupBox( tr( ResultCaption ) );
   view_box->setLayout( view_layout );
   
   QVBoxLayout* const data_layout = new QVBoxLayout;
   data_layout->addWidget( dirs_box );
   data_layout->addWidget( view_box );

   //------- buttons -------
   QVBoxLayout* const btn_layout = new QVBoxLayout;
   btn_layout->addStretch();
   btn_layout->addWidget( run_btn_ );
   btn_layout->addWidget( close_btn_ );

   //------- main --------
   QHBoxLayout* const main_layout = new QHBoxLayout;
   main_layout->addLayout( data_layout );
   main_layout->addLayout( btn_layout );
   setLayout( main_layout );

   connect( run_btn_  , SIGNAL( clicked() ), this, SLOT( start_slot() ) );
   connect( close_btn_, SIGNAL( clicked() ), this, SLOT( close_slot() ) );
}
// end of QBtCompareDirsDialog

//*******************************************************************
// set_dirs                                                   PUBLIC
//*******************************************************************
void QBtCompareDirsDialog::set_dirs( const QString& in_left_path, const QString& in_right_path )
{
   left_dirinfo_ ->setText( left_path_ = in_left_path );
   right_dirinfo_->setText( right_path_ = in_right_path ); 
   run_btn_->setEnabled( true );
}
// end of set_dirs

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtCompareDirsDialog::showEvent( QShowEvent* const in_event )
{
   QDialog::showEvent( in_event );
   QBtShared::resize( this, 40, 50 );
}
// end of showEvent

//*******************************************************************
// start_slot                                           PRIVATE slot
//*******************************************************************
void QBtCompareDirsDialog::start_slot()
{
   if( ready_ ) {
      // porownanie wybranych plikow
      const QTreeWidgetItem* const item = view_->currentItem();
      if( !item ) return;
      if( item->data( 1, Qt::UserRole ).toBool() ) return;
      
      if( Error != item->text( 1 ) ) {
         if( Equal == item->text( 1 ) ) {
            QMessageBox::information( this, tr( Caption ), tr( TheSame ) );
         }
         else {
            diff( item->data( 0, Qt::UserRole ).toString(),
                 item->data( 2, Qt::UserRole ).toString() );
         }
      }
   }
   else {
      // odczytywanie danych
      if( left_path_.isEmpty() || right_path_.isEmpty() ) return;
      QBtShared::set_cursor( Qt::BusyCursor );
      run_btn_->setEnabled( false );
      close_btn_->setText( tr( Break ) );
      runned_ = continue_ = true;
      ready_ = false;
      compare( left_path_, right_path_ );
      view_->resizeColumnToContents( 0 );
      view_->resizeColumnToContents( 1 );
      run_btn_->setText( tr( Compare ) );
      close_btn_->setText( tr( Close ) );
      run_btn_->setEnabled( true );
      runned_ = false;
      ready_ = true;
      set_focus();
      QBtShared::restore_cursor();
      QBtShared::idle();
      
      if( !continue_ ) reject();
      else             continue_ = false;
   }
}
// end of start_slot

//*******************************************************************
// set_focus                                                 PRIVATE
//*******************************************************************
void QBtCompareDirsDialog::set_focus()
{
   view_->setFocus( Qt::OtherFocusReason );

   QTreeWidgetItem* const root = view_->invisibleRootItem();
   if( root->childCount() ) {
      view_->setCurrentItem( root->child( 0 ) );
   }
}
// end of set_focus

//*******************************************************************
// close_slot                                           PRIVATE slot
//*******************************************************************
void QBtCompareDirsDialog::close_slot()
{
   if( continue_ ) continue_ = false;
   else            accept();
}
// end of close_slot

//*******************************************************************
// read_dir                                                  PRIVATE
//-------------------------------------------------------------------
// Rekursywne odczytywanie calej zawartosci wskazanego katalogu.
// Odczytana zawrtosc katalogu jest zapamietywania w hash-tablicy.
//*******************************************************************
void QBtCompareDirsDialog::read_dir( const QString& in_parent,
                                     const QString& in_dir,
                                     DirMap& out_data ) const
{
   static const int sflag = QDir::AllDirs
                          | QDir::Files
                          | QDir::NoDotAndDotDot
                          | QDir::Readable
                          | QDir::Writable
                          | QDir::Hidden;
                   
   const QDir dir( in_dir, "*", QDir::Unsorted, QFlags<QDir::Filter>( sflag ) );
   const QFileInfoList items = dir.entryInfoList();

   FileMap files_map = FileMap();
   QFileInfoList::const_iterator it = items.begin();
   const QFileInfoList::const_iterator end = items.end();
   while( continue_ && ( it != end ) ) {
      if( it->isDir() ) read_dir( in_parent, it->absoluteFilePath(), out_data );
      else              files_map.insert( it->fileName(), *it );
      ++it;
   }
   QString path = in_dir;
   out_data.insert( path.remove( in_parent ), files_map );
}
// end of read_dir

//*******************************************************************
// join_dirs                                                 PRIVATE
//*******************************************************************
void QBtCompareDirsDialog::join_dirs( AllItems& out_set, const DirMap& in_map  ) const
{
   if( in_map.empty() ) return;
   
   DirMap::const_iterator it = in_map.begin();
   const DirMap::const_iterator end = in_map.end();
   while( continue_ && ( it != end ) ) {
      out_set.insert( it.key(), QString() );
      ++it;
   }
}
// end of join_dirs

//*******************************************************************
// join_files                                                PRIVATE
//*******************************************************************
void QBtCompareDirsDialog::join_files( AllItems& out_set, const FileMap& in_map  ) const
{
   if( in_map.empty() ) return;
   
   FileMap::const_iterator it = in_map.begin();
   const FileMap::const_iterator end = in_map.end();
   while( continue_ && ( it != end ) ) {
      out_set.insert( it.key(), QString() );
      ++it;
   }
}
// end of join_files

//*******************************************************************
// compare                                                   PRIVATE
//-------------------------------------------------------------------
// Porownywanie zawartosci przyslanych katalogo (rekursywnie).
//*******************************************************************
void QBtCompareDirsDialog::compare( const QString& in_ldir, const QString& in_rdir )
{
   QBtShared::idle();
   if( !continue_ ) return;

   DirMap data1 = DirMap();
   DirMap data2 = DirMap();
   read_dir( in_ldir, in_ldir, data1 );
   read_dir( in_rdir, in_rdir, data2 );

   AllItems data_all = AllItems();
   join_dirs( data_all, data1 );
   join_dirs( data_all, data2 );
   if( data_all.empty() ) return;

   RowData row_data;
   AllItems::const_iterator it = data_all.begin();
   const AllItems::const_iterator end = data_all.end();
   while( continue_ && ( it != end ) ) {
      const QString name = it.key();
      const DirMap::const_iterator it1 = data1.find( name );
      const DirMap::const_iterator it2 = data2.find( name );
      const bool is1 = ( it1 != data1.end() );
      const bool is2 = ( it2 != data2.end() );

      row_data.reset();
      if( !name.isEmpty() ) {
         row_data.is_dir( true );
         row_data.separator( Dir );

         if( is1 ) row_data.set_lft( name, in_ldir + name );
         if( is2 ) row_data.set_rgt( name, in_rdir + name );
         add_row( row_data );
      }
      
      if( is1 && is2 ) compare_files( it1.value(), it2.value() );
      else {
         if( is1 ) compare_files( it1.value(), FileMap() );
         else      compare_files( FileMap(), it2.value() );
      }
      ++it;
   }
}
// end of compare

//*******************************************************************
// compare_files                                             PRIVATE
//-------------------------------------------------------------------
// Porownanie wszystkich przyslanych plikow.
// Kryterium porownywania jest czas ich ostatniej modyfikacji.
//*******************************************************************
void QBtCompareDirsDialog::compare_files( const FileMap& in_data1, const FileMap& in_data2 )
{
   QBtShared::idle();
   if( !continue_ ) return;

   AllItems data_all = AllItems();
   join_files( data_all, in_data1 );
   join_files( data_all, in_data2 );
   if( data_all.empty() ) return;

   RowData row_data;
   AllItems::const_iterator it = data_all.begin();
   const AllItems::const_iterator end = data_all.end();
   while( continue_ && ( it != end ) ) {
      const QString name = it.key();
      const FileMap::const_iterator it1 = in_data1.find( name );
      const FileMap::const_iterator it2 = in_data2.find( name );
      const bool is1 = ( it1 != in_data1.end() );
      const bool is2 = ( it2 != in_data2.end() );

      row_data.reset();
      if( is1 ) row_data.set_lft( name, it1.value().absoluteFilePath() );
      if( is2 ) row_data.set_rgt( name, it2.value().absoluteFilePath() );
      if( is1 && is2 ) {
         row_data.separator( check_files( row_data.path1(), row_data.path2() ) );
      }
      add_row( row_data );
      ++it;
   }
}
// end of compare_files

//*******************************************************************
// add_row                                                   PRIVATE
//*******************************************************************
void QBtCompareDirsDialog::add_row( const RowData& in_data ) const
{
   QBrush bkg_color[3] = { QColor( 255, 255, 255 ) };
   QBrush frg_color[3] = { QColor(   0,   0,   0 ) };

   if( in_data.separator().isEmpty() ) {
      if( !in_data.is_lft() ) {
         frg_color[0] = QColor( 255, 0,   0 );
         frg_color[2] = QColor(   0, 0, 255 );
      }
      if( !in_data.is_rgt() ) {
         frg_color[0] = QColor(   0, 0, 255 );
         frg_color[2] = QColor( 255, 0,   0 );
      }
   }
   else {
      if( !in_data.is_dir() ) {
         if( Equal == in_data.separator() ) {
            bkg_color[0] = bkg_color[1] = bkg_color[2] = QColor( 220, 230, 220 );
         }
         else {
            bkg_color[0] = bkg_color[1] = bkg_color[2] = QColor( 255, 100, 100 );
            frg_color[0] = frg_color[1] = frg_color[2] = QColor( 255, 255, 255 );
         }
      }
   }
   
   QTreeWidgetItem* const new_item = new QTreeWidgetItem;
   for( int i = 0; i < 3; ++i ) {
      new_item->setBackground( i, bkg_color[i] );
      new_item->setForeground( i, frg_color[i] );
   }

   new_item->setText( 0, in_data.is_lft() ? in_data.name1() : tr( NotPresent ) );
   new_item->setText( 1, in_data.separator() );
   new_item->setText( 2, in_data.is_rgt() ? in_data.name2() : tr( NotPresent ) );
   new_item->setData( 0, Qt::UserRole, in_data.path1() );
   new_item->setData( 1, Qt::UserRole, in_data.is_dir() );
   new_item->setData( 2, Qt::UserRole, in_data.path2() );
   new_item->setTextAlignment ( 1, Qt::AlignHCenter );
   view_->invisibleRootItem()->addChild( new_item );

   QBtShared::idle();
}
// end of add_row

//*******************************************************************
// check_files                                               PRIVATE
//*******************************************************************
QString QBtCompareDirsDialog::check_files( const QFileInfo& in_file1,
                                           const QFileInfo& in_file2 )
{
   if( in_file1.size() != in_file2.size() )  return NotEqual;
   return cmp_contents( in_file1.absoluteFilePath(), in_file2.absoluteFilePath() );
}
// end of check_files

//*******************************************************************
// cmp_contents                                              PRIVATE
//*******************************************************************
QString QBtCompareDirsDialog::cmp_contents(  const QString& in_src_fpath,
                                             const QString& in_dst_fpath )
{
   static const int BytesCount = 4096;
   QString retval = Error;

   QBtShared::idle();
   if( !continue_ ) return retval;

   QFile src( in_src_fpath );
   if( src.open( QIODevice::ReadOnly ) ) {
      QFile dst( in_dst_fpath );  
      if( dst.open( QIODevice::ReadOnly ) ) {
         QBtShared::idle();
         retval = Equal;
         
         while( continue_ && !src.atEnd() && !dst.atEnd() ) {
            if( src.read( BytesCount ) != dst.read( BytesCount ) ) {
               retval = NotEqual;
               break;
            }
            if( src.error() != QFile::NoError ) {
               QMessageBox::critical( this, tr( Caption ),
                                      tr( ReadError ).arg( in_src_fpath ).arg( src.errorString() ) );
               retval = Error;
               break;
            }
            if( dst.error() != QFile::NoError ) {
               QMessageBox::critical( this, tr( Caption ),
                                      tr( ReadError ).arg( in_dst_fpath ).arg( dst.errorString() ) );
               retval = Error;
               break;
            }
         }
         dst.close();
      }
      else {
         QMessageBox::critical( this, tr( Caption ),
                                tr( CantOpen ).arg( in_dst_fpath ).arg( dst.errorString() ) );
         retval = Error;
      }
      src.close();
   }
   else {
         QMessageBox::critical( this, tr( Caption ),
                                tr( CantOpen ).arg( in_src_fpath ).arg( src.errorString() ) );
         retval = Error;
   }
   
   return retval; 
}
// end of cmp_contents

//*******************************************************************
// diff                                                      PRIVATE
//*******************************************************************
void QBtCompareDirsDialog::diff( const QString& in_fname_1, const QString& in_fname_2 ) const
{
   const QString fname1  = QBtShared::quoted_fpath( in_fname_1 );
   const QString fname2  = QBtShared::quoted_fpath( in_fname_2 );
   const QString beediff = QBtShared::slashed_dir( QDir::homePath() ) + "/beediff/beediff %1 %2";
   bool          ok      = true;
   
   QBtSettings stt;
   QVariant data;
   if( stt.read( QBtConfig::DIFF_GROUP + QBtConfig::USE_DEFAULT_KEY, data ) ) {
      ok = data.toBool();
   }

   QString muster = beediff;
   if( !ok ) {
      if( stt.read( QBtConfig::DIFF_GROUP + QBtConfig::COMMAND_KEY, data ) ) {
         muster = data.toString().trimmed();
         if( muster.isEmpty() ) {
            return;
         }
      }
   }

   const QString cmd = muster.arg( fname1 ).arg( fname2 );
   QProcess process;
   process.startDetached( cmd );
}
// end of diff
