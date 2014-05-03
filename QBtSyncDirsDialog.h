#ifndef INCLUDED_QBtSyncDirsDialog_h
#define INCLUDED_QBtSyncDirsDialog_h
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
 * File         : QBtSyncDirsDialog.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.at
 * Creation date: 2008.09.16
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QDialog>
#include <QFileInfo>
#include <QHash>
#include <QMap>

/*------- forward declarations:
-------------------------------------------------------------------*/
class QPushButton;
class QTreeWidget;
class QBtInfoLabel;
class QTreeWidgetItem;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtSyncDirsDialog : public QDialog
{
   Q_OBJECT

//******* TYPES *******
private:
   typedef QHash< QString, QFileInfo > FileMap;
   typedef QHash< QString, FileMap   > DirMap;
   typedef QMap < QString, QString   > AllItems;
   class RowData {
      public:
         RowData()
         : name1_     ( QString() )
         , name2_     ( QString() )
         , path1_     ( QString() )
         , path2_     ( QString() )
         , separator_ ( QString() )
         , is_lft_    ( false )
         , is_rgt_    ( false )
         , is_dir_    ( false )
         {}
         ~RowData() {}
      private:
         RowData( const RowData& );
         RowData& operator=( const RowData& );
         
      //******* MEMBERS *******
      private:
         QString name1_;
         QString name2_;
         QString path1_;
         QString path2_;
         QString separator_;
         bool    is_lft_;
         bool    is_rgt_;
         bool    is_dir_;
      public:
         const QString& name1    () const { return name1_;     }
         const QString& name2    () const { return name2_;     }
         const QString& path1    () const { return path1_;     }
         const QString& path2    () const { return path2_;     }
         const QString& separator() const { return separator_; }
         bool           is_lft   () const { return is_lft_;    }
         bool           is_rgt   () const { return is_rgt_;    }
         bool           is_dir   () const { return is_dir_;    }
         bool           are_both () const { return ( is_lft_ && is_rgt_ ); }

         void name1    ( const QString& in_text ) { name1_     = in_text; }
         void name2    ( const QString& in_text ) { name2_     = in_text; }
         void path1    ( const QString& in_text ) { path1_     = in_text; }
         void path2    ( const QString& in_text ) { path2_     = in_text; }
         void separator( const QString& in_text ) { separator_ = in_text; }
         void is_lft   ( const bool     in_flag ) { is_lft_    = in_flag; }
         void is_rgt   ( const bool     in_flag ) { is_rgt_    = in_flag; }
         void is_dir   ( const bool     in_flag ) { is_dir_    = in_flag; }

         void reset () {
            name1_ = name2_ = path1_ = path2_ = separator_ = QString();
            is_lft_ = is_rgt_ = is_dir_ = false;
         }
         void set_lft( const QString& in_name, const QString& in_path ) {
            is_lft_ = true;
            name1_  = in_name;
            path1_  = in_path;
         }
         void set_rgt( const QString& in_name, const QString& in_path ) {
            is_rgt_ = true;
            name2_  = in_name;
            path2_  = in_path;
         }
   };
   
//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtSyncDirsDialog( QWidget* );
private:
   QBtSyncDirsDialog( const QBtSyncDirsDialog& );
   QBtSyncDirsDialog& operator=( const QBtSyncDirsDialog& );

//******* CONSTANTS *******
private:
   static const char* const Caption;
   static const char* const DirsCaption;
   static const char* const ResultCaption;
   static const char* const Operation;
   static const char* const LftPanel;
   static const char* const RgtPanel;
   static const char* const Run;
   static const char* const Synchronize;
   static const char* const Close;
   static const char* const Break;
   static const char* const LeftDir;
   static const char* const RightDir;
   static const char* const NotPresent;
   static const char* const CantOpen;
   static const char* const CantCreate;
   static const char* const ReadError;
   static const char* const WriteError;
   static const QString     Dir;
   static const QString     Equal;
   static const QString     NotEqual;
   static const QString     Error;
   static const QString     ToLeft;
   static const QString     ToRight;

//******* MEMBERS *******
private:
   QTreeWidget*  const view_;
   QPushButton*  const run_btn_;
   QPushButton*  const close_btn_;
   QBtInfoLabel* const left_dirinfo_;
   QBtInfoLabel* const right_dirinfo_;
   QString             left_path_;
   QString             right_path_;
   bool                runned_;
   bool                continue_;
   bool                ready_;

//******** METHODS *******
public:
   void    set_dirs     ( const QString&, const QString& );
private:
   void    showEvent    ( QShowEvent* );
   void    synchronize  ();
   bool    copy         ( const QString&  , const QString&   );
   void    read_dir     ( const QString&  , const QString&, DirMap& ) const;
   void    compare      ( const QString&  , const QString&   ) const;
   void    compare_files( const FileMap&  , const FileMap&   ) const;
   void    join_dirs    ( AllItems&       , const DirMap&    ) const;
   void    join_files   ( AllItems&       , const FileMap&   ) const;
   QString check_files  ( const QFileInfo&, const QFileInfo& ) const;
   void    add_row      ( const RowData& ) const;
   void    set_focus    ();
private slots:
   void start_slot();
   void close_slot();
};

 #endif // INCLUDED_QBtSyncDirsDialog_h
