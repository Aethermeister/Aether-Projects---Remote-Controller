#include "systemfunctionslistwidget.h"
#include "ui_systemfunctionslistwidget.h"
#include "mainwindow.h"
#include "listwidgetitembutton.h"

#include <QtWidgets/QMenu>

SystemFunctionsListWidget::SystemFunctionsListWidget(QWebSocket *active_socket, QWidget *parent) :
    RemoteFunctionBaseWidget(active_socket, parent),
    ui(new Ui::SystemFunctionsListWidget)
{
    ui->setupUi(this);

    InitializeUi();
    connect(ui->m_back_btn, &QPushButton::clicked, this, [=] {
        MainWindow::Instance()->ShowRemoteFunctionsList();
    });
}

SystemFunctionsListWidget::~SystemFunctionsListWidget()
{
    delete ui;
}

void SystemFunctionsListWidget::InitializeUi()
{
    ui->m_system_functions_list_widget->setStyleSheet("QListView::item { height: 100px; }");

    CreateClientCountButton();
    CreateLockWorkstationButton();
    CreateShutdownButton();
    CreateRestartButton();
}

void SystemFunctionsListWidget::CreateClientCountButton()
{
    auto* client_count_item = new QListWidgetItem(ui->m_system_functions_list_widget);
    ui->m_system_functions_list_widget->addItem(client_count_item);
    auto* client_count_button = new QPushButton("Connected clients");
    ui->m_system_functions_list_widget->setItemWidget(client_count_item, client_count_button);
    connect(client_count_button, &QPushButton::clicked, this, [=] {
        SendCommand(CreateSimpleCommand(CommandType::GET_CLIENT_COUNT));
    });
}

void SystemFunctionsListWidget::CreateLockWorkstationButton()
{
    auto* lock_workstation_item = new QListWidgetItem(ui->m_system_functions_list_widget);
    ui->m_system_functions_list_widget->addItem(lock_workstation_item);
    auto* lock_workstation_button = new QPushButton("Lock Workstation");
    ui->m_system_functions_list_widget->setItemWidget(lock_workstation_item, lock_workstation_button);
    connect(lock_workstation_button, &QPushButton::clicked, this, [=] {
        SendCommand(CreateSimpleCommand(CommandType::LOCK_WORKSTATION));
    });
}

void SystemFunctionsListWidget::CreateShutdownButton()
{
    auto* shutdown_item = new QListWidgetItem(ui->m_system_functions_list_widget);
    ui->m_system_functions_list_widget->addItem(shutdown_item);
    auto* shutdown_button = new ListWidgetItemButton("Shutdown", ui->m_system_functions_list_widget);
    ui->m_system_functions_list_widget->setItemWidget(shutdown_item, shutdown_button);
    connect(shutdown_button, &QPushButton::clicked, this, [=] {
        SendCommand(CreateSimpleCommand(CommandType::SHUTDOWN_SYSTEM));
    });

    connect(shutdown_button->MoreButton(), &QPushButton::clicked, this, [=] {
        auto* more_menu = new QMenu(shutdown_button->MoreButton());

        more_menu->addAction(CreateShutdownActionWithTimeout("5 M", 300));
        more_menu->addAction(CreateShutdownActionWithTimeout("15 M", 900));
        more_menu->addAction(CreateShutdownActionWithTimeout("30 M", 1800));
        more_menu->addAction(CreateShutdownActionWithTimeout("1 H", 3600));

        auto* cancel_action = new QAction("Cancel", more_menu);
        more_menu->addAction(cancel_action);
        connect(cancel_action, &QAction::triggered, this, [=] {
            SendCommand(CreateSimpleCommand(CommandType::CANCEL_SHUTDOWN));
        });

        more_menu->exec(shutdown_button->mapToGlobal(shutdown_button->MoreButton()->pos()));
    });
}

QAction *SystemFunctionsListWidget::CreateShutdownActionWithTimeout(const QString& text, int timeout)
{
    auto* timeout_action = new QAction(text);
    timeout_action->setProperty("timeout", timeout);
    connect(timeout_action, &QAction::triggered, this, &SystemFunctionsListWidget::ShutdownWithTimeout);

    return timeout_action;
}

void SystemFunctionsListWidget::CreateRestartButton()
{
    auto* restart_item = new QListWidgetItem(ui->m_system_functions_list_widget);
    ui->m_system_functions_list_widget->addItem(restart_item);
    auto* restart_button = new ListWidgetItemButton("Restart", ui->m_system_functions_list_widget);
    ui->m_system_functions_list_widget->setItemWidget(restart_item, restart_button);
    connect(restart_button, &QPushButton::clicked, this, [=] {
        SendCommand(CreateSimpleCommand(CommandType::RESTART_SYSTEM));
    });

    connect(restart_button->MoreButton(), &QPushButton::clicked, this, [=] {
        auto* more_menu = new QMenu(restart_button->MoreButton());

        more_menu->addAction(CreateRestartActionWithTimeout("5 M", 300));
        more_menu->addAction(CreateRestartActionWithTimeout("15 M", 900));
        more_menu->addAction(CreateRestartActionWithTimeout("30 M", 1800));
        more_menu->addAction(CreateRestartActionWithTimeout("1 H", 3600));

        auto* cancel_action = new QAction("Cancel", more_menu);
        more_menu->addAction(cancel_action);
        connect(cancel_action, &QAction::triggered, this, [=] {
            SendCommand(CreateSimpleCommand(CommandType::CANCEL_RESTART));
        });

        more_menu->exec(restart_button->mapToGlobal(restart_button->MoreButton()->pos()));
    });
}

QAction *SystemFunctionsListWidget::CreateRestartActionWithTimeout(const QString &text, int timeout)
{
    auto* timeout_action = new QAction(text);
    timeout_action->setProperty("timeout", timeout);
    connect(timeout_action, &QAction::triggered, this, &SystemFunctionsListWidget::RestartWithTimeout);

    return timeout_action;
}

void SystemFunctionsListWidget::ShutdownWithTimeout()
{
    const auto timeout = sender()->property("timeout").toInt();
    auto shutdown_command = CreateSimpleCommand(CommandType::SHUTDOWN_SYSTEM);
    shutdown_command["timeout"] = timeout;

    SendCommand(shutdown_command);
}

void SystemFunctionsListWidget::RestartWithTimeout()
{
    const auto timeout = sender()->property("timeout").toInt();
    auto shutdown_command = CreateSimpleCommand(CommandType::RESTART_SYSTEM);
    shutdown_command["timeout"] = timeout;

    SendCommand(shutdown_command);
}
