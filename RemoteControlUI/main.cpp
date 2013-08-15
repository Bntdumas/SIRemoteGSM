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
#include "util.h"

#ifdef DBUSPROVIDER
# include "dbusprovider.h"
#endif
#ifdef DUMMYPROVIDER
# include "dummyprovider.h"
#endif
#ifdef BBSMSPROVIDER
# include "bbsmsprovider.h"
#endif
#include "basicfilemapper.h"

int main(int argc, char *argv[])
{
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

    BasicFileMapper* mapper = new BasicFileMapper();

    Util::instance->setParent(model);

#ifdef DBUSPROVIDER
    registry->registerProvider(new DBusProvider());
#endif
#ifdef DUMMYPROVIDER
    registry->registerProvider(new DummyProvider());
#endif

#ifdef BBSMSPROVIDER
    registry->registerMapper(mapper, new BBSMSProvider());
#endif

#if QT_VERSION >= 0x050000
    QtQuick2ApplicationViewer viewer;
    viewer.setResizeMode(QtQuick2ApplicationViewer::SizeRootObjectToView);
#else
    QmlApplicationViewer viewer;
    viewer.setResizeMode(QmlApplicationViewer::SizeRootObjectToView);
#endif
    viewer.rootContext()->setContextProperty("incommingRunnersModel", proxyModel);
    viewer.rootContext()->setContextProperty("runnersModel", QVariant::fromValue(qobject_cast<QObject*>(mapper->model())));
    viewer.rootContext()->setContextProperty("mapper", mapper);
    viewer.rootContext()->setContextProperty("Util", Util::instance);
    viewer.setMainQmlFile(QStringLiteral("qml/RemoteControlUI/main.qml"));
    viewer.showMaximized();

    return app.exec();
}
