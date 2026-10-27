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
#include "QBtOverwriteAnswer.h"
#include "QBtWorkspace.h"
#include "QBtPanel.h"
#include "QBtSettings.h"
#include "QBtConfig.h"
#include "QBtEventsController.h"
#include "QBtView.h"
#include "BtTypes.h"
#include "QBtCopyDialog.h"
#include "QBtPackDialog.h"
#include "QBtShared.h"
#include <QSplitter>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QFileInfo>
#include <QInputDialog>
#include <QResource>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QtDebug>

/*------- local constants:
-------------------------------------------------------------------*/
const char* const QBtWorkspace::InputCaption  = QT_TR_NOOP( "New file name" );
const char* const QBtWorkspace::InputPrompt   = QT_TR_NOOP( "Enter a package file name:" );

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
    hb->setContentsMargins( 0, 0, 0, 0 );
    hb->setSpacing( 0 );
    hb->addWidget( splitter_ );

    restore();

    QBtEventsController::instance()->append( this, QBtEvent::SWITCH_TAB_REQUEST );
    QBtEventsController::instance()->append( this, QBtEvent::F5_KEY );
    QBtEventsController::instance()->append( this, QBtEvent::F9_KEY );
    QBtEventsController::instance()->append( this, QBtEvent::DROP_FILES );
    QBtEventsController::instance()->append( this, QBtEvent::OPEN_OPOSITE );
    QBtEventsController::instance()->append( this, QBtEvent::OPEN_EDITOR );
    QBtEventsController::instance()->append( this, QBtEvent::OPEN_DIR );
    QBtEventsController::instance()->append( this, QBtEvent::EXECUTE );
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
    case QBtEvent::DROP_FILES:
        drop_files( event->data(0).toMap() );
        break;
    case QBtEvent::OPEN_OPOSITE:
        open_oposite();
        break;
    case QBtEvent::OPEN_DIR:
        open_dir( event->data(0).toString() );
        break;
    case QBtEvent::OPEN_EDITOR:
        open_editor();
        break;
    case QBtEvent::EXECUTE:
        QDesktopServices::openUrl( QUrl::fromLocalFile( event->data(0).toString() ) );
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

    QBtCopyDialog dialog( this );
    if( dialog.executeCopy(data, dst->current_path(), QBtOverwriteAnswer::ASK) != QDialog::Accepted ) return;

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


void QBtWorkspace::open_editor()
{
    QBtView* src = 0;
    QBtView* dst = 0;
    if( !src_and_dst_view( src, dst ) ) return;

    BtShared::openInEditor( src->selected_file_path() );
}

void QBtWorkspace::drop_files(const QMap<QString, QVariant> &userInfo)
{
    QStringList files = userInfo["files"].toStringList();
    QString dropPath = userInfo["drop_target"].toString();
    SelectionsSet dropedFiles;
    foreach (QString file, files) {
        dropedFiles.insert(file);
    }
    auto command = QBtOverwriteAnswer::ASK;
    if(!userInfo.contains("action")) {
        command = QBtOverwriteAnswer::AUTORENAME;
    }

    QBtCopyDialog dialog( this );
    if( dialog.executeCopy(dropedFiles, dropPath, command) != QDialog::Accepted ) return;
    left_panel_->current_view()->refresh();
    right_panel_->current_view()->refresh();
}

void QBtWorkspace::open_oposite()
{
    QBtView* src = 0;
    QBtView* dst = 0;
    if( !src_and_dst_view( src, dst ) ) return;
    QString source_path = src->selected_file_path();
    if( QFileInfo( source_path ).isDir() ) {
        dst->update(source_path);
        return;
    }
    dst->update(src->current_path());
}

void QBtWorkspace::open_dir(const QString &path)
{
    QBtView* src = 0;
    QBtView* dst = 0;
    if( !src_and_dst_view( src, dst ) ) return;
    src->update(path);
}
