#ifndef INCLUDED_QBtViewDelegate_h
#define INCLUDED_QBtViewDelegate_h
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
 * File         : QBtViewDelegate.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 29.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QItemDelegate>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QBtView;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtViewDelegate : public QItemDelegate
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtViewDelegate( QBtView* = 0 );
private:
   QBtViewDelegate( const QBtViewDelegate& );
   QBtViewDelegate& operator=( const QBtViewDelegate& );

//******* MEMBERS *******
private:
   const QBtView* const view_;

private:
   void paint( QPainter*, const QStyleOptionViewItem&, const QModelIndex& ) const;
};

#endif // INCLUDED_QBtViewDelegate_h
