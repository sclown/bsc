#ifndef QBTPATHBOX_H
#define QBTPATHBOX_H

#include <QComboBox>

class QBtPathBox : public QComboBox
{
public:
    QBtPathBox();

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    void setPath(const QString &in_path);
    void reset();
};

#endif // QBTPATHBOX_H
