#ifndef INCLUDED_QBtTimeStamp_h
#define INCLUDED_QBtTimeStamp_h
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
 * File         : QBtTimeStamp.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 2008.09.18
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>
#include <QDate>
#include <QTime>
#include <QFileInfo>
#include "BtTypes.h"

/*------- forward declaration:
-------------------------------------------------------------------*/
class QPushButton;
class QLabel;
class QCheckBox;
class QDateEdit;
class QTimeEdit;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtTimeStamp : public QDialog
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtTimeStamp( QWidget*, const SelectionsSet&, const QString& );
private:
   QBtTimeStamp( const QBtTimeStamp& );
   QBtTimeStamp& operator=( const QBtTimeStamp& );

//******* CONSTANTS *******
private:
   static const char* const Caption;
   static const char* const DataCaption;
   static const char* const MusterCaption;
   static const char* const Now;
   static const char* const Accept;
   static const char* const Cancel;
   static const char* const UseMuster;
   static const QString     Date;
   static const QString     Time;
   static const QString     DateTime;

//******* MEMBERS *******
private:
   const SelectionsSet data_;
   const QFileInfo     muster_fi_;
   QPushButton*  const now_btn_;
   QPushButton*  const accept_btn_;
   QPushButton*  const cancel_btn_;
   QLabel*       const muster_;
   QCheckBox*    const use_muster_;
   QDateEdit*    const date_edit_;
   QTimeEdit*    const time_edit_;
   QDate               date_bak_;
   QTime               time_bak_;

//******* METHODS *******
private slots:
   void now   ();
   void touch ();
   void muster( bool );
};

#endif // INCLUDED_QBtTimeStamp_h
