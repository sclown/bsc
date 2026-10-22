#ifndef QBTMACTOOLS_H
#define QBTMACTOOLS_H

#include <QtGlobal>
#include <QUrl>

#if defined(Q_OS_MAC)
// Implemented in QBTMacTools.mm (compiled only on macOS).
bool isMacSpecialURL( const QUrl& url );
QUrl resolveMacSpecialURL( const QUrl& url );
// Brings an already running instance to the front; false when none is running.
bool activateAnotherInstance();
#else
inline bool isMacSpecialURL( const QUrl& ) { return false; }
inline QUrl resolveMacSpecialURL( const QUrl& url ) { return url; }
inline bool activateAnotherInstance() { return false; }
#endif

#endif // QBTMACTOOLS_H
