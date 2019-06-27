#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_EnableHighDpiScaling);
    a.setStyle("fusion");
    MainWindow w;
    w.show();

    return a.exec();
}
