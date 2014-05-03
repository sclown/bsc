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
 * File         : QBtInfoLabel.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.at
 * Creation date: 2008.09.17
 *******************************************************************/

 /*------- include files:
 ------------------------------------------------------------------*/
 #include "QBtInfoLabel.h"
 #include "QBtShared.h"
 #include <QPaintEvent>
 #include <QPainter>
 #include <QFileInfo>

/*------- constants:
-------------------------------------------------------------------*/
const QString QBtInfoLabel::Spacer = "/.../";
const QString QBtInfoLabel::Slash  = "/";

//*******************************************************************
// QBtInfoLabel                                          CONSTRUCTOR
//*******************************************************************
QBtInfoLabel::QBtInfoLabel( QWidget* in_parent )
: QLabel( in_parent )
{
   //setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Preferred );
}
// end of QBtInfoLabel

//*******************************************************************
// setText                                                    PUBLIC
//*******************************************************************
void QBtInfoLabel::setText( const QString& in_text )
{
   const QFontMetrics fm( font() );
   QFileInfo fi( in_text );
   setMinimumWidth( fm.width( Spacer + fi.fileName() ) );
   QLabel::setText( in_text );
}
// end of setText

//*******************************************************************
// paintEvent                                      PRIVATE inherited
//*******************************************************************
void QBtInfoLabel::paintEvent( QPaintEvent* const in_event )
{
   Q_UNUSED( in_event )

   QPainter p( this );
   p.drawText( rect(), Qt::TextSingleLine, elide( contentsRect().width() ) );
   p.end();
}
// end of paintEvent

//*******************************************************************
// elide
//*******************************************************************
QString QBtInfoLabel::elide( const int in_width ) const
{
   const QFontMetrics fm( font() );
   
   QString buffer = text();
   if( fm.width( buffer ) <= in_width ) return buffer;

   const QStringList words    = buffer.split( Slash, QString::SkipEmptyParts );
         int         i        = 0;
         int         j        = words.count() - 1;
         QString     lft_part = QString();
         QString     rgt_part = words[j--];
         bool        lft_ok   = true;
         bool        rgt_ok   = true;

   while( lft_ok || rgt_ok ) {
      lft_ok = false;
      if( i < j ) {
         const QString tmp = lft_part + Slash + words[i];
         if( fm.width( tmp + Spacer + rgt_part ) < in_width ) {
            lft_part = tmp;
            lft_ok = true;
            ++i;
         }
      }
      //.............................................
      rgt_ok = false;
      if( i < j ) {
         const QString tmp = words[j] + Slash + rgt_part;
         if( fm.width( lft_part + Spacer + tmp ) < in_width ) {
            rgt_part = tmp;
            rgt_ok = true;
            --j;
         }
      }
      
   }
   return ( lft_part + Spacer + rgt_part );
}
// end of elide
