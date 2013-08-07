#include <QtGlobal>
#if QT_VERSION >= 0x050000
# include <QGuiApplication>
# include <QQmlContext>
# include "qtquick2applicationviewer.h"
#else
# include <QApplication>
# include <QDeclarativeContext>
# include "qmlapplicationviewer.h"

typedef QString QStringLiteral;
#endif

#include <QSortFilterProxyModel>
#include <QDebug>
#include <stdio.h>

#ifdef Q_OS_BLACKBERRY
# include <bb/cascades/OrientationSupport>
#endif

#include "incommingrunnersmodel.h"
#include "sortfilterproxymodel.h"
#include "provider.h"
#include "registry.h"

#ifdef DBUSPROVIDER
# include "dbusprovider.h"
#endif
#ifdef DUMMYPROVIDER
# include "dummyprovider.h"
#endif
#ifdef BBSMSPROVIDER
# include "bbsmsprovider.h"
# include "basicfilemapper.h"
#endif

#ifdef CUSTOMDEBUGHANDLER
#include <stdio.h>
void customMessageHandler(QtMsgType type, const char* msg)
{
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "%s\n", msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "%s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "%s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "%s\n", msg);
        abort();
    }
}
#endif

int main(int argc, char *argv[])
{
#ifdef CUSTOMDEBUGHANDLER
    qInstallMsgHandler(customMessageHandler);
#endif
#if QT_VERSION >= 0x050000
    QGuiApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    app.setOrganizationName("KDAB");
    app.setApplicationName("RemoteControlUI");

#ifdef Q_OS_BLACKBERRY
    //bb::cascades::OrientationSupport::instance()->setSupportedDisplayOrientation(bb::cascades::SupportedDisplayOrientation::DisplayLandscape);
#endif

    IncommingRunnersModel* model = new IncommingRunnersModel();

    Registry* registry = new Registry();
    registry->setModel(model);

    SortFilterProxyModel* proxyModel = new SortFilterProxyModel();
    proxyModel->setSourceModel(model);
    proxyModel->setSortRole(IncommingRunnersModel::RealTimeRole);
    proxyModel->resort();
    QObject::connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), proxyModel, SLOT(resort()));

#ifdef DBUSPROVIDER
    registry->registerProvider(new DBusProvider());
#endif
#ifdef DUMMYPROVIDER
    registry->registerProvider(new DummyProvider());
#endif
#ifdef BBSMSPROVIDER
    registry->registerMapper(new BasicFileMapper(), new BBSMSProvider());
#endif

#if QT_VERSION >= 0x050000
    QtQuick2ApplicationViewer viewer;
    viewer.setResizeMode(QtQuick2ApplicationViewer::SizeRootObjectToView);
#else
    QmlApplicationViewer viewer;
    viewer.setResizeMode(QmlApplicationViewer::SizeRootObjectToView);
#endif
    viewer.rootContext()->setContextProperty("incommingRunnersModel", proxyModel);
    viewer.setMainQmlFile(QStringLiteral("qml/RemoteControlUI/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
