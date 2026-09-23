#include "QBtPathBox.h"
#include "3rdprty/dircompleter.h"
#include "QBTMacTools.h"
#include "QBtShared.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDir>
#include <QLineEdit>
#include <QAbstractItemView>
#include <QAbstractProxyModel>
#include <QFileSystemModel>
#include <QKeyEvent>

QBtPathBox::QBtPathBox()
{
    setAcceptDrops(true);
    setEditable(true);
    // The file system model loads directories asynchronously, so inline
    // completion finds nothing on first keystroke; a popup keeps updating
    // as entries arrive.
    DirCompleter *completer = new DirCompleter(this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setMaxVisibleItems(12);
    setCompleter(completer);
    lineEdit()->setAcceptDrops(false);
    lineEdit()->installEventFilter(this);
    completer->popup()->installEventFilter(this);

    // QCompleter only re-runs itself after an async load in some cases;
    // refresh the suggestions whenever the directory being typed arrives.
    if (auto *fsModel = qobject_cast<QFileSystemModel*>(completer->model())) {
        connect(fsModel, &QFileSystemModel::directoryLoaded, this, [this] {
            if (lineEdit()->hasFocus() && !lineEdit()->text().isEmpty()) {
                QComboBox::completer()->setCompletionPrefix(lineEdit()->text());
                QComboBox::completer()->complete();
            }
        });
    }
}

bool QBtPathBox::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress &&
        static_cast<QKeyEvent*>(event)->key() == Qt::Key_Tab &&
        (watched == lineEdit() || watched == completer()->popup())) {
        completeWithTab();
        return true;
    }
    return QComboBox::eventFilter(watched, event);
}

// Tab accepts the highlighted (or first) suggestion, appends a slash and
// immediately offers the entries of that directory.
bool QBtPathBox::completeWithTab()
{
    QCompleter* const c = completer();
    QModelIndex index = c->popup()->isVisible() ? c->popup()->currentIndex() : QModelIndex();
    if (!index.isValid()) {
        c->setCompletionPrefix(lineEdit()->text());
        if (c->completionCount() == 0) return false;
        c->setCurrentRow(0);
        index = c->currentIndex();
    }

    // Both the popup and currentIndex() work on the completion proxy model,
    // while pathFromIndex() expects an index of the source model.
    const auto* const proxy = qobject_cast<const QAbstractProxyModel*>(c->completionModel());
    QString text = c->pathFromIndex(proxy ? proxy->mapToSource(index) : index);
    if (!text.endsWith('/')) text += '/';
    lineEdit()->setText(text);

    c->setCompletionPrefix(text);
    c->complete();
    return true;
}

void QBtPathBox::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void QBtPathBox::dropEvent(QDropEvent *event)
{
    auto files = QBtShared::urlsToPathes(event->mimeData()->urls());
    if( !files.empty() ) {
        handleDrop(files.front());
    }
    event->acceptProposedAction();
}

void QBtPathBox::setPath( const QString& in_path )
{
   bool oldState = blockSignals(true);

   const qint32 idx = findText( in_path );
   if( idx != -1 ) {
      removeItem( idx );
   }
   insertItem( 0, in_path );
   setCurrentIndex( 0 );
   blockSignals(oldState);

   // Warm up the completion model so suggestions for the current folder
   // (and its parent) are ready as soon as the user starts typing.
   if (auto *fsModel = qobject_cast<QFileSystemModel*>(completer()->model())) {
      for (const QString &dir : { in_path, QFileInfo(in_path).absolutePath() }) {
         const QModelIndex idx = fsModel->index(dir);
         if (fsModel->canFetchMore(idx)) fsModel->fetchMore(idx);
      }
   }
}

void QBtPathBox::reset()
{
    bool oldState = blockSignals(true);
    setCurrentIndex( 0 );
    blockSignals(oldState);

}

void QBtPathBox::handleDrop(const QString &path)
{
    if( path.isEmpty() ) {
        return;
    }
    QFileInfo pathInfo(path);
    QFileInfo folderInfo = QBtShared::pathFromFSItem(pathInfo);
    if(!folderInfo.isReadable()) {
        return;
    }
    setPath(folderInfo.absoluteFilePath());
    emit textActivated(folderInfo.absoluteFilePath());
    if( pathInfo.isFile() ) {
        emit selectFile(pathInfo.fileName());
    }
}
