#ifndef INCLUDED_QBtDirSelector_h
#define INCLUDED_QBtDirSelector_h
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
 * File         : QBtDirSelector.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.at
 * Creation date: 2008.09.16
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QTreeView;
class QPushButton;
class QDirModel;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtDirSelector : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtDirSelector( QWidget*, const QString& );
private:
   QBtDirSelector( const QBtDirSelector& );
   QBtDirSelector& operator=( const QBtDirSelector& );

//******* CONSTANTS *******
private:
   static const char* const Caption;
   static const char* const Select;
   static const char* const Close;

//******* MEMBERS *******
private:
   QTreeView*   const view_;
   QPushButton* const select_btn_;
   QPushButton* const cancel_btn_;
   QDirModel*   const model_;
   QString            path_;

//******* METHODS *******
public:
   QString path();
private:
   void showEvent( QShowEvent* );
};

#endif // INCLUDED_QBtDirSelector_h
