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
 * File         : QBtConfigButtons.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtConfigButtons.h"
#include "QBtConfig.h"
#include "QBtButtonSpace.h"
#include "QBtSettings.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QFontDialog>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtConfigButtons::CAPTION_MODE    = QT_TR_NOOP( "Display mode" );
const char* const QBtConfigButtons::CAPTION_FONT    = QT_TR_NOOP( "Used font" );
const char* const QBtConfigButtons::CAPTION_EXAMPLE = QT_TR_NOOP( "Example" );
const char* const QBtConfigButtons::CAPTION_FONTS   = QT_TR_NOOP( "Accessible fonts" );
const char* const QBtConfigButtons::ONLY_FN         = QT_TR_NOOP( "&Only function key" );
const char* const QBtConfigButtons::ONLY_TEXT       = QT_TR_NOOP( "Only &description" );
const char* const QBtConfigButtons::FULL_DESC       = QT_TR_NOOP( "&Full description" );
const char* const QBtConfigButtons::TWO_LINES       = QT_TR_NOOP( "&Two line description" );
const char* const QBtConfigButtons::DEFAULT_FONT    = QT_TR_NOOP( "&Default font" );
const char* const QBtConfigButtons::CUSTOM_FONT     = QT_TR_NOOP( "&Custom font" );
const char* const QBtConfigButtons::EXAMPLE         = QT_TR_NOOP( "Example" );
const char* const QBtConfigButtons::SELECT_FONT     = QT_TR_NOOP( "Fonts" );
const char* const QBtConfigButtons::FONT_ICON       = ":/img/fonts.png";


//*******************************************************************
// QBtConfigButtons                                       CONSTRUCTOR
//*******************************************************************
QBtConfigButtons::QBtConfigButtons( QWidget* in_parent ) : QWidget( in_parent )
, only_fn_      ( new QRadioButton( tr( ONLY_FN      ) ) )
, only_text_    ( new QRadioButton( tr( ONLY_TEXT    ) ) )
, full_desc_    ( new QRadioButton( tr( FULL_DESC    ) ) )
, two_lines_    ( new QRadioButton( tr( TWO_LINES    ) ) )
, default_font_ ( new QRadioButton( tr( DEFAULT_FONT ) ) )
, custom_font_  ( new QRadioButton( tr( CUSTOM_FONT  ) ) )
, select_font_  ( new QPushButton( QIcon( FONT_ICON), tr( SELECT_FONT ) ) )
, example_      ( new QPushButton )
, font_desc_    ( new QLabel )
, font_         ( QFont() )
, mode_         ( QBtButtonSpace::FULL_DESC )
{
   //------- Mode -------
   QVBoxLayout* const vb_mode = new QVBoxLayout;
   vb_mode->addWidget( only_fn_ );
   vb_mode->addWidget( only_text_ );
   vb_mode->addWidget( full_desc_ );
   vb_mode->addWidget( two_lines_ );
   vb_mode->addStretch();
   QGroupBox* const gbox_mode = new QGroupBox( tr( CAPTION_MODE ) );
   gbox_mode->setLayout( vb_mode );

   //------- Font -------
   QVBoxLayout* const vb_font = new QVBoxLayout;
   vb_font->addWidget( default_font_ );
   vb_font->addWidget( custom_font_ );
   vb_font->addWidget( select_font_ );
   vb_font->addStretch( 100 );
   QGroupBox* const gbox_font = new QGroupBox( tr( CAPTION_FONT ) );
   gbox_font->setLayout( vb_font );

   //------- Example -------
   QVBoxLayout* const vb_example = new QVBoxLayout;
   vb_example->addStretch( 100 );
   vb_example->addWidget( example_ );
   vb_example->addWidget( font_desc_ );
   vb_example->addStretch( 100 );
   QGroupBox* const gbox_example = new QGroupBox( tr( CAPTION_EXAMPLE ) );
   gbox_example->setLayout( vb_example );
   
   QHBoxLayout* const hb_main = new QHBoxLayout( this );
   hb_main->addWidget( gbox_mode );
   hb_main->addWidget( gbox_font );
   hb_main->addWidget( gbox_example );

   connect( only_fn_     , SIGNAL( toggled(bool) ), this, SLOT( only_fn     (bool) ) );
   connect( only_text_   , SIGNAL( toggled(bool) ), this, SLOT( only_text   (bool) ) );
   connect( full_desc_   , SIGNAL( toggled(bool) ), this, SLOT( full_desc   (bool) ) );
   connect( two_lines_   , SIGNAL( toggled(bool) ), this, SLOT( two_lines   (bool) ) );
   connect( default_font_, SIGNAL( toggled(bool) ), this, SLOT( default_font(bool) ) );
   connect( custom_font_ , SIGNAL( toggled(bool) ), this, SLOT( custom_font (bool) ) );
   connect( select_font_ , SIGNAL( clicked()     ), this, SLOT( select_font ()     ) );
   
   restore();
   update_state();
}
// end of QBtConfigButtons

//*******************************************************************
// save                                                       PUBLIC
//*******************************************************************
void QBtConfigButtons::save() const
{
   QBtConfig* const cfg = QBtConfig::instance();
   cfg->btn_mode( mode_ );
   cfg->btn_font( font_ );
   cfg->save_data();
}
// end of save

//*******************************************************************
// restore                                                   PRIVATE
//*******************************************************************
void QBtConfigButtons::restore()
{
   QBtConfig* const cfg = QBtConfig::instance();
   mode_ = cfg->btn_mode();
   font_ = cfg->btn_font();
}
// end of restore

//*******************************************************************
// update_state                                              PRIVATE
//*******************************************************************
void QBtConfigButtons::update_state()
{
   switch( mode_ ) {
      case QBtButtonSpace::ONLY_FN:
         only_fn_->setChecked( true );
         break;
      case QBtButtonSpace::ONLY_TEXT:
         only_text_->setChecked( true );
         break;
      case QBtButtonSpace::FULL_DESC:
         full_desc_->setChecked( true );
         break;
      case QBtButtonSpace::TWO_LINES:
         two_lines_->setChecked( true );
         break;
   }
   
   if( font_ == QBtConfig::instance()->font() ) {
      default_font_->setChecked( true );
   }
   else {
      custom_font_->setChecked( true );
   }
}
// end of update_state

//*******************************************************************
// update_example                                            PRIVATE
//*******************************************************************
void QBtConfigButtons::update_example()
{
   QString text = "";
   
   switch( mode_ ) {
      case QBtButtonSpace::ONLY_FN:
         text = "F44";
         break;
      case QBtButtonSpace::ONLY_TEXT:
         text = tr( EXAMPLE );
         break;
      case QBtButtonSpace::FULL_DESC:
         text  = "F44";
         text += ' ';
         text += tr( EXAMPLE );
         break;
      case QBtButtonSpace::TWO_LINES:
         text  = "F44";
         text += '\n';
         text += tr( EXAMPLE );
         break;
   }
   example_->setFont( font_ );
   example_->setText( text );
   font_desc_->setFont( font_ );
   font_desc_->setText( font_.toString() );
}
// end of update_example

//*******************************************************************
// only_fn                                              PRIVATE slot
//*******************************************************************
void QBtConfigButtons::only_fn( const bool in_state )
{
   if( in_state ) {
      mode_ = QBtButtonSpace::ONLY_FN;
      update_example();
   }
}
// end of only_fn

//*******************************************************************
// only_text                                            PRIVATE slot
//*******************************************************************
void QBtConfigButtons::only_text( const bool in_state )
{
   if( in_state ) {
      mode_ = QBtButtonSpace::ONLY_TEXT;
      update_example();
   }
}
// end of only_text

//*******************************************************************
// full_desc                                            PRIVATE slot
//*******************************************************************
void QBtConfigButtons::full_desc( const bool in_state )
{
   if( in_state ) {
      mode_ = QBtButtonSpace::FULL_DESC;
      update_example();
   }
}
// end of full_desc

//*******************************************************************
// two_line                                             PRIVATE slot
//*******************************************************************
void QBtConfigButtons::two_lines( const bool in_state )
{
   if( in_state ) {
      mode_ = QBtButtonSpace::TWO_LINES;
      update_example();
   }
}
// end of two_lines

//*******************************************************************
// default_font                                         PRIVATE slot
//*******************************************************************
void QBtConfigButtons::default_font( const bool in_state )
{
   if( in_state ) {
      select_font_->setEnabled( false );
      font_ = QBtConfig::instance()->btn_font();
      update_example();
   }
}
// end of default_font

//*******************************************************************
// custom_font                                          PRIVATE slot
//*******************************************************************
void QBtConfigButtons::custom_font( const bool in_state )
{
   if( in_state ) {
      select_font_->setEnabled( true );
   }
}
// end of custom_font

//*******************************************************************
// select_font                                          PRIVATE slot
//*******************************************************************
void QBtConfigButtons::select_font()
{
   bool ok;
   const QFont new_font = QFontDialog::getFont( &ok, font_, this, tr( CAPTION_FONTS ) );
   if( ok ) {
      font_ = new_font;
      update_example();
   }
}
// end of select_font
