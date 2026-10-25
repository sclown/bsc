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
#include "QBtViewDelegate.h"
#include "QBtViewModel.h"
#include "QBtViewItem.h"
#include "QBtView.h"
#include <QApplication>
#include <QPalette>
#include <QPainter>
#include <QtDebug>

//*******************************************************************
// QBtViewDelegate                                       CONSTRUCTOR
//*******************************************************************
QBtViewDelegate::QBtViewDelegate( QBtView* const in_view )
: QItemDelegate ( in_view )
, view_         ( in_view )
{}
// end of QBtViewDelegate

//*******************************************************************
// sizeHint                                        PRIVATE inherited
//*******************************************************************
QSize QBtViewDelegate::sizeHint( const QStyleOptionViewItem& in_options,
                                 const QModelIndex&          in_index ) const
{
   return QItemDelegate::sizeHint( in_options, in_index ) + QSize( 0, 2 * ROW_PADDING );
}
// end of sizeHint

//*******************************************************************
// paint                                           PRIVATE inherited
//*******************************************************************
void QBtViewDelegate::paint(  QPainter*                   in_painter,
                              const QStyleOptionViewItem& in_options,
                              const QModelIndex&          in_index ) const
{
    QItemDelegate::paint( in_painter, in_options, in_index );
    if(in_index.column() == 0) {
       QBtViewModel* model = dynamic_cast< QBtViewModel* >( view_->model() );
       const QBtViewItem*  const   hit = model->head_item( in_index );
       if(hit && !hit->iconLoaded()) {
           model->queryIcon(in_index);
       }
    }
}
// end of paint
