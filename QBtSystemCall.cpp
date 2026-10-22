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
bool QBtSystemCall::run( const QString& in_program, const QStringList& in_args, const QString& in_working_dir )
{
   buffer_.clear();
   exit_code_ = -1;

   QProcess process;
   process.setProcessChannelMode( QProcess::MergedChannels );
   if( !in_working_dir.isEmpty() ) process.setWorkingDirectory( in_working_dir );
   process.start( in_program, in_args );
   if( !process.waitForStarted() ) {
      buffer_ = process.errorString().toLocal8Bit();
      return false;
   }
   process.waitForFinished( -1 );
   buffer_    = process.readAll();
   exit_code_ = process.exitCode();
   return ( QProcess::NormalExit == process.exitStatus() ) && ( 0 == exit_code_ );
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

