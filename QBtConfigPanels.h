#ifndef INCLUDED_QBtConfigPanels_h
#define INCLUDED_QBtConfigPanels_h
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
 * File         : QBtConfigPanels.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 26.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QWidget>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QPushButton;
class QLabel;
class QBtColorDemo;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtConfigPanels : public QWidget
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtConfigPanels( QWidget* = 0 );
private:
   QBtConfigPanels( const QBtConfigPanels& );
   QBtConfigPanels& operator=( const QBtConfigPanels& );

//******* CONSTANTS ********
private:
   static const char* const EXAMPLE;
   static const char* const FONT;
   static const char* const BACKGROUND;
   static const char* const FOREGROUND;
   static const char* const DEFAULT;
   static const char* const WIDTH_FOCUS;
   static const char* const WIDTHOUT_FOCUS;
   static const char* const NC_NS;
   static const char* const NC_S;
   static const char* const C_NS;
   static const char* const C_S;
   static const char* const ICO_FONT;
   static const char* const ICO_COLOR;
      
//******* MEMBERS *******
private:
   QPushButton  * const font_;
   QLabel       * const font_example_;
   QBtColorDemo * const f_nc_ns_demo_;
   QPushButton  * const f_nc_ns_def_;
   QPushButton  * const f_nc_ns_bkg_;
   QPushButton  * const f_nc_ns_txt_;
   QBtColorDemo * const f_nc_s_demo_;
   QPushButton  * const f_nc_s_def_;
   QPushButton  * const f_nc_s_bkg_;
   QPushButton  * const f_nc_s_txt_;
   QBtColorDemo * const f_c_ns_demo_;
   QPushButton  * const f_c_ns_def_;
   QPushButton  * const f_c_ns_bkg_;
   QPushButton  * const f_c_ns_txt_;
   QBtColorDemo * const f_c_s_demo_;
   QPushButton  * const f_c_s_def_;
   QPushButton  * const f_c_s_bkg_;
   QPushButton  * const f_c_s_txt_;
   QBtColorDemo * const nf_nc_ns_demo_;
   QPushButton  * const nf_nc_ns_def_;
   QPushButton  * const nf_nc_ns_bkg_;
   QPushButton  * const nf_nc_ns_txt_;
   QBtColorDemo * const nf_nc_s_demo_;
   QPushButton  * const nf_nc_s_def_;
   QPushButton  * const nf_nc_s_bkg_;
   QPushButton  * const nf_nc_s_txt_;
   QBtColorDemo * const nf_c_ns_demo_;
   QPushButton  * const nf_c_ns_def_;
   QPushButton  * const nf_c_ns_bkg_;
   QPushButton  * const nf_c_ns_txt_;
   QBtColorDemo * const nf_c_s_demo_;
   QPushButton  * const nf_c_s_def_;
   QPushButton  * const nf_c_s_bkg_;
   QPushButton  * const nf_c_s_txt_;
   
//******* METHODS *******
private:
   void init() const;
public:
   void save() const;
private slots:
   void select_font ();
   //...................
   void f_nc_ns_def ();
   void f_nc_s_def  ();
   void f_c_ns_def  ();
   void f_c_s_def   ();
   //..................
   void nf_nc_ns_def();
   void nf_nc_s_def ();
   void nf_c_ns_def ();
   void nf_c_s_def  ();
};

#endif // INCLUDED_QBtConfigPanels_h
