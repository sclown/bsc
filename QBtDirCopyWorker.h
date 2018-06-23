#ifndef QBTDIRCOPYWORKER_H
#define QBTDIRCOPYWORKER_H

#include <QObject>
#include <QFileInfo>
#include "QBtOverwriteAnswer.h"

class QBtDirCopyWorker : public QObject
{
    Q_OBJECT

    class QBtCopyState
    {
    public:
            QFileInfoList items;
            quint64 index;
            QString dest;

            QBtCopyState() : index(0) {
            }
            QBtCopyState(QFileInfoList lst, QString dst) : items(lst), index(0), dest(dst) {
            }

    };
    typedef QList<QBtCopyState> QBtCopyList;

public:



public:
    explicit QBtDirCopyWorker(QObject *parent = nullptr);
private:
    void copy_list();
    void copy_next(const QString &in_src_path, const QString &in_dst_path);
    void copy_dir(const QString &in_src_dir, const QString &in_dst_dir);
    void copy_file(const QString &in_src_path, const QString &dst_path);
    void copy_link(const QString &in_src_path, const QString &dst_path);
    bool can_update(const QString &in_src_path, const QString &in_dst_path) const;

private:
    enum {
       BLOCK_SIZE = 8192000
    };
    bool break_;
    char block_ [ BLOCK_SIZE ];
    QBtCopyList list_;
    QBtCopyState invalidSate_;
    QBtCopyState *currentState_;
    QBtOverwriteAnswer answer_;

signals:
   void ask_overwrite (const QString&);
   void paths  (const QString&, const QString&);
   void reset_progress ( quint32 );
   void progress ( quint32 );
   void failure ();
   void finished ();

public slots:
   void copy(QStringList sources, QString dest);
   void resume(QBtOverwriteAnswer answer);

};

#endif // QBTDIRCOPYWORKER_H
