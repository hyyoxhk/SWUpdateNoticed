#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSharedMemory>
#include <QLocale>
#include <QLibraryInfo>
#include <QTranslator>
#include <QQmlContext>
#include <QScreen>

#include "swupdate.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSharedMemory sharedMemory("swupdate-shared");
    if (!sharedMemory.create(1)) {
        if (sharedMemory.error() == QSharedMemory::AlreadyExists) {
            if (sharedMemory.attach(QSharedMemory::ReadOnly)) {
                sharedMemory.detach();
            } else if (sharedMemory.error() != QSharedMemory::AlreadyExists) {
                return 1;
            }
        } else {
            return 1;
        }
        if (!sharedMemory.create(1)) {
            return 0;
        }
    }

    QString locale = QLocale::system().name();
    QTranslator qtTranslator;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QString qtTranslationPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#else
    QString qtTranslationPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
#endif
    if(qtTranslator.load(QStringLiteral("qt_") + locale, qtTranslationPath))
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
    engine.rootContext()->setContextProperty("swidth", int(screen->geometry().width()));
    engine.rootContext()->setContextProperty("sheight", int(screen->geometry().height()));

    engine.rootContext()->setContextProperty("SWUpdateIpc", SWUpdate::self());

//    qmlRegisterSingletonInstance("org.swupdate.demo", 1, 0, "SWUpdateIpc", SWUpdate::self());
//    qmlRegisterType<SWUpdate>("org.swupdate.demo", 1, 0, "SWUpdateIpc");

    engine.load(url);

    return app.exec();
}
