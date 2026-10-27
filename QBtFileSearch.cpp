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
#include "QBtFileSearch.h"
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QStringList>

namespace BtSearch {

namespace {

bool content_matches( const QString& path, const Spec& spec, const QRegularExpression& rx )
{
   QFile file( path );
   if( !file.open( QIODevice::ReadOnly ) ) return false;
   const QString text = QTextStream( &file ).readAll();
   if( spec.content_regex ) return rx.match( text ).hasMatch();
   return text.contains( spec.content, spec.case_insensitive ? Qt::CaseInsensitive : Qt::CaseSensitive );
}

}

QRegularExpression name_matcher( const Spec& spec )
{
   if( spec.name_regex ) return QRegularExpression( spec.name_pattern );
   QStringList parts;
   for( const QString& glob : spec.name_pattern.split( ';', Qt::SkipEmptyParts ) ) {
      parts << QRegularExpression::wildcardToRegularExpression( glob.trimmed() );
   }
   return QRegularExpression( parts.join( '|' ) );
}

QRegularExpression content_matcher( const Spec& spec )
{
   if( !spec.content_regex ) return QRegularExpression();
   return QRegularExpression( spec.content, spec.case_insensitive
                                            ? QRegularExpression::CaseInsensitiveOption
                                            : QRegularExpression::NoPatternOption );
}

void run( QPromise<QString>& promise, const Spec spec )
{
   const QRegularExpression name_rx    = name_matcher( spec );
   const QRegularExpression content_rx = content_matcher( spec );

   // Breadth-first over an explicit queue. Non-recursive listings are
   // used on purpose: QDirIterator's recursive mode does not honour the
   // hidden-entry filter in every Qt release, and a queue lets hidden
   // and symlinked directories be pruned before they are entered.
   const QDir::Filters filters = QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot;
   QStringList pending{ QDir( spec.root ).absolutePath() };
   int dirs_seen = 0;
   while( !pending.isEmpty() ) {
      if( promise.isCanceled() ) return;
      const QString dir_path = pending.takeFirst();
      promise.setProgressValueAndText( ++dirs_seen, dir_path );

      const QFileInfoList entries = QDir( dir_path ).entryInfoList( filters, QDir::Name | QDir::DirsLast );
      for( const QFileInfo& fi : entries ) {
         if( promise.isCanceled() ) return;
         if( fi.isHidden() ) continue;
         if( fi.isDir() ) {
            if( !fi.isSymLink() ) pending.append( fi.absoluteFilePath() );
            continue;
         }
         if( !name_rx.match( fi.fileName() ).hasMatch() ) continue;
         if( !spec.content.isEmpty() && !content_matches( fi.absoluteFilePath(), spec, content_rx ) ) continue;
         promise.addResult( fi.absoluteFilePath() );
      }
   }
}

}
