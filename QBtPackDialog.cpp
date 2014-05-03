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
 * File         : QBtPackDialog.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 11.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtPackDialog.h"
#include "QBtInfoField.h"
#include "QBtSystemCall.h"
#include "QBtShared.h"
#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QApplication>
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtPackDialog::CAPTION       = QT_TR_NOOP( "Packing" );
const char* const QBtPackDialog::SRC_AND_DST   = QT_TR_NOOP( "Source and destination" );
const char* const QBtPackDialog::SOURCE        = QT_TR_NOOP( "Source:" );
const char* const QBtPackDialog::DESTINATION   = QT_TR_NOOP( "Destination:" );
const char* const QBtPackDialog::TOOL          = QT_TR_NOOP( "Compression tools" );
const char* const QBtPackDialog::COMPRESSION   = QT_TR_NOOP( "Com&pression" );
const char* const QBtPackDialog::REMOVE        = QT_TR_NOOP( "Remo&ve source files" );
const char* const QBtPackDialog::RUN           = QT_TR_NOOP( "&Run" );
const char* const QBtPackDialog::CANCEL        = QT_TR_NOOP( "&Cancel" );
const char* const QBtPackDialog::GZIP          = QT_TR_NOOP( "gzip (gz)" );
const char* const QBtPackDialog::BZIP2         = QT_TR_NOOP( "bzip2 (bz2)" );
const char* const QBtPackDialog::ZIP           = QT_TR_NOOP( "zip (zip)" );
const QString     QBtPackDialog::TAR_EXT       = ".tar";
const QString     QBtPackDialog::GZIP_EXT      = ".gz";
const QString     QBtPackDialog::BZIP2_EXT     = ".bz2";
const QString     QBtPackDialog::ZIP_EXT       = ".zip";

//*******************************************************************
// QBtPackDialog                                         CONSTRUCTOR
//*******************************************************************
QBtPackDialog::QBtPackDialog( QWidget* const in_parent ) : QDialog( in_parent )
, name_          ( QString() )
, dst_dir_       ( QString() )
, dst_path_      ( QString() )
, source_        ( new QBtInfoField )
, destination_   ( new QBtInfoField )
, compression_   ( new QCheckBox( tr( COMPRESSION ) ) )
, remove_        ( new QCheckBox( tr( REMOVE ) ) )
, ratio_         ( new QComboBox )
, run_           ( new QPushButton( tr( RUN ) ) )
, cancel_        ( new QPushButton( tr( CANCEL ) ) )
, tool_gbox_     ( new QGroupBox( tr( TOOL ) ) )
, gzip_          ( new QRadioButton( tr( GZIP ) ) )
, bzip2_         ( new QRadioButton( tr( BZIP2 ) ) )
, zip_           ( new QRadioButton( tr( ZIP ) ) )
, started_       ( false )
, break_         ( false )
, compress_type_ ( USE_TAR )
, fm_            ( font() )
, data_          ( SelectionsSet() )
{
   setWindowTitle( tr( CAPTION ) );

   QGroupBox* const src_dst_gbox = new QGroupBox( tr( SRC_AND_DST ) );
   QGridLayout* src_dst_layout = new QGridLayout( src_dst_gbox );
   src_dst_layout->setMargin( 4 );
   src_dst_layout->setSpacing( 2 );
   src_dst_layout->addWidget( new QLabel( tr( SOURCE ) ), 0, 0, Qt::AlignRight );
   src_dst_layout->addWidget( new QLabel( tr( DESTINATION ) ), 1, 0, Qt::AlignRight );
   src_dst_layout->addWidget( source_, 0, 1 );
   src_dst_layout->addWidget( destination_, 1, 1 );

   QGridLayout* const btn_layout = new QGridLayout;
   btn_layout->addWidget( compression_, 0, 0 );
   btn_layout->addWidget( remove_, 1, 0 );
   btn_layout->addWidget( ratio_, 0, 1 );
   btn_layout->addWidget( run_, 0, 2 );
   btn_layout->addWidget( cancel_, 1, 2 );
   btn_layout->setColumnStretch( 0, 100 );
   btn_layout->setColumnStretch( 1, 30 );

   QHBoxLayout* const tool_layout = new QHBoxLayout( tool_gbox_ );
   tool_layout->addWidget( gzip_ );
   tool_layout->addWidget( bzip2_ );
   tool_layout->addWidget( zip_ );

   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addWidget( src_dst_gbox );
   main_layout->addLayout( btn_layout );
   main_layout->addWidget( tool_gbox_ );
   setLayout( main_layout );

   for( qint32 i = 9; i >= 1; --i ) {
      ratio_->addItem( QString::number( i ) );
   }
   gzip_->setChecked( true );
   tool_gbox_->setEnabled( false );
   ratio_->setEnabled( false );

   connect( run_        , SIGNAL( clicked() )      , this, SLOT( run() ) );
   connect( cancel_     , SIGNAL( clicked() )      , this, SLOT( reject() ) );
   connect( compression_, SIGNAL( toggled( bool ) ), this, SLOT( compression( bool ) ) );
   connect( gzip_       , SIGNAL( clicked() )      , this, SLOT( type_changed() ) );
   connect( bzip2_      , SIGNAL( clicked() )      , this, SLOT( type_changed() ) );
   connect( zip_        , SIGNAL( clicked() )      , this, SLOT( type_changed() ) );
}
// end of QBtPackDialog

//*******************************************************************
// reject                                     PRIVATE inherited slot
//*******************************************************************
void QBtPackDialog::reject()
{
   if( !started_ ) QDialog::reject();
   break_ = true;
}
// end of reject

//*******************************************************************
// compression                                          PRIVATE slot
//*******************************************************************
void QBtPackDialog::compression( const bool in_state )
{
   tool_gbox_->setEnabled( in_state );
   ratio_->setEnabled( in_state );
   display_dst();
}
// end of compression

//*******************************************************************
// type_changed                                         PRIVATE slot
//*******************************************************************
void QBtPackDialog::type_changed()
{
   display_dst();
}
// end of type_changed

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtPackDialog::showEvent( QShowEvent* const in_event )
{
   QBtShared::resize_width( this, 30 );
   QDialog::showEvent( in_event );
   
   {
      QPalette p = QPalette();
      p.setColor( source_->backgroundRole(), palette().color( backgroundRole() ) );
      p.setColor( source_->foregroundRole(), palette().color( foregroundRole() ) );
      source_->setPalette( p );
   }
   {
      QPalette p = QPalette();
      p.setColor( destination_->backgroundRole(), palette().color( backgroundRole() ) );
      p.setColor( destination_->foregroundRole(), palette().color( foregroundRole() ) );
      destination_->setPalette( p );
   }
   remove_ext();
   display_src( *data_.begin() );
   display_dst();
}
// end of showEvent

//*******************************************************************
// set_source                                                 PUBLIC
//*******************************************************************
void QBtPackDialog::set_source( const SelectionsSet& in_data )
{
   data_ = in_data;
}
// end of set_source

//*******************************************************************
// set_destination                                            PUBLIC
//*******************************************************************
void QBtPackDialog::set_destination( const QString& in_dst_dir, const QString& in_name )
{
   dst_dir_ = in_dst_dir;
   name_ = in_name;
}
// end of set_destination

//*******************************************************************
// get_dst_path                                               PUBLIC
//*******************************************************************
const QString& QBtPackDialog::get_dst_path() const
{
   return dst_path_;
}
// end of get_dst_path

//*******************************************************************
// remove_ext                                                PRIVATE
//*******************************************************************
void QBtPackDialog::remove_ext()
{
   static const QString EXT[] = { TAR_EXT, GZIP_EXT, BZIP2_EXT, ZIP_EXT };
   static const qint32  n     = sizeof( EXT ) / sizeof( EXT[ 0 ] );

   for( qint32 i = 0; i < n; ++i ) {
      if( name_.endsWith( EXT[i], Qt::CaseInsensitive ) ) {
         name_.chop( EXT[ i ].length() );
      }
   }
}
// end of remove_ext

//*******************************************************************
// display_src                                               PRIVATE
//*******************************************************************
void QBtPackDialog::display_src( const QString& in_path )
{
   QString path = in_path;
   QBtShared::elide( fm_, source_->width(), path );
   source_->setText( path );
}
// end of display_dst

//*******************************************************************
// display_dst                                               PRIVATE
//*******************************************************************
void QBtPackDialog::display_dst()
{
   QString path = path2display();
   QBtShared::elide( fm_, destination_->width(), path );
   destination_->setText( path );
}
// end of display_dst

//*******************************************************************
// path2display                                             PRIVATE
//-------------------------------------------------------------------
// Z docelowego katalogu oraz nazwy pliku tworzymy docelowa sciezke
// okreslajaca spakowany plik wraz z WLASCIWYMI rozszerzeniami.
//*******************************************************************
QString QBtPackDialog::path2display()
{
   QString result = QString();

   //-------------------------------
   QString path = dst_dir_;
   if( !path.endsWith( QDir::separator() ) ) {
      path += QDir::separator();
   }
   path += name_;
   //-------------------------------

   const QString path_tar = path + TAR_EXT;
   
   if( compression_->isChecked() ) {
      if( gzip_->isChecked() ) {
         compress_type_ = USE_GZIP;
         return ( path_tar + GZIP_EXT );
      }
      else if( bzip2_->isChecked() ) {
         compress_type_ = USE_BZIP2;
         return ( path_tar + BZIP2_EXT );
      }
      else if( zip_->isChecked() ) {
         compress_type_ = USE_ZIP;
         return ( path + ZIP_EXT );
      }
   }

   compress_type_ = USE_TAR;
   return path_tar;
}
// end of path2display

//*******************************************************************
// run                                                  PRIVATE slot
//*******************************************************************
void QBtPackDialog::run()
{
   QApplication::setOverrideCursor( Qt::WaitCursor );
   ( USE_ZIP == compress_type_ ) ? run_zip() : run_tar();
   QApplication::restoreOverrideCursor();
   accept();
}
// end of run

//*******************************************************************
// run_tar                                                   PRIVATE
//*******************************************************************
void QBtPackDialog::run_tar()
{
   const QChar space = ' ';

   QBtSystemCall sc;
   
   SelectionsSet::const_iterator it = data_.begin();
   const SelectionsSet::const_iterator end = data_.end();
   if( it == end ) return;
   
   QFileInfo fi( *it );
   const QString src_dir  = fi.absolutePath();
   
   dst_path_ = dst_dir_;
   if( !dst_path_.endsWith( QDir::separator() ) ) {
      dst_path_ += QDir::separator();
   }
   dst_path_ += name_;
   dst_path_ += TAR_EXT;

   QString create_cmd = "tar --verbose";
   create_cmd += space;
   create_cmd += "--create";
   create_cmd += space;
   create_cmd += "--directory=";
   create_cmd += src_dir;
   create_cmd += space;
   create_cmd += "--file=";
   create_cmd += dst_path_;
   create_cmd += space;
   
   QString update_cmd = "tar --verbose";
   update_cmd += space;
   update_cmd += "--update";
   update_cmd += space;
   update_cmd += "--directory=";
   update_cmd += src_dir;
   update_cmd += space;
   update_cmd += "--file=";
   update_cmd += dst_path_;
   update_cmd += space;
   
   fi.setFile( *it );
   display_src( *it );
   sc.run( create_cmd + fi.fileName() );
   
   ++it;
   while( it != end ) {
      fi.setFile( *it );
      display_src( *it );
      sc.run( update_cmd + fi.fileName() );
      ++it;
   }

   // Ewentualna kompresja.
   if( compression_->isChecked() ) {
      const bool is_gzip = ( USE_GZIP == compress_type_ );
      QString cmd = is_gzip ? "gzip -" : "bzip2 -";
      cmd += ratio_->currentText();
      cmd += space;
      cmd += dst_path_;
      sc.run( cmd );
      dst_path_ += ( is_gzip ? GZIP_EXT : BZIP2_EXT );
   }
}
// end of run_tar

//*******************************************************************
// run_zip                                                   PRIVATE
//*******************************************************************
void QBtPackDialog::run_zip()
{
   const QChar space = ' ';

   QBtSystemCall sc;
   
   SelectionsSet::const_iterator it = data_.begin();
   const SelectionsSet::const_iterator end = data_.end();
   if( it == end ) return;
   
   dst_path_ = dst_dir_;
   if( !dst_path_.endsWith( QDir::separator() ) ) {
      dst_path_ += QDir::separator();
   }
   dst_path_ += name_;
   dst_path_ += ZIP_EXT;

   QString create_cmd = "zip";
   create_cmd += space;
   create_cmd += "-";
   create_cmd += ratio_->currentText();
   create_cmd += space;
   create_cmd += "-r";
   create_cmd += space;
   create_cmd += dst_path_;
   create_cmd += space;
   
   QString update_cmd = "zip";
   update_cmd += space;
   update_cmd += "-";
   update_cmd += ratio_->currentText();
   update_cmd += space;
   update_cmd += "-u";
   update_cmd += space;
   update_cmd += "-r";
   update_cmd += space;
   update_cmd += dst_path_;
   update_cmd += space;

   display_src( *it );
   sc.run( create_cmd + *it );
   
   ++it;
   while( it != end ) {
      display_src( *it );
      sc.run( update_cmd + *it );
      ++it;
   }
}
// end of run_zip
