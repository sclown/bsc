#include "QBtPathBox.h"
#include "3rdprty/dircompleter.h"
#include "QBTMacTools.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDir>

QBtPathBox::QBtPathBox()
{
    setAcceptDrops(true);
    setEditable(true);
    DirCompleter *completer = new DirCompleter(this);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    setCompleter(completer);

}

void QBtPathBox::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void QBtPathBox::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    QString path;
    foreach (QUrl url, urls) {
        if(url.scheme() == "file"){
            if(isMacSpecialURL(url)) {
                url = resolveMacSpecialURL(url);
            }
            path = url.path();
            setPath(path);
            emit activated(path);
            break;
        }
    }
    event->acceptProposedAction();
}

void QBtPathBox::setPath( const QString& in_path )
{
   QDir dir( in_path );
   if( in_path.isEmpty() || !dir.exists() || !dir.isReadable() ) {
       reset();
       return;
   }
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


