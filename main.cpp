#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "graphicwidgetdeclarative.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QmlApplicationViewer viewer;
    qmlRegisterType<GraphicWidgetDeclarative>("BTCTrader", 1, 0, "GraphicWidgetDeclarative");

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/GoxMobile/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
