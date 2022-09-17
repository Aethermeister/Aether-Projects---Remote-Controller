#include "serverrunner.h"
#include "logger.h"

#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QUrl>

#include <QtNetwork/QNetworkInterface>

#include <iostream>

struct NetworkInterfaceData
{
    explicit NetworkInterfaceData(const QString& name, const QString& mac, const QString& ip)
        : m_name(name), m_mac(mac), m_ip(ip) {}
    explicit NetworkInterfaceData(const QString& ip)
        : m_ip(ip) {}

    QString m_name;
    QString m_mac;
    QString m_ip;

    bool operator ==(const NetworkInterfaceData& nid) const
    {
        return m_ip == nid.m_ip;
    }
};

QVector<NetworkInterfaceData> GetActiveNetworkInterfaces();
void ConfigureNetwork(const QVector<NetworkInterfaceData>& network_interfaces);
void StartServer(const NetworkInterfaceData& network_interface_data);
QString CreateServerID(const QString& mac);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("Remote Controller Computer Server");
    QCoreApplication::setOrganizationName("Aether Projects");
    QCoreApplication::setOrganizationDomain("Aether Projects");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
    QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);

    parser.process(a);

    Logger::Init(parser.isSet(dbgOption));

    qDebug().noquote() << QString("========================= %0 v%1 =========================").arg(QCoreApplication::applicationName()).arg(QCoreApplication::applicationVersion());

//    const auto active_network_interfaces = GetActiveNetworkInterfaces();
//    if(active_network_interfaces.count() == 1)
//    {
//        StartServer(active_network_interfaces.at(0));
//    }
//    else if(active_network_interfaces.count() > 1)
//    {
//        ConfigureNetwork(active_network_interfaces);
//    }
//    else
//    {
//        qDebug() << "No valid network interface was found";
//        return -1;
//    }

    ServerRunner::StartServer();

//    while (true)
//    {
//        std::cout << '>';
//        QTextStream input_stream(stdin);
//        const auto command = input_stream.readLine();

//        if(command == "start")
//        {
//            ServerRunner::StartServer();
//        }
//        else if(command == "stop")
//        {
//            ServerRunner::StopServer();
//        }
//        else if(command == "configure")
//        {

//        }
//        else if(command == "forget")
//        {

//        }
//        else if(command == "exit")
//        {
//            return 0;
//        }
//        else
//        {
//            qDebug() << "Invalid command";
//        }
//    }

    return a.exec();
}


QVector<NetworkInterfaceData> GetActiveNetworkInterfaces()
{
    QVector<NetworkInterfaceData> active_network_interfaces;

    for(const auto& network_interface : QNetworkInterface::allInterfaces())
    {
        if((network_interface.flags() & QNetworkInterface::InterfaceFlag::IsUp) && (network_interface.flags() & QNetworkInterface::InterfaceFlag::IsRunning))
        {
            for(const auto& address_entry : network_interface.addressEntries())
            {
                const auto host_address = address_entry.ip();
                if (host_address.protocol() == QAbstractSocket::IPv4Protocol && host_address != QHostAddress::LocalHost)
                {
                    active_network_interfaces << NetworkInterfaceData(network_interface.humanReadableName(), network_interface.hardwareAddress(), host_address.toString());
                }
            }
        }
    }

    QSettings settings;
    const auto stored_ip = settings.value("server_ip").toString();
    if(!stored_ip.isEmpty())
    {
        if(auto it = std::find(active_network_interfaces.begin(), active_network_interfaces.end(), NetworkInterfaceData(stored_ip)); it != active_network_interfaces.end())
        {
            return { *it };
        }
    }

    return active_network_interfaces;
}

void ConfigureNetwork(const QVector<NetworkInterfaceData>& network_interfaces)
{
    qDebug().noquote() << QString("Multiple active network interface was found. (%0)").arg(network_interfaces.count());

    int counter = 1;
    for(const auto& network_interface_data : network_interfaces)
    {
        qDebug().noquote() << QString("%0.").arg(counter);
        qDebug().noquote() << QString("   Name:   %0").arg(network_interface_data.m_name);
        qDebug().noquote() << QString("   Ip:     %0").arg(network_interface_data.m_ip);
        qDebug() << "";

        ++counter;
    }

    bool is_network_configured = false;
    while (!is_network_configured)
    {
        qDebug() << "Enter the ip address of the selected network interface";

        QTextStream stream(stdin);
        const auto selected_ip = stream.readLine();


        if(auto it = std::find(network_interfaces.begin(), network_interfaces.end(), NetworkInterfaceData(selected_ip)); it != network_interfaces.end())
        {
            QSettings settings;
            settings.setValue("server_ip", selected_ip);

            StartServer(*it);
            is_network_configured = true;
        }
        else
        {
            qDebug() << "Invalid ip address. Try again...";
        }
    }
}

void StartServer(const NetworkInterfaceData& network_interface_data)
{
    qDebug() << "========== Starting Server ==========";
    qDebug() << '\n';
    qDebug().noquote() << QString("Server ID:   %0").arg(CreateServerID(network_interface_data.m_mac));
    qDebug().noquote() << QString("Ip address:  %0").arg(network_interface_data.m_ip);
    qDebug().noquote() << QString("Port:        %0").arg(7777);
}

QString CreateServerID(const QString& mac)
{
    return mac.toUtf8().toBase64().toBase64();
}
