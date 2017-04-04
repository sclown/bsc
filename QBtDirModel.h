#ifndef INCLUDED_QBtDirModel_h
#define INCLUDED_QBtDirModel_h

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
 * File         : QBtDirModel.h
 * Author       : Piotr Pszczolkowski
 * Contact      : piotr@beesoft.org
 * Creation date: 27.08.2007
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include "QBtViewModel.h"
#include <QFileSystemWatcher>
#include <QThread>
#include <memory>

typedef std::shared_ptr<QIcon> QIconPtr;

/*------- forward declarations:
-------------------------------------------------------------------*/
class QBtDirListWorker;
class QBtFileInfo;

/*------- class declaration:
-------------------------------------------------------------------*/
class QBtDirModel : public QBtViewModel
{
   Q_OBJECT

//******* CONSTRUCTION / DESTRUCTION *******
public:
   QBtDirModel( QObject* = 0 );
   ~QBtDirModel();
private:
   QBtDirModel( const QBtDirModel& );
   QBtDirModel& operator=( const QBtDirModel& );

   QVariant dropInfo_;
   QFileSystemWatcher watcher_;
   QThread thread_;
   QBtDirListWorker *worker_;

//******* METHODS *******
public:
   void setup              (const QString& , int, Qt::SortOrder);
   void update              (const QString&);
   void update              ( const QModelIndex& );
   void refresh             ();
   bool is_dir              ( const QModelIndex& );
   bool is_executable       ( const QModelIndex& );
   bool is_readable         ( const QModelIndex& );
   void cdup                ();
   bool removeRows          ( int row, int count, const QModelIndex &parent );
   bool setItemData         ( const QModelIndex &index, const QMap<int, QVariant> &roles );
   virtual void sort        (int column, Qt::SortOrder order);
private:
   void append_row          ( qint32, const QBtFileInfo&, const QStringList& );
   QStringList mimeTypes() const;
   Qt::ItemFlags flags(const QModelIndex &index) const;
   Qt::DropActions supportedDropActions() const;
   QMimeData* mimeData(const QModelIndexList &indexes) const;
   bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
private slots:
   void work_started_slot   ();
   void items_count_slot    ( qint32 );
   void item_info_slot      ( qint32, QVariant, QStringList );
   void item_icon_slot      ( qint32, QIconPtr );
   void work_finished_slot  ( QString );
   void directory_changed_slot ( const QString& );
   void delayed_refresh     ();
   void notify_drop         ();

signals:
   void list( const QString &path, int column , quint8 order );
   void icon( qint32 row, const QString &path );

};

#endif // INCLUDED_QBtDirModel_h
