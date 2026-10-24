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
public:
   QBtFileInfo() = default;
   QBtFileInfo( const QFileInfo& fi )
   : is_dir_      ( fi.isDir()            )
   , is_symlink_  ( fi.isSymLink()        )
   , is_readable_ ( fi.isReadable()       )
   , size_        ( fi.size()             )
   , permission_  ( fi.permissions()      )
   , lastmod_     ( fi.lastModified()     )
   , owner_       ( fi.owner()            )
   , group_       ( fi.group()            )
   , full_name_   ( fi.fileName()         )
   , path_        ( fi.absoluteFilePath() )
   , ext_         ( fi.suffix()           )
   {}

   bool               is_dir      () const { return is_dir_;      }
   bool               is_symlink  () const { return is_symlink_;  }
   bool               is_readable () const { return is_readable_; }
   qint64             size        () const { return size_;        }
   QFile::Permissions permission  () const { return permission_;  }
   const QDateTime&   lastmod     () const { return lastmod_;     }
   const QString&     owner       () const { return owner_;       }
   const QString&     group       () const { return group_;       }
   const QString&     path        () const { return path_;        }   // absolute path
   const QString&     full_name   () const { return full_name_;   }   // name with every extension
   const QString&     ext         () const { return ext_;         }

private:
   bool               is_dir_      = false;
   bool               is_symlink_  = false;
   bool               is_readable_ = false;
   qint64             size_        = 0;
   QFile::Permissions permission_;
   QDateTime          lastmod_;
   QString            owner_;
   QString            group_;
   QString            full_name_;
   QString            path_;
   QString            ext_;
};
Q_DECLARE_METATYPE( QBtFileInfo )

#endif // INCLUDED_QBtFileInfo_h
