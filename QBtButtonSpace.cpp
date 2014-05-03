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
 * File         : QBtButtonSpace.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtButtonSpace.h"
#include "QBtEventsController.h"
#include "QBtSettings.h"
#include "QBtConfig.h"
#include <QPushButton>
#include <QSignalMapper>
#include <QHBoxLayout>
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtButtonSpace::LABEL[] = {
   QT_TR_NOOP( "Help"   ),
   QT_TR_NOOP( "Access" ),
   QT_TR_NOOP( "View"   ),
   QT_TR_NOOP( "Edit"   ),
   QT_TR_NOOP( "Copy"   ),
   QT_TR_NOOP( "Rename" ),
   QT_TR_NOOP( "MkDir"  ),
   QT_TR_NOOP( "Delete" ),
   QT_TR_NOOP( "Pack"   ),
   QT_TR_NOOP( "Quit"   )
};

//*******************************************************************
// QBtButtonSpace                                        CONSTRUCTOR
//*******************************************************************
QBtButtonSpace::QBtButtonSpace( QWidget* const in_parent ) : QWidget( in_parent )
{
   QSignalMapper* const mapper = new QSignalMapper( this );
   connect( mapper, SIGNAL( mapped( int ) ), this, SLOT( clicked( int ) ) );

   QHBoxLayout* const hbt = new QHBoxLayout( this );
   hbt->setMargin( 0 );

   for( qint32 i = F1; i < BUTTONS_COUNT; ++i ) {
      button_[ i ] = new QPushButton;
      button_[ i ]->setFocusPolicy( Qt::NoFocus );
      hbt->addWidget( button_[ i ] );
      connect( button_[ i ], SIGNAL( clicked() ), mapper, SLOT( map() ) );
      mapper->setMapping( button_[ i ], i );
   }
}
// end of QBtButtonSpace

//*******************************************************************
// retranslate                                                PUBLIC
//*******************************************************************
void QBtButtonSpace::retranslate()
{
   QBtConfig* const cfg = QBtConfig::instance();
   const int   mode = cfg->btn_mode();;
   const QFont font = cfg->btn_font();
   
   for( qint32 i = F1; i < BUTTONS_COUNT;  ++i ) {
      const QString number = QString::number( i + 1 );
      const QString label  = tr( LABEL[ i ] );
      QString text = "";

      switch( mode ) {
         case ONLY_FN:
            text = "F" + number;
            break;
         case ONLY_TEXT:
            text = label;
            break;
         case FULL_DESC:
            text  = "F";
            text += number;
            text += ' ';
            text += label;
            break;
         case TWO_LINES:
            text  = "F";
            text += number;
            text += '\n';
            text += label;
         break;
      }
      button_[ i ]->setFont( font );
      button_[ i ]->setText( text );
   }
}
// end of retranslate

//*******************************************************************
// clicked                                              PRIVATE slot
//*******************************************************************
void QBtButtonSpace::clicked( const int in_idx )
{
   if( ( in_idx >= F1 ) && ( in_idx <= F10 ) ) {
      QBtEventsController::instance()->send_event( QBtEvent::F1_KEY + in_idx );
   }
}
// end of clicked
