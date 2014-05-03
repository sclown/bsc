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
 * File         : QBtConfigPanels.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtConfigPanels.h"
#include "QBtConfig.h"
#include "QBtColorDemo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QFont>
#include <QFontDialog>
#include <QGroupBox>
#include <QColorDialog>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtConfigPanels::EXAMPLE        = QT_TR_NOOP( "Example text" );
const char* const QBtConfigPanels::FONT           = QT_TR_NOOP( "&Font" );
const char* const QBtConfigPanels::BACKGROUND     = QT_TR_NOOP( "Background" );
const char* const QBtConfigPanels::FOREGROUND     = QT_TR_NOOP( "Text" );
const char* const QBtConfigPanels::DEFAULT        = QT_TR_NOOP( "Default" );
const char* const QBtConfigPanels::WIDTH_FOCUS    = QT_TR_NOOP( "Item in view width focus" );
const char* const QBtConfigPanels::WIDTHOUT_FOCUS = QT_TR_NOOP( "Item in view widthout focus" );
const char* const QBtConfigPanels::NC_NS          = QT_TR_NOOP( "Normal" );
const char* const QBtConfigPanels::NC_S           = QT_TR_NOOP( "No current, selected" );
const char* const QBtConfigPanels::C_NS           = QT_TR_NOOP( "Current, no selected" );
const char* const QBtConfigPanels::C_S            = QT_TR_NOOP( "Current, selected" );
const char* const QBtConfigPanels::ICO_FONT       = "img/fonts.png";
const char* const QBtConfigPanels::ICO_COLOR      = ICO_COLOR;


//*******************************************************************
// QBtConfigPanels                                       CONSTRUCTOR
//*******************************************************************
QBtConfigPanels::QBtConfigPanels( QWidget* in_parent )
: QWidget        ( in_parent )
, font_          ( new QPushButton( QIcon( ICO_FONT ), tr( FONT ) ) )
, font_example_  ( new QLabel( tr( EXAMPLE ) ) )
, f_nc_ns_demo_  ( new QBtColorDemo( true ) ) 
, f_nc_ns_def_   ( new QPushButton( QIcon( ICO_COLOR ), tr( DEFAULT    ) ) )
, f_nc_ns_bkg_   ( new QPushButton( QIcon( ICO_COLOR ), tr( BACKGROUND ) ) )
, f_nc_ns_txt_   ( new QPushButton( QIcon( ICO_COLOR ), tr( FOREGROUND ) ) )
, f_nc_s_demo_   ( new QBtColorDemo( true ) ) 
, f_nc_s_def_    ( new QPushButton( QIcon( ICO_COLOR ), tr( DEFAULT    ) ) )
, f_nc_s_bkg_    ( new QPushButton( QIcon( ICO_COLOR ), tr( BACKGROUND ) ) )
, f_nc_s_txt_    ( new QPushButton( QIcon( ICO_COLOR ), tr( FOREGROUND ) ) )
, f_c_ns_demo_   ( new QBtColorDemo( true ) ) 
, f_c_ns_def_    ( new QPushButton( QIcon( ICO_COLOR ), tr( DEFAULT    ) ) )
, f_c_ns_bkg_    ( new QPushButton( QIcon( ICO_COLOR ), tr( BACKGROUND ) ) )
, f_c_ns_txt_    ( new QPushButton( QIcon( ICO_COLOR ), tr( FOREGROUND ) ) )
, f_c_s_demo_    ( new QBtColorDemo( true ) ) 
, f_c_s_def_     ( new QPushButton( QIcon( ICO_COLOR ), tr( DEFAULT    ) ) )
, f_c_s_bkg_     ( new QPushButton( QIcon( ICO_COLOR ), tr( BACKGROUND ) ) )
, f_c_s_txt_     ( new QPushButton( QIcon( ICO_COLOR ), tr( FOREGROUND ) ) )
, nf_nc_ns_demo_ ( new QBtColorDemo( true ) ) 
, nf_nc_ns_def_  ( new QPushButton( QIcon( ICO_COLOR ), tr( DEFAULT    ) ) )
, nf_nc_ns_bkg_  ( new QPushButton( QIcon( ICO_COLOR ), tr( BACKGROUND ) ) )
, nf_nc_ns_txt_  ( new QPushButton( QIcon( ICO_COLOR ), tr( FOREGROUND ) ) )
, nf_nc_s_demo_  ( new QBtColorDemo( true ) ) 
, nf_nc_s_def_   ( new QPushButton( QIcon( ICO_COLOR ), tr( DEFAULT    ) ) )
, nf_nc_s_bkg_   ( new QPushButton( QIcon( ICO_COLOR ), tr( BACKGROUND ) ) )
, nf_nc_s_txt_   ( new QPushButton( QIcon( ICO_COLOR ), tr( FOREGROUND ) ) )
, nf_c_ns_demo_  ( new QBtColorDemo( true ) ) 
, nf_c_ns_def_   ( new QPushButton( QIcon( ICO_COLOR ), tr( DEFAULT    ) ) )
, nf_c_ns_bkg_   ( new QPushButton( QIcon( ICO_COLOR ), tr( BACKGROUND ) ) )
, nf_c_ns_txt_   ( new QPushButton( QIcon( ICO_COLOR ), tr( FOREGROUND ) ) )
, nf_c_s_demo_   ( new QBtColorDemo( true ) ) 
, nf_c_s_def_    ( new QPushButton( QIcon( ICO_COLOR ), tr( DEFAULT    ) ) )
, nf_c_s_bkg_    ( new QPushButton( QIcon( ICO_COLOR ), tr( BACKGROUND ) ) )
, nf_c_s_txt_    ( new QPushButton( QIcon( ICO_COLOR ), tr( FOREGROUND ) ) )
{
   f_nc_ns_demo_->setText( tr( NC_NS ) );
   f_nc_s_demo_ ->setText( tr( NC_S ) );
   f_c_ns_demo_ ->setText( tr( C_NS ) );
   f_c_s_demo_  ->setText( tr( C_S ) );
   //............................................
   nf_nc_ns_demo_->setText( tr( NC_NS ) );
   nf_nc_s_demo_ ->setText( tr( NC_S ) );
   nf_c_ns_demo_ ->setText( tr( C_NS ) );
   nf_c_s_demo_  ->setText( tr( C_S ) );
   
   QHBoxLayout* const font_layout = new QHBoxLayout;
   font_layout->addWidget( font_ );
   font_layout->addWidget( font_example_ );
   font_layout->addStretch();
   //--------------------------------------------
   QGroupBox* const fbox = new QGroupBox( tr( WIDTH_FOCUS ) );
   QGridLayout* fbox_layout = new QGridLayout( fbox );
   fbox_layout->addWidget( f_nc_ns_demo_, 0, 0 );
   fbox_layout->addWidget( f_nc_ns_def_ , 0, 1 );
   fbox_layout->addWidget( f_nc_ns_bkg_ , 0, 2 );
   fbox_layout->addWidget( f_nc_ns_txt_ , 0, 3 );
   //............................................
   fbox_layout->addWidget( f_nc_s_demo_ , 1, 0 );
   fbox_layout->addWidget( f_nc_s_def_  , 1, 1 );
   fbox_layout->addWidget( f_nc_s_bkg_  , 1, 2 );
   fbox_layout->addWidget( f_nc_s_txt_  , 1, 3 );
   //............................................
   fbox_layout->addWidget( f_c_ns_demo_ , 2, 0 );
   fbox_layout->addWidget( f_c_ns_def_  , 2, 1 );
   fbox_layout->addWidget( f_c_ns_bkg_  , 2, 2 );
   fbox_layout->addWidget( f_c_ns_txt_  , 2, 3 );
   //............................................
   fbox_layout->addWidget( f_c_s_demo_  , 3, 0 );
   fbox_layout->addWidget( f_c_s_def_   , 3, 1 );
   fbox_layout->addWidget( f_c_s_bkg_   , 3, 2 );
   fbox_layout->addWidget( f_c_s_txt_   , 3, 3 );
   //--------------------------------------------
   QGroupBox* const wfbox = new QGroupBox( tr( WIDTHOUT_FOCUS ) );
   QGridLayout* wfbox_layout = new QGridLayout( wfbox );
   wfbox_layout->addWidget( nf_nc_ns_demo_, 0, 0 );
   wfbox_layout->addWidget( nf_nc_ns_def_ , 0, 1 );
   wfbox_layout->addWidget( nf_nc_ns_bkg_ , 0, 2 );
   wfbox_layout->addWidget( nf_nc_ns_txt_ , 0, 3 );
   //............................................
   wfbox_layout->addWidget( nf_nc_s_demo_ , 1, 0 );
   wfbox_layout->addWidget( nf_nc_s_def_  , 1, 1 );
   wfbox_layout->addWidget( nf_nc_s_bkg_  , 1, 2 );
   wfbox_layout->addWidget( nf_nc_s_txt_  , 1, 3 );
   //............................................
   wfbox_layout->addWidget( nf_c_ns_demo_ , 2, 0 );
   wfbox_layout->addWidget( nf_c_ns_def_  , 2, 1 );
   wfbox_layout->addWidget( nf_c_ns_bkg_  , 2, 2 );
   wfbox_layout->addWidget( nf_c_ns_txt_  , 2, 3 );
   //............................................
   wfbox_layout->addWidget( nf_c_s_demo_  , 3, 0 );
   wfbox_layout->addWidget( nf_c_s_def_   , 3, 1 );
   wfbox_layout->addWidget( nf_c_s_bkg_   , 3, 2 );
   wfbox_layout->addWidget( nf_c_s_txt_   , 3, 3 );
   //--------------------------------------------
   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addLayout( font_layout );
   main_layout->addWidget( fbox );
   main_layout->addWidget( wfbox );
   main_layout->addStretch();
   setLayout( main_layout );

   init();

   connect( font_        , SIGNAL( clicked() ), this, SLOT( select_font () ) );
   //..........................................................................
   connect( f_nc_ns_def_ , SIGNAL( clicked() ), this, SLOT( f_nc_ns_def () ) );
   connect( f_nc_s_def_  , SIGNAL( clicked() ), this, SLOT( f_nc_s_def  () ) );
   connect( f_c_ns_def_  , SIGNAL( clicked() ), this, SLOT( f_c_ns_def  () ) );
   connect( f_c_s_def_   , SIGNAL( clicked() ), this, SLOT( f_c_s_def   () ) );
   //...........................................................................
   connect( nf_nc_ns_def_, SIGNAL( clicked() ), this, SLOT( nf_nc_ns_def() ) );
   connect( nf_nc_s_def_ , SIGNAL( clicked() ), this, SLOT( nf_nc_s_def () ) );
   connect( nf_c_ns_def_ , SIGNAL( clicked() ), this, SLOT( nf_c_ns_def () ) );
   connect( nf_c_s_def_  , SIGNAL( clicked() ), this, SLOT( nf_c_s_def  () ) );
   //...........................................................................
   connect( f_nc_ns_bkg_ , SIGNAL( clicked() ), f_nc_ns_demo_, SLOT( select_bkg() ) );
   connect( f_nc_ns_txt_ , SIGNAL( clicked() ), f_nc_ns_demo_, SLOT( select_clr() ) );
   connect( f_nc_s_bkg_  , SIGNAL( clicked() ), f_nc_s_demo_ , SLOT( select_bkg() ) );
   connect( f_nc_s_txt_  , SIGNAL( clicked() ), f_nc_s_demo_ , SLOT( select_clr() ) );
   connect( f_c_ns_bkg_  , SIGNAL( clicked() ), f_c_ns_demo_ , SLOT( select_bkg() ) );
   connect( f_c_ns_txt_  , SIGNAL( clicked() ), f_c_ns_demo_ , SLOT( select_clr() ) );
   connect( f_c_s_bkg_   , SIGNAL( clicked() ), f_c_s_demo_  , SLOT( select_bkg() ) );
   connect( f_c_s_txt_   , SIGNAL( clicked() ), f_c_s_demo_  , SLOT( select_clr() ) );
   //...........................................................................
   connect( nf_nc_ns_bkg_ , SIGNAL( clicked() ), nf_nc_ns_demo_, SLOT( select_bkg() ) );
   connect( nf_nc_ns_txt_ , SIGNAL( clicked() ), nf_nc_ns_demo_, SLOT( select_clr() ) );
   connect( nf_nc_s_bkg_  , SIGNAL( clicked() ), nf_nc_s_demo_ , SLOT( select_bkg() ) );
   connect( nf_nc_s_txt_  , SIGNAL( clicked() ), nf_nc_s_demo_ , SLOT( select_clr() ) );
   connect( nf_c_ns_bkg_  , SIGNAL( clicked() ), nf_c_ns_demo_ , SLOT( select_bkg() ) );
   connect( nf_c_ns_txt_  , SIGNAL( clicked() ), nf_c_ns_demo_ , SLOT( select_clr() ) );
   connect( nf_c_s_bkg_   , SIGNAL( clicked() ), nf_c_s_demo_  , SLOT( select_bkg() ) );
   connect( nf_c_s_txt_   , SIGNAL( clicked() ), nf_c_s_demo_  , SLOT( select_clr() ) );
}
// end of QBtConfigPanels

//*******************************************************************
// init                                                      PRIVATE
//*******************************************************************
void QBtConfigPanels::init() const
{
   const QBtConfig* const cfg = QBtConfig::instance();
   font_example_->setFont( cfg->font() );
   //--------------------------------------------
   f_nc_ns_demo_->fonts     ( cfg->font() );
   f_nc_ns_demo_->background( cfg->bkg_f_nc_ns_color() );
   f_nc_ns_demo_->color     ( cfg->txt_f_nc_ns_color() );
   //............................................
   f_nc_s_demo_->fonts      ( cfg->font() );
   f_nc_s_demo_->background ( cfg->bkg_f_nc_s_color() );
   f_nc_s_demo_->color      ( cfg->txt_f_nc_s_color() );
   //............................................
   f_c_ns_demo_->fonts      ( cfg->font() );
   f_c_ns_demo_->background ( cfg->bkg_f_c_ns_color() );
   f_c_ns_demo_->color      ( cfg->txt_f_c_ns_color() );
   //............................................
   f_c_s_demo_->fonts       ( cfg->font() );
   f_c_s_demo_->background  ( cfg->bkg_f_c_s_color() );
   f_c_s_demo_->color       ( cfg->txt_f_c_s_color() );
   //--------------------------------------------
   nf_nc_ns_demo_->fonts     ( cfg->font() );
   nf_nc_ns_demo_->background( cfg->bkg_nf_nc_ns_color() );
   nf_nc_ns_demo_->color     ( cfg->txt_nf_nc_ns_color() );
   //............................................
   nf_nc_s_demo_->fonts      ( cfg->font() );
   nf_nc_s_demo_->background ( cfg->bkg_nf_nc_s_color() );
   nf_nc_s_demo_->color      ( cfg->txt_nf_nc_s_color() );
   //............................................
   nf_c_ns_demo_->fonts      ( cfg->font() );
   nf_c_ns_demo_->background ( cfg->bkg_nf_c_ns_color() );
   nf_c_ns_demo_->color      ( cfg->txt_nf_c_ns_color() );
   //............................................
   nf_c_s_demo_->fonts       ( cfg->font() );
   nf_c_s_demo_->background  ( cfg->bkg_nf_c_s_color() );
   nf_c_s_demo_->color       ( cfg->txt_nf_c_s_color() );
}
// end of init

//*******************************************************************
// save                                                      PRIVATE
//*******************************************************************
void QBtConfigPanels::save() const
{
   QBtConfig* const cfg = QBtConfig::instance();

   cfg->font( font_example_->font() );
   //--------------------------------------------
   cfg->bkg_f_nc_ns_color ( f_nc_ns_demo_->background() );
   cfg->txt_f_nc_ns_color ( f_nc_ns_demo_->color     () );
   //............................................
   cfg->bkg_f_nc_s_color  ( f_nc_s_demo_->background () );
   cfg->txt_f_nc_s_color  ( f_nc_s_demo_->color      () );
   //............................................
   cfg->bkg_f_c_ns_color  ( f_c_ns_demo_->background () );
   cfg->txt_f_c_ns_color  ( f_c_ns_demo_->color      () );
   //............................................
   cfg->bkg_f_c_s_color   ( f_c_s_demo_->background  () );
   cfg->txt_f_c_s_color   ( f_c_s_demo_->color       () );
   //--------------------------------------------
   cfg->bkg_nf_nc_ns_color( nf_nc_ns_demo_->background() );
   cfg->txt_nf_nc_ns_color( nf_nc_ns_demo_->color     () );
   //............................................
   cfg->bkg_nf_nc_s_color ( nf_nc_s_demo_->background () );
   cfg->txt_nf_nc_s_color ( nf_nc_s_demo_->color      () );
   //............................................
   cfg->bkg_nf_c_ns_color ( nf_c_ns_demo_->background () );
   cfg->txt_nf_c_ns_color ( nf_c_ns_demo_->color      () );
   //............................................
   cfg->bkg_nf_c_s_color  ( nf_c_s_demo_->background  () );
   cfg->txt_nf_c_s_color  ( nf_c_s_demo_->color       () );

   cfg->save_data();
}
// end of save

//*******************************************************************
// select_font                                          PRIVATE slot
//*******************************************************************
void QBtConfigPanels::select_font()
{
   bool ok = bool();
   const QFont old_font = font_example_->font();
   const QFont new_font = QFontDialog::getFont( &ok, old_font, this );
   if( ok ) {
      if( old_font != new_font ) {
         font_example_ ->setFont( new_font );
         f_nc_ns_demo_ ->setFont( new_font );
         f_nc_s_demo_  ->setFont( new_font );
         f_c_ns_demo_  ->setFont( new_font );
         f_c_s_demo_   ->setFont( new_font );
         nf_nc_ns_demo_->setFont( new_font );
         nf_nc_s_demo_ ->setFont( new_font );
         nf_c_ns_demo_ ->setFont( new_font );
         nf_c_s_demo_  ->setFont( new_font );
      }
   }
}
// end of select_font

//###################################################################
//#                                                                 #
//#               PRZYWROCENIE USTAWIEN DOMYSLNYCH                  #
//#                                                                 #
//###################################################################


//*******************************************************************
// f_nc_ns_def                                          PRIVATE slot
//*******************************************************************
void QBtConfigPanels::f_nc_ns_def()
{
   const QBtConfig* const cfg = QBtConfig::instance();
   f_nc_ns_demo_->background( cfg->bkg_f_nc_ns_def_color() );
   f_nc_ns_demo_->color     ( cfg->txt_f_nc_ns_def_color() );
}
//end of f_nc_ns_def

//*******************************************************************
// f_nc_s_def                                           PRIVATE slot
//*******************************************************************
void QBtConfigPanels::f_nc_s_def()
{
   const QBtConfig* const cfg = QBtConfig::instance();
   f_nc_s_demo_->background( cfg->bkg_f_nc_s_def_color() );
   f_nc_s_demo_->color     ( cfg->txt_f_nc_s_def_color() );
}
//end of f_nc_s_def

//*******************************************************************
// f_c_ns_def                                           PRIVATE slot
//*******************************************************************
void QBtConfigPanels::f_c_ns_def()
{
   const QBtConfig* const cfg = QBtConfig::instance();
   f_c_ns_demo_->background( cfg->bkg_f_c_ns_def_color() );
   f_c_ns_demo_->color     ( cfg->txt_f_c_ns_def_color() );
}
//end of f_c_ns_def

//*******************************************************************
// f_c_s_def                                            PRIVATE slot
//*******************************************************************
void QBtConfigPanels::f_c_s_def()
{
   const QBtConfig* const cfg = QBtConfig::instance();
   f_c_s_demo_->background( cfg->bkg_f_c_s_def_color() );
   f_c_s_demo_->color     ( cfg->txt_f_c_s_def_color() );
}
//end of f_c_s_def

//*******************************************************************
// nf_nc_ns_def                                         PRIVATE slot
//*******************************************************************
void QBtConfigPanels::nf_nc_ns_def()
{
   const QBtConfig* const cfg = QBtConfig::instance();
   nf_nc_ns_demo_->background( cfg->bkg_nf_nc_ns_def_color() );
   nf_nc_ns_demo_->color     ( cfg->txt_nf_nc_ns_def_color() );
}
//end of nf_nc_ns_def

//*******************************************************************
// nf_nc_s_def                                          PRIVATE slot
//*******************************************************************
void QBtConfigPanels::nf_nc_s_def()
{
   const QBtConfig* const cfg = QBtConfig::instance();
   nf_nc_s_demo_->background( cfg->bkg_nf_nc_s_def_color() );
   nf_nc_s_demo_->color     ( cfg->txt_nf_nc_s_def_color() );
}
//end of nf_nc_s_def

//*******************************************************************
// nf_c_ns_def                                          PRIVATE slot
//*******************************************************************
void QBtConfigPanels::nf_c_ns_def()
{
   const QBtConfig* const cfg = QBtConfig::instance();
   nf_c_ns_demo_->background( cfg->bkg_nf_c_ns_def_color() );
   nf_c_ns_demo_->color     ( cfg->txt_nf_c_ns_def_color() );
}
//end of nf_c_ns_def

//*******************************************************************
// nf_c_s_def                                           PRIVATE slot
//*******************************************************************
void QBtConfigPanels::nf_c_s_def()
{
   const QBtConfig* const cfg = QBtConfig::instance();
   nf_c_s_demo_->background( cfg->bkg_nf_c_s_def_color() );
   nf_c_s_demo_->color     ( cfg->txt_nf_c_s_def_color() );
}
//end of nf_c_s_def
