#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectionlistwidget.h"
#include "addnewconnectionwidget.h"
#include "remotefunctionslistwidget.h"
#include "systemfunctionslistwidget.h"
#include "filelistwidget.h"

#include <QMessageBox>
#include <QMenu>

MainWindow* MainWindow::m_instance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_instance = this;
    InitializeConnections();
    InitializeUi();
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
    auto* actual_content = ui->m_container_widget->findChild<QWidget*>();
    if(actual_content)
    {
        ui->m_container_widget_layout->removeWidget(actual_content);
        actual_content->setParent(nullptr);
        actual_content->deleteLater();
    }
}

void MainWindow::ShowNewWidget(QWidget* new_widget)
{
    const auto is_back_button_visible = dynamic_cast<ConnectionListWidget*>(new_widget) == nullptr;
    ui->m_back_btn->setVisible(is_back_button_visible);

    ui->m_widget_title_lbl->setText(new_widget->property("widget_title").toString());
    ui->m_container_widget_layout->addWidget(new_widget);
}

void MainWindow::ShowInfoMenu()
{
    auto* info_menu = new QMenu(ui->m_info_btn);

    auto* about_action = new QAction("About");
    connect(about_action, &QAction::triggered, this, [=] {
        const QString about_message = QString(
                "<h3>Remote Controller</h3>"
                "<p>Version: %0</p>"
                "<br>"
                "<pre><p><i>\tAether Projects 2022</i></p></pre>").arg(QCoreApplication::applicationVersion());
        QMessageBox::about(this, "About", about_message);
    });
    info_menu->addAction(about_action);

    auto* about_qt_action = new QAction("About Qt");
    connect(about_qt_action, &QAction::triggered, this, [=] { QMessageBox::aboutQt(this, "About Qt"); });
    info_menu->addAction(about_qt_action);

    info_menu->exec(mapToGlobal(ui->m_info_btn->pos()));
}

void MainWindow::ShowConnectionList()
{    
    DeleteActualContent();

    if(m_active_socket)
    {
        m_active_socket->close();
        m_active_socket->deleteLater();
    }

    ShowNewWidget(new ConnectionListWidget(ui->m_container_widget));
}

void MainWindow::ShowAddConnectionWidget()
{
    DeleteActualContent();
    ShowNewWidget(new AddNewConnectionWidget(ui->m_container_widget));
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

    ShowNewWidget(new RemoteFunctionsListWidget(m_active_socket, ui->m_container_widget));
}

void MainWindow::ShowSystemFunctionsList()
{
    DeleteActualContent();

    ShowNewWidget(new SystemFunctionsListWidget(m_active_socket, ui->m_container_widget));
}

void MainWindow::ShowFilesList()
{
    DeleteActualContent();

    ShowNewWidget(new FileListWidget(m_active_socket, ui->m_container_widget));
}

void MainWindow::InitializeConnections()
{
    connect(ui->m_back_btn, &QPushButton::clicked, this, &MainWindow::RequestBack);
    connect(ui->m_info_btn, &QPushButton::clicked, this, &MainWindow::ShowInfoMenu);
}

void MainWindow::InitializeUi()
{
    auto back_button_size_policy = ui->m_back_btn->sizePolicy();
    back_button_size_policy.setRetainSizeWhenHidden(true);
    ui->m_back_btn->setSizePolicy(back_button_size_policy);

    ShowConnectionList();
}

