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
 * File         : QBtFileViewer.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 10.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtSystemCall.h"
#include <QtDebug>

//*******************************************************************
// run                                                        PUBLIC
//*******************************************************************
void QBtSystemCall::run( const QString& in_cmd )
{
   buffer_.clear();
   QProcess process;
   process.start( in_cmd );
   while( process.waitForReadyRead() ) {
      buffer_ += process.readAll();
   }
}
// end of run

//*******************************************************************
// result                                                     PUBLIC
//*******************************************************************
QString QBtSystemCall::result() const
{
   return QString::fromLocal8Bit( buffer_ );
}
// end of result
