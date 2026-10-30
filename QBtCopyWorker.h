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

struct QBtCopyProgress
{
    QString source;
    QString destination;
    qint64  done = 0;
    qint64  size = 0;
};

class QBtCopyWorker : public QObject
{
    Q_OBJECT

public:
    using Ask = std::function<QBtOverwriteAnswer(const QString&, QBtOverwriteAnswer::ErrorType)>;
    enum class Mode { COPY, MOVE };

    explicit QBtCopyWorker( QObject* parent = nullptr );

    QFuture<void> start( Mode mode, const QStringList& sources, const QString& destination,
                         std::optional<QBtOverwriteAnswer> sticky, Ask ask );

signals:
    void progress( const QBtCopyProgress& );

private:
    static constexpr qint64 BLOCK_SIZE = 1024 * 1024;

    void run     ( QPromise<void>& promise, const QStringList& sources, const QString& destination );
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

    using Operation = bool (QBtCopyWorker::*)( const QString& source, const QString& destination );

    QPromise<void>*     promise_;
    Mode                mode_;
    Operation           transfer_;
    Ask                 ask_;
    std::optional<QBtOverwriteAnswer> answer_;
    bool                stop_;
    std::vector<char>   block_;
};

#endif // QBTCOPYWORKER_H
