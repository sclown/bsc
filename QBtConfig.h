#ifndef INCLUDED_QBtConfig_h
#define INCLUDED_QBtConfig_h

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
 * File         : QBtConfig.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 24.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QString>

/*------- class declaration:
-------------------------------------------------------------------*/
// Settings key names shared by the widgets that persist their state
// (window geometry, splitter, panel paths and sort order).
struct QBtConfig
{
   static const QString MAIN_WINDOW_GROUP;
   static const QString POS_KEY;
   static const QString SIZE_KEY;
   static const QString STATE_KEY;
   static const QString WORKSPACE_GROUP;
   static const QString SPLITTER_KEY;
   static const QString LEFT_PANEL_GROUP;
   static const QString RIGHT_PANEL_GROUP;
   static const QString CURR_DIR_KEY;
   static const QString SELECTED_KEY;
   static const QString FOLDERS;
   static const QString SORT_COLUMN;
   static const QString SORT_ORDER;
};

#endif // INCLUDED_QBtConfig_h
