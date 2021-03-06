#ifndef DTNG_DEFINE_H
#define DTNG_DEFINE_H

#include <QtGlobal>
#include <QString>
/*
   TERMINALWIDGET_VERSION is (major << 16) + (minor << 8) + patch.
   can be used like #if (TERMINALWIDGET_VERSION >= QT_VERSION_CHECK(0, 7, 1))
*/
#define TERMINALWIDGET_VERSION      QT_VERSION_CHECK(TERMINALWIDGET_VERSION_MAJOR, TERMINALWIDGET_VERSION_MINOR, TERMINALWIDGET_VERSION_PATCH)
#define KEY_WIDGET_RUN   (Qt::Key_Enter)

const QString TERM_WIDGET_NAME = "Konsole::TerminalScreen";

#endif // DTNG_DEFINE_H
