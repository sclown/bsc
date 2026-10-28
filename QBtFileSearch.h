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

#ifndef INCLUDED_QBtFileSearch_h
#define INCLUDED_QBtFileSearch_h

/*------- include files:
-------------------------------------------------------------------*/
#include <QString>
#include <QRegularExpression>
#include <QPromise>

/*------- declarations:
-------------------------------------------------------------------*/
// Recursive file search used by the Finder dialog. Pure functions,
// no Qt object: meant to run on a thread-pool thread via
// QtConcurrent::run( BtSearch::run, spec ).
namespace BtSearch {

struct Spec {
   QString root;
   QString name_pattern;
   bool    name_regex       = false;
   QString content;
   bool    content_regex    = false;
   bool    case_insensitive = false;
};

QRegularExpression name_matcher( const Spec& );
QRegularExpression content_matcher( const Spec& );

void run( QPromise<QString>& promise, const Spec spec );

}

#endif // INCLUDED_QBtFileSearch_h
