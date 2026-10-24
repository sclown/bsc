#ifndef INCLUDED_QBtViewItem_h
#define INCLUDED_QBtViewItem_h

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
 * File         : QBtViewItem.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 29.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QStandardItem>
#include "QBtFileInfo.h"

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtViewItem : public QStandardItem
{
//******* TYPES *******
public:
//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtViewItem() : QStandardItem()
   , finfo_    ( QBtFileInfo() )
   {}
private:
   QBtViewItem( const QBtViewItem& rhs ) : QStandardItem( rhs )
   , finfo_    ( rhs.finfo_     )
   {}
   QBtViewItem& operator=( const QBtViewItem& rhs )
   {
      if( this != &rhs ) {
         QStandardItem::operator=( rhs );
         finfo_    = rhs.finfo_;
      }
      return *this;
   }

//******* METHODS *******
public:
   const QBtFileInfo& finfo    () const                        { return finfo_;           }
   void               finfo    ( const QBtFileInfo& in_finfo ) { finfo_ = in_finfo;       }
   bool iconLoaded() const { return !icon_.isNull(); }
   void updateIcon(QIcon icon) { icon_ = icon; QStandardItem::setIcon(icon);}
//******* MEMBERS *******
private:
   QBtFileInfo finfo_;
   QIcon    icon_;
};

#endif // INCLUDED_QBtViewItem_h
