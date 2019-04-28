#include "QBtPathBox.h"
#include "3rdprty/dircompleter.h"
#include "QBTMacTools.h"
#include "QBtShared.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDir>
#include <QLineEdit>

QBtPathBox::QBtPathBox()
{
    setAcceptDrops(true);
    setEditable(true);
    DirCompleter *completer = new DirCompleter(this);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    setCompleter(completer);
    lineEdit()->setAcceptDrops(false);
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
    emit activated(folderInfo.absoluteFilePath());
    if( pathInfo.isFile() ) {
        emit selectFile(pathInfo.fileName());
    }
}
