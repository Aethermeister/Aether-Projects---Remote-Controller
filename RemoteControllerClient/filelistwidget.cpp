#include "filelistwidget.h"
#include "ui_filelistwidget.h"
#include "listwidgetitembutton.h"
#include "mainwindow.h"

#include <QtWidgets/QMenu>

FileListWidget::FileListWidget(QWebSocket* active_socket, QWidget *parent) :
    RemoteFunctionBaseWidget(active_socket, parent),
    ui(new Ui::FileListWidget)
{
    ui->setupUi(this);

    ui->m_files_list_widget->setStyleSheet("QListView::item { height: 50px; }");

    connect(ui->m_back_btn, &QPushButton::clicked, this, [=] {
        MainWindow::Instance()->ShowRemoteFunctionsList();
    });

    connect(m_active_socket, &QWebSocket::textMessageReceived, this, [=] (const QString& message) {
        const auto response_object = ParseResponse(message);
        const auto response_command = response_object.value("command").toString();

        if(CommandTypes.key(response_command) == CommandType::RESPONSE_FOLDER_CONTENT)
        {
            ListFiles(response_object.value("drives_root").toBool(), response_object.value("entries").toArray());
        }
    });

    SendCommand(CreateSimpleCommand(CommandType::GET_FOLDER_CONTENT));
}

FileListWidget::~FileListWidget()
{
    delete ui;
}

void FileListWidget::ListFiles(bool is_drives_root, const QJsonArray &entries)
{
    ui->m_files_list_widget->clear();

    if(!is_drives_root)
    {
        CreateDrivesButton();
    }

    for(const auto& entry_ref : entries)
    {
        CreateEntryButton(entry_ref.toObject());
    }
}

void FileListWidget::CreateDrivesButton()
{
    auto* drives_item = new QListWidgetItem(ui->m_files_list_widget);
    auto* drives_button = new ListWidgetItemButton("=====Drives=====", ui->m_files_list_widget);
    drives_button->SetMoreButtonVisibility(false);

    connect(drives_button, &QPushButton::clicked, this, [=] {
        SendCommand(CreateSimpleCommand(CommandType::GET_FOLDER_CONTENT));
    });

    ui->m_files_list_widget->addItem(drives_item);
    ui->m_files_list_widget->setItemWidget(drives_item, drives_button);
}

void FileListWidget::CreateEntryButton(const QJsonObject &entry_object)
{
    const auto name = entry_object.value("name").toString();
    const auto absolute_path = entry_object.value("absolute_path").toString();
    const auto type = entry_object.value("type").toString();

    auto* entry_item = new QListWidgetItem(ui->m_files_list_widget);
    auto* entry_button = new ListWidgetItemButton(name, ui->m_files_list_widget);
    entry_button->SetTextAlignment(ButtonTextAlignment::LEFT);

    entry_button->setProperty("absolute_path", absolute_path);
    entry_button->setProperty("type", type);

    connect(entry_button->MoreButton(), &QPushButton::clicked, this, &FileListWidget::ShowMoreMenu);
    connect(entry_button, &QPushButton::clicked, this, [=] {
        if(type != "file")
        {
            auto command_object = CreateSimpleCommand(CommandType::GET_FOLDER_CONTENT);
            command_object["root_folder"] = absolute_path;
            SendCommand(command_object);
        }
    });

    ui->m_files_list_widget->addItem(entry_item);
    ui->m_files_list_widget->setItemWidget(entry_item, entry_button);
}

void FileListWidget::ShowMoreMenu()
{
    auto* parent_list_widget_item_button = dynamic_cast<ListWidgetItemButton*>(sender()->parent());
    if(parent_list_widget_item_button)
    {
        auto* more_button = dynamic_cast<QPushButton*>(sender());
        auto* more_menu = new QMenu(more_button);

        const auto type = parent_list_widget_item_button->property("type").toString();
        if(type == "file")
        {
            auto* content_action = new QAction("Content");
            connect(content_action, &QAction::triggered, this, [=]{
                auto content_command_object = CreateSimpleCommand(CommandType::GET_FILE_CONTENT);
                content_command_object["absolute_path"] = parent_list_widget_item_button->property("absolute_path").toString();
                SendCommand(content_command_object);
            });

            more_menu->addAction(content_action);

            auto* download_action = new QAction("Download");
            more_menu->addAction(download_action);
        }

        if(type != "drive")
        {
            auto* delete_action = new QAction("Delete");
            connect(delete_action, &QAction::triggered, this, [=]{
                auto delete_command_object = CreateSimpleCommand(CommandType::DELETE_SELECTED_FILE);
                delete_command_object["absolute_path"] = parent_list_widget_item_button->property("absolute_path").toString();
                SendCommand(delete_command_object);
            });

            more_menu->addAction(delete_action);
        }

        auto* properties_action = new QAction("Properties");
        more_menu->addAction(properties_action);

        more_menu->exec(parent_list_widget_item_button->mapToGlobal(more_button->pos()));
    }
}
