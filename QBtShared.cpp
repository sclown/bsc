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
#include "QBtShared.h"
#include "QBtFileInfo.h"
#include "QBTMacTools.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFontMetrics>
#include <QDateTime>
#include <QProcess>
#include <QWidget>
#include <QDir>
#include <QUrl>

/*------- constants:
-------------------------------------------------------------------*/
const QChar       QBtShared::DIGIT_SEPARATOR = ',';
const QString     QBtShared::PARENT_DIR      = "..";
const QString     QBtShared::THIS_DIR        = ".";
const QString QBtShared::ORGANISATION    = "beesoft.org";
const QString QBtShared::VERSION         = "4.1.1";
const QString QBtShared::PROGRAM_NAME    = "Beesoft Commander";

/*------- variables:
-------------------------------------------------------------------*/
QString QBtShared::buffer_;


//*******************************************************************
// full_program_name
//*******************************************************************
const QString& QBtShared::program_name()
{
   buffer_  = PROGRAM_NAME;
   buffer_ += ", ver. ";
   buffer_ += VERSION;

   return buffer_;
}
// end of program_name

//*******************************************************************
// resize
//*******************************************************************
void QBtShared::resize( QWidget* const in_widget, const qint32 in_width, const qint32 in_height )
{
   const qint32 screen_width = qApp->desktop()->availableGeometry().width();
   const qint32 screen_height = qApp->desktop()->availableGeometry().height();
   const qreal  width_percent = static_cast<qreal>( in_width ) / 100.0;
   const qreal  height_percent = static_cast<qreal>( in_height ) / 100.0;    
   const qint32 dx = static_cast<qint32>( screen_width * width_percent );
   const qint32 dy = static_cast<qint32>( screen_height * height_percent );

   resize( in_widget, QSize( dx, dy ));
}
void QBtShared::resize( QWidget* const in_widget, const QSize& in_size )
{
    const qint32 screen_width  = qApp->desktop()->availableGeometry().width();
    const qint32 screen_height = qApp->desktop()->availableGeometry().height();
    const qint32 x  = ( screen_width  - in_size.width()  + 1 ) >> 1;
    const qint32 y  = ( screen_height - in_size.height() + 1 ) >>  1;
    
    in_widget->move( x, y );
    in_widget->resize( in_size.width(), in_size.height() );
}
void QBtShared::resize_width( QWidget* const in_widget, const qint32 in_width )
{
   const qint32 screen_width  = qApp->desktop()->screenGeometry( 0 ).width();
   const qreal  width_percent = ( static_cast<qreal>( in_width ) ) / 100.0;
   
   QSize s = in_widget->size();
   s.setWidth( static_cast<qint32>( screen_width * width_percent ) );
   resize( in_widget, s );
}

// end of resize

//*******************************************************************
// reverse
//*******************************************************************
QString& QBtShared::reverse( QString& in_text )
{
   const qint32 n = in_text.length();
   if( n ) {
      qint32 idx1 = 0;
      qint32 idx2 = n - 1;
      while( idx1 < idx2 ) {
         const QChar tmp = in_text[ idx1 ];
         in_text[ idx1 ] = in_text[ idx2 ];
         in_text[ idx2 ] = tmp;
         ++idx1;
         --idx2;
      }
   }
   return in_text;
}
// end of reverse

//*******************************************************************
// num2str
//*******************************************************************
QString QBtShared::num2str( const qint64 in_value, const QChar& in_sep )
{
   QString   text = QString::number( in_value );
   const qint32 n = text.length();

   if( n < 4 ) return text;
   
   reverse( text );
   
   const qint32 n_div_3 = n / 3;
   const qint32 m       = ( n % 3 )  ? n_div_3 : ( n_div_3 - 1 );
   QString result( n + m, ' ' );

   qint32 src_idx = 0;
   qint32 dst_idx = 0;
   qint32 k       = 0;

   while( src_idx < n ) {
      if( 3 == k ) {
         result[ dst_idx++ ] = in_sep;
         k = 0;
      }
      result[ dst_idx++ ]= text[ src_idx++ ];
      ++k;
   }   
   
   return reverse( result );
}
// end of num2str

//*******************************************************************
// access
//*******************************************************************
QString QBtShared::access( const QBtFileInfo& in_fi )
{
   const QFile::Permissions p = in_fi.permission();
   QString buffer = "----------";

        if( in_fi.is_dir()     ) buffer[ 0 ] = 'd';
   else if( in_fi.is_symlink() ) buffer[ 0 ] = 'l';
   
   if( p & QFile::ReadUser   ) buffer[ 1 ] = 'r';
   if( p & QFile::WriteUser  ) buffer[ 2 ] = 'w';
   if( p & QFile::ExeUser    ) buffer[ 3 ] = 'x';
   if( p & QFile::ReadGroup  ) buffer[ 4 ] = 'r';
   if( p & QFile::WriteGroup ) buffer[ 5 ] = 'w';
   if( p & QFile::ExeGroup   ) buffer[ 6 ] = 'x';
   if( p & QFile::ReadOther  ) buffer[ 7 ] = 'r';
   if( p & QFile::WriteOther ) buffer[ 8 ] = 'w';
   if( p & QFile::ExeOther   ) buffer[ 9 ] = 'x';

   return buffer;
}
// access

//*******************************************************************
// idle
//*******************************************************************
void QBtShared::idle()
{
   QApplication::processEvents();
}
// end of idle

//*******************************************************************
// is_gnome
//*******************************************************************
bool QBtShared::is_gnome()
{
   const char* const str = getenv( "GNOME_DESKTOP_SESSION_ID" );
   if( str ) {
      if( QString( str ) == "Default" ) {
         return true;
      }
   }
   return false;
}
// end of is_gnome

//*******************************************************************
// elide
//*******************************************************************
void QBtShared::elide( const QFontMetrics& in_fm, const qint32 in_width, QString& in_out_path )
{
   static const QString SLASH  = "/";
   static const QString SPACER = "/.../";
   static const QString SPACE  = " ";
   
   if( in_fm.width( in_out_path ) > in_width ) {
      const QStringList    WORDS  = in_out_path.split( SLASH, QString::SkipEmptyParts );

      int     i        = 0;
      int     j        = WORDS.count() - 1;
      QString lft_part = SPACE;
      QString rgt_part = WORDS[j--];
      bool    lft_ok   = true;
      bool    rgt_ok   = true;
      QString tmp;

      while( lft_ok || rgt_ok ) {
         lft_ok = false;
         if( i < j ) {
            tmp = lft_part + SLASH + WORDS[i];
            if( in_fm.width( tmp + SPACER + rgt_part ) < in_width ) {
               lft_part = tmp;
               lft_ok = true;
               ++i;
            }
         }
         //.............................................
         rgt_ok = false;
         if( i < j ) {
            tmp = WORDS[j] + SLASH + rgt_part;
            if( in_fm.width( lft_part + SPACER + tmp ) < in_width ) {
               rgt_part = tmp;
               rgt_ok = true;
               --j;
            }
         }
      }
      in_out_path = lft_part + SPACER + rgt_part;
   }
}
// end of elide

//*******************************************************************
// auto_rename
//*******************************************************************
QString QBtShared::auto_rename( const QString& path )
{
   const QFileInfo fi( path );
   const QString base_name = fi.baseName();
   const QString full_ext = fi.completeSuffix();
   
   QString dir = fi.path();
   if( !dir.endsWith( '/' ) ) dir += '/';

   QString new_name = QString();
   
   for(int i = 1; i < 10000; ++i ) {
      new_name  = dir;
      new_name += base_name;
      new_name += ".";
      new_name += QString::number( i );
      if( !full_ext.isEmpty() ) {
         new_name += ".";
         new_name += full_ext;
      }
      if( !QFile::exists( new_name ) ) {
         return new_name;
      }
   }
   return path;
}
// end of auto_rename

//*******************************************************************
// is_regular_file
//*******************************************************************
bool QBtShared::is_regular_file( const QString& in_fname )
{
   if( in_fname.isEmpty() ) return false;
   return ( ( THIS_DIR != in_fname ) && ( PARENT_DIR != in_fname ) );
}
// end of is_regular_file

//*******************************************************************
// is_binary_file                                             PUBLIC
//-------------------------------------------------------------------
// Sprawdzamy czy wskazany plik jest plikiem binarnym.
// UWAGA:
//    Nie wykonujemy tutaj kontroli praw dostepu do pliku.
//    Zakladamy, ze mamy prawo do jego odczytu.
//      Kontrola powinna    byc zostac wykonana wczesniej.
//*******************************************************************
bool QBtShared::is_binary_file( const QString& in_fpath )
{
   bool retval = false;
   
   QFile file( in_fpath );
   if( file.open( QIODevice::ReadOnly  ) ) {
      QDataStream in( &file );
      char buffer[ 8 ];
      in.readRawData( buffer, 4 );
      retval = (( 'E' == buffer[1] ) && ( 'L' == buffer[2] ) && ( 'F' == buffer[3] ));
      file.close();
   }
   return retval;
}
// end of is_binary_file

//*******************************************************************
// set_cursor
//*******************************************************************
void QBtShared::set_cursor( const QCursor& in_cursor )
{
   if( !QApplication::overrideCursor() || ( QApplication::overrideCursor()->shape() != in_cursor.shape() ) ) {
      QApplication::setOverrideCursor( in_cursor );
   }
}
// end of set_cursor

//*******************************************************************
// restore_cursor
//*******************************************************************
void QBtShared::restore_cursor()
{
   QApplication::restoreOverrideCursor();
}
// end of restore_cursor

//*******************************************************************
// slashed_dir
//*******************************************************************
QString QBtShared::slashed_dir( const QString& in_dir )
{
   return ( in_dir.endsWith( QDir::separator() ) ? in_dir : ( in_dir + QDir::separator() ) );
}
// end of slashed_dir

//*******************************************************************
// quoted_fpath
//*******************************************************************
QString QBtShared::quoted_fpath( const QString& in_fpath )
{
   if( in_fpath.startsWith( '\"' ) ) return in_fpath;
   return ( "\"" + in_fpath + "\"" );
}
// end of quoted_fpath

//*******************************************************************
// touch                                                PRIVATE slot
//*******************************************************************
void QBtShared::touch( const QString& in_muster_fpath, const QString& in_dst_fpath )
{
   static const QString Touch ="touch -c -t %1 %2";
   static const QString Format ="yyyyMMddhhmm.ss";

   const QString dt  = QFileInfo( in_muster_fpath ).lastModified().toString( Format );
   const QString cmd = Touch.arg( dt ).arg( QBtShared::quoted_fpath( in_dst_fpath ) );
   QProcess process;
   process.startDetached( cmd );
}
// end of touch

QStringList QBtShared::urlsToPathes(const QList<QUrl> &urls)
{
    QStringList pathes;
    for(QUrl url: urls) {
        if(isMacSpecialURL(url)) {
            url = resolveMacSpecialURL(url);
        }
        if(url.scheme() == "file"){
            pathes += url.path();
        }
    }
    return pathes;
}

QFileInfo QBtShared::pathFromFSItem(const QFileInfo &info)
{
    return info.isDir()? info : info.absolutePath();
}
