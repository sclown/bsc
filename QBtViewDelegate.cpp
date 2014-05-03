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
 * File         : QBtViewDelegate.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 29.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtViewDelegate.h"
#include "QBtViewModel.h"
#include "QBtViewItem.h"
#include "QBtView.h"
#include "QBtConfig.h"
#include <QApplication>
#include <QPalette>
#include <QtDebug>

//*******************************************************************
// QBtViewDelegate                                       CONSTRUCTOR
//*******************************************************************
QBtViewDelegate::QBtViewDelegate( QBtView* const in_view )
: QItemDelegate ( in_view )
, view_         ( in_view )
{}
// end of QBtViewDelegate

//*******************************************************************
// paint                                           PRIVATE inherited
//*******************************************************************
void QBtViewDelegate::paint(  QPainter*                   in_painter,
                              const QStyleOptionViewItem& in_options,
                              const QModelIndex&          in_index ) const
{
   const QBtViewModel* const model = dynamic_cast< QBtViewModel* >( view_->model() );
   const QBtViewItem*  const   hit = model->head_item( in_index );
   QStandardItem* const        cit = model->itemFromIndex( in_index );
   QBtConfig* const            cfg = QBtConfig::instance();
   
   const bool current_row  = ( view_->currentIndex().row() == in_index.row() );
   const bool selected     = ( hit ) ? hit->selected() : false;
   const bool has_focus    = view_->hasFocus();

   QStyleOptionViewItem options = in_options;
   QPalette palette             = options.palette;

      
   if( has_focus ) {
      if( current_row ) {
         if( selected ) {
            palette.setColor( QPalette::Highlight      , cfg->bkg_f_c_s_color() );
            palette.setColor( QPalette::HighlightedText, cfg->txt_f_c_s_color() );
         }
         else {
            palette.setColor( QPalette::Highlight      , cfg->bkg_f_c_ns_color() );
            palette.setColor( QPalette::HighlightedText, cfg->txt_f_c_ns_color() );
         }
      }
      else {
         if( selected ) {
            cit->setBackground( cfg->bkg_f_nc_s_color() );
            cit->setForeground( cfg->txt_f_nc_s_color() );
         }
         else {
            cit->setBackground( cfg->bkg_f_nc_ns_color() );
            cit->setForeground( cfg->txt_f_nc_ns_color() );
         }
      }
   }
   else {
      if( current_row ) {
         if( selected ) {
            palette.setColor( QPalette::Highlight      , cfg->bkg_nf_c_s_color() );
            palette.setColor( QPalette::HighlightedText, cfg->txt_nf_c_s_color() );
         }
         else {
            palette.setColor( QPalette::Highlight      , cfg->bkg_nf_c_ns_color() );
            palette.setColor( QPalette::HighlightedText, cfg->txt_nf_c_ns_color() );
         }
      }
      else {
         if( selected ) {
            cit->setBackground( cfg->bkg_nf_nc_s_color() );
            cit->setForeground( cfg->txt_nf_nc_s_color() );
         }
         else {
            cit->setBackground( cfg->bkg_nf_nc_ns_color() );
            cit->setForeground( cfg->txt_nf_nc_ns_color() );
         }
      }
   }
   options.palette = palette;
   QItemDelegate::paint( in_painter, options, in_index );
}
// end of paint
