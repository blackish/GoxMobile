#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "graphicwidgetdeclarative.h"
#include "btctrader.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    qmlRegisterType<BTCTrader>("btctrader", 1, 0, "BTCTrader");
    qmlRegisterType<GraphicWidgetDeclarative>("btctrader", 1, 0, "GraphicWidgetDeclarative");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/GoxMobile/main.qml"));
    viewer.showExpanded();


    return app->exec();
}
