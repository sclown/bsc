#ifndef INCLUDED_QBtAttrDialog_h
#define INCLUDED_QBtAttrDialog_h

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
 * File         : QBtAttrDialog.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 06.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "BtTypes.h"
#include <QDialog>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QCheckBox;
class QPushButton;
class QLabel;

/*------- class declarations:
-------------------------------------------------------------------*/
class QBtAttrDialog : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtAttrDialog( QWidget*, const SelectionsSet& );
private:
   QBtAttrDialog( const QBtAttrDialog& );
   QBtAttrDialog& operator=( const QBtAttrDialog& );

//******* CONSTANTS *******
private:
   static const char* const CAPTION;
   static const char* const FILE_NAME;
   static const char* const USER;
   static const char* const GROUP;
   static const char* const OTHER;
   static const char* const RECURSIVE;
   static const char* const ACCEPT;
   static const char* const CLOSE;
   static const char* const READ;
   static const char* const WRITE;
   static const char* const EXECUTE;

//******* MEMBERS *******
private:
   SelectionsSet      data_;
   QLabel*      const path_;
   QCheckBox*   const user_read_;
   QCheckBox*   const user_write_;
   QCheckBox*   const user_exec_;
   QCheckBox*   const group_read_;
   QCheckBox*   const group_write_;
   QCheckBox*   const group_exec_;
   QCheckBox*   const other_read_;
   QCheckBox*   const other_write_;
   QCheckBox*   const other_exec_;
   QCheckBox*   const recursive_;
   QPushButton* const close_;
   QPushButton* const accept_;

//******* METHODS *******
private:
   void    set_info();
   QString get_info() const;
   bool    are_dirs() const;
   void    execute ( const QString& ) const;
private slots:
   void    accept  ();
};

#endif // INCLUDED_QBtAttrDialog_h
