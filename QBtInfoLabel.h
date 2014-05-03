#ifndef INCLUDED_QBtInfoLabel_h
#define INCLUDED_QBtInfoLabel_h
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
 * File         : QBtInfoLabel.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.at
 * Creation date: 2008.09.17
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QLabel>

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtInfoLabel : public QLabel
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtInfoLabel( QWidget* = 0 );
private:
   QBtInfoLabel( const QBtInfoLabel& );
   QBtInfoLabel& operator=( const QBtInfoLabel& );

//******* CONSTANTS *******
private:
   static const QString Spacer;
   static const QString Slash;
   
//******* METHODS *******
public:
   void setText( const QString& );
private:
   void    paintEvent( QPaintEvent* );
   QString elide     ( int ) const;
};

#endif //INCLUDED_QBtInfoLabel_h
