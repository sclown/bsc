#ifndef INCLUDED_QBtWiper_h
#define INCLUDED_QBtWiper_h

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
 * File         : QBtWiper.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 2007.12.16
 *-------------------------------------------------------------------
 * These algorithms are from book:
 * John Viega and Matt Messier - "Secure Programming Cookbook for C and C++"
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QObject>
#include <QString>
#include <QDialog>
#include <unistd.h>

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtWiper : public QObject
{
    Q_OBJECT

// ******* CONSTRUCTION / DESTRUCTION *******
public:
    QBtWiper( QWidget* );
private:
    QBtWiper( const QBtWiper& );
    QBtWiper& operator=( const QBtWiper& );

// ******* TYPES *******
private:
    enum {
        WIPE_BUFSIZE     = 4096,
        SINGLE_PATS_SIZE = 16,
        TRIPLE_PATS_ROWS = 6,
        TRIPLE_PATS_COLS = 3,
    };

// ******* CONSTANTS *******
private:
    static const unsigned char SINGLE_PATS [ SINGLE_PATS_SIZE ];
    static const unsigned char TRIPLE_PATS [ TRIPLE_PATS_ROWS][TRIPLE_PATS_COLS ];
    static const char* const   WIPE_MESSAGE;

// ******* MEMBERS *******
private:
    QString        file_path_;
    qint32         devrand_fd_;
    qint32         devrand_fd_noblock_;
    qint32         devurand_fd_;
    bool           break_;
    unsigned char  buffer_[ WIPE_BUFSIZE ];

// ******* METHODS *******
public:
   qint32 wipe               ( const QString& );
private:
   qint32  fd_wipe           ( qint32 );
   qint32  pattern_pass      ( qint32, unsigned char*, size_t, size_t );
   qint32  random_pass       ( qint32, size_t );
   qint32  write_data        ( qint32, const void*, size_t );
   qint32  make_fd_noblocking( qint32 );
   qint32  rand_init         ();
   qint32  rand              ( unsigned char*, size_t );
   void    progress          ( qint32 );
   bool    can_do            ();
signals:
   void    wipe_progress     ( const QString& );
};

#endif // INCLUDED_QBtWiper_h
