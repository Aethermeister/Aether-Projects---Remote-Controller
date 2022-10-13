#include "serverrunner.h"
#include "logger.h"

#include <windows.h>

#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QUrl>

#include <iostream>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("Remote Controller Computer Server");
    QCoreApplication::setOrganizationName("Aether Projects");
    QCoreApplication::setOrganizationDomain("Aether Projects");
    QCoreApplication::setApplicationVersion("1.0");

    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    const auto application_path = QString("\"%0\"").arg(QCoreApplication::applicationFilePath().replace("/", "\\"));
    if(settings.contains(QCoreApplication::applicationName()))
    {
        const auto stored_run_path = settings.value(QCoreApplication::applicationName()).toString();
        if(stored_run_path != application_path)
        {
            settings.setValue(QCoreApplication::applicationName(), application_path);
        }
    }
    else
    {
        settings.setValue(QCoreApplication::applicationName(), application_path);
    }

    QCommandLineParser parser;
    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
    QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);

    parser.process(a);

    Logger::Init(parser.isSet(dbgOption));

    qDebug().noquote() << QString("========================= %0 v%1 =========================").arg(QCoreApplication::applicationName()).arg(QCoreApplication::applicationVersion());

    ServerRunner::StartServer();

    return a.exec();
}


