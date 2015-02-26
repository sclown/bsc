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
 * File         : QBtWorkspace.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtWorkspace.h"
#include "QBtPanel.h"
#include "QBtSettings.h"
#include "QBtConfig.h"
#include "QBtEventsController.h"
#include "QBtView.h"
#include "BtTypes.h"
#include "QBtSystemCall.h"
#include "QBtDirCopyDialog.h"
#include "QBtPackDialog.h"
#include "QBtTimeStamp.h"
#include "QBtShared.h"
#include "QBtCompareFileDialog.h"
#include "QBtCompareDirsDialog.h"
#include "QBtSyncDirsDialog.h"
#include "QBtFileJoiner.h"
#include "QBtDirSelector.h"
#include <QSplitter>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QFileInfo>
#include <QInputDialog>
#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QtDebug>

/*------- local constants:
-------------------------------------------------------------------*/
const char* const QBtWorkspace::InputCaption  = QT_TR_NOOP( "New file name" );
const char* const QBtWorkspace::InputPrompt   = QT_TR_NOOP( "Enter a package file name:" );
const char* const QBtWorkspace::CompareFiles  = QT_TR_NOOP( "Compare files." );
const char* const QBtWorkspace::NoFiles       = QT_TR_NOOP( "You can compare only files." );
const char* const QBtWorkspace::FilesJoining  = QT_TR_NOOP( "Files joining" );
const char* const QBtWorkspace::EnterFileName = QT_TR_NOOP( "Enter destination file name:" );
const char* const QBtWorkspace::NoWritableDir = QT_TR_NOOP( "You can't write to this directory: %1.\n"
                                                            "Check permission access." );
const char* const QBtWorkspace::CanOverwrite  = QT_TR_NOOP( "Destination file:%1\n"
                                                            "already exist. Would you like to use this one?" );
const char* const QBtWorkspace::Md5           = QT_TR_NOOP( "Md5 sum" );
const char* const QBtWorkspace::SyncNoDirs    = QT_TR_NOOP( "To synchronize directories you must select directories.\n"
                                                            "At last one of selected items is not the directory." );
const char* const QBtWorkspace::CmpNoDirs     = QT_TR_NOOP( "To compare directories you must select directories.\n"
                                                            "At last one of selected items is not the directory." );
const char* const QBtWorkspace::UserError     = QT_TR_NOOP( "Users wrong action" );
const QString     QBtWorkspace::Md5Create     = "md5sum %1 > %2";
const QString     QBtWorkspace::Md5Check      = "md5sum -c %1";


//*******************************************************************
// QBtWorkspace                                          CONSTRUCTOR
//*******************************************************************
QBtWorkspace::QBtWorkspace( QWidget* const in_parent ) : QWidget( in_parent )
, splitter_   ( new QSplitter( Qt::Horizontal )            )
, left_panel_ ( new QBtPanel ( LEFT_PANEL_ID , splitter_ ) )
, right_panel_( new QBtPanel ( RIGHT_PANEL_ID, splitter_ ) )
{
   setFocusPolicy( Qt::NoFocus );
   splitter_->setFocusPolicy( Qt::ClickFocus );
   
   splitter_->setHandleWidth( 5 );
   splitter_->setOpaqueResize();
   
   QHBoxLayout* const hb = new QHBoxLayout( this );
   hb->setMargin( 0 );
   hb->setSpacing( 0 );
   hb->addWidget( splitter_ );

   restore();
   
   QBtEventsController::instance()->append( this, QBtEvent::SWITCH_TAB_REQUEST );
   QBtEventsController::instance()->append( this, QBtEvent::F5_KEY );
   QBtEventsController::instance()->append( this, QBtEvent::F9_KEY );
   QBtEventsController::instance()->append( this, QBtEvent::COMPARE_FILES );
   QBtEventsController::instance()->append( this, QBtEvent::COMPARE_DIRS );
   QBtEventsController::instance()->append( this, QBtEvent::SYNC_DIRS );
   QBtEventsController::instance()->append( this, QBtEvent::JOIN_FILES );
   QBtEventsController::instance()->append( this, QBtEvent::DIR_TREE );
   QBtEventsController::instance()->append( this, QBtEvent::MD5_CREATE );
   QBtEventsController::instance()->append( this, QBtEvent::MD5_CHECK );
   QBtEventsController::instance()->append( this, QBtEvent::DATE_TIME );
   QBtEventsController::instance()->append( this, QBtEvent::DROP_FILES );
   QBtEventsController::instance()->append( this, QBtEvent::OPEN_OPOSITE );
}
// end of QBtWorkspace

//*******************************************************************
// ~QBtWorkspace                                          DESTRUCTOR
//*******************************************************************
QBtWorkspace::~QBtWorkspace()
{
   save();
   QBtEventsController::instance()->remove( this );
}
// end of ~QBtWorkspace

//*******************************************************************
// keyPressEvent                                   PRIVATE inherited
//*******************************************************************
void QBtWorkspace::keyPressEvent( QKeyEvent* in_event )
{
   switch( in_event->key() ) {
      case Qt::Key_F5:
         in_event->accept();
         copy();
         break;
      case Qt::Key_F9:
         in_event->accept();
         pack();
         break;
   }
   QWidget::keyPressEvent( in_event );
}
// end of keyPressEvent

//*******************************************************************
// customEvent                                     PRIVATE inherited
//*******************************************************************
void QBtWorkspace::customEvent( QEvent* const in_event )
{
   const QBtEvent* const event = dynamic_cast< QBtEvent* >( in_event );
   const int type = static_cast<int>( event->type() );

   switch( type ) {
      case QBtEvent::SWITCH_TAB_REQUEST:
         switch_panels();
         break;
      case QBtEvent::F5_KEY:
         copy();
         break;
      case QBtEvent::F9_KEY:
         pack();
         break;
      case QBtEvent::COMPARE_FILES:
         compare_files();
         break;
      case QBtEvent::COMPARE_DIRS:
         compare_dirs();
         break;
      case QBtEvent::SYNC_DIRS:
         sync_dirs();
         break;
      case QBtEvent::JOIN_FILES:
         join_files();
         break;
      case QBtEvent::DIR_TREE:
         dir_tree();
         break;
      case QBtEvent::MD5_CREATE:
         md5_create();
         break;
      case QBtEvent::MD5_CHECK:
         md5_check();
         break;
      case QBtEvent::DATE_TIME:
         date_time();
         break;
      case QBtEvent::DROP_FILES:
         drop_files( event->data(0).toMap() );
         break;
      case QBtEvent::OPEN_OPOSITE:
         open_oposite();
         break;
   }
}
// end of customEvent

//*******************************************************************
// save                                                      PRIVATE
//*******************************************************************
void QBtWorkspace::save()
{
   QBtSettings stt;
   stt.save( QBtConfig::WORKSPACE_GROUP + QBtConfig::SPLITTER_KEY, splitter_->saveState() );
}
// end of save

//*******************************************************************
// restore                                                   PRIVATE
//*******************************************************************
void QBtWorkspace::restore()
{
   QBtSettings stt;
   QVariant data;
   if( stt.read( QBtConfig::WORKSPACE_GROUP + QBtConfig::SPLITTER_KEY, data ) ) {
      splitter_->restoreState( data.toByteArray() );
   }
   else {
      splitter_->setStretchFactor( 0, 1 );
      splitter_->setStretchFactor( 1, 1 );
   }
}
// end of restore

//*******************************************************************
// switch_panel                                              PRIVATE
//*******************************************************************
void QBtWorkspace::switch_panels()
{
   splitter_->insertWidget( 0, right_panel_ );
   
   QBtPanel* const tmp = left_panel_;
   left_panel_ = right_panel_;
   right_panel_ = tmp;

   left_panel_ ->set_tab_id( LEFT_PANEL_ID  );
   right_panel_->set_tab_id( RIGHT_PANEL_ID );
}
// end of switch_panels

//*******************************************************************
// srv_and_dst_view                                          PRIVATE
//*******************************************************************
bool QBtWorkspace::src_and_dst_view( QBtView*& out_src, QBtView*& out_dst ) const
{
   QBtView* const v1 = left_panel_ ->current_view();
   QBtView* const v2 = right_panel_->current_view();
   if( !v1 || !v2 ) return false;

   if( v1->hasFocus() ) {
      out_src = v1;
      out_dst = v2;
   }
   else {
      out_src = v2;
      out_dst = v1;
   }
   return true;
}
// end of src_and_dst_view

//*******************************************************************
// get_selections                                            PRIVATE
//*******************************************************************
bool QBtWorkspace::get_selections( QBtView* const in_view, SelectionsSet& out_data ) const
{
   out_data = in_view->get_selected_items();
   if( out_data.empty() ) {
      if( QBtShared::is_regular_file( in_view->selected_file_full_name() ) ) {
         out_data.insert( in_view->selected_file_path() );
      }
   }
   return !out_data.empty();
}
// end of get_selections

//*******************************************************************
// copy                                                      PRIVATE
//*******************************************************************
void QBtWorkspace::copy()
{
   QBtView* src = 0;
   QBtView* dst = 0;
   if( !src_and_dst_view( src, dst ) ) {
      return;
   }
   
   SelectionsSet data = SelectionsSet();
   if( !get_selections( src, data ) ) {
      return;
   }

   QBtDirCopyDialog dialog( this );
   dialog.set_source( data );
   dialog.set_destination( dst->current_path() );
   if( dialog.exec() != QDialog::Accepted ) return;

   src->unselect_all();
   src->refresh();
   dst->refresh();
}
// end of copy

//*******************************************************************
// pack                                                      PRIVATE
//*******************************************************************
void QBtWorkspace::pack()
{
   QBtView* src = 0;
   QBtView* dst = 0;
   if( !src_and_dst_view( src, dst ) ) return;
   
   SelectionsSet data = SelectionsSet();
   if( !get_selections( src, data ) ) return;

   bool ok;
   const QString pack_file_name = QInputDialog::getText( this, tr( InputCaption ),
                                                               tr( InputPrompt ),
                                                               QLineEdit::Normal,
                                                               QString(),
                                                               &ok );
   if( !ok ) return;
   if( pack_file_name.trimmed().isEmpty() ) return;

   QBtPackDialog dialog( this );
   dialog.set_source( data );
   dialog.set_destination( dst->current_path(), pack_file_name.trimmed() );
   if( dialog.exec() != QDialog::Accepted ) return;
   
   src->unselect_all();
   src->refresh();
   dst->refresh( QFileInfo( dialog.get_dst_path() ).fileName() );
}
// end of pack

//*******************************************************************
// compare_files                                             PRIVATE
//*******************************************************************
void QBtWorkspace::compare_files()
{
   QBtView* const v1 = left_panel_ ->current_view();
   QBtView* const v2 = right_panel_->current_view();
   if( !v1 || !v2 ) return;

   SelectionsSet d1 = SelectionsSet();
   SelectionsSet d2 = SelectionsSet();
   get_selections( v1, d1 );
   get_selections( v2, d2 );
   if( d1.size() != 1 ) return;
   if( d2.size() != 1 ) return;

   const QFileInfo f1( *d1.begin() );
   const QFileInfo f2( *d2.begin() );
   
   if( f1.isFile() && f2.isFile() ) {
      const QString fpath1 = f1.absoluteFilePath();
      const QString fpath2 = f2.absoluteFilePath();
      
      QBtCompareFileDialog dialog( this, fpath1, fpath2 );
      if( QDialog::Accepted == dialog.exec() ) {
         diff( fpath1, fpath2 );
      }
   }
   else {
      QMessageBox::information( this, tr( CompareFiles ), tr( NoFiles ) );
   }
}
// end of compare_files   

//*******************************************************************
// compare_dirs                                              PRIVATE
//*******************************************************************
void QBtWorkspace::compare_dirs()
{
   QBtView* const src = left_panel_ ->current_view();
   QBtView* const dst = right_panel_->current_view();
   if( !src || !dst ) return;

   SelectionsSet d1 = SelectionsSet();
   SelectionsSet d2 = SelectionsSet();
   get_selections( src, d1 );
   get_selections( dst, d2 );
   if( d1.size() != 1 ) return;
   if( d2.size() != 1 ) return;
   
   const QFileInfo fi1( *d1.begin() );
   const QFileInfo fi2( *d2.begin() );

   if( fi1.isDir() && fi2.isDir() ) {
      QBtCompareDirsDialog dialog( this );
      dialog.set_dirs( fi1.absoluteFilePath(), fi2.absoluteFilePath() );
      dialog.exec();
   }
   else {
      QMessageBox::critical( this, tr( UserError ), tr( CmpNoDirs ) );
   }
}
// end of compare_dirs

//*******************************************************************
// sync_dirs                                                 PRIVATE
//*******************************************************************
void QBtWorkspace::sync_dirs()
{
   QBtView* const src = left_panel_ ->current_view();
   QBtView* const dst = right_panel_->current_view();
   if( !src || !dst ) return;

   SelectionsSet d1 = SelectionsSet();
   SelectionsSet d2 = SelectionsSet();
   get_selections( src, d1 );
   get_selections( dst, d2 );
   if( d1.size() != 1 ) return;
   if( d2.size() != 1 ) return;
   
   const QFileInfo fi1( *d1.begin() );
   const QFileInfo fi2( *d2.begin() );

   if( fi1.isDir() && fi2.isDir() ) {
      QBtSyncDirsDialog dialog( this );
      dialog.set_dirs( fi1.absoluteFilePath(), fi2.absoluteFilePath() );
      dialog.exec();
   }
   else {
      QMessageBox::critical( this, tr( UserError ), tr( SyncNoDirs ) );
   }
}
// end of sync_dirs

//*******************************************************************
// diff                                                      PRIVATE
//*******************************************************************
void QBtWorkspace::diff( const QString& in_fname_1, const QString& in_fname_2 ) const
{
   const QString fname1  = QBtShared::quoted_fpath( in_fname_1 );
   const QString fname2  = QBtShared::quoted_fpath( in_fname_2 );
   const QString beediff = QBtShared::slashed_dir( QDir::homePath() ) + "/beediff/beediff %1 %2";
   bool          ok      = true;
   
   QBtSettings stt;
   QVariant data;
   if( stt.read( QBtConfig::DIFF_GROUP + QBtConfig::USE_DEFAULT_KEY, data ) ) {
      ok = data.toBool();
   }

   QString muster = beediff;
   if( !ok ) {
      if( stt.read( QBtConfig::DIFF_GROUP + QBtConfig::COMMAND_KEY, data ) ) {
         muster = data.toString().trimmed();
         if( muster.isEmpty() ) {
            return;
         }
      }
   }

   QProcess process;
   const QString cmd = muster.arg( fname1 ).arg( fname2 );
   process.startDetached( cmd );
}
// end of diff

//*******************************************************************
// join_files                                                PRIVATE
//*******************************************************************
void QBtWorkspace::join_files()
{
   QBtView* src = 0;
   QBtView* dst = 0;
   if( !src_and_dst_view( src, dst ) ) return;

   // aby moc cos laczyc musza byc zaznaczone co najmniej 1 plik.
   SelectionsSet data = SelectionsSet();
   get_selections( src, data );
   if( data.empty() ) return;

   // uzytkownik musi miec uprawnienia do zapisu w docelowym katalogu
   QString dst_dir = dst->current_path();
   const QFileInfo dst_dir_fi( dst_dir );
   if( !dst_dir_fi.isWritable() ) {
      QMessageBox::critical( this, tr( FilesJoining ), tr( NoWritableDir ).arg( dst_dir ) );
      return;
   }

   // uzytkownik musi podac nazwe pliku wynikowego
   bool ok;
   const QString fname = QInputDialog::getText( this,
                                                tr( FilesJoining ),
                                                tr( EnterFileName ),
                                                QLineEdit::Normal,
                                                QString(),
                                                &ok );
   if( !ok || fname.trimmed().isEmpty() ) return;

   // Absolutna sciezka do pliku wynikowego.
   if( !dst_dir.endsWith( '/' ) ) dst_dir += '/';
   const QString dst_fpath = dst_dir + fname;

   // jesli plik juz istnieje uzytkownik musi podjac decyzje co z tym fantem zrobic
   const QFileInfo dst_fi( dst_fpath );
   if( dst_fi.exists() ) {
      const int answer = QMessageBox::question( this,
                                                tr( FilesJoining ),
                                                tr( CanOverwrite ).arg( dst_fpath ),
                                                QMessageBox::Yes,
                                                QMessageBox::No );
      if( QMessageBox::No == answer ) return;
   }

   // wykonaj laczenie plikow
   QBtFileJoiner dialog( this, data, dst_fpath );
   if( QDialog::Accepted == dialog.exec() ) {
      // odswiez pokazywana zawartosc i ustaw sie na tym pliku.
      dst->refresh( QFileInfo( dst_fpath ).fileName() );
       if( src->current_path() == dst->current_path() ) src->refresh();
   }
}
// end of join_files

//*******************************************************************
// dir_tree                                                  PRIVATE
//*******************************************************************
void QBtWorkspace::dir_tree()
{
   QBtView* src = 0;
   QBtView* dst = 0;
   if( !src_and_dst_view( src, dst ) ) return;

   QBtDirSelector dialog( this,  src->current_path() );
   if( QDialog::Accepted == dialog.exec() ) {
      // przejdz do tego katalogu i pokaz jego zawartosc
      src->update( dialog.path() );
   }
}
// end of dir_tree

//*******************************************************************
// md5_create                                                PRIVATE
//*******************************************************************
void QBtWorkspace::md5_create()
{
   QBtView* src = 0;
   QBtView* dst = 0;
   if( !src_and_dst_view( src, dst ) ) return;
   
   SelectionsSet data = SelectionsSet();
   get_selections( src, data );
   if( data.size() != 1 ) return;
   
   const QString src_fpath = *data.begin();
   const QString src_dir   = src->current_path();
   const QString dst_fpath = src_fpath + ".md5sum";

   if( QFileInfo( dst_fpath ).exists() ) QFile::remove( dst_fpath );
   if( QFileInfo( src_fpath ).isDir() ) {
      return;
   }
   if( !QFileInfo( src_dir ).isWritable() ) {
      QMessageBox::critical( this, tr( Md5 ), tr( NoWritableDir ).arg( src_dir ) );
      return;
   }

   QBtShared::set_cursor( Qt::BusyCursor );
   const QString cmd = Md5Create.arg( QBtShared::quoted_fpath( src_fpath ),
                                      QBtShared::quoted_fpath( dst_fpath ) );
   system( cmd.toLocal8Bit() );
   QBtShared::restore_cursor();
   
   src->refresh( QFileInfo( dst_fpath).fileName() );
   if( src->current_path() == dst->current_path() ) dst->refresh();
}
// end of md5_create

//*******************************************************************
// md5_check                                                 PRIVATE
//*******************************************************************
void QBtWorkspace::md5_check()
{
   QBtView* src = 0;
   QBtView* dst = 0;
   if( !src_and_dst_view( src, dst ) ) return;

   SelectionsSet data = SelectionsSet();
   get_selections( src, data );
   if( data.size() != 1 ) return;
   
   const QString src_fpath = *data.begin();
   
   QBtSystemCall sc;
   sc.run( Md5Check.arg( QBtShared::quoted_fpath( src_fpath ) ) );
   const QString msg = sc.result();

   QMessageBox::information( this, tr( Md5 ), msg );
}
// end of md5_check

//*******************************************************************
// date_time                                                 PRIVATE
//*******************************************************************
void QBtWorkspace::date_time()
{
   QBtView* src = 0;
   QBtView* dst = 0;
   if( !src_and_dst_view( src, dst ) ) return;

   SelectionsSet d1 = SelectionsSet();
   SelectionsSet d2 = SelectionsSet();
   get_selections( src, d1 );
   get_selections( dst, d2 );
   if( d1.empty() ) return;
   if( d2.size() != 1 ) return;
   
   QBtTimeStamp dialog( this, d1, *d2.begin() );
   if( QDialog::Accepted == dialog.exec() ) {
      src->refresh();
      if( src->current_path() == dst->current_path() ) dst->refresh();
   }
}
// end of date_time

void QBtWorkspace::drop_files(const QMap<QString, QVariant> &userInfo)
{
    QStringList files = userInfo["files"].toStringList();
    QString dropPath = userInfo["drop_target"].toString();
    SelectionsSet dropedFiles;
    foreach (QString file, files) {
        dropedFiles.insert(file);
    }


    QBtDirCopyDialog dialog( this );
    dialog.set_source( dropedFiles );
    dialog.set_destination( dropPath );
    if( dialog.exec() != QDialog::Accepted ) return;
    left_panel_->current_view()->refresh();
    right_panel_->current_view()->refresh();
}

void QBtWorkspace::open_oposite()
{
    QBtView* src = 0;
    QBtView* dst = 0;
    if( !src_and_dst_view( src, dst ) ) return;
    QString source_path = src->selected_file_path();
    if( !QFileInfo( source_path ).isDir() ) {
       return;
    }
    dst->update(source_path);
}
