#ifndef INCLUDED_QBtInfoField_h
#define INCLUDED_QBtInfoField_h

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
 * File         : QBtCopyDialog.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 02.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QLineEdit>

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtInfoField : public QLineEdit
{
public:
   QBtInfoField( QWidget* const in_parent = 0 ) : QLineEdit( in_parent )
   {
      setReadOnly( true );
      setFrame( false );
      setAlignment( Qt::AlignLeft );
      setFocusPolicy( Qt::NoFocus );
      //setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
   }
private:
   QBtInfoField( const QBtInfoField& );
   QBtInfoField& operator=( const QBtInfoField& );
};

#endif // INCLUDED_QBtInfoField_h
