#ifndef GSMMODULE_GLOBAL_H
#define GSMMODULE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GSMMODULE_LIBRARY)
#  define GSMMODULE_EXPORT Q_DECL_EXPORT
#else
#  define GSMMODULE_EXPORT Q_DECL_IMPORT
#endif

#endif // GSMMODULE_GLOBAL_H
