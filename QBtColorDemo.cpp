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
 * File         : QBtColorDemo.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 12.05.2008
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtColorDemo.h"
#include <QColor>
#include <QColorDialog>

/*------- constants:
-------------------------------------------------------------------*/
const int         QBtColorDemo::HEIGHT = 14;
const char* const QBtColorDemo::TEXT   = QT_TR_NOOP( "Text example" );

//*******************************************************************
// QBtColorDemo                                          CONSTRUCTOR
//*******************************************************************
QBtColorDemo::QBtColorDemo( const bool in_with_text, QWidget* const in_parent )
: QLineEdit( in_parent )
{
   if( in_with_text ) {
      setText( tr( TEXT ) );
      setAlignment( Qt::AlignHCenter );
   }
   else {
      setMinimumHeight( HEIGHT );
      setMaximumHeight( HEIGHT );
   }
  
   setReadOnly( true );
   setFocusPolicy( Qt::NoFocus );
}
// end of QBtColorDemo

//*******************************************************************
// background                                                 PUBLIC
//*******************************************************************
void QBtColorDemo::background( const QColor& in_color )
{
   QPalette p = palette();
   p.setColor( backgroundRole(), in_color );
   setPalette( p );
}
//-----------------------------------------------
QColor QBtColorDemo::background() const
{
   return palette().color( backgroundRole() );
}
// end of background

//*******************************************************************
// color                                                      PUBLIC
//*******************************************************************
void QBtColorDemo::color( const QColor& in_color )
{
   QPalette p = palette();
   p.setColor( foregroundRole(), in_color );
   setPalette( p );
}
//-----------------------------------------------
QColor QBtColorDemo::color() const
{
   return palette().color( foregroundRole() );
}
// end of color

//*******************************************************************
// font                                                       PUBLIC
//*******************************************************************
void QBtColorDemo::fonts( const QFont& in_font )
{
   setFont( in_font );
}
//-----------------------------------------------
QFont QBtColorDemo::fonts() const
{
   return font();
}
// end of font

//###################################################################
//#                                                                 #
//#                           S L O T S                             #
//#                                                                 #
//###################################################################


//*******************************************************************
// select_bkg                                           PRIVATE slot
//*******************************************************************
void QBtColorDemo::select_bkg()
{
   const QColor current_color = background();
   const QColor new_color = QColorDialog::getColor( current_color, this );
   if( new_color.isValid() ) {
      if( new_color != current_color ) {
         background( new_color );
      }
   }
}
// end of select_bkg

//*******************************************************************
// select_clr                                           PRIVATE slot
//*******************************************************************
void QBtColorDemo::select_clr()
{
   const QColor current_color = color();
   const QColor new_color = QColorDialog::getColor( current_color, this );
   if( new_color.isValid() ) {
      if( new_color != current_color ) {
         color( new_color );
      }
   }
}
// end of select_clr
