#ifndef QBTCOPYWORKER_H
#define QBTCOPYWORKER_H

#include "QBtOverwriteAnswer.h"
#include <QObject>
#include <QFuture>
#include <QPromise>
#include <QString>
#include <QStringList>
#include <functional>
#include <optional>
#include <vector>

/*------- update sent to the dialog:
-------------------------------------------------------------------
 * What the worker is doing right now: the item being copied and how
 * far into it the copy is. Links and empty files report size 0.
-------------------------------------------------------------------*/
struct QBtCopyProgress
{
    QString source;
    QString destination;
    qint64  done = 0;
    qint64  size = 0;
};

/*------- class declaration:
-------------------------------------------------------------------
 * Copies files and directories on a thread from the global pool.
 *
 * Whenever the user has to decide something (an existing target, an
 * unreadable source, an unwritable target) the worker calls the ask
 * callback from the pool thread and blocks until it returns. The
 * caller is responsible for moving that question onto the GUI thread.
 * Progress is reported through progress(), one update per item and
 * per block copied; cancellation comes through the future.
-------------------------------------------------------------------*/
class QBtCopyWorker : public QObject
{
    Q_OBJECT

public:
    using Ask = std::function<QBtOverwriteAnswer(const QString&, QBtOverwriteAnswer::ErrorType)>;

    explicit QBtCopyWorker( QObject* parent = nullptr );

    // Starts one copy run. A sticky answer, e.g. AUTORENAME for drops,
    // decides every conflict up front; without one the user is asked.
    QFuture<void> start( const QStringList& sources, const QString& destination,
                         std::optional<QBtOverwriteAnswer> sticky, Ask ask );

signals:
    void progress( const QBtCopyProgress& );

private:
    // Read/write chunk. Throughput is flat from ~64 KiB up to a few MiB;
    // larger chunks only delay Break and progress updates.
    static constexpr qint64 BLOCK_SIZE = 1024 * 1024;

    void run        ( QPromise<void>& promise, const QStringList& sources, const QString& destination );
    bool stopped    () const;
    void copy_into  ( const QString& source, const QString& dest_dir );
    void copy_item  ( const QString& source, const QString& destination );
    void copy_dir   ( const QString& source, const QString& destination );
    void copy_link  ( const QString& source, const QString& destination );
    void copy_file  ( const QString& source, const QString& destination );
    bool resolve    ( const QString& source, QString& destination );
    void report     ( const QString& path, QBtOverwriteAnswer::ErrorType type );
    QBtOverwriteAnswer decide( const QString& path, QBtOverwriteAnswer::ErrorType type );
    static bool is_inside( const QString& path, const QString& dir );
    static bool can_update( const QString& source, const QString& destination );

    QPromise<void>*     promise_;
    Ask                 ask_;
    std::optional<QBtOverwriteAnswer> answer_;
    bool                stop_;
    std::vector<char>   block_;
};

#endif // QBTCOPYWORKER_H
