#ifndef QBTPATHBOX_H
#define QBTPATHBOX_H

#include <QComboBox>

class QBtPathBox : public QComboBox
{
    Q_OBJECT

public:
    QBtPathBox();

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void setPath(const QString &in_path);
    void reset();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
   void selectFile (const QString&);

private:
    void handleDrop(const QString& path);
    bool completeWithTab();

};

#endif // QBTPATHBOX_H
