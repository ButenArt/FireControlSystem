#include "FCS_mainwindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << "Программа запущена!";
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug() << "Окно создано, запускаем event loop!";
    return a.exec();
}
