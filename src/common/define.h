#ifndef DTNG_DEFINE_H
#define DTNG_DEFINE_H

#include <QtGlobal>

/*
   TERMINALWIDGET_VERSION is (major << 16) + (minor << 8) + patch.
   can be used like #if (TERMINALWIDGET_VERSION >= QT_VERSION_CHECK(0, 7, 1))
*/
#define TERMINALWIDGET_VERSION      QT_VERSION_CHECK(TERMINALWIDGET_VERSION_MAJOR, TERMINALWIDGET_VERSION_MINOR, TERMINALWIDGET_VERSION_PATCH)

#endif // DTNG_DEFINE_H