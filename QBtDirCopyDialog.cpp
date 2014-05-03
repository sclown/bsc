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
 * File         : QBtDirCopyDialog.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 02.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtDirCopyDialog.h"
#include "QBtDirParser.h"
#include "QBtShared.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QMessageBox>
#include <QDateTime>
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtDirCopyDialog::CAPTION = QT_TR_NOOP( "Copy processing" );
const QString     QBtDirCopyDialog::CHOWN   = "chown %1:%2 %3";


//*******************************************************************
// QBtDirCopyDialog                                         CONSTRUCTOR
//*******************************************************************
QBtDirCopyDialog::QBtDirCopyDialog( QWidget* const in_parent ) : QBtCopyDialog( in_parent )
{
   setWindowTitle( tr( CAPTION ) );
}
// end of QBtDirCopyDialog


//*******************************************************************
// start                                      PRIVATE inherited slot
//*******************************************************************
void QBtDirCopyDialog::start()
{
   const QFileInfo fi( destpath_ );
   if( !fi.isWritable() ) {
      QMessageBox::critical( this, tr( CAPTION ), tr( DIR_NOT_WRITABLE ).arg( destpath_ ) );
      finished();
      return;
   }
   
   started();
   //--------------------------------------------
   SelectionsSet::const_iterator it = sources_.begin();
   const SelectionsSet::const_iterator end = sources_.end();
   while( !break_ && ( it != end ) ) {
      copy_next( *it, destpath_ );
      ++it;
   }
   //--------------------------------------------
   finished();
}
// end of start

//*******************************************************************
// copy_next                                                 PRIVATE
//*******************************************************************
void QBtDirCopyDialog::copy_next( const QString& in_src_path, const QString& in_dst_dir )
{
   QBtShared::idle();
   if( break_ ) return;

   QFileInfo fi( in_src_path );
   QString   file_path = fi.absoluteFilePath();
   QString   file_name = fi.fileName();

   // KATALOG ---------------------------------------------
   if( fi.isDir() ) {
      if( fi.isReadable() ) {
         QDir dst_subdir( in_dst_dir );
         dst_subdir.mkdir( file_name );
         if( dst_subdir.cd( file_name ) ) {
            copy_dir( file_path, dst_subdir.absolutePath() );
            if( do_remove() ) {
               remove_dir( file_path );
            }
            QFileInfo sfi( file_path );
            QFile dfi( dst_subdir.absolutePath() );
            if( do_permissions() ) {
               dfi.setPermissions( sfi.permissions() );
            }
            if( do_owner() ) {
               sc_.run( CHOWN.arg( sfi.owner() ).arg( sfi.group() ).arg( dst_subdir.absolutePath() ) );
            }
         }
         else {
            //  Nie mozna utworzyc podkatalogu.
            QMessageBox::critical( this, tr( CAPTION ), tr( MKDIR_ERROR ).arg( file_name ) );
         }
      }
      else {
         // Brak praw dostepu. Nic nierob. Wracamy.
         QMessageBox::warning( this, tr( CAPTION ), tr( DIR_NOT_READABLE ).arg( file_path ) );
      }
   }
   // NORMALNY PLIK ---------------------------------------
   else {
      copy_file( file_path, in_dst_dir );
   }
}
// end of copy_next

//*******************************************************************
// copy_dir                                        PRIVATE inherited
//*******************************************************************
void QBtDirCopyDialog::copy_dir( const QString& in_src_dir, const QString& in_dst_dir )
{
   QBtShared::idle();
   if( break_ ) return;

   QDir dir( in_src_dir );
   dir.setFilter( QDir::AllDirs | QDir::Files );
   dir.setSorting( QDir::Name | QDir::DirsFirst | QDir::IgnoreCase );

   const QFileInfoList data = dir.entryInfoList();
   QFileInfoList::const_iterator it = data.begin();
   const QFileInfoList::const_iterator end = data.end();
   while( !break_ && ( it != end ) ) {
      if( QBtShared::is_regular_file( it->fileName() ) ) {
         copy_next( it->absoluteFilePath(), in_dst_dir );
      }
      ++it;
   }
}
// end of copy_dir

//*******************************************************************
// copy_file                                       PRIVATE inherited
//*******************************************************************
void QBtDirCopyDialog::copy_file( const QString& in_src_path, const QString& in_dst_dir )
{
   // Sprwadz czy czy plik jest do odczytu
   const QFileInfo fi( in_src_path );
   if( !fi.isReadable() ) {
      QMessageBox::critical( this, tr( CAPTION ), tr( FILE_NOT_READABLE ).arg( in_src_path ) );
      return;
   }

   QString dst_path( in_dst_dir );
   if( !dst_path.endsWith( '/' ) ) dst_path += "/";
   dst_path += fi.fileName();
   
   display_paths( in_src_path, dst_path );

   // Najpierw sprawdzamy czy w ogole mozemy kopiowac plik.
   // Jezeli sa jakiekolwiek watpliwosci (np. plik juz istnieje)
   // zgode na kopiowanie musi wydac uzytkownik.
   if( can_copy( in_src_path, dst_path ) ) {
      // Po uzyskaniu zgody na kopiowanie bierzemy sie do roboty.
      QFile in( in_src_path );
      QFile out( dst_path );
      if( in.open( QIODevice::ReadOnly ) ) {
         const quint32 nbytes = in.size(); // Liczba bajtow do przekopiowania.
         quint32 copied = 0;               // Licznik juz przekopiowanych bajtow.
         
         if( out.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
            reset_progress( nbytes );
            QBtShared::idle();
            // Petla kopiujaca.
            while( !in.atEnd() ) {
               const quint32 n = in.read( block_, BLOCK_SIZE );
               out.write( block_, n );
               set_progress( copied += n );
               QBtShared::idle();
            }
            out.close();
            in.close();
            
           // czynnosci po kopiowaniu
            if( do_remove() ) {
               if( !in.remove() ) {
                  QMessageBox::warning( this, tr( CAPTION ), tr( CANT_DEL_FILE ).arg( in_src_path ) );
               }
            }
            if( do_permissions() ) {
               out.setPermissions( in.permissions() );
            }
            if( do_owner() ) {
               sc_.run( CHOWN.arg( fi.owner() ).arg( fi.group() ).arg( dst_path ) );
            }
            if( do_datime() ) {
               QBtShared::touch( in_src_path, dst_path );
            }
         }
         else {
            QMessageBox::critical( this, tr( CAPTION ), tr( OPEN_WRITE_ERROR ).arg( dst_path ) );
         }
         in.close();
      }
      else {
         QMessageBox::critical( this, tr( CAPTION ), tr( OPEN_READ_ERROR ).arg( in_src_path ) );
      }
   }
}
// end of copy_file

//*******************************************************************
// can_update                                      PRIVATE inherited
//*******************************************************************
bool QBtDirCopyDialog::can_update( const QString& in_src_path, const QString& in_dst_path ) const
{
   const QFileInfo src_fi( in_src_path );
   const QFileInfo dst_fi( in_dst_path );
   return ( src_fi.lastModified() > dst_fi.lastModified() );
}
// end of can_update

//*******************************************************************
// remove_dir                                      PRIVATE inherited
//*******************************************************************
void QBtDirCopyDialog::remove_dir( const QString& in_path )
{
   QDir dir( in_path );
   const QString dir_name = dir.dirName();
   if( dir.cdUp() ) {
      if( !dir.rmdir( dir_name ) ) {
         QMessageBox::critical( this, tr( CAPTION ), tr( CANT_DEL_DIR ).arg( in_path ) );
      }
   }
}
// end of remove_dir
