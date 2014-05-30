#ifndef INCLUDED_QBtEvent_h
#define INCLUDED_QBtEvent_h

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
 * File         : QBtEvent.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 24.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QEvent>
#include <QVariant>
#include <vector>

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtEvent : public QEvent
{
// ******* TYPES *******
public:
   enum {
      UNKNOWN = QEvent::User,
      F1_KEY,
      F2_KEY,
      F3_KEY,
      F4_KEY,
      F5_KEY,
      F6_KEY,
      F7_KEY,
      F8_KEY,
      F9_KEY,
      F10_KEY,
      CONFIG_CHANGED,
      SHOW_LEFT_FSTAB,
      SHOW_RIGHT_FSTAB,
      SELECT_REQUEST,
      DESELECT_REQUEST,
      REVSEL_REQUEST,
      NEW_TAB_REQUEST,
      DEL_TAB_REQUEST,
      SWITCH_TAB_REQUEST,
      REFRESH_REQUEST,
      FILTER_CHANGED_REQUEST,
      FIND_REQUEST,
      WIPE,
      COMPARE_FILES,
      COMPARE_DIRS,
      SYNC_DIRS,
      JOIN_FILES,
      DIR_TREE,
      MD5_CREATE,
      MD5_CHECK,
      DATE_TIME,
      DROP_FILES,
   };

public:
   // ------- 0 -------------------------------------------
   explicit QBtEvent( qint32 const in_event_id )
   : QEvent( static_cast<QEvent::Type>( in_event_id ) )
   {
      data_.reserve( 0 );
   }
   //------- 1 --------------------------------------------
   explicit QBtEvent(   const qint32 in_event_id,
                        const QVariant in_val )
   : QEvent( static_cast<QEvent::Type>( in_event_id ) )
   {
      data_.reserve( 1 );
      data_.push_back( in_val );
   }
   //------- 2 --------------------------------------------
   explicit QBtEvent(   const qint32 in_event_id,
                        const QVariant in_val1,
                        const QVariant in_val2 )
   : QEvent( static_cast<QEvent::Type>( in_event_id ) )
   {
      data_.reserve( 2 );
      data_.push_back( in_val1 );
      data_.push_back( in_val2 );
   }
   //------- 3 --------------------------------------------
   explicit QBtEvent(   const qint32 in_event_id,
                        const QVariant in_val1,
                        const QVariant in_val2,
                        const QVariant in_val3 )
   : QEvent( static_cast<QEvent::Type>( in_event_id ) )
   {
      data_.reserve( 3 );
      data_.push_back( in_val1 );
      data_.push_back( in_val2 );
      data_.push_back( in_val3 );
   }
   //------- 4 --------------------------------------------
   explicit QBtEvent(   const int in_event_id,
                        const QVariant in_val1,
                        const QVariant in_val2,
                        const QVariant in_val3,
                        const QVariant in_val4 )
   : QEvent( static_cast<QEvent::Type>( in_event_id ) )
   {
      data_.reserve( 4 );
      data_.push_back( in_val1 );
      data_.push_back( in_val2 );
      data_.push_back( in_val3 );
      data_.push_back( in_val4 );
   }
private:
   std::vector<QVariant> data_;
public:
   const QVariant& data( qint32 ) const;
private:
   QBtEvent( const QBtEvent& );
   QBtEvent& operator=( const QBtEvent& );
};

//*******************************************************************
// data                                                PUBLIC inline
//*******************************************************************
inline const QVariant& QBtEvent::data( const qint32 in_idx ) const
{
   return data_.at( in_idx );
}

#endif // INCLUDED_QBtEvent_h
