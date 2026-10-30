#ifndef QBTCOPYWORKER_H
#define QBTCOPYWORKER_H

#include "QBtOverwriteAnswer.h"
#include <QObject>
#include <QFuture>
#include <QPromise>
#include <QString>
#include <QStringList>
#include <optional>
#include <vector>

struct QBtCopyProgress
{
    QString source;
    QString destination;
    qint64  done = 0;
    qint64  size = 0;
};

class QBtCopyChannel : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

signals:
    void progress( const QBtCopyProgress& );
    void ask     ( const QString& path, QBtOverwriteAnswer::ErrorType type, QBtOverwriteAnswer* answer );
};

class QBtCopyJob
{
public:
    enum class Mode { COPY, MOVE };

    static QFuture<void> start( QBtCopyChannel& channel, Mode mode, const QStringList& sources,
                                const QString& destination, std::optional<QBtOverwriteAnswer> sticky );

private:
    static constexpr qint64 BLOCK_SIZE = 1024 * 1024;
    using Operation = bool (QBtCopyJob::*)( const QString& source, const QString& destination );

    QBtCopyJob( QPromise<void>& promise, QBtCopyChannel& channel, Mode mode, std::optional<QBtOverwriteAnswer> sticky );

    void run     ( const QStringList& sources, const QString& destination );
    bool stopped () const;

    bool transfer_into( const QString& source, const QString& dest_dir );
    bool transfer     ( const QString& source, const QString& destination );
    bool copy( const QString& source, const QString& destination );
    bool move( const QString& source, const QString& destination );
    // directories
    bool copy_dir  ( const QString& source, const QString& destination );
    bool move_dir  ( const QString& source, const QString& destination );
    bool write_dir ( const QString& source, const QString& destination );
    // symbolic links
    bool copy_link ( const QString& source, const QString& destination );
    bool move_link ( const QString& source, const QString& destination );
    bool write_link( const QString& source, const QString& destination );
    // files
    bool copy_file ( const QString& source, const QString& destination );
    bool move_file ( const QString& source, const QString& destination );
    bool write_file( const QString& source, const QString& destination );
    // shared helpers
    bool rename_item  ( const QString& source, const QString& destination, bool* cross_device = nullptr );
    bool remove_source( const QString& source );
    bool resolve      ( const QString& source, QString& destination );
    void report       ( const QString& path, QBtOverwriteAnswer::ErrorType type );
    QBtOverwriteAnswer decide( const QString& path, QBtOverwriteAnswer::ErrorType type );
    static bool is_same   ( const QString& a, const QString& b );
    static bool is_inside ( const QString& path, const QString& dir );
    static bool can_update( const QString& source, const QString& destination );
    static bool renames( const QStringList& sources, const QString& destination, bool move_mode );

    QPromise<void>&     promise_;
    QBtCopyChannel&     channel_;
    const Mode          mode_;
    const Operation     transfer_;
    std::optional<QBtOverwriteAnswer> answer_;
    bool                stop_;
    std::vector<char>   block_;
};

#endif // QBTCOPYWORKER_H
