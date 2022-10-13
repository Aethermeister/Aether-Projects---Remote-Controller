#include "connectionlistwidget.h"
#include "ui_connectionlistwidget.h"

#include "addnewconnectiondialog.h"
#include "connectionlistwidgetbutton.h"
#include "mainwindow.h"

#include <QMessageBox>

ConnectionListWidget::ConnectionListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionListWidget)
{
    ui->setupUi(this);

    InitializeConnections();

    ui->m_connections_list_widget->setStyleSheet("QListView::item { height: 100px; }");
}

ConnectionListWidget::~ConnectionListWidget()
{
    delete ui;
}

void ConnectionListWidget::InitializeConnections() const
{
    connect(ui->m_add_new_btn, &QPushButton::clicked, this, &ConnectionListWidget::ShowAddNewConnectionDialog);
}

void ConnectionListWidget::ShowAddNewConnectionDialog()
{
    auto* add_new_connection_dialog = new AddNewConnectionDialog(this);
    const auto dialog_result = add_new_connection_dialog->exec();

    if(dialog_result == QDialog::DialogCode::Accepted)
    {
        auto* item = new QListWidgetItem(ui->m_connections_list_widget);
        ui->m_connections_list_widget->insertItem(0, item);

        auto* connection_list_button = new ConnectionListWidgetButton(add_new_connection_dialog->DisplayName(), add_new_connection_dialog->IPAddress(), add_new_connection_dialog->Port());
        ui->m_connections_list_widget->setItemWidget(item, connection_list_button);

        connect(connection_list_button, &QPushButton::clicked, this, &ConnectionListWidget::ConnectToComputer);
    }

    add_new_connection_dialog->deleteLater();
}

void ConnectionListWidget::ConnectToComputer()
{
    auto* sender_connection_button = dynamic_cast<ConnectionListWidgetButton*>(sender());
    auto* socket = new QWebSocket();

    connect(socket, &QWebSocket::connected, this, [=]() { MainWindow::Instance()->ShowRemoteFunctionsList(socket); });
    connect(socket, &QWebSocket::disconnected, this, [=]() { QMessageBox::information(this, "Connection error", "Connection can't be established"); });

    socket->open(QUrl(QString("ws://%0:%1").arg(sender_connection_button->IPAddress(),sender_connection_button->Port())));
}
