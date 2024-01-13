#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <mediaplayer.h>
#include <QDebug>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaMetaData>
#include <SystemProvider.h>
#include <weathermodel.h>


//import ui

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

   // QMediaMetaData mediaMetaData;
    //QMediaPlayer player;
    //QAudioOutput audio;
    mediaplayer player;
    WeatherModel weatherModel;
    SystemProvider systemProvider;
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/infosystemk/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);


    //qDebug() <<  systemProvider.time();
  //  engine.addImageProvider(QLatin1String("imgProvider"), new systemImgProvider);
   systemProvider.setDriverName("Driver");
   engine.rootContext()->setContextProperty("system", &systemProvider);
   engine.rootContext()->setContextProperty("player", &player);
   engine.rootContext()->setContextProperty("WeatherModel", &weatherModel);

    return app.exec();
}
