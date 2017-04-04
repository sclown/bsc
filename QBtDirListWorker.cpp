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
#include "QBtDirListWorker.h"
#include "QBtFileInfo.h"
#include "QBtShared.h"
#include "QBtConfig.h"
#include <QFileIconProvider>
#include <QPainter>
#include <QIcon>
#include <QDir>
#include <QFileInfo>
#include <QVariant>
#include <QDebug>


QBtDirListWorker::QBtDirListWorker()
{
   qRegisterMetaType<QIconPtr>("QIconPtr");

}

void QBtDirListWorker::list(const QString &path, int column, quint8 order)
{
    QDir dir( path );
    if( !dir.exists() && !dir.isReadable() ) {
        return;
    }
    emit work_started();
    //-------------------------------
    dir.setFilter( QDir::AllDirs | QDir::Files | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot  );
    auto sortFlags = QDir::DirsFirst | QDir::IgnoreCase;
    switch (column) {
    case 0:
        sortFlags |= QDir::Name;
        if( order == Qt::DescendingOrder ) {
            sortFlags |= QDir::Reversed;
        }
        break;
    case 1:
        sortFlags |= QDir::Size;
        if( order == Qt::AscendingOrder ) {
            sortFlags |= QDir::Reversed;
        }
        break;
    default:
        sortFlags |= QDir::Time;
        if( order == Qt::AscendingOrder ) {
            sortFlags |= QDir::Reversed;
        }
        break;
    }
    dir.setSorting( sortFlags );
    auto data = dir.entryInfoList();
    auto n = data.size();
    emit items_count( n );
    //-------------------------------
    auto row = 1;
    for( auto info : data ) {
        const QBtFileInfo fi( info );

        if( "." == fi.full_name() ) continue;
        if( ".." == fi.full_name() ) continue;

        QStringList fields;
        auto name   = fi.full_name();
        auto ext    = fi.ext();
        if( name.isEmpty() ) {
            name = '.' + ext;
            ext.clear();
        }
        auto access = QBtShared::access( fi );
        auto size   = QBtShared::num2str( fi.size() );
        auto date   = fi.lastmod().date().toString( "dd-MM-yyyy" );
        auto time   = fi.lastmod().time().toString( "hh:mm:ss" );
        auto owner  = fi.owner();
        auto group  = fi.group();
        fields << name /*<< ext*/ << size << date << time << access << owner << group;
        emit item_info( row++, QVariant::fromValue( fi ), fields );
    }
    if( path != QDir::rootPath() ) {
        const QBtFileInfo fi( QFileInfo( dir.absoluteFilePath("..") ) );
        emit item_info( 0, QVariant::fromValue( fi ), QStringList("..") );
    }
    emit work_finished( path );
}

void QBtDirListWorker::icon(quint32 row, const QString &path)
{
    auto info = QFileInfo(path);
    auto icon = std::make_shared<QIcon>(provider_.icon(info));
    if(info.isSymLink()) {
        auto size = 16;
        QIcon overlay(":/img/symlink.png");
        auto pix = icon->pixmap(size);
        QPainter painter(&pix);
        painter.drawPixmap(QPoint(0,size/2), overlay.pixmap(size/2));
        painter.end();
        icon = std::make_shared<QIcon>(QIcon(pix));
    }
    emit item_icon(row, icon);

}
