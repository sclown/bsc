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
 * File         : QBtViewModel.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 28.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtViewSelectionModel.h"
#include "QBtViewItem.h"
#include "QBtViewModel.h"
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const QString     QBtViewSelectionModel::PARENT_DIR             = "..";


//*******************************************************************
// QBtViewSelectionModel                                  CONSTRUCTOR
//*******************************************************************
QBtViewSelectionModel::QBtViewSelectionModel( QBtViewModel* model, QObject* const in_parent ) : QItemSelectionModel( model, in_parent )
, model_ ( model )
{
    int a=0;

}
// end of QBtViewSelectionModel



void QBtViewSelectionModel::setCurrentIndex(const QModelIndex &index, QItemSelectionModel::SelectionFlags command)
{
    QItemSelectionModel::setCurrentIndex(index, command);
}

void QBtViewSelectionModel::select( const QModelIndex &index, QItemSelectionModel::SelectionFlags command )
{
//    if(index.row()==0)
//    {
//        return;
//    }
    QItemSelectionModel::select(index, command);
//    if((command & (QItemSelectionModel::Current | QItemSelectionModel::ClearAndSelect))) {
//        setCurrentIndex(index,command);
//    }
}

void QBtViewSelectionModel::select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command)
{
    if((selection.size()>1 || (selection.size() && selection.front().height()>1)) && selection.contains(model_->index(0,0)))
    {
        QItemSelection row(model_->index(0,0), model_->index(0, model_->columnCount()-1));
        QItemSelection sel_copy(selection);
        sel_copy.merge(row, QItemSelectionModel::Deselect);
        QItemSelectionModel::select(sel_copy, command);
        return;
    }
    QItemSelectionModel::select(selection, command);
}

void QBtViewSelectionModel::select_all(QItemSelectionModel::SelectionFlags command)
{
    QItemSelection allItems(model_->index(0,0), model_->index(model_->rowCount()-1, model_->columnCount()-1));
    select( allItems, command );
}

void QBtViewSelectionModel::select_mask(QString mask, QItemSelectionModel::SelectionFlags command)
{
   QModelIndexList selection = mask_items(mask);
   for(int i=0; i< selection.size(); ++i)
   {
       select(selection[i], command);
   }
}

SelectionsSet QBtViewSelectionModel::get_selected_items() const
{
    QModelIndexList selection = selectedIndexes();
    SelectionsSet result;
    for( int i=0; i< selection.size(); ++i)
    {
        QModelIndex index = selection[i];
        if(index.row() ==0)
        {
            continue;
        }
        if(index.column()==0)
        {
            result.insert(model_->file_path(index));
        }
    }
    return result;
}

int QBtViewSelectionModel::selection_count()
{
    QModelIndexList selection = selectedIndexes();
    int count = 0;
    for( int i=0; i< selection.size(); ++i)
    {
        QModelIndex index = selection[i];
        if(index.row() ==0)
        {
            continue;
        }
        if(index.column()==0)
        {
            ++count;
        }
    }
    return count;
}

QModelIndexList QBtViewSelectionModel::mask_items(const QString &mask) const
{
    QModelIndexList result;
    QRegExp exp( mask, Qt::CaseSensitive, QRegExp::Wildcard );

    const qint32 n = model_->rowCount();
    for( qint32 i = 0; i < n; ++i ) {
       QBtViewItem* it = model_->head_item( i );
       if( it ) {
          if( PARENT_DIR == it->text() ) continue;
          if( exp.indexIn( it->finfo().full_name() ) != -1 ) {
            result.append( model_->row_indexes(i) );
          }
       }
    }
    return result;
}

