#include "remotefunctionslistwidget.h"
#include "ui_remotefunctionslistwidget.h"
#include "mainwindow.h"

RemoteFunctionsListWidget::RemoteFunctionsListWidget(QWebSocket *active_socket, QWidget *parent) :
    RemoteFunctionBaseWidget(active_socket, parent),
    ui(new Ui::RemoteFunctionsListWidget)
{
    ui->setupUi(this);

    InitializeUi();
    connect(ui->m_disconnect_btn, &QPushButton::clicked, this, [=]() {
        m_active_socket->close();
    });
}

RemoteFunctionsListWidget::~RemoteFunctionsListWidget()
{
    delete ui;
}

void RemoteFunctionsListWidget::InitializeUi()
{
    ui->m_functions_list_widget->setStyleSheet("QListView::item { height: 100px; }");

    auto* system_functions_item = new QListWidgetItem(ui->m_functions_list_widget);
    ui->m_functions_list_widget->addItem(system_functions_item);
    auto* system_functions_button = new QPushButton("System");
    ui->m_functions_list_widget->setItemWidget(system_functions_item, system_functions_button);
    connect(system_functions_button, &QPushButton::clicked, this, [=]() {
        MainWindow::Instance()->ShowSystemFunctionsList();
    });

    auto* file_list_item = new QListWidgetItem(ui->m_functions_list_widget);
    ui->m_functions_list_widget->addItem(file_list_item);
    auto* file_list_button = new QPushButton("Files");
    ui->m_functions_list_widget->setItemWidget(file_list_item, file_list_button);
    connect(file_list_button, &QPushButton::clicked, this, [=]() {
        MainWindow::Instance()->ShowFilesList();
    });
}
