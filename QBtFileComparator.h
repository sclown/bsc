#ifndef INCLUDED_QBtFileComparator_h
#define INCLUDED_QBtFileComparator_h

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
 * File         : QBtFileComparator.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 2008-01-20
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QObject>

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtFileComparator : public QObject
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtFileComparator( QObject* = 0 );
private:
   QBtFileComparator( const QBtFileComparator& );
   QBtFileComparator& operator=( const QBtFileComparator& );

//******* TYPES *******
public:
   enum {
      BUFFER_SIZE = 4096,
      EQUAL = 0,
      NOT_EQUAL,
      IO_ERROR,
   };

//******* MEMBERS *******
private:
   bool loop_;
   
//******* METHODS *******
public:
   void  stop    ();
   qint32 compare( const QString&, const QString& );
signals:
   void init_progress ( qint64 );
   void progress      ( qint64 );
};

#endif // INCLUDED_QBtFileComparator_h
