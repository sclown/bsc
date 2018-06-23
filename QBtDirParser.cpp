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
 * File         : QBtDirParser.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 05.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtDirParser.h"
#include "QBtShared.h"
#include "QBtMacTools.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtDebug>


//*******************************************************************
// cancel                                                     PUBLIC
//*******************************************************************
void QBtDirParser::cancel()
{
   break_ = true;
}
// end of cancel

//*******************************************************************
// run                                                        PUBLIC
//*******************************************************************
void QBtDirParser::run( const QString& in_dir_path,
                        const QString& in_fname_muster,
                        const bool     in_file_filter )
{
   fname_muster_ = in_fname_muster;
   file_filter_ = in_file_filter;
   break_ = false;
   do_it( in_dir_path );
   emit finished();
}
// end of run

//*******************************************************************
// do_it                                                     PRIVATE
//*******************************************************************
void QBtDirParser::do_it( const QString& in_path )
{
   QBtShared::idle();
   if( break_ ) return;

   const QFileInfo fi( in_path );
   
   if( fi.isDir() ) {
       moveToTrash(in_path);


//      if( fi.isExecutable() && fi.isReadable() ) {
//         emit change_dir( in_path );
//         parse_dir( in_path );
//         if( remove_ ) remove_dir( in_path );
//      }
//      else {
//         emit cant_read_dir( in_path );
//      }
   }
   else {
      emit current_item( in_path );
      if( remove_ ) remove_file( in_path );
   }
}
// end of do_it

//*******************************************************************
// parse_dir                                                 PRIVATE
//*******************************************************************
void QBtDirParser::parse_dir( const QString& in_dir_path )
{
   QBtShared::idle();
   if( break_ ) return;

   QDir dir( in_dir_path );
   QDir::Filters filter = QDir::AllDirs | QDir::Files | QDir::System /*| QDir::NoSymLinks*/;
   if( !file_filter_ ) filter |= QDir::Hidden;
   dir.setFilter ( filter );
   dir.setNameFilters( fname_muster_.split( ';', QString::SkipEmptyParts ) );
   const QFileInfoList data = dir.entryInfoList();

   QFileInfoList::const_iterator it = data.begin();
   const QFileInfoList::const_iterator end = data.end();
   while( !break_ && ( it != end ) ) {
      if( QBtShared::is_regular_file( it->fileName() ) ) {
         do_it( it->absoluteFilePath() );
      }
      ++it;
   }
}
// end of parse_dir

//*******************************************************************
// remove_file                                               PRIVATE
//*******************************************************************
void QBtDirParser::remove_file( const QString& in_file_path )
{
   if( break_ ) return;
   
   if( !QFile::remove( in_file_path ) ) {
      emit cant_remove_file( in_file_path );
   }
}
// end of current_item

//*******************************************************************
// remove_dir                                                PRIVATE
//*******************************************************************
void QBtDirParser::remove_dir( const QString& in_dir_path )
{
   if( break_ ) return;

   QDir dir( in_dir_path );
   const QString dir_name = dir.dirName();
   if( dir.cdUp() ) {
      if( !dir.rmdir( dir_name ) ) {
         emit cant_remove_dir( in_dir_path );
      }
   }
}
// end of remove_dir
