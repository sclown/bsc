#ifndef QBTPATHBOX_H
#define QBTPATHBOX_H

#include <QComboBox>

class QBtPathBox : public QComboBox
{
    Q_OBJECT

public:
    QBtPathBox();

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    void setPath(const QString &in_path);
    void reset();

signals:
   void selectFile (const QString&);

private:
    void handleDrop(const QString& path);

};

#endif // QBTPATHBOX_H
