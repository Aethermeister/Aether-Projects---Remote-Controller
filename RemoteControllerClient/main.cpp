#include "mainwindow.h"

#include <QApplication>
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Remote Controller Client");
    QCoreApplication::setOrganizationName("Aether Projects");
    QCoreApplication::setOrganizationDomain("Aether Projects");
    QCoreApplication::setApplicationVersion("1.0");

    QFile stylesheet_file("://Styles/stylesheet.qss");
    if(stylesheet_file.open(QIODevice::ReadOnly))
    {
        const auto stylesheet = stylesheet_file.readAll();
        a.setStyleSheet(stylesheet);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
