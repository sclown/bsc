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
#include "QBtWiper.h"
#include "QBtShared.h"
#include <QEvent>
#include <QApplication>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>

/*------- local constants:
-------------------------------------------------------------------*/
const unsigned char QBtWiper::SINGLE_PATS [ SINGLE_PATS_SIZE ] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
};
const unsigned char QBtWiper::TRIPLE_PATS [ TRIPLE_PATS_ROWS ][ TRIPLE_PATS_COLS ] = {
    { 0x92, 0x49, 0x24 },
    { 0x49, 0x24, 0x92 },
    { 0x24, 0x92, 0x49 },
    { 0x6D, 0xB6, 0xDB },
    { 0xB6, 0xDB, 0x6D },
    { 0xDB, 0x6D, 0xB6 }
};
const char* const QBtWiper::WIPE_MESSAGE = QT_TR_NOOP( "Wipe file (step #%1/7) : %2" );

//*******************************************************************
// QBtWiper                                             CONSTRUCTOR
//*******************************************************************
QBtWiper::QBtWiper( QWidget* const in_parent ) : QObject( in_parent )
, file_path_         ( QString() )
, devrand_fd_        ( -1 )
, devrand_fd_noblock_( -1 )
, devurand_fd_       ( -1 )
, break_             ( false )
{}
// end of QBtWiper

//*******************************************************************
// wipe                                                       PUBLIC
//*******************************************************************
qint32 QBtWiper::wipe( const QString& in_file_path )
{
   qint32 retval = -1;
   file_path_ = in_file_path;
   
   if( can_do() ) {
      const qint32 fd = open( file_path_.toLocal8Bit(), O_RDWR );
      if( fd != -1 ) {
         retval = fd_wipe( fd );
         close( fd );
      }
   }
   return retval;
}
// end of wipe


//###################################################################
//#                                                                 #
//#                       P R I V A T E                             #
//#                                                                 #
//###################################################################


//*******************************************************************
// fd_wipe                                                   PRIVATE
//*******************************************************************
qint32 QBtWiper::fd_wipe( const int in_fd )
{
    struct stat finfo;

    progress( 0 );
    if( -1 == fstat( in_fd, &finfo )) {
        return -1;
    }
    if(  0 == finfo.st_size ) {
        return  0;
    }

    // czterokrotnie wypelniamy plik liczbami losowymi
    progress( 1 );
    if( !can_do() ) return -1;
    for( int pass = 0; can_do() && ( pass < 4 ); pass++ ) {
        if( -1 == random_pass( in_fd, finfo.st_size )) {
            return -1;
        }
    }

    // jednorkotnie wypelniamy plik szosta liczba
    // (indeks 5, wartosc 0x55) z 'SINGLE_PATS'
    progress( 2 );
    if( !can_do() ) return -1;
    memset( buffer_, SINGLE_PATS[5], WIPE_BUFSIZE );
    if( -1 == pattern_pass( in_fd, buffer_, WIPE_BUFSIZE, finfo.st_size )) {
        return -1;
    }

    // jednokrotnie wypelniamy plik jedenasta liczba
    // (indeks 10, wartosc 0xAA) z 'SINGLE_PATS'
    progress( 3 );
    if( !can_do() ) return -1;
    memset( buffer_, SINGLE_PATS[10], WIPE_BUFSIZE );
    if( -1 == pattern_pass( in_fd, buffer_, WIPE_BUFSIZE, finfo.st_size )) {
        return -1;
    }

    // wypelniamy caly plik trzy razy, uzywajac kolejno
    // trzy trojki z 'TRIPLE_PATS'
    progress( 4 );
    if( !can_do() ) return -1;
    const int pattern_size = sizeof( TRIPLE_PATS[0] );
    for( int pass = 0; can_do() && ( pass < 3 ); pass++ ) {
        const unsigned char* const pattern = TRIPLE_PATS[ pass ];
        const int count = WIPE_BUFSIZE / pattern_size;
        for( int i = 0; can_do() && ( i < count );  i++ ) {
            memcpy( buffer_ + ( i * pattern_size ), pattern, pattern_size );
        }
        if( -1 == pattern_pass( in_fd, buffer_, pattern_size * count, finfo.st_size )) {
           return -1;
        }
    }

    // Wypelnienie calego pliku po kolei kazda z liczb z 'SINGLE_PATS'
    progress( 5 );
    if( !can_do() ) return -1;
    for( unsigned int pass = 0; can_do() && ( pass < SINGLE_PATS_SIZE ); pass++ ) {
        memset( buffer_, SINGLE_PATS[pass], WIPE_BUFSIZE );
        if( -1 == pattern_pass( in_fd, buffer_, WIPE_BUFSIZE, finfo.st_size )) {
            return -1;
        }
    }
    
    // wypelniamy caly plik uzywajac wszytkich
    // trojek z 'TRIPLE_PATS'
    progress( 6 );
    if( !can_do() ) return -1;
    for( unsigned int pass = 0; can_do() && ( pass < TRIPLE_PATS_ROWS ); pass++ ) {
        const unsigned char* const pattern = TRIPLE_PATS[ pass ];
        const int count = WIPE_BUFSIZE / pattern_size;
        for( int i = 0; can_do() && ( i < count ); i++ ) {
            memcpy( buffer_ + ( i * pattern_size ), pattern, pattern_size );
        }
        if( -1 == pattern_pass( in_fd, buffer_, pattern_size * count, finfo.st_size )) {
            return -1;
        }
    }

    progress( 7 );
    if( !can_do() ) return -1;
    for( int pass = 0; can_do() && ( pass < 4 ); pass++ ) {
        if( -1 == random_pass( in_fd, finfo.st_size )) {
            return -1;
        }
    }

    return can_do() ? 0 : -1;
}
// end of fd_wipe

//*******************************************************************
// pattern_pass                                              PRIVATE
//*******************************************************************
qint32 QBtWiper::pattern_pass( qint32         in_fd,
                               unsigned char* in_buffer,
                               size_t         in_bufsize,
                               size_t         in_filesize )
{
   if( !can_do() ) return -1;
    
   size_t towrite;
   if( ( 0 == in_bufsize ) || ( lseek( in_fd, 0, SEEK_SET ) != 0 ) ) {
      return -1;
   }
    
   while( can_do() && (  in_filesize > 0 ) ) {
      towrite = ( in_filesize > in_bufsize ) ? in_bufsize : in_filesize;
      if( 0 == write_data( in_fd, in_buffer, towrite ) ) {
         return -1;
      }
      in_filesize -= towrite;
   }
   fsync( in_fd );
   return 0;
}
// end of pattern_pass

//*******************************************************************
// random_pass                                               PRIVATE
//*******************************************************************
qint32 QBtWiper::random_pass( qint32 in_fd, size_t in_nbytes )
{
   if( !can_do() ) return -1;

   size_t towrite;
   unsigned char buffer[ WIPE_BUFSIZE ];
   size_t wipe_bufsize = static_cast< size_t >( WIPE_BUFSIZE );

   if( lseek( in_fd, 0, SEEK_SET ) != 0 )  return -1;
   
   while( can_do() && ( in_nbytes > 0 ) ) {
      towrite = ( in_nbytes > wipe_bufsize ) ? wipe_bufsize : in_nbytes;
      if( -1 == rand( buffer, towrite ) ) {
         return -1;
      }
      if( 0 == write_data( in_fd, buffer, towrite ) ) {
         return -1;
      }
      in_nbytes -= towrite;
   }
   fsync( in_fd );
   return 0;
}
// end of random_pass

//*******************************************************************
// write_data                                                PRIVATE
//*******************************************************************
qint32 QBtWiper::write_data(  const qint32      in_fd,
                              const void* const in_buffer,
                              const size_t      in_nbytes )
{
   if( !can_do() ) return 0;

   const char* const ptr = static_cast< const char* const >( in_buffer );
   size_t  towrite = size_t();
   size_t  written = size_t();
   qint32  n       = qint32();

   do {
      if( !can_do() ) return 0;
        
      if( ( in_nbytes - written ) > SSIZE_MAX ) towrite = SSIZE_MAX;
      else towrite = in_nbytes - written;

      if( ( n = write( in_fd, ptr + written, towrite ) ) >= 0 ) {
         written += n;
      }
      else if( errno != EINTR ) {
         return 0;
      }
   } while( written < in_nbytes );

   return 1;
}
// end of write_data

//*******************************************************************
// make_fd_noblocking                                        PRIVATE
//*******************************************************************
qint32 QBtWiper::make_fd_noblocking( qint32 in_fd )
{
   qint32 flags = fcntl( in_fd, F_GETFL );
   if( flags != -1 ) {
      flags |= O_NONBLOCK;
      if( fcntl( in_fd, F_SETFL, flags ) != -1 ) {
         return 0;
      }
   }
   return -1;
}
// end of make_fd_noblocking
    
//*******************************************************************
// rand_init                                                 PRIVATE
//*******************************************************************
qint32 QBtWiper::rand_init()
{
    devrand_fd_         = open( "/dev/random" , O_RDONLY );
    devrand_fd_noblock_ = open( "/dev/random" , O_RDONLY );
    devurand_fd_        = open( "/dev/urandom", O_RDONLY );
        
   if( ( -1 == devrand_fd_ ) || ( -1 == devrand_fd_noblock_ ) ) {
      return -3;
   }
   if( -1 == devurand_fd_ ) {
      return -2;
   }
   return make_fd_noblocking( devrand_fd_noblock_ );
}
// end of rand_init

//*******************************************************************
// rand                                                      PRIVATE
//*******************************************************************
qint32 QBtWiper::rand( unsigned char* const out_buffer, size_t in_nbytes )
{
    ssize_t        bytes_read;
    unsigned char* where = out_buffer;
    
   if(( -1 == devrand_fd_ ) || ( -1 == devrand_fd_noblock_ ) || ( -1 == devurand_fd_ ) ) {
      if( rand_init() ) return -1;
   }
    
   while( in_nbytes ) {
      if( -1 == ( bytes_read = read( devurand_fd_, where, in_nbytes ))) {
         if( EINTR == errno ) continue;
         else                 return -1;
      }
      where     += bytes_read;
      in_nbytes -= bytes_read;
   }
   return 0;
}
// end of rand

//*******************************************************************
// progress                                                  PRIVATE
//*******************************************************************
void QBtWiper::progress( const qint32 in_progress )
{
   if( can_do() ) {
      const QString msg = tr( WIPE_MESSAGE  ).arg( in_progress ).arg( file_path_ );
      emit wipe_progress( msg );
   }
}
// end of progress

//*******************************************************************
// can_do                                                    PRIVATE
//*******************************************************************
bool QBtWiper::can_do()
{
    QBtShared::idle();
    return !break_;
}
// end of can_do
