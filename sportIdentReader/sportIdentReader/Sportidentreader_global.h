#ifndef SPORTIDENTREADER_GLOBAL_H
#define SPORTIDENTREADER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SPORTIDENTREADER_LIBRARY)
#  define SPORTIDENTREADERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SPORTIDENTREADERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SPORTIDENTREADER_GLOBAL_H
