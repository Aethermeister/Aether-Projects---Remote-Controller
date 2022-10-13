#include "connectionlistwidgetbutton.h"
#include "ui_connectionlistwidgetbutton.h"

ConnectionListWidgetButton::ConnectionListWidgetButton(const QString &display_name, const QString &ip_address, const QString &port, QWidget *parent) :
    QPushButton(parent),
    ui(new Ui::ConnectionListWidgetButton),
    m_display_name(display_name),
    m_ip_address(ip_address),
    m_port(port)
{
    ui->setupUi(this);

    InitializeUi();

//    m_socket = new QWebSocket();
//    connect(m_socket, &QWebSocket::connected, [=]() { qDebug() << "Connected"; });
//    m_socket->open(QUrl(QString("ws://%0:%1").arg(m_ip_address).arg(m_port)));

//    qDebug() << m_socket->isValid();
}

ConnectionListWidgetButton::~ConnectionListWidgetButton()
{
    delete ui;
}

const QString &ConnectionListWidgetButton::IPAddress() const
{
    return m_ip_address;
}

const QString &ConnectionListWidgetButton::Port() const
{
    return m_port;
}

void ConnectionListWidgetButton::InitializeUi()
{
    ui->m_display_name_lbl->setText(m_display_name);
}
