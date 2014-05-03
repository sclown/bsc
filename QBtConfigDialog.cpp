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
 * File         : QBtConfigDialog.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtConfigDialog.h"
#include "QBtConfigPanels.h"
#include "QBtConfigButtons.h"
#include "QBtConfigApps.h"
#include "QBtConfigExtensions.h"
#include "QBtEventsController.h"
#include <QTabBar>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtConfigDialog::CAPTION    = QT_TR_NOOP( "Program configuration" );
const char* const QBtConfigDialog::PANELS     = QT_TR_NOOP( "&Panels" );
const char* const QBtConfigDialog::BUTTONS    = QT_TR_NOOP( "Function &buttons" );
const char* const QBtConfigDialog::CONSOLE    = QT_TR_NOOP( "Applications" );
const char* const QBtConfigDialog::EXTENSIONS = QT_TR_NOOP( "E&xtensions" );
const char* const QBtConfigDialog::APPLY      = QT_TR_NOOP( "&Apply" );
const char* const QBtConfigDialog::EXIT       = QT_TR_NOOP( "&Exit" );


//*******************************************************************
// QBtConfigDialog                                       CONSTRUCTOR
//*******************************************************************
QBtConfigDialog::QBtConfigDialog( QWidget* const in_parent ) : QDialog( in_parent )
, tbar_       ( new QTabBar )
, wstack_     ( new QStackedWidget )
, apply_      ( new QPushButton( tr( APPLY ) ) )
, exit_       ( new QPushButton( tr( EXIT  ) ) )
, panels_     ( new QBtConfigPanels )
, buttons_    ( new QBtConfigButtons )
, apps_       ( new QBtConfigApps )
, extensions_ ( new QBtConfigExtensions )
{
   setWindowTitle( tr( CAPTION ) );

   tbar_->insertTab( PANELS_IDX    , tr( PANELS     ) );
   tbar_->insertTab( BUTTONS_IDX   , tr( BUTTONS    ) );
   tbar_->insertTab( CONSOLE_IDX   , tr( CONSOLE    ) );
   tbar_->insertTab( EXTENSIONS_IDX, tr( EXTENSIONS ) );

   wstack_->insertWidget( PANELS_IDX    , panels_     );
   wstack_->insertWidget( BUTTONS_IDX   , buttons_    );
   wstack_->insertWidget( CONSOLE_IDX   , apps_    );
   wstack_->insertWidget( EXTENSIONS_IDX, extensions_ );

   QHBoxLayout* const hb = new QHBoxLayout;
   hb->addStretch( 100 );
   hb->addWidget( apply_ );
   hb->addWidget( exit_ );
   
   QVBoxLayout* vb = new QVBoxLayout( this );
   vb->setMargin( 2 );
   vb->addWidget( tbar_ );
   vb->addWidget( wstack_ );
   vb->addLayout( hb );

   connect( tbar_ , SIGNAL( currentChanged(int) ), this, SLOT( tab_changed(int) ) );
   connect( apply_, SIGNAL( clicked() ), this, SLOT( apply () ) );
   connect( exit_ , SIGNAL( clicked() ), this, SLOT( reject() ) );
}
// end of QBtConfigDialog

//*******************************************************************
// tab_changed                                          PRIVATE slot
//*******************************************************************
void QBtConfigDialog::tab_changed( int in_idx )
{
   wstack_->setCurrentIndex( in_idx );
}
// end of tab_changed

//*******************************************************************
// apply                                                PRIVATE slot
//*******************************************************************
void QBtConfigDialog::apply()
{
   buttons_->save();
   apps_->save();
   extensions_->save();
   panels_->save();
   QBtEventsController::instance()->send_event( QBtEvent::CONFIG_CHANGED );
}
// end of apply
