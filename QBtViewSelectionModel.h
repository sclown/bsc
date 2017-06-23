#ifndef INCLUDED_QBtViewSelectionModel_h
#define INCLUDED_QBtViewSelectionModel_h

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
 * File         : QBtViewSelectionModel.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "BtTypes.h"
#include <QItemSelectionModel>
#include <vector>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QBtViewItem;
class QBtFileInfo;
class QBtViewModel;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtViewSelectionModel : public QItemSelectionModel
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
    QBtViewSelectionModel( QBtViewModel* model, QObject* = 0 );
private:
   QBtViewSelectionModel( const QBtViewSelectionModel& );
   QBtViewSelectionModel& operator=( const QBtViewSelectionModel& );

//******* CONSTANTS *******
private:
   static const QString     PARENT_DIR;

//******* MEMEBERS *******
private:
   QBtViewModel* model_;

   //******* METHODS *******
public:
   void select_all(QItemSelectionModel::SelectionFlags command);
   void select_mask(QString mask, QItemSelectionModel::SelectionFlags command);
   SelectionsSet get_selected_items() const;
   int selection_count();
private:
   QModelIndexList mask_items(const QString &mask) const;

public slots:
   virtual void setCurrentIndex(const QModelIndex &index, QItemSelectionModel::SelectionFlags command);
   virtual void select( const QModelIndex &index, SelectionFlags command );
   virtual void select( const QItemSelection & selection, QItemSelectionModel::SelectionFlags command );

signals:


};

#endif // INCLUDED_QBtViewSelectionModel_h
