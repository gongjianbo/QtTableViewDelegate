#include <QApplication>
#include <QStyleFactory>
//#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //qDebug()<<QStyleFactory::keys();
    QApplication::setStyle(QStyleFactory::create("windowsvista"));
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
