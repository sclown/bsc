#ifndef INCLUDED_QBtFileInfo_h
#define INCLUDED_QBtFileInfo_h

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
 * File         : QBtFileInfo.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 31.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QFileInfo>
#include <QString>
#include <QDateTime>
#include <QMetaType>

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtFileInfo
{
//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtFileInfo()
   : is_dir_        ( false                )
   , is_executable_ ( false                )
   , is_file_       ( false                )
   , is_symlink_    ( false                )
   , is_readable_   ( false                )
   , is_writable_   ( false                )
   , size_          ( qint64()             )
   , permission_    ( QFile::Permissions() )
   , lastmod_       ( QDateTime()          )
   , owner_         ( QString()            )
   , group_         ( QString()            )
   , full_name_     ( QString()            )
   , path_          ( QString()            )
   , name_          ( QString()            )
   , ext_           ( QString()            )
   {}
   QBtFileInfo( const QBtFileInfo& rhs )
   : is_dir_        ( rhs.is_dir_        )
   , is_executable_ ( rhs.is_executable_ )
   , is_file_       ( rhs.is_file_       )
   , is_symlink_    ( rhs.is_symlink_    )
   , is_readable_   ( rhs.is_readable_   )
   , is_writable_   ( rhs.is_writable_   )
   , size_          ( rhs.size_          )
   , permission_    ( rhs.permission_    )
   , lastmod_       ( rhs.lastmod_       )
   , owner_         ( rhs.owner_         )
   , group_         ( rhs.group_         )
   , full_name_     ( rhs.full_name_     )
   , path_          ( rhs.path_          )
   , name_          ( rhs.name_          )
   , ext_           ( rhs.ext_           )
   {}
   QBtFileInfo( const QFileInfo& fi )
   : is_dir_        ( fi.isDir()            )
   , is_executable_ ( fi.isExecutable()     )
   , is_file_       ( fi.isFile()           )
   , is_symlink_    ( fi.isSymLink()        )
   , is_readable_   ( fi.isReadable()       )
   , is_writable_   ( fi.isWritable()       )
   , size_          ( fi.size()             )
   , permission_    ( fi.permissions()      )
   , lastmod_       ( fi.lastModified()     )
   , owner_         ( fi.owner()            )
   , group_         ( fi.group()            )
   , full_name_     ( fi.fileName()         )
   , path_          ( fi.absoluteFilePath() )
   , name_          ( fi.completeBaseName() )
   , ext_           ( fi.suffix()           )
   {}
   QBtFileInfo& operator=( const QBtFileInfo& rhs )
   {
      if( this != &rhs ) {
         is_dir_        = rhs.is_dir_        ;
         is_executable_ = rhs.is_executable_ ;
         is_file_       = rhs.is_file_       ;
         is_symlink_    = rhs.is_symlink_    ;
         is_readable_   = rhs.is_readable_   ;
         is_writable_   = rhs.is_writable_   ;
         size_          = rhs.size_          ;
         permission_    = rhs.permission_    ;
         lastmod_       = rhs.lastmod_       ;
         owner_         = rhs.owner_         ;
         group_         = rhs.group_         ;
         full_name_     = rhs.full_name_     ;
         path_          = rhs.path_          ;
         name_          = rhs.name_          ;
         ext_           = rhs.ext_           ;
      }
      return *this;
   }
   
   ~QBtFileInfo()
   {}

   bool               is_dir       () const { return is_dir_;        }
   bool               is_executable() const { return is_executable_; }
   bool               is_file      () const { return is_file_;       }
   bool               is_symlink   () const { return is_symlink_;    }
   bool               is_readable  () const { return is_readable_;   }
   bool               is_writable  () const { return is_writable_;   }
   qint64             size         () const { return size_;          }
   QFile::Permissions permission   () const { return permission_;    }
   const QDateTime&   lastmod      () const { return lastmod_;       }
   const QString&     owner        () const { return owner_;         }
   const QString&     group        () const { return group_;         }
   const QString&     path         () const { return path_;          }
   const QString&     full_name    () const { return full_name_;     }
   const QString&     name         () const { return name_;          }
   const QString&     ext          () const { return ext_;           }

   
//******* MEMBERS *******
private:
   bool      is_dir_;
   bool      is_executable_;
   bool      is_file_;
   bool      is_symlink_;
   bool      is_readable_;
   bool      is_writable_;
   qint64    size_;
   QFile::Permissions permission_;
   QDateTime lastmod_;
   QString   owner_;
   QString   group_;
   QString   full_name_;   // nazwa ze wszystkimi rozszerzeniami
   QString   path_;
   QString   name_;        // nazwa bez ostatniego rozszerzenia
   QString   ext_;         // ostatnie rozszerzenie
};
Q_DECLARE_METATYPE( QBtFileInfo )

#endif // INCLUDED_QBtFileInfo_h
