#include "connectionlistwidget.h"
#include "ui_connectionlistwidget.h"

#include "mainwindow.h"
#include "listwidgetitembutton.h"

#include <QMessageBox>

ConnectionListWidget::ConnectionListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionListWidget)
{
    ui->setupUi(this);

    InitializeUi();
}

ConnectionListWidget::~ConnectionListWidget()
{
    delete ui;
}

void ConnectionListWidget::InitializeUi()
{
    ui->m_connections_list_widget->setStyleSheet("QListView::item { height: 100px; }");

    ListDevices();
}

void ConnectionListWidget::ListDevices()
{
    ui->m_connections_list_widget->clear();

    CreateAddButton();

    const auto connected_devices = ConnectedDevices();

    for(const auto& connected_device : connected_devices)
    {
        auto* connection_item = new QListWidgetItem(ui->m_connections_list_widget);
        ui->m_connections_list_widget->insertItem(0, connection_item);

        auto* connection_button = new ListWidgetItemButton(connected_device.DisplayName, ui->m_connections_list_widget);
        connection_button->setProperty("ip_address", connected_device.IPAddress);
        connection_button->setProperty("port", connected_device.Port);
        connection_button->setProperty("display_name", connected_device.DisplayName);

        connect(connection_button, &QPushButton::clicked, this, &ConnectionListWidget::ConnectToComputer);

        connect(connection_button->MoreButton(), &QPushButton::clicked, this, [=] {
            auto* more_menu = new QMenu(connection_button->MoreButton());

            auto* remove_action = new QAction("Remove");
            connect(remove_action, &QAction::triggered, this, [=] {
                RemoveDevice(connected_device);
                ListDevices();
            });

            more_menu->addAction(remove_action);
            more_menu->exec(connection_button->mapToGlobal(connection_button->MoreButton()->pos()));
        });

        ui->m_connections_list_widget->setItemWidget(connection_item, connection_button);
    }
}

void ConnectionListWidget::CreateAddButton()
{
    auto* add_connection_item = new QListWidgetItem(ui->m_connections_list_widget);
    ui->m_connections_list_widget->insertItem(0, add_connection_item);

    auto* add_connection_button = new ListWidgetItemButton("Add New", ui->m_connections_list_widget);
    auto button_font = add_connection_button->font();
    button_font.setBold(true);
    add_connection_button->setFont(button_font);
    add_connection_button->SetMoreButtonVisibility(false);
    add_connection_button->setIcon(QIcon("://Images/baseline_add_circle_outline_white_24dp.png"));
    add_connection_button->setIconSize({ 30, 30 });
    connect(add_connection_button, &QPushButton::clicked, this, [=] { MainWindow::Instance()->ShowAddConnectionWidget(); });

    ui->m_connections_list_widget->setItemWidget(add_connection_item, add_connection_button);
}

void ConnectionListWidget::ConnectToComputer()
{
    auto* sender_connection_button = dynamic_cast<ListWidgetItemButton*>(sender());
    auto* socket = new QWebSocket();

    connect(socket, &QWebSocket::connected, this, [=] { MainWindow::Instance()->ShowRemoteFunctionsList(socket); });
    connect(socket, &QWebSocket::disconnected, this, [=] { QMessageBox::information(this, "Connection error", "Connection can't be established"); });

    const auto ip_address = sender_connection_button->property("ip_address").toString();
    const auto port = sender_connection_button->property("port").toString();
    socket->open(QUrl(QString("ws://%0:%1").arg(ip_address, port)));
}
