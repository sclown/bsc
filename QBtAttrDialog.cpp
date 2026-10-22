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
 * File         : QBtAttrDialog.cpp
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 06.09.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtAttrDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QMessageBox>
#include <QtDebug>

/*------- constants:
-------------------------------------------------------------------*/
const char* const QBtAttrDialog::CAPTION   = QT_TR_NOOP( "Access permissions"  );
const char* const QBtAttrDialog::FILE_NAME = QT_TR_NOOP( "File name: "  );
const char* const QBtAttrDialog::USER      = QT_TR_NOOP( "User"  );
const char* const QBtAttrDialog::GROUP     = QT_TR_NOOP( "Group"  );
const char* const QBtAttrDialog::OTHER     = QT_TR_NOOP( "Other"  );
const char* const QBtAttrDialog::RECURSIVE = QT_TR_NOOP( "&Recursive"  );
const char* const QBtAttrDialog::ACCEPT    = QT_TR_NOOP( "&Accept"  );
const char* const QBtAttrDialog::CLOSE     = QT_TR_NOOP( "&Close"  );
const char* const QBtAttrDialog::READ      = QT_TR_NOOP( "read"  );
const char* const QBtAttrDialog::WRITE     = QT_TR_NOOP( "write"  );
const char* const QBtAttrDialog::EXECUTE   = QT_TR_NOOP( "execute"  );
const char* const QBtAttrDialog::CANT_CHANGE = QT_TR_NOOP( "Permissions of %n item(s) could not be changed, e.g.:\n%1" );


//*******************************************************************
// QBtAttDialog                                          CONSTRUCTOR
//*******************************************************************
QBtAttrDialog::QBtAttrDialog( QWidget* in_parent, const SelectionsSet& in_data ) : QDialog( in_parent )
, data_       ( in_data                            )
, path_       ( new QLabel     ( *data_.begin()  ) )
, user_read_  ( new QCheckBox  ( tr( READ      ) ) )
, user_write_ ( new QCheckBox  ( tr( WRITE     ) ) )
, user_exec_  ( new QCheckBox  ( tr( EXECUTE   ) ) )
, group_read_ ( new QCheckBox  ( tr( READ      ) ) )
, group_write_( new QCheckBox  ( tr( WRITE     ) ) )
, group_exec_ ( new QCheckBox  ( tr( EXECUTE   ) ) )
, other_read_ ( new QCheckBox  ( tr( READ      ) ) )
, other_write_( new QCheckBox  ( tr( WRITE     ) ) )
, other_exec_ ( new QCheckBox  ( tr( EXECUTE   ) ) )
, recursive_  ( new QCheckBox  ( tr( RECURSIVE ) ) )
, close_      ( new QPushButton( tr( CLOSE     ) ) )
, accept_     ( new QPushButton( tr( ACCEPT    ) ) )
{
   setWindowTitle( tr( CAPTION ) );

   QHBoxLayout* const info_layout = new QHBoxLayout;
   info_layout->addWidget( new QLabel( tr( FILE_NAME ) ) );
   info_layout->addWidget( path_ );
   info_layout->setStretchFactor( path_, 100 );
   
   QGroupBox* const gbox_user = new QGroupBox( tr( USER ) );
   QVBoxLayout* const user_layout = new QVBoxLayout( gbox_user );
   user_layout->addWidget( user_read_ );
   user_layout->addWidget( user_write_ );
   user_layout->addWidget( user_exec_ );
   
   QGroupBox* const gbox_group = new QGroupBox( tr( GROUP ) );
   QVBoxLayout* const group_layout = new QVBoxLayout( gbox_group );
   group_layout->addWidget( group_read_ );
   group_layout->addWidget( group_write_ );
   group_layout->addWidget( group_exec_ );
   
   QGroupBox* const gbox_other = new QGroupBox( tr( OTHER ) );
   QVBoxLayout* const other_layout = new QVBoxLayout( gbox_other );
   other_layout->addWidget( other_read_ );
   other_layout->addWidget( other_write_ );
   other_layout->addWidget( other_exec_ );

   QHBoxLayout* const gbox_layout = new QHBoxLayout;
   gbox_layout->addWidget( gbox_user );
   gbox_layout->addWidget( gbox_group );
   gbox_layout->addWidget( gbox_other );

   QHBoxLayout* const btn_layout = new QHBoxLayout;
   btn_layout->addWidget( recursive_ );
   btn_layout->addStretch( 100 );
   btn_layout->addWidget( close_ );
   btn_layout->addWidget( accept_ );
   
   QVBoxLayout* const main_layout = new QVBoxLayout;
   main_layout->addLayout( info_layout );
   main_layout->addLayout( gbox_layout );
   main_layout->addLayout( btn_layout );
   setLayout( main_layout );

   set_info();
   
   connect( close_ , SIGNAL( clicked() ), this, SLOT( reject() ) );
   connect( accept_, SIGNAL( clicked() ), this, SLOT( accept() ) );
}
// end of QBtAttDialog

//*******************************************************************
// set_info                                                  PRIVATE
//*******************************************************************
void QBtAttrDialog::set_info()
{
   if( !are_dirs() ) recursive_->setEnabled( false );

   const QFile::Permissions p = QFileInfo( *data_.begin() ).permissions();
   
   if( p & QFile::ReadUser   ) user_read_  ->setChecked( true );
   if( p & QFile::WriteUser  ) user_write_ ->setChecked( true );
   if( p & QFile::ExeUser    ) user_exec_  ->setChecked( true );
   if( p & QFile::ReadGroup  ) group_read_ ->setChecked( true );
   if( p & QFile::WriteGroup ) group_write_->setChecked( true );
   if( p & QFile::ExeGroup   ) group_exec_ ->setChecked( true );
   if( p & QFile::ReadOther  ) other_read_ ->setChecked( true );
   if( p & QFile::WriteOther ) other_write_->setChecked( true );
   if( p & QFile::ExeOther   ) other_exec_ ->setChecked( true );
}
// end of set_info

//*******************************************************************
// get_info                                                  PRIVATE
//*******************************************************************
QFileDevice::Permissions QBtAttrDialog::get_permissions() const
{
   QFileDevice::Permissions p;
   if( user_read_  ->isChecked() ) p |= QFileDevice::ReadOwner  | QFileDevice::ReadUser;
   if( user_write_ ->isChecked() ) p |= QFileDevice::WriteOwner | QFileDevice::WriteUser;
   if( user_exec_  ->isChecked() ) p |= QFileDevice::ExeOwner   | QFileDevice::ExeUser;
   if( group_read_ ->isChecked() ) p |= QFileDevice::ReadGroup;
   if( group_write_->isChecked() ) p |= QFileDevice::WriteGroup;
   if( group_exec_ ->isChecked() ) p |= QFileDevice::ExeGroup;
   if( other_read_ ->isChecked() ) p |= QFileDevice::ReadOther;
   if( other_write_->isChecked() ) p |= QFileDevice::WriteOther;
   if( other_exec_ ->isChecked() ) p |= QFileDevice::ExeOther;
   return p;
}
// end of get_permissions

//*******************************************************************
// are_dirs                                                  PRIVATE
//*******************************************************************
bool QBtAttrDialog::are_dirs() const
{
   SelectionsSet::const_iterator it = data_.begin();
   const SelectionsSet::const_iterator end = data_.end();
   while( it != end ) {
      if( QFileInfo( *it ).isDir() ) {
         return true;
      }
      ++it;
   }
   return false;
}
// end of are_dirs

//*******************************************************************
// accept                                               PRIVATE slot
//*******************************************************************
void QBtAttrDialog::accept()
{
   const QFileDevice::Permissions permissions = get_permissions();
   const bool recursive = recursive_->isChecked();
   QStringList failed;

   for( const QString& path : data_ ) {
      path_->setText( path );
      if( !QFile::setPermissions( path, permissions ) ) failed << path;
      if( recursive && QFileInfo( path ).isDir() ) {
         QDirIterator it( path,
                          QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot | QDir::NoSymLinks,
                          QDirIterator::Subdirectories );
         while( it.hasNext() ) {
            const QString sub = it.next();
            if( !QFile::setPermissions( sub, permissions ) ) failed << sub;
         }
      }
   }

   if( !failed.isEmpty() ) {
      QMessageBox::warning( this, tr( CAPTION ), tr( CANT_CHANGE, "", failed.size() ).arg( failed.first() ) );
   }
   QDialog::accept();
}
// end of accept
