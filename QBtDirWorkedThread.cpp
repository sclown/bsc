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
 * File         : QBtDirWorkedThread.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 31.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtDirWorkedThread.h"
#include "QBtFileInfo.h"
#include "QBtShared.h"
#include "QBtConfig.h"
#include <QDir>
#include <QFileInfo>
#include <QVariant>
#include <QDebug>


//*******************************************************************
// QBtDirWorkedThread                                    CONSTRUCTOR
//*******************************************************************
QBtDirWorkedThread::QBtDirWorkedThread( QObject* const in_parent ) : QBtWorkedThread( in_parent )
{}
// end of QBtDirWorkedThread

//*******************************************************************
// update                                                     PUBLIC
//*******************************************************************
void QBtDirWorkedThread::update( const QString& in_path )
{
   path_ = in_path;
   start();
}
// end of update

//*******************************************************************
// run                                           PROTECTED inherited
//*******************************************************************
void QBtDirWorkedThread::run()
{
   QDir dir( path_ );
   if( !dir.exists() && !dir.isReadable() ) {
       return;
   }
  //-------------------------------
  const quint32 filter_state =
     QBtConfig::instance()->filter()
     ? ( QDir::AllDirs | QDir::Files | QDir::System )
     : ( QDir::AllDirs | QDir::Files | QDir::System | QDir::Hidden );
  dir.setFilter( static_cast< QDir::Filters >( filter_state | QDir::NoDotAndDotDot ) );
  dir.setSorting( QDir::Name | QDir::DirsFirst | QDir::IgnoreCase );
  const QFileInfoList data = dir.entryInfoList();
  //-------------------------------
  emit work_started();
  const qint32 n = data.size();
  emit items_count( n );
  //-------------------------------
  for( qint32 i = 0, row = 1; i < n; ++i ) {
     const QBtFileInfo fi( QFileInfo( data.at( i ) ) );

     if( "." == fi.full_name() ) continue;
     if( ".." == fi.full_name() ) continue;

     QStringList data;
    QString name   = fi.full_name();
    QString ext    = fi.ext();
    if( name.isEmpty() ) {
       name = '.' + ext;
       ext.clear();
    }
    const QString access = QBtShared::access( fi );
    const QString size   = QBtShared::num2str( fi.size() );
    const QString date   = fi.lastmod().date().toString( "dd-MM-yyyy" );
    const QString time   = fi.lastmod().time().toString( "hh:mm:ss" );
    const QString owner  = fi.owner();
    const QString group  = fi.group();
    data << name /*<< ext*/ << size << date << time << access << owner << group;
    emit item_info( row++, QVariant::fromValue( fi ), data );
  }
  if( path_ != QDir::rootPath() ) {
      const QBtFileInfo fi( QFileInfo( dir.absoluteFilePath("..") ) );
     QString dotdot = fi.path();
     emit item_info( 0, QVariant::fromValue( fi ), QStringList("..") );
  }
  emit work_finished( path_ );
}
// end of run
