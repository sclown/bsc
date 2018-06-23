#ifndef QBTMACTOOLS_H
#define QBTMACTOOLS_H

#include <QUrl>

bool isMacSpecialURL(QUrl url);
QUrl resolveMacSpecialURL(QUrl url);
void activateAnotherInstance();
void moveToTrash(QString path);


#endif // QBTMACTOOLS_H
