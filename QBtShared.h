#ifndef INCLUDED_QBtShared_h
#define INCLUDED_QBtShared_h

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
 * File         : QBtShared.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 24.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QObject>
#include <QString>
#include <QFile>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QWidget;
class QSize;
class QBtFileInfo;
class QFontMetrics;
class QCursor;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtShared : public QObject
{
   Q_OBJECT

// ******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtShared() : QObject() {}
private:
   QBtShared( const QBtShared& );
   QBtShared& operator=( const QBtShared& );

// ******* MEMBERS *******
private:
   static QString buffer_;

// ******* CONSTANTS *******
public:
   static const QChar   DIGIT_SEPARATOR;
   static const QString THIS_DIR;
   static const QString PARENT_DIR;
   static const QString ORGANISATION;
   static const QString VERSION;
   static const QString PROGRAM_NAME;

// ******* METHODS *******
public:
   static const QString& program_name();
   static void           resize         ( QWidget*, qint32, qint32 );
   static void           resize         ( QWidget*, const QSize& );
   static void           resize_width   ( QWidget*, qint32 );
   static QString&       reverse        ( QString& );
   static QString        num2str        ( qint64, const QChar& = DIGIT_SEPARATOR );
   static QString        access         ( const QBtFileInfo& );
   static void           idle           ();
   static void           elide          ( const QFontMetrics&, qint32, QString& );
   static QString        auto_rename    ( const QString& );
   static bool           is_regular_file( const QString& );
   static bool           is_binary_file ( const QString& );
   static bool           is_gnome       ();
   static void           set_cursor     ( const QCursor& );
   static void           restore_cursor ();
   static QString        slashed_dir    ( const QString& );
   static QString        quoted_fpath   ( const QString& );
   static void           touch          ( const QString&, const QString& );
};

#endif // INCLUDED_QBtShared_h
