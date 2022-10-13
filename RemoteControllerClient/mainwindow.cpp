#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectionlistwidget.h"
#include "remotefunctionslistwidget.h"
#include "systemfunctionslistwidget.h"
#include "filelistwidget.h"

#include <QMessageBox>

MainWindow* MainWindow::m_instance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_instance = this;

    ShowConnectionList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow *MainWindow::Instance()
{
    return m_instance;
}

void MainWindow::DeleteActualContent()
{
    auto* actual_content = findChild<QWidget*>();
    if(actual_content)
    {
        ui->m_main_window_layout->removeWidget(actual_content);
        actual_content->setParent(nullptr);
        actual_content->deleteLater();
    }
}

void MainWindow::ShowConnectionList()
{    
    DeleteActualContent();

    if(m_active_socket)
    {
        m_active_socket->close();
        m_active_socket->deleteLater();
    }

    auto* connection_list_widget = new ConnectionListWidget(this);
    ui->m_main_window_layout->addWidget(connection_list_widget);
}

void MainWindow::ShowRemoteFunctionsList(QWebSocket *new_socket)
{
    if(new_socket)
    {
        m_active_socket = new_socket;
        connect(m_active_socket, &QWebSocket::disconnected, this, [=] {
            QMessageBox::information(this, "Disconnected", "You are disconnected from the server");
            ShowConnectionList();
        });
    }

    DeleteActualContent();

    auto* remote_functions_list_widget = new RemoteFunctionsListWidget(m_active_socket, this);
    ui->m_main_window_layout->addWidget(remote_functions_list_widget);
}

void MainWindow::ShowSystemFunctionsList()
{
    DeleteActualContent();

    auto* system_functions_list_widget = new SystemFunctionsListWidget(m_active_socket, this);
    ui->m_main_window_layout->addWidget(system_functions_list_widget);
}

void MainWindow::ShowFilesList()
{
    DeleteActualContent();

    auto* file_list_widget = new FileListWidget(m_active_socket, this);
    ui->m_main_window_layout->addWidget(file_list_widget);
}

