/********************************************************************
 * Copyright (C) 2005-2007 Piotr Pszczolkowski
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
 * File         : QBtMainWindow.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 24.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtMainWindow.h"
#include "QBtSettings.h"
#include "QBtConfig.h"
#include "QBtShared.h"
#include "QBtEventsController.h"
#include "QBtSystemInfo.h"
#include "QBtWorkspace.h"
#include "QBtButtonSpace.h"
#include "QBtEventsController.h"
#include "QBtConfigDialog.h"
#include "QBtHelp.h"
#include "QBtFinder.h"
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QIcon>
#include <QMessageBox>
#include <QToolBar>
#include <QVBoxLayout>
#include <QFrame>
#include <QKeyEvent>
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const QString     QBtMainWindow::IMG_LOC         = ":/img/";
//------- Menu -------
const char* const QBtMainWindow::MENU_SYSTEM     = QT_TR_NOOP( "&System" );
const char* const QBtMainWindow::MENU_OPERATIONS = QT_TR_NOOP( "&Operations" );
const char* const QBtMainWindow::MENU_LANGS      = QT_TR_NOOP( "Lan&guages" );
const char* const QBtMainWindow::MENU_HELP       = QT_TR_NOOP( "&Help" );
//------- System -------
const char* const QBtMainWindow::REFRESH_ICON   = "refresh.png";
const char* const QBtMainWindow::FILTER_ICON    = "filter.png";
const char* const QBtMainWindow::CONFIGURE_ICON = "configure.png";
const char* const QBtMainWindow::EXIT_ICON      = "exit.png";
const char* const QBtMainWindow::REFRESH        = QT_TR_NOOP( "&Refresh files window" );
const char* const QBtMainWindow::FILTER         = QT_TR_NOOP( "&File filter" );
const char* const QBtMainWindow::CONFIGURE      = QT_TR_NOOP( "&Configure" );
const char* const QBtMainWindow::EXIT           = QT_TR_NOOP( "E&xit" );
const char* const QBtMainWindow::FILTER_ACCEL   = QT_TR_NOOP( "Ctrl+F" );
const char* const QBtMainWindow::EXIT_ACCEL     = QT_TR_NOOP( "Ctrl+Q" );
//------- Operations -------
const char* const QBtMainWindow::NEW_TAB_ICON        = "tab_new.png";
const char* const QBtMainWindow::DEL_TAB_ICON        = "tab_remove.png";
const char* const QBtMainWindow::SWITCH_TABS_ICON    = "switch.png";
const char* const QBtMainWindow::FILE_JOIN_ICON      = "joining.png";
const char* const QBtMainWindow::DIR_TREE_ICON       = "tree.png";
const char* const QBtMainWindow::CMP_FILES_ICON      = "cmpfiles.png";
const char* const QBtMainWindow::CMP_DIRS_ICON       = "cmpdir.png";
const char* const QBtMainWindow::DIR_SYNC_ICON       = "dirsynch.png";
const char* const QBtMainWindow::DATE_TIME_ICON      = "datetime.png";
const char* const QBtMainWindow::SELECT_MASK_ICON    = "select.png";
const char* const QBtMainWindow::UNSELECT_MASK_ICON  = "unselect.png";
const char* const QBtMainWindow::SELECT_REV_ICON     = "revers.png";
const char* const QBtMainWindow::FTP_CONNECT_ICON    = "connect.png";
const char* const QBtMainWindow::FTP_DISCONNECT_ICON = "disconnect.png";
const char* const QBtMainWindow::SYSTEM_INFO_ICON    = "system.png";
const char* const QBtMainWindow::FIND_ICON           = "find.png";
const char* const QBtMainWindow::NEW_TAB             = QT_TR_NOOP( "&Create new tab" );
const char* const QBtMainWindow::DEL_TAB             = QT_TR_NOOP( "&Remove current tab" );
const char* const QBtMainWindow::SWITCH_TABS         = QT_TR_NOOP( "&Switch panels" );
const char* const QBtMainWindow::FILE_JOIN           = QT_TR_NOOP( "Files &joining" );
const char* const QBtMainWindow::DIR_TREE            = QT_TR_NOOP( "&Tree of directories" );
const char* const QBtMainWindow::CMP_FILES           = QT_TR_NOOP( "Compare &files" );
const char* const QBtMainWindow::CMP_DIRS            = QT_TR_NOOP( "Compare &directories" );
const char* const QBtMainWindow::DIR_SYNC            = QT_TR_NOOP( "Directories synchronisation" );
const char* const QBtMainWindow::DATE_TIME           = QT_TR_NOOP( "&Last date/time modification" );
const char* const QBtMainWindow::SELECT_MASK         = QT_TR_NOOP( "&Select files via mask" );
const char* const QBtMainWindow::UNSELECT_MASK       = QT_TR_NOOP( "&Unselect files via mask" );
const char* const QBtMainWindow::SELECT_REV          = QT_TR_NOOP( "&Reverse selections" );
const char* const QBtMainWindow::MD5_CREATE          = QT_TR_NOOP( "Create MD5 control file" );
const char* const QBtMainWindow::MD5_CHECK           = QT_TR_NOOP( "Check MD5 file" );
const char* const QBtMainWindow::FTP_CONNECT         = QT_TR_NOOP( "FTP connect" );
const char* const QBtMainWindow::FTP_DISCONNECT      = QT_TR_NOOP( "FTP disconnect" );
const char* const QBtMainWindow::SYSTEM_INFO         = QT_TR_NOOP( "&System information" );
const char* const QBtMainWindow::FIND                = QT_TR_NOOP( "&Find" );
const char* const QBtMainWindow::NEW_TAB_ACCEL       = QT_TR_NOOP( "Ctrl+T" );
const char* const QBtMainWindow::DEL_TAB_ACCEL       = QT_TR_NOOP( "Ctrl+W" );
const char* const QBtMainWindow::SWITCH_TABS_ACCEL   = QT_TR_NOOP( "Ctrl+S");
const char* const QBtMainWindow::FILE_JOIN_ACCEL     = QT_TR_NOOP( "Ctrl+J" );
const char* const QBtMainWindow::MD5_CREATE_ACCEL    = QT_TR_NOOP( "Ctrl+5" );
const char* const QBtMainWindow::MD5_CHECK_ACCEL     = QT_TR_NOOP( "Alt+5" );
const char* const QBtMainWindow::FIND_ACCEL          = QT_TR_NOOP( "Ctrl+F7" );
//------- Languages -------
const char* const QBtMainWindow::CS_LANG = QT_TR_NOOP( "&Czech (cs)"   );
const char* const QBtMainWindow::DE_LANG = QT_TR_NOOP( "&Deutsch (de)" );
const char* const QBtMainWindow::EN_LANG = QT_TR_NOOP( "&English (en)" );
const char* const QBtMainWindow::PL_LANG = QT_TR_NOOP( "&Polish (pl)"  );
const char* const QBtMainWindow::RU_LANG = QT_TR_NOOP( "&Russian (ru)" );
const char* const QBtMainWindow::ES_LANG = QT_TR_NOOP( "&Spanish (es)" );
//------- Help -------
const char* const QBtMainWindow::ABOUT_QT_ICON  = "qt.png";
const char* const QBtMainWindow::ABOUT_BSC_ICON = "bsc.png";
const char* const QBtMainWindow::ABOUT_QT       = QT_TR_NOOP( "About &Qt" );
const char* const QBtMainWindow::ABOUT_BSC      = QT_TR_NOOP( "&About a Beesoft Commander" );


//*******************************************************************
// QBtMainWindow                                         CONSTRUCTOR
//*******************************************************************
QBtMainWindow::QBtMainWindow() : QMainWindow()
//------- Menu --------
, menu_system_     ( new QMenu   )
, menu_operations_ ( new QMenu   )
, menu_langs_      ( new QMenu   )
, menu_help_       ( new QMenu   )
//------- Toolbars -------
, system_tbar_     ( new QToolBar )
, operations_tbar_ ( new QToolBar )
//------- System -------
, refresh_         ( new QAction( QIcon( IMG_LOC + REFRESH_ICON   ), "", this ) )
, filter_          ( new QAction( QIcon( IMG_LOC + FILTER_ICON    ), "", this  ) )
, configure_       ( new QAction( QIcon( IMG_LOC + CONFIGURE_ICON ), "", this  ) )
, exit_            ( new QAction( QIcon( IMG_LOC + EXIT_ICON      ), "", this  ) )
//------- Operations -------
, new_tab_         ( new QAction( QIcon( IMG_LOC + NEW_TAB_ICON        ), "", this  ) )
, del_tab_         ( new QAction( QIcon( IMG_LOC + DEL_TAB_ICON        ), "", this  ) )
, switch_tabs_     ( new QAction( QIcon( IMG_LOC + SWITCH_TABS_ICON    ), "", this  ) )
, file_join_       ( new QAction( QIcon( IMG_LOC + FILE_JOIN_ICON      ), "", this  ) )
, dir_tree_        ( new QAction( QIcon( IMG_LOC + DIR_TREE_ICON       ), "", this  ) )
, cmp_files_       ( new QAction( QIcon( IMG_LOC + CMP_FILES_ICON      ), "", this  ) )
, cmp_dirs_        ( new QAction( QIcon( IMG_LOC + CMP_DIRS_ICON       ), "", this  ) )
, dir_sync_        ( new QAction( QIcon( IMG_LOC + DIR_SYNC_ICON       ), "", this  ) )
, date_time_       ( new QAction( QIcon( IMG_LOC + DATE_TIME_ICON      ), "", this  ) )
, select_mask_     ( new QAction( QIcon( IMG_LOC + SELECT_MASK_ICON    ), "", this  ) )
, unselect_mask_   ( new QAction( QIcon( IMG_LOC + UNSELECT_MASK_ICON  ), "", this  ) )
, select_rev_      ( new QAction( QIcon( IMG_LOC + SELECT_REV_ICON     ), "", this  ) )
, md5_create_      ( new QAction( this ) )
, md5_check_       ( new QAction( this ) )
, ftp_connect_     ( new QAction( QIcon( IMG_LOC + FTP_CONNECT_ICON    ), "", this  ) )
, ftp_disconnect_  ( new QAction( QIcon( IMG_LOC + FTP_DISCONNECT_ICON ), "", this  ) )
, system_info_     ( new QAction( QIcon( IMG_LOC + SYSTEM_INFO_ICON    ), "", this  ) )
, find_            ( new QAction( QIcon( IMG_LOC + FIND_ICON           ), "", this  ) )
//------- Languages -------
, cs_lang_         ( new QAction( this ) )
, de_lang_         ( new QAction( this ) )
, en_lang_         ( new QAction( this ) )
, pl_lang_         ( new QAction( this ) )
, ru_lang_         ( new QAction( this ) )
, es_lang_         ( new QAction( this ) )
, lang_            ( new QActionGroup( this ) )
//------- Help -------
, about_qt_        ( new QAction( QIcon( IMG_LOC + ABOUT_QT_ICON  ), "", this  ) )
, about_bsc_       ( new QAction( QIcon( IMG_LOC + ABOUT_BSC_ICON ), "", this  ) )
//------- Pozostale -------
, workspace_       ( new QBtWorkspace )
, buttons_         ( new QBtButtonSpace( this ) )
{
   // NOT IMPLEMENTED
   menu_langs_->setEnabled( false );
   ftp_connect_->setEnabled( false );
   ftp_disconnect_->setEnabled( false );
   
   setWindowTitle( QBtShared::program_name() );
   filter_->setCheckable( true );

   system_tbar_->setObjectName( "SYSTEM_TBAR" );
   operations_tbar_->setObjectName( "OPERATIONS_TBAR" );
   
   create_menu();
   create_tool_bars();
   update_action();

   QFrame* const frame = new QFrame( this );
   setCentralWidget( frame );

   QVBoxLayout* vb = new QVBoxLayout;
   vb->setMargin( 0 );
   vb->setSpacing( 0 );
   vb->addWidget( workspace_ );
   vb->addWidget( buttons_   );
   vb->setStretchFactor( workspace_, 100 );
   frame->setLayout( vb );
 
   QBtEventsController::instance()->append( this, QBtEvent::F1_KEY         );
   QBtEventsController::instance()->append( this, QBtEvent::F10_KEY        );
   QBtEventsController::instance()->append( this, QBtEvent::CONFIG_CHANGED );
}
// end of QBtMainWindow

//*******************************************************************
// ~QBtMainWindow                                         DESTRUCTOR
//*******************************************************************
QBtMainWindow::~QBtMainWindow()
{
   save_settings();
   QBtEventsController::instance()->remove( this );
}
// end of ~QBtMainWindow

//*******************************************************************
// save_settings                                             PRIVATE
//*******************************************************************
void QBtMainWindow::save_settings() const
{
   QBtSettings stt;
   stt.save( QBtConfig::MAIN_WINDOW_GROUP + QBtConfig::POS_KEY, pos() );
   stt.save( QBtConfig::MAIN_WINDOW_GROUP + QBtConfig::SIZE_KEY, size() );
   stt.save( QBtConfig::MAIN_WINDOW_GROUP + QBtConfig::STATE_KEY, saveState() );

   QBtConfig* const cfg = QBtConfig::instance();
   cfg->filter( filter_->isChecked() );
   cfg->save_data();
}
// end of save_settings

//*******************************************************************
// restore_settings                                          PRIVATE
//*******************************************************************
void QBtMainWindow::restore_settings()
{
   QBtSettings stt;
   QVariant data;
    if( stt.read( QBtConfig::MAIN_WINDOW_GROUP + QBtConfig::POS_KEY, data ) ) {
      move( data.toPoint() );
   }
   if( stt.read( QBtConfig::MAIN_WINDOW_GROUP + QBtConfig::SIZE_KEY, data ) ) {
      resize( data.toSize() );
   }
   if( stt.read( QBtConfig::MAIN_WINDOW_GROUP + QBtConfig::STATE_KEY, data ) ) {
      restoreState( data.toByteArray() );
   }
   const bool state = QBtConfig::instance()->filter();
   filter_->setChecked( state ? true : false );
}
// end of restore_setting

//*******************************************************************
// showEvent                                       PRIVATE inherited
//*******************************************************************
void QBtMainWindow::showEvent( QShowEvent* const in_event )
{
   retranslate();
   
   QBtShared::resize( this, 90, 90 );
   restore_settings();
   QMainWindow::showEvent( in_event );
}
// end of showEvent

//*******************************************************************
// keyPressEvent                                   PRIVATE inherited
//*******************************************************************
void QBtMainWindow::keyPressEvent( QKeyEvent* in_event )
{
   switch( in_event->key() ) {
      case Qt::Key_F1:
         in_event->accept();
         f1_key();
         break;
      case Qt::Key_F10:
         in_event->accept();
         f10_key();
         break;
   }
}
// end of keyPressEvent

//*******************************************************************
// customEvent                                     PRIVATE inherited
//*******************************************************************
void QBtMainWindow::customEvent( QEvent* const in_event )
{
   QBtEvent* const event = dynamic_cast< QBtEvent* >( in_event );
   const qint32 type = static_cast< qint32 >( event->type() );
   
   switch( type ) {
      case QBtEvent::F1_KEY:
         f1_key();
         break;
      case QBtEvent::F10_KEY:
         f10_key();
         break;
      case QBtEvent::CONFIG_CHANGED:
         retranslate();
         break;
   }
}
// end of customEvent

//*******************************************************************
// create_menu                                               PRIVATE
//*******************************************************************
void QBtMainWindow::create_menu()
{
   //------- Menu -------
   menuBar()->addMenu( menu_system_     );
   menuBar()->addMenu( menu_operations_ );
   menuBar()->addMenu( menu_langs_      );
   menuBar()->addMenu( menu_help_       );
   //------- System -------
   menu_system_->addAction( refresh_       );
   menu_system_->addAction( filter_        );
   menu_system_->addSeparator();
   menu_system_->addAction( configure_     );
   menu_system_->addAction( exit_          );
   //------- Operations -------
   menu_operations_->addAction( new_tab_        );
   menu_operations_->addAction( del_tab_        );
   menu_operations_->addAction( switch_tabs_    );
   menu_operations_->addSeparator();
   menu_operations_->addAction( find_           );
   menu_operations_->addAction( file_join_      );
   menu_operations_->addAction( dir_tree_       );
   menu_operations_->addAction( cmp_files_      );
   menu_operations_->addAction( cmp_dirs_       );
   menu_operations_->addAction( dir_sync_       );
   menu_operations_->addAction( date_time_      );
   menu_operations_->addSeparator();
   menu_operations_->addAction( select_mask_    );
   menu_operations_->addAction( unselect_mask_  );
   menu_operations_->addAction( select_rev_     );
   menu_operations_->addSeparator();
   menu_operations_->addAction( md5_create_     );
   menu_operations_->addAction( md5_check_      );
   //menu_operations_->addSeparator();
   //menu_operations_->addAction( ftp_connect_    );
   //menu_operations_->addAction( ftp_disconnect_ );
   menu_operations_->addSeparator();
   menu_operations_->addAction( system_info_    );
   //------- Languages -------
   menu_langs_->addAction( cs_lang_ );
   menu_langs_->addAction( de_lang_ );
   menu_langs_->addAction( en_lang_ );
   menu_langs_->addAction( pl_lang_ );
   menu_langs_->addAction( ru_lang_ );
   menu_langs_->addAction( es_lang_ );
   //------- Help -------
   menu_help_->addAction( about_qt_  );
   menu_help_->addAction( about_bsc_ );
}
// end of create_menu

//*******************************************************************
// create_tool_bars                                          PRIVATE
//*******************************************************************
void QBtMainWindow::create_tool_bars()
{
   //------- System -------
   addToolBar( system_tbar_ );
   system_tbar_->addAction( exit_ );
   system_tbar_->addSeparator();
   system_tbar_->addAction( filter_ );
   system_tbar_->addAction( refresh_ );
   
   //------- Operations -------
   addToolBar( operations_tbar_ );
   operations_tbar_->addAction( find_ );
   operations_tbar_->addAction( dir_tree_ );
   operations_tbar_->addAction( cmp_files_ );
   operations_tbar_->addAction( cmp_dirs_ );
   operations_tbar_->addSeparator();
   operations_tbar_->addAction( select_mask_ );
   operations_tbar_->addAction( unselect_mask_ );
   operations_tbar_->addAction( select_rev_ );
   operations_tbar_->addSeparator();
   operations_tbar_->addAction( ftp_connect_ );
   operations_tbar_->addAction( ftp_disconnect_ );
}
// end of create_tool_bars

//*******************************************************************
// update_action                                              PRVATE
//*******************************************************************
void QBtMainWindow::update_action()
{
   //------- System -------
   connect( refresh_  , SIGNAL( triggered()           ), this, SLOT( refresh()         ) );
   connect( filter_   , SIGNAL( triggered()           ), this, SLOT( filter ()         ) );
   connect( configure_, SIGNAL( triggered()           ), this, SLOT( configure()       ) );
   connect( exit_     , SIGNAL( triggered()           ), qApp, SLOT( closeAllWindows() ) );
   //------- Operations -------
   connect( new_tab_       , SIGNAL( triggered() ), this, SLOT( new_tab()        ) );
   connect( del_tab_       , SIGNAL( triggered() ), this, SLOT( del_tab()        ) );
   connect( switch_tabs_   , SIGNAL( triggered() ), this, SLOT( switch_tabs()    ) );
   connect( file_join_     , SIGNAL( triggered() ), this, SLOT( file_join()      ) );
   connect( dir_tree_      , SIGNAL( triggered() ), this, SLOT( dir_tree()       ) );
   connect( cmp_files_     , SIGNAL( triggered() ), this, SLOT( cmp_files()      ) );
   connect( cmp_dirs_      , SIGNAL( triggered() ), this, SLOT( cmp_dirs()       ) );
   connect( dir_sync_      , SIGNAL( triggered() ), this, SLOT( sync_dirs()      ) );
   connect( date_time_     , SIGNAL( triggered() ), this, SLOT( date_time()      ) );
   connect( select_mask_   , SIGNAL( triggered() ), this, SLOT( select_mask()    ) );
   connect( unselect_mask_ , SIGNAL( triggered() ), this, SLOT( unselect_mask()  ) );
   connect( select_rev_    , SIGNAL( triggered() ), this, SLOT( select_rev()     ) );
   connect( md5_create_    , SIGNAL( triggered() ), this, SLOT( md5_create()     ) );
   connect( md5_check_     , SIGNAL( triggered() ), this, SLOT( md5_check()      ) );
   connect( ftp_connect_   , SIGNAL( triggered() ), this, SLOT( ftp_connect()    ) );
   connect( ftp_disconnect_, SIGNAL( triggered() ), this, SLOT( ftp_disconnect() ) );
   connect( system_info_   , SIGNAL( triggered() ), this, SLOT( system_info()    ) );
   connect( find_          , SIGNAL( triggered() ), this, SLOT( find()           ) );
   //------- Languages -------
   cs_lang_->setCheckable( true );
   de_lang_->setCheckable( true );
   en_lang_->setCheckable( true );
   pl_lang_->setCheckable( true );
   ru_lang_->setCheckable( true );
   es_lang_->setCheckable( true );
   lang_->setExclusive( true );
   lang_->addAction( cs_lang_ );
   lang_->addAction( de_lang_ );
   lang_->addAction( en_lang_ );
   lang_->addAction( pl_lang_ );
   lang_->addAction( ru_lang_ );
   lang_->addAction( es_lang_ );
   connect( lang_, SIGNAL( triggered( QAction* ) ), this, SLOT( lang( QAction* ) ) );
   //------- Help -------
   connect( about_qt_ , SIGNAL( triggered() ), this, SLOT( about_qt()  ) );
   connect( about_bsc_, SIGNAL( triggered() ), this, SLOT( about_bsc() ) );
}
// end of update_action

//*******************************************************************
// retranslate                                               PRIVATE
//*******************************************************************
void QBtMainWindow::retranslate()
{
   menu_system_     ->setTitle( tr( MENU_SYSTEM     ) );
   menu_operations_ ->setTitle( tr( MENU_OPERATIONS ) );
   menu_langs_      ->setTitle( tr( MENU_LANGS      ) );
   menu_help_       ->setTitle( tr( MENU_HELP       ) );
   //------- System -------
   refresh_      ->setText( tr( REFRESH       ) );
   filter_       ->setText( tr( FILTER        ) );
   configure_    ->setText( tr( CONFIGURE     ) );
   exit_         ->setText( tr( EXIT          ) );
   filter_       ->setShortcut( tr( FILTER_ACCEL ) );
   exit_         ->setShortcut( tr( EXIT_ACCEL   ) );
   //------- Operations -------
   new_tab_       ->setText( tr( NEW_TAB        ) );
   del_tab_       ->setText( tr( DEL_TAB        ) );
   switch_tabs_   ->setText( tr( SWITCH_TABS    ) );
   file_join_     ->setText( tr( FILE_JOIN      ) );
   dir_tree_      ->setText( tr( DIR_TREE       ) );
   cmp_files_     ->setText( tr( CMP_FILES      ) );
   cmp_dirs_      ->setText( tr( CMP_DIRS       ) );
   dir_sync_      ->setText( tr( DIR_SYNC       ) );
   date_time_     ->setText( tr( DATE_TIME      ) );
   select_mask_   ->setText( tr( SELECT_MASK    ) );
   unselect_mask_ ->setText( tr( UNSELECT_MASK  ) );
   select_rev_    ->setText( tr( SELECT_REV     ) );
   md5_create_    ->setText( tr( MD5_CREATE     ) );
   md5_check_     ->setText( tr( MD5_CHECK      ) );
   ftp_connect_   ->setText( tr( FTP_CONNECT    ) );
   ftp_disconnect_->setText( tr( FTP_DISCONNECT ) );
   system_info_   ->setText( tr( SYSTEM_INFO    ) );
   find_          ->setText( tr( FIND           ) );
   new_tab_       ->setShortcut( tr( NEW_TAB_ACCEL     ) );
   del_tab_       ->setShortcut( tr( DEL_TAB_ACCEL     ) );
   switch_tabs_   ->setShortcut( tr( SWITCH_TABS_ACCEL ) );
   file_join_     ->setShortcut( tr( FILE_JOIN_ACCEL   ) );
   md5_create_    ->setShortcut( tr( MD5_CREATE_ACCEL  ) );
   md5_check_     ->setShortcut( tr( MD5_CHECK_ACCEL   ) );
   find_          ->setShortcut( tr( FIND_ACCEL        ) );
   //------- Languages -------
   cs_lang_->setText( tr( CS_LANG ) );
   de_lang_->setText( tr( DE_LANG ) );
   en_lang_->setText( tr( EN_LANG ) );
   pl_lang_->setText( tr( PL_LANG ) );
   ru_lang_->setText( tr( RU_LANG ) );
   es_lang_->setText( tr( ES_LANG ) );
   //------- Help -------
   about_qt_ ->setText( tr( ABOUT_QT  ) );
   about_bsc_->setText( tr( ABOUT_BSC ) );

   buttons_->retranslate();
}
// end of retranslate

//*******************************************************************
// f1_key                                                    PRIVATE
//*******************************************************************
void QBtMainWindow::f1_key()
{
   QBtHelp dialog( this );
   dialog.exec();
}
// end of f1_key

//*******************************************************************
// f10_key                                                   PRIVATE
//*******************************************************************
void QBtMainWindow::f10_key()
{
   qApp->closeAllWindows();
}
// end of f10_key



//###################################################################
//#                                                                 #
//#                           S L O T S                             #
//#                                                                 #
//###################################################################

//*******************************************************************
// refresh                                              PRIVATE slot
//*******************************************************************
void QBtMainWindow::refresh()
{
   QBtEventsController::instance()->send_event( QBtEvent::REFRESH_REQUEST );
}
// end of refresh

//*******************************************************************
// filter                                               PRIVATE slot
//-------------------------------------------------------------------
// Uzytkownik zmienil ustawienia filtra.
//*******************************************************************
void QBtMainWindow::filter()
{
   QBtConfig* const cfg = QBtConfig::instance();
   cfg->filter( filter_->isChecked() );
   cfg->save_data();
   
   QBtEventsController::instance()->send_event( QBtEvent::FILTER_CHANGED_REQUEST );
}
// end of filter

//*******************************************************************
// configure                                            PRIVATE slot
//*******************************************************************
void QBtMainWindow::configure()
{
   QBtConfigDialog dialog( this );
   dialog.exec();
}
// end of configure

//*******************************************************************
// new_tab                                              PRIVATE slot
//*******************************************************************
void QBtMainWindow::new_tab()
{
   QBtEventsController::instance()->send_event( QBtEvent::NEW_TAB_REQUEST );
}
// end of new_tab

//*******************************************************************
// del_tab                                              PRIVATE slot
//*******************************************************************
void QBtMainWindow::del_tab()
{
   QBtEventsController::instance()->send_event( QBtEvent::DEL_TAB_REQUEST );
}
// end of del_tab

//*******************************************************************
// switch_tabs                                          PRIVATE slot
//*******************************************************************
void QBtMainWindow::switch_tabs()
{
   QBtEventsController::instance()->send_event( QBtEvent::SWITCH_TAB_REQUEST );
}
// end of switch_tabs

//*******************************************************************
// file_join                                            PRIVATE slot
//*******************************************************************
void QBtMainWindow::file_join()
{
   QBtEventsController::instance()->send_event( QBtEvent::JOIN_FILES );
}
// end of file_join

//*******************************************************************
// dir_tree                                             PRIVATE slot
//*******************************************************************
void QBtMainWindow::dir_tree()
{
   QBtEventsController::instance()->send_event( QBtEvent::DIR_TREE );
}
// end of dir_tree

//*******************************************************************
// cmp_files                                            PRIVATE slot
//*******************************************************************
void QBtMainWindow::cmp_files()
{
   QBtEventsController::instance()->send_event( QBtEvent::COMPARE_FILES );
}
// end of cmp_files

//*******************************************************************
// cmp_dirs                                             PRIVATE slot
//*******************************************************************
void QBtMainWindow::cmp_dirs()
{
   QBtEventsController::instance()->send_event( QBtEvent::COMPARE_DIRS );
}
// end of cmp_dirs

//*******************************************************************
// sync_dirs                                            PRIVATE slot
//*******************************************************************
void QBtMainWindow::sync_dirs()
{
   QBtEventsController::instance()->send_event( QBtEvent::SYNC_DIRS );
}
// end of sync_dirs

//*******************************************************************
// date_time                                            PRIVATE slot
//*******************************************************************
void QBtMainWindow::date_time()
{
   QBtEventsController::instance()->send_event( QBtEvent::DATE_TIME );
}
// end of date_time

//*******************************************************************
// select_mask                                          PRIVATE slot
//*******************************************************************
void QBtMainWindow::select_mask()
{
   QBtEventsController::instance()->send_event( QBtEvent::SELECT_REQUEST );
}
// end of select_mask

//*******************************************************************
// unselect_mask                                        PRIVATE slot
//*******************************************************************
void QBtMainWindow::unselect_mask()
{
   QBtEventsController::instance()->send_event( QBtEvent::DESELECT_REQUEST );
}
// end of select_mask

//*******************************************************************
// select_rev                                           PRIVATE slot
//*******************************************************************
void QBtMainWindow::select_rev()
{
   QBtEventsController::instance()->send_event( QBtEvent::REVSEL_REQUEST );
}
// end of select_mask

//*******************************************************************
// md5_create                                           PRIVATE slot
//*******************************************************************
void QBtMainWindow::md5_create()
{
   QBtEventsController::instance()->send_event( QBtEvent::MD5_CREATE );
}
// end of md5_create

//*******************************************************************
// md5_check                                            PRIVATE slot
//*******************************************************************
void QBtMainWindow::md5_check()
{
   QBtEventsController::instance()->send_event( QBtEvent::MD5_CHECK );
}
// end of md5_check

void QBtMainWindow::ftp_connect()
{
}

void QBtMainWindow::ftp_disconnect()
{
}

//*******************************************************************
// system_info                                          PRIVATE slot
//*******************************************************************
void QBtMainWindow::system_info()
{
   QBtSystemInfo dialog( this );
   dialog.exec();
}
// end of system_info

//*******************************************************************
// find                                                 PRIVATE slot
//*******************************************************************
void QBtMainWindow::find()
{
   QBtEventsController::instance()->send_event( QBtEvent::FIND_REQUEST );
}
// end of find

void QBtMainWindow::lang( QAction* )
{
}

//*******************************************************************
// about_qt                                             PRIVATE slot
//*******************************************************************
void QBtMainWindow::about_qt()
{
   QMessageBox::aboutQt( this );
}
// end of about_qt

//*******************************************************************
// about_bsc                                            PRIVATE slot
//*******************************************************************
void QBtMainWindow::about_bsc()
{
   static const QString text =
      "<qt>"
      "<h3>%1</h3>"
      "<font color=blue>This is a two panel file manager.</font>"
      "<hr>"
      "Author: Piotr Pszczolkowski<br>"
      "Contact: piotr@beesoft.at<br>"
      "Home page: www.beesoft.at"
      "</qt>";
   QMessageBox::about( this, "About Beesoft Commander", text.arg( QBtShared::program_name() ) );
}
// end of about_bsc
