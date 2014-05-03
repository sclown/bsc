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
 * File         : QBtConfigApps.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtConfigApps.h"
#include "QBtConfig.h"
#include "QBtSettings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QGroupBox>

/*------- local constants:
-------------------------------------------------------------------*/
const char* const QBtConfigApps::CONSOLE         = QT_TR_NOOP( "Console" );
const char* const QBtConfigApps::DEFAULT_CONSOLE = QT_TR_NOOP( "&default console" );
const char* const QBtConfigApps::OTHER_CONSOLE   = QT_TR_NOOP( "&other console" );

const char* const QBtConfigApps::DIFF         = QT_TR_NOOP( "Diff program" );
const char* const QBtConfigApps::DEFAULT_DIFF = QT_TR_NOOP( "&default diff (beediff)" );
const char* const QBtConfigApps::OTHER_DIFF   = QT_TR_NOOP( "&other diff" );


//*******************************************************************
// QBtConfigApps                                       CONSTRUCTOR
//*******************************************************************
QBtConfigApps::QBtConfigApps( QWidget* in_parent ) : QWidget( in_parent )
, console_default_ ( new QRadioButton( tr( DEFAULT_CONSOLE ) ) )
, console_other_   ( new QRadioButton( tr( OTHER_CONSOLE   ) ) )
, console_command_ ( new QLineEdit )
, diff_default_    ( new QRadioButton( tr( DEFAULT_DIFF ) ) )
, diff_other_      ( new QRadioButton( tr( OTHER_DIFF   ) ) )
, diff_command_    ( new QLineEdit )
{
   console_default_->setChecked( true  );
   console_other_  ->setChecked( false );
   console_command_->setEnabled( false );
   diff_default_   ->setChecked( true  );
   diff_other_     ->setChecked( false );
   diff_command_   ->setEnabled( false );

   
   // console
   QHBoxLayout* const other_console_layout = new QHBoxLayout;
   other_console_layout->addWidget( console_other_ );
   other_console_layout->addWidget( console_command_ );

   QGroupBox* const console_gbox = new QGroupBox( tr( CONSOLE ) );  
   QVBoxLayout* const console_layout = new QVBoxLayout( console_gbox );
   console_layout->addWidget( console_default_ );
   console_layout->addLayout( other_console_layout );

   // diff
   QHBoxLayout* const other_diff_layout = new QHBoxLayout;
   other_diff_layout->addWidget( diff_other_   );
   other_diff_layout->addWidget( diff_command_ );

   QGroupBox* const diff_gbox = new QGroupBox( tr( DIFF ) );  
   QVBoxLayout* const diff_layout = new QVBoxLayout( diff_gbox );
   diff_layout->addWidget( diff_default_ );
   diff_layout->addLayout( other_diff_layout );


   // main layout
   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addWidget( console_gbox );
   main_layout->addWidget( diff_gbox );
   main_layout->addStretch();
   setLayout( main_layout );

   connect( console_other_, SIGNAL( toggled( bool ) ),
            this          , SLOT  ( other_console_selected( bool ) ) );
   connect( diff_other_   , SIGNAL( toggled( bool ) ),
            this          , SLOT  ( other_diff_selected( bool ) ) );

   restore_state();
}
// end of QBtConfigApps

//*******************************************************************
// other_console_selected                               PRIVATE slot
//*******************************************************************
void QBtConfigApps::other_console_selected( const bool in_state )
{
   console_command_->setEnabled( in_state );
}
// end of other_console_selected

//*******************************************************************
// other_diff_selected                                  PRIVATE slot
//*******************************************************************
void QBtConfigApps::other_diff_selected( const bool in_state )
{
   diff_command_->setEnabled( in_state );
}
// end of other_diff_selected

//*******************************************************************
// restore_state                                             PRIVATE
//*******************************************************************
void QBtConfigApps::restore_state()
{
   QBtSettings stt;
   QVariant data;
   
   if( stt.read( QBtConfig::TERMINAL_GROUP + QBtConfig::USE_DEFAULT_KEY, data ) ) {
      data.toBool() ? console_default_->setChecked( true ) : console_other_->setChecked( true );
   }
   if( stt.read( QBtConfig::TERMINAL_GROUP + QBtConfig::COMMAND_KEY, data ) ) {
      console_command_->setText( data.toString().trimmed() );
   }
   if( stt.read( QBtConfig::DIFF_GROUP + QBtConfig::USE_DEFAULT_KEY, data ) ) {
      data.toBool() ? diff_default_->setChecked( true ) : diff_other_->setChecked( true );
   }
   if( stt.read( QBtConfig::DIFF_GROUP + QBtConfig::COMMAND_KEY, data ) ) {
      diff_command_->setText( data.toString().trimmed() );
   }
}
// end of restore_state

//*******************************************************************
// save_state                                                PRIVATE
//*******************************************************************
void QBtConfigApps::save_state()
{
   QBtSettings stt;
   stt.save( QBtConfig::TERMINAL_GROUP + QBtConfig::USE_DEFAULT_KEY, console_default_->isChecked() );
   stt.save( QBtConfig::TERMINAL_GROUP + QBtConfig::COMMAND_KEY    , console_command_->text().trimmed() );
   stt.save( QBtConfig::DIFF_GROUP     + QBtConfig::USE_DEFAULT_KEY, diff_default_->isChecked() );
   stt.save( QBtConfig::DIFF_GROUP     + QBtConfig::COMMAND_KEY    , diff_command_->text().trimmed() );
}
// end of save_state

//*******************************************************************
// save                                                       PUBLIC
//*******************************************************************
void QBtConfigApps::save()
{
   save_state();
}
// end of save
