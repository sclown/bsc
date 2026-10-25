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
#include <QPushButton>
#include <QSignalMapper>
#include <QHBoxLayout>
#include <QKeySequence>
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
   connect( mapper, SIGNAL( mappedInt( int ) ), this, SLOT( clicked( int ) ) );

   QHBoxLayout* const hbt = new QHBoxLayout( this );
   hbt->setContentsMargins( 0, 0, 0, 0 );

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
   for( qint32 i = F1; i < BUTTONS_COUNT;  ++i ) {
      const QString number = QString::number( i + 1 );
      const QString label  = tr( LABEL[ i ] );

      // Captions show the plain number: the button answers to both
      // F<n> and Alt(Option)+<n> (Alt+0 for F10).
      const QString digit = QString::number( ( i + 1 ) % 10 );
      button_[ i ]->setText( number + ' ' + label );
      button_[ i ]->setToolTip( QString( "%1 (F%2, %3)" ).arg( label, number,
         QKeySequence( Qt::ALT | ( Qt::Key_0 + digit.toInt() ) ).toString( QKeySequence::NativeText ) ) );
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
