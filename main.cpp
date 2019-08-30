#include <QApplication>
#include <QQmlApplicationEngine>

#include <ball.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Wiederholrate der Tatatur tiefer stellen
    //Funktioniert nur bei unixoiden Betriebssystemen
    app.setKeyboardInputInterval(0);

    qmlRegisterType<Ball>("PongBall", 1, 0, "Ball");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

//Blabla
