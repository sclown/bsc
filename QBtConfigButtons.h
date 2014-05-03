#ifndef INCLUDED_QBtConfigButtons_h
#define INCLUDED_QBtConfigButtons_h

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
 * File         : QBtConfigButtons.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QWidget>
#include <QFont>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QRadioButton;
class QPushButton;
class QLabel;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtConfigButtons : public QWidget
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtConfigButtons( QWidget* = 0 );
private:
   QBtConfigButtons( const QBtConfigButtons& );
   QBtConfigButtons& operator=( const QBtConfigButtons& );

//******* CONSTANTS ********
private:
   static const char* const CAPTION_MODE;
   static const char* const CAPTION_FONT;
   static const char* const CAPTION_EXAMPLE;
   static const char* const CAPTION_FONTS;
   static const char* const ONLY_FN;
   static const char* const ONLY_TEXT;
   static const char* const FULL_DESC;
   static const char* const TWO_LINES;
   static const char* const DEFAULT_FONT;
   static const char* const CUSTOM_FONT;
   static const char* const EXAMPLE;
   static const char* const SELECT_FONT;
   static const char* const FONT_ICON;

//******* MEMBERS *******
private:
   QRadioButton* const only_fn_;
   QRadioButton* const only_text_;
   QRadioButton* const full_desc_;
   QRadioButton* const two_lines_;
   QRadioButton* const default_font_;
   QRadioButton* const custom_font_;
   QPushButton*  const select_font_;
   QPushButton*  const example_;
   QLabel*       const font_desc_;
   QFont               font_;
   qint32              mode_;
   

//******* METHODS *******
public:
   void save           () const;
private:
   void restore        ();
   void update_example ();
   void update_state   ();
private slots:
   void only_fn        ( bool );
   void only_text      ( bool );
   void full_desc      ( bool );
   void two_lines      ( bool );
   void default_font   ( bool );
   void custom_font    ( bool );
   void select_font    ();
};

#endif // INCLUDED_QBtConfigButtons_h
