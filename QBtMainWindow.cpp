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
#include "QBtHelp.h"
#include "QBtFinder.h"
#include "QBtCommand.h"
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QIcon>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFrame>
#include <QKeyEvent>
#include <QLineEdit>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QAbstractSpinBox>
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const QString     QBtMainWindow::IMG_LOC         = ":/img/";
//------- Menu -------
const char* const QBtMainWindow::MENU_SYSTEM     = QT_TR_NOOP( "&System" );
const char* const QBtMainWindow::MENU_OPERATIONS = QT_TR_NOOP( "&Operations" );
const char* const QBtMainWindow::MENU_HELP       = QT_TR_NOOP( "&Help" );
//------- System -------
const char* const QBtMainWindow::REFRESH_ICON   = "refresh.png";
const char* const QBtMainWindow::EXIT_ICON      = "exit.png";
const char* const QBtMainWindow::REFRESH        = QT_TR_NOOP( "&Refresh files window" );
const char* const QBtMainWindow::EXIT           = QT_TR_NOOP( "E&xit" );
const char* const QBtMainWindow::EXIT_ACCEL     = QT_TR_NOOP( "Ctrl+Q" );
//------- Operations -------
const char* const QBtMainWindow::NEW_TAB_ICON        = "tab_new.png";
const char* const QBtMainWindow::DEL_TAB_ICON        = "tab_remove.png";
const char* const QBtMainWindow::SWITCH_TABS_ICON    = "switch.png";
const char* const QBtMainWindow::SELECT_MASK_ICON    = "select.png";
const char* const QBtMainWindow::UNSELECT_MASK_ICON  = "unselect.png";
const char* const QBtMainWindow::SELECT_REV_ICON     = "revers.png";
const char* const QBtMainWindow::SYSTEM_INFO_ICON    = "system.png";
const char* const QBtMainWindow::FIND_ICON           = "find.png";
const char* const QBtMainWindow::NEW_TAB             = QT_TR_NOOP( "&Create new tab" );
const char* const QBtMainWindow::DEL_TAB             = QT_TR_NOOP( "&Remove current tab" );
const char* const QBtMainWindow::SWITCH_TABS         = QT_TR_NOOP( "&Switch panels" );
const char* const QBtMainWindow::SELECT_MASK         = QT_TR_NOOP( "&Select files via mask" );
const char* const QBtMainWindow::UNSELECT_MASK       = QT_TR_NOOP( "&Unselect files via mask" );
const char* const QBtMainWindow::SELECT_REV          = QT_TR_NOOP( "&Reverse selections" );
const char* const QBtMainWindow::SYSTEM_INFO         = QT_TR_NOOP( "&System information" );
const char* const QBtMainWindow::FIND                = QT_TR_NOOP( "&Find" );
const char* const QBtMainWindow::NEW_TAB_ACCEL       = QT_TR_NOOP( "Ctrl+T" );
const char* const QBtMainWindow::DEL_TAB_ACCEL       = QT_TR_NOOP( "Ctrl+W" );
const char* const QBtMainWindow::SWITCH_TABS_ACCEL   = QT_TR_NOOP( "Ctrl+S");
const char* const QBtMainWindow::FIND_ACCEL          = QT_TR_NOOP( "Ctrl+F7" );
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
, menu_help_       ( new QMenu   )
//------- System -------
, refresh_         ( new QAction( QIcon( IMG_LOC + REFRESH_ICON   ), "", this ) )
, exit_            ( new QAction( QIcon( IMG_LOC + EXIT_ICON      ), "", this  ) )
//------- Operations -------
, new_tab_         ( new QAction( QIcon( IMG_LOC + NEW_TAB_ICON        ), "", this  ) )
, del_tab_         ( new QAction( QIcon( IMG_LOC + DEL_TAB_ICON        ), "", this  ) )
, switch_tabs_     ( new QAction( QIcon( IMG_LOC + SWITCH_TABS_ICON    ), "", this  ) )
, select_mask_     ( new QAction( QIcon( IMG_LOC + SELECT_MASK_ICON    ), "", this  ) )
, unselect_mask_   ( new QAction( QIcon( IMG_LOC + UNSELECT_MASK_ICON  ), "", this  ) )
, select_rev_      ( new QAction( QIcon( IMG_LOC + SELECT_REV_ICON     ), "", this  ) )
, system_info_     ( new QAction( QIcon( IMG_LOC + SYSTEM_INFO_ICON    ), "", this  ) )
, find_            ( new QAction( QIcon( IMG_LOC + FIND_ICON           ), "", this  ) )
//------- Help -------
, about_qt_        ( new QAction( QIcon( IMG_LOC + ABOUT_QT_ICON  ), "", this  ) )
, about_bsc_       ( new QAction( QIcon( IMG_LOC + ABOUT_BSC_ICON ), "", this  ) )
//------- Pozostale -------
, workspace_       ( new QBtWorkspace )
, buttons_         ( new QBtButtonSpace( this ) )
{
   setWindowTitle( QBtShared::program_name() );

   create_menu();
   update_action();

   QFrame* const frame = new QFrame( this );
   setCentralWidget( frame );

   QVBoxLayout* vb = new QVBoxLayout;
   vb->setContentsMargins( 0, 0, 0, 0 );
   vb->setSpacing( 0 );
   vb->addWidget( workspace_ );
   vb->addWidget( buttons_   );
   vb->setStretchFactor( workspace_, 100 );
   frame->setLayout( vb );
 
   QBtEventsController::instance()->append( this, QBtEvent::F1_KEY         );
   QBtEventsController::instance()->append( this, QBtEvent::F10_KEY        );

   // Alt(Option)+1..0 act as F1..F10 (see eventFilter).
   qApp->installEventFilter( this );
}

//*******************************************************************
// eventFilter                                     PRIVATE inherited
//*******************************************************************
// Turns Alt(Option)+digit into the matching function key, so every
// existing F-key handler also answers to it: Alt+1 -> F1 ... Alt+0 -> F10.
// Text fields keep Alt+digit for typing special characters.
bool QBtMainWindow::eventFilter( QObject* const in_obj, QEvent* const in_event )
{
   if( in_event->type() != QEvent::KeyPress ) return false;

   QKeyEvent* const key = static_cast< QKeyEvent* >( in_event );
   if( ( key->modifiers() & ~Qt::KeypadModifier ) != Qt::AltModifier ) return false;

   // The app-wide filter sees the event once per widget it propagates to;
   // act only on the first receiver, inside this window.
   QWidget* const target = QApplication::focusWidget() ? QApplication::focusWidget() : this;
   if( in_obj != target || target->window() != this ) return false;
   const QComboBox* const combo = qobject_cast< QComboBox* >( target );
   if( qobject_cast< QLineEdit* >( target ) || qobject_cast< QTextEdit* >( target )
    || qobject_cast< QPlainTextEdit* >( target ) || qobject_cast< QAbstractSpinBox* >( target )
    || ( combo && combo->isEditable() ) ) return false;

   int digit = -1;
   if( key->key() >= Qt::Key_0 && key->key() <= Qt::Key_9 ) {
      digit = key->key() - Qt::Key_0;
   }
#if defined(Q_OS_MAC)
   else {
      // Option+digit may report the composed character (e.g. "∞"), so
      // fall back to the physical key (kVK_ANSI_0..9).
      switch( key->nativeVirtualKey() ) {
         case 0x1D: digit = 0; break;
         case 0x12: digit = 1; break;
         case 0x13: digit = 2; break;
         case 0x14: digit = 3; break;
         case 0x15: digit = 4; break;
         case 0x17: digit = 5; break;
         case 0x16: digit = 6; break;
         case 0x1A: digit = 7; break;
         case 0x1C: digit = 8; break;
         case 0x19: digit = 9; break;
      }
   }
#endif
   if( digit < 0 ) return false;

   const int fkey = ( digit == 0 ) ? Qt::Key_F10 : Qt::Key_F1 + digit - 1;
   QKeyEvent fn( QEvent::KeyPress, fkey, Qt::NoModifier );
   QApplication::sendEvent( target, &fn );
   return true;
}
// end of eventFilter
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
   handleCommand();
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

bool QBtMainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::WindowActivate) {
        handleCommand();
        // window was activated
    }
    return QWidget::event(event);
}

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
   menuBar()->addMenu( menu_help_       );
   //------- System -------
   menu_system_->addAction( refresh_       );
   menu_system_->addSeparator();
   menu_system_->addAction( exit_          );
   //------- Operations -------
   menu_operations_->addAction( new_tab_        );
   menu_operations_->addAction( del_tab_        );
   menu_operations_->addAction( switch_tabs_    );
   menu_operations_->addSeparator();
   menu_operations_->addAction( find_           );
   menu_operations_->addSeparator();
   menu_operations_->addAction( select_mask_    );
   menu_operations_->addAction( unselect_mask_  );
   menu_operations_->addAction( select_rev_     );
   menu_operations_->addSeparator();
   menu_operations_->addAction( system_info_    );
   //------- Help -------
   menu_help_->addAction( about_qt_  );
   menu_help_->addAction( about_bsc_ );
}
// end of create_menu

//*******************************************************************
// update_action                                              PRVATE
//*******************************************************************
void QBtMainWindow::update_action()
{
   //------- System -------
   connect( refresh_  , SIGNAL( triggered()           ), this, SLOT( refresh()         ) );
   connect( exit_     , SIGNAL( triggered()           ), qApp, SLOT( closeAllWindows() ) );
   //------- Operations -------
   connect( new_tab_       , SIGNAL( triggered() ), this, SLOT( new_tab()        ) );
   connect( del_tab_       , SIGNAL( triggered() ), this, SLOT( del_tab()        ) );
   connect( switch_tabs_   , SIGNAL( triggered() ), this, SLOT( switch_tabs()    ) );
   connect( select_mask_   , SIGNAL( triggered() ), this, SLOT( select_mask()    ) );
   connect( unselect_mask_ , SIGNAL( triggered() ), this, SLOT( unselect_mask()  ) );
   connect( select_rev_    , SIGNAL( triggered() ), this, SLOT( select_rev()     ) );
   connect( system_info_   , SIGNAL( triggered() ), this, SLOT( system_info()    ) );
   connect( find_          , SIGNAL( triggered() ), this, SLOT( find()           ) );
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
   menu_help_       ->setTitle( tr( MENU_HELP       ) );
   //------- System -------
   refresh_      ->setText( tr( REFRESH       ) );
   exit_         ->setText( tr( EXIT          ) );
   exit_         ->setShortcut( tr( EXIT_ACCEL   ) );
   //------- Operations -------
   new_tab_       ->setText( tr( NEW_TAB        ) );
   del_tab_       ->setText( tr( DEL_TAB        ) );
   switch_tabs_   ->setText( tr( SWITCH_TABS    ) );
   select_mask_   ->setText( tr( SELECT_MASK    ) );
   unselect_mask_ ->setText( tr( UNSELECT_MASK  ) );
   select_rev_    ->setText( tr( SELECT_REV     ) );
   system_info_   ->setText( tr( SYSTEM_INFO    ) );
   find_          ->setText( tr( FIND           ) );
   new_tab_       ->setShortcut( tr( NEW_TAB_ACCEL     ) );
   del_tab_       ->setShortcut( tr( DEL_TAB_ACCEL     ) );
   switch_tabs_   ->setShortcut( tr( SWITCH_TABS_ACCEL ) );
   find_          ->setShortcut( tr( FIND_ACCEL        ) );
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
