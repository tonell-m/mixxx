#pragma once

#include <QtDebug>
#include <QString>

#include "errordialoghandler.h"

#ifdef MIXXX_DEBUG_ASSERTIONS_ENABLED
#define DBG(value) \
    (qDebug().nospace() << #value << " = " << (value) << " [" << __FILE__ << ":" << __LINE__ << "]")
#else
#define DBG(value)                                                             \
    static_assert(false,                                                       \
            "DBG() is not allowed in builds without debug assertions, please " \
            "remove it!")
#endif

template <typename T>
QString toDebugString(const T& object) {
    QString output;
#ifndef QT_NO_DEBUG_OUTPUT
    QDebug deb(&output);
    deb << object;
#else
    Q_UNUSED(object);
#endif
    return output;
}

// Calling this will report a qFatal and quit Mixxx, possibly disgracefully. Use
// very sparingly! A modal message box will be issued to the user which allows
// the Qt event loop to continue processing. This means that you must not call
// this function from a code section which is not re-entrant (e.g. paintEvent on
// a QWidget).
inline void reportFatalErrorAndQuit(const QString& message) {
    QByteArray message_bytes = message.toLocal8Bit();
    qFatal("%s", message_bytes.constData());
    ErrorDialogHandler* dialogHandler = ErrorDialogHandler::instance();
    if (dialogHandler) {
        dialogHandler->requestErrorDialog(DLG_FATAL, message, true);
    }
}

// Calling this will report a qCritical and quit Mixxx, possibly
// disgracefully. Use very sparingly! A modal message box will be issued to the
// user which allows the Qt event loop to continue processing. This means that
// you must not call this function from a code section which is not re-entrant
// (e.g. paintEvent on a QWidget).
inline void reportCriticalErrorAndQuit(const QString& message) {
    qCritical() << message;
    ErrorDialogHandler* dialogHandler = ErrorDialogHandler::instance();
    if (dialogHandler) {
        dialogHandler->requestErrorDialog(DLG_CRITICAL, message, true);
    }
}
