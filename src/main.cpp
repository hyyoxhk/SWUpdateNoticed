#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLocale>
#include <QLibraryInfo>
#include <QTranslator>
#include <QQmlContext>
#include <QScreen>

#include "swupdate.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QString locale = QLocale::system().name();
    QTranslator qtTranslator;
    if(qtTranslator.load(QStringLiteral("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        qApp->installTranslator(&qtTranslator);

    QTranslator appTranslator;
    if(appTranslator.load(QStringLiteral("SWUpdateNoticed_") + locale, QStringLiteral(SWUPDATENOTICED_DATA_DIR) + QStringLiteral("/translations")))
        qApp->installTranslator(&appTranslator);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    QScreen *screen = app.primaryScreen();
    if (screen->geometry().width() < screen->geometry().height()){
           // Portrait orientation
           engine.rootContext()->setContextProperty("swidth", int(screen->geometry().width()));
           engine.rootContext()->setContextProperty("sheight", int(screen->geometry().width()/1.77)); //1.77 is 16:9 aspect ratio
       } else {
           // Landscape orientation
           if (screen->geometry().width() < 1280 || screen->geometry().height() < 720){
               engine.rootContext()->setContextProperty("swidth", screen->geometry().width() - 6); //Discount 6 pixels for xwayland borders
               engine.rootContext()->setContextProperty("sheight", screen->geometry().height() - 65); //Discount 65 pixels for xwayland header and borders
           }else{
               engine.rootContext()->setContextProperty("swidth", int(screen->geometry().width()/1.5));
               engine.rootContext()->setContextProperty("sheight", int(screen->geometry().height()/1.5));
           }
       }

    engine.rootContext()->setContextProperty("SWUpdateIpc", SWUpdate::self());

//    qmlRegisterSingletonInstance("org.swupdate.demo", 1, 0, "SWUpdateIpc", SWUpdate::self());
//    qmlRegisterType<SWUpdate>("org.swupdate.demo", 1, 0, "SWUpdateIpc");

    engine.load(url);

    return app.exec();
}
