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
 * File         : QBtCanOverwrite.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 03.09.2007
 *******************************************************************/

/*-------- include files:
-------------------------------------------------------------------*/
#include "QBtCanOverwrite.h"
#include "QBtShared.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtCanOverwrite::CAPTION        = QT_TR_NOOP( "Question about overwriting" );
const char* const QBtCanOverwrite::MESSAGE        = QT_TR_NOOP( "Destination file already exists:" );
const char* const QBtCanOverwrite::SKIP           = QT_TR_NOOP( "&Skip" );
const char* const QBtCanOverwrite::OVERWRITE      = QT_TR_NOOP( "&Overwrite" );
const char* const QBtCanOverwrite::UPDATE         = QT_TR_NOOP( "&Update" );
const char* const QBtCanOverwrite::RENAME         = QT_TR_NOOP( "&Rename" );
const char* const QBtCanOverwrite::DONT_ASK_AGAIN = QT_TR_NOOP( "Don't ask again" );
const char* const QBtCanOverwrite::CANCEL         = QT_TR_NOOP( "&Cancel" );


//*******************************************************************
// QBtCanOverwrite                                       CONSTRUCTOR
//*******************************************************************
QBtCanOverwrite::QBtCanOverwrite( QWidget* const in_parent, const QString& in_path ) : QDialog( in_parent )
, dont_ask_ ( new QCheckBox  ( tr( DONT_ASK_AGAIN ) ) )
, skip_     ( new QPushButton( tr( SKIP           ) ) )
, overwrite_( new QPushButton( tr( OVERWRITE      ) ) )
, update_   ( new QPushButton( tr( UPDATE         ) ) )
, rename_   ( new QPushButton( tr( RENAME         ) ) )
, cancel_   ( new QPushButton( tr( CANCEL         ) ) )
, path_     ( new QLabel         ( in_path        ) )
{
   setWindowTitle( tr( CAPTION ) );

   QFrame* const frame = new QFrame;
   frame->setLineWidth( 0 );
   frame->setMidLineWidth( 1 );
   frame->setFrameShape( QFrame::Box );
   frame->setFrameShadow( QFrame::Sunken );
   QVBoxLayout* const frame_layout = new QVBoxLayout( frame );
   frame_layout->addWidget( new QLabel( tr( MESSAGE ) ) );
   frame_layout->addWidget( path_ );
   QPalette p = path_->palette();
   p.setColor( path_->foregroundRole(), Qt::red );
   path_->setPalette( p );

   // Dolna czesc dialogu z przyciskami.
   QVBoxLayout* const cbx_layout = new QVBoxLayout;
   cbx_layout->addWidget( dont_ask_ );
   cbx_layout->addStretch( 100 );
   // przyciski
   QGridLayout* const grid = new QGridLayout;
   grid->setSpacing( 3 );
   grid->addWidget( rename_   , 0, 0 );
   grid->addWidget( update_   , 0, 1 );
   grid->addWidget( overwrite_, 1, 1 );
   grid->addWidget( skip_     , 0, 2 );
   grid->addWidget( cancel_   , 1, 2 );

   QHBoxLayout* const btn_layout = new QHBoxLayout;
   btn_layout->addLayout( cbx_layout );
   btn_layout->addLayout( grid );
   btn_layout->setStretchFactor( cbx_layout, 100 );
   
   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addWidget( frame );
   main_layout->addLayout( btn_layout );
   setLayout( main_layout );

   connect( skip_     , SIGNAL( clicked() ), this, SLOT( skip     () ) );
   connect( overwrite_, SIGNAL( clicked() ), this, SLOT( overwrite() ) );
   connect( update_   , SIGNAL( clicked() ), this, SLOT( update   () ) );
   connect( rename_   , SIGNAL( clicked() ), this, SLOT( rename   () ) );
   connect( cancel_   , SIGNAL( clicked() ), this, SLOT( cancel   () ) );
}
// end of QBtCanOverwrite

//*******************************************************************
// ask_again                                                  PUBLIC
//*******************************************************************
bool QBtCanOverwrite::ask_again() const
{
   return !dont_ask_->isChecked();
}
// end of ask_again

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtCanOverwrite::showEvent( QShowEvent* in_event )
{
   QBtShared::resize_width( this, 35 );
   QDialog::showEvent( in_event );
}
// end of showEvent

//*******************************************************************
// skip                                                 PRIVATE slot
//*******************************************************************
void QBtCanOverwrite::skip()
{
   done( SKIP_FILE );
}
// end of skip

//*******************************************************************
// overwrite                                            PRIVATE slot
//*******************************************************************
void QBtCanOverwrite::overwrite()
{
   done( OVERWRITE_FILE );
}
// end of overwrite

//*******************************************************************
// update                                               PRIVATE slot
//*******************************************************************
void QBtCanOverwrite::update()
{
   done( UPDATE_FILE );
}
// end of update

//*******************************************************************
// rename                                               PRIVATE slot
//*******************************************************************
void QBtCanOverwrite::rename()
{
   done( RENAME_FILE );
}
// end of rename

//*******************************************************************
// cancel                                               PRIVATE slot
//*******************************************************************
void QBtCanOverwrite::cancel()
{
   done( CANCEL_FILE );
}
// end of cancel
