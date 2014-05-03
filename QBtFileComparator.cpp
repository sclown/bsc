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
 * File         : QBtFileComparator.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 2008-01-20
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtFileComparator.h"
#include <QFile>

//*******************************************************************
// QBtFileComparator                                     CONSTRUCTOR
//*******************************************************************
QBtFileComparator::QBtFileComparator( QObject* const in_parent )
: QObject( in_parent )
, loop_  ( true )
{
}
// end of QBtFileComparator

//*******************************************************************
// stop                                                       PUBLIC
//*******************************************************************
void QBtFileComparator::stop()
{
   loop_ = false;
}
// end of stop   

//*******************************************************************
// compare                                                    PUBLIC
//*******************************************************************
qint32 QBtFileComparator::compare(  const QString& in_fname1,
                                    const QString& in_fname2 )
{
   qint32 retval                  = IO_ERROR;
   char   buffer1[ BUFFER_SIZE ] = { 0 };
   char   buffer2[ BUFFER_SIZE ] = { 0 };
   
   QFile in1( in_fname1 );
   QFile in2( in_fname2 );  
   emit init_progress( in1.size() );

   if( in1.size() != in2.size() ) {
      emit progress( in1.size() );
      return NOT_EQUAL;
   }

   qint64 total = qint64();
   if( in1.open( QIODevice::ReadOnly ) ) {
      if( in2.open( QIODevice::ReadOnly ) ) {
         while( loop_ && !in1.atEnd() && !in2.atEnd() ) {
            const quint32 n1 = in1.read( buffer1, BUFFER_SIZE );
            const quint32 n2 = in2.read( buffer2, BUFFER_SIZE );
            retval = ( n1 == n2 ) ? memcmp( buffer1, buffer2, n1 ) : NOT_EQUAL;
            if( EQUAL != retval ) break;
            total += n1;
            emit progress( total );
         }
         in2.close();
      }
      in1.close();
   }
   return retval; 
}
// end of compare
