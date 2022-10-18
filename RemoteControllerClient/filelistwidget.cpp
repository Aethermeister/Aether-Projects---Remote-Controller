#include "filelistwidget.h"
#include "ui_filelistwidget.h"
#include "listwidgetitembutton.h"
#include "mainwindow.h"

#include <QtCore/QStandardPaths>
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtWidgets/QMessageBox>

QMap<QString, QString> FileListWidget::EntryTypeIcons
{
    { "drive", "://Images/baseline_storage_white_24dp.png" },
    { "dir", "://Images/baseline_folder_white_24dp.png" },
    { "file", "://Images/baseline_description_white_24dp.png" },
};

FileListWidget::FileListWidget(QWebSocket* active_socket, QWidget* parent) :
    RemoteFunctionBaseWidget(active_socket, parent),
    ui(new Ui::FileListWidget)
{
    ui->setupUi(this);

    ui->m_files_list_widget->setStyleSheet("QListView::item { height: 50px; }");

    connect(MainWindow::Instance(), &MainWindow::RequestBack, this, [=] {
        MainWindow::Instance()->ShowRemoteFunctionsList();
    });

    connect(m_active_socket, &QWebSocket::textMessageReceived, this, [=] (const QString& message) {
        QStringList response_commands;
        const auto response_object = ParseResponse(message, &response_commands);

        if(response_commands.contains(CommandTypes.value(CommandType::RESPONSE_FOLDER_CONTENT)))
        {
            ListFiles(response_object.value("drives_root").toBool(), response_object.value("entries").toArray());
            m_last_used_folder_path = response_object.value("current_path").toString();
        }
        if(response_commands.contains(CommandTypes.value(CommandType::REFRESH_FOLDER_CONTENT)))
        {
            SendCommand(CreateCommand(CommandType::GET_FOLDER_CONTENT, {{ "root_folder", m_last_used_folder_path }}));
        }
        if(response_commands.contains(CommandTypes.value(CommandType::RESPONSE_FILE_CONTENT)))
        {
            QMessageBox::about(this, response_object.value("filename").toString(), response_object.value("file_content").toString());
        }
        if(response_commands.contains(CommandTypes.value(CommandType::RESPONSE_DOWNLOAD_FILE)))
        {
            CreateDownloadedFile(response_object.value("filename").toString(), response_object.value("file_content").toString());
        }
    });

    m_last_used_folder_path = GetLastUsedFolderPath();
    SendCommand(CreateCommand(CommandType::GET_FOLDER_CONTENT, {{ "root_folder", m_last_used_folder_path }}));
}

FileListWidget::~FileListWidget()
{
    SaveLastUsedFolderPath();
    delete ui;
}

QString FileListWidget::GetLastUsedFolderPath()
{
    QSettings settings;
    return settings.value("last_used_folder_path").toString();
}

void FileListWidget::SaveLastUsedFolderPath()
{
    QSettings settings;
    settings.setValue("last_used_folder_path", m_last_used_folder_path);
}

void FileListWidget::ListFiles(bool is_drives_root, const QJsonArray& entries)
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
        SendCommand(CreateCommand(CommandType::GET_FOLDER_CONTENT));
    });

    ui->m_files_list_widget->addItem(drives_item);
    ui->m_files_list_widget->setItemWidget(drives_item, drives_button);
}

void FileListWidget::CreateEntryButton(const QJsonObject& entry_object)
{
    const auto name = entry_object.value("name").toString();
    const auto absolute_path = entry_object.value("absolute_path").toString();
    const auto type = entry_object.value("type").toString();

    auto* entry_item = new QListWidgetItem(ui->m_files_list_widget);
    auto* entry_button = new ListWidgetItemButton(name, ui->m_files_list_widget);
    entry_button->SetTextAlignment(ButtonTextAlignment::LEFT);
    entry_button->setIcon(QIcon(EntryTypeIcons.value(type)));
    entry_button->setIconSize({ 30, 30 });

    entry_button->setProperty("absolute_path", absolute_path);
    entry_button->setProperty("type", type);
    if((type == "dir" && name == "..") || type == "drive")
    {
        entry_button->SetMoreButtonVisibility(false);
    }

    connect(entry_button->MoreButton(), &QPushButton::clicked, this, &FileListWidget::ShowMoreMenu);
    connect(entry_button, &QPushButton::clicked, this, [=] {
        if(type != "file")
        {
            SendCommand(CreateCommand(CommandType::GET_FOLDER_CONTENT, {{ "root_folder", absolute_path }}));
        }
    });

    ui->m_files_list_widget->addItem(entry_item);
    ui->m_files_list_widget->setItemWidget(entry_item, entry_button);
}

void FileListWidget::CreateContentAction(QMenu* menu, const QString& absolute_path)
{
    auto* content_action = new QAction("Content", menu);
    connect(content_action, &QAction::triggered, this, [=]{
        SendCommand(CreateCommand(CommandType::GET_FILE_CONTENT, {{ "absolute_path", absolute_path }}));
    });

    menu->addAction(content_action);
}

void FileListWidget::CreateDownloadAction(QMenu* menu, const QString& absolute_path)
{
    auto* download_action = new QAction("Download", menu);
    connect(download_action, &QAction::triggered, this, [=]{
        SendCommand(CreateCommand(CommandType::DOWNLOAD_FILE, {{ "absolute_path", absolute_path }}));
    });
    menu->addAction(download_action);
}

void FileListWidget::CreateDeleteAction(QMenu* menu, const QString& absolute_path, const QString& type)
{
    auto* delete_action = new QAction("Delete", menu);
    connect(delete_action, &QAction::triggered, this, [=]{
        const auto confirmation = QMessageBox::warning(this, "Confirmation", "Delete selected file or folder?",
                                                       QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::Cancel);

        if(confirmation == QMessageBox::StandardButton::Yes)
        {
            if(type == "file")
            {
                SendCommand(CreateCommand(CommandType::DELETE_SELECTED_FILE, {{ "absolute_path", absolute_path }}));
            }
            else if(type == "dir")
            {
                SendCommand(CreateCommand(CommandType::DELETE_SELECTED_FOLDER, {{ "absolute_path", absolute_path }}));
            }
        }
    });

    menu->addAction(delete_action);
}

void FileListWidget::CreateDownloadedFile(const QString& filename, const QString& file_content)
{
    const auto download_folder = QStandardPaths::standardLocations(QStandardPaths::StandardLocation::DownloadLocation).at(0);
    QFile downloaded_file(download_folder + "\\" + filename);
    if(downloaded_file.open(QIODevice::WriteOnly))
    {
        downloaded_file.write(file_content.toUtf8());
        QMessageBox::information(this, "Download successful", "Download completed");
    }
    else
    {
        QMessageBox::critical(this, "Download error", "Selected file could not be downloaded");
    }
}

void FileListWidget::ShowMoreMenu()
{
    auto* parent_list_widget_item_button = dynamic_cast<ListWidgetItemButton*>(sender()->parent());
    if(parent_list_widget_item_button)
    {
        auto* more_button = dynamic_cast<QPushButton*>(sender());
        auto* more_menu = new QMenu(more_button);

        const auto absolute_path = parent_list_widget_item_button->property("absolute_path").toString();
        const auto type = parent_list_widget_item_button->property("type").toString();

        if(type == "file")
        {
            CreateContentAction(more_menu, absolute_path);
            CreateDownloadAction(more_menu, absolute_path);
        }

        if(type != "drive")
        {
            CreateDeleteAction(more_menu, absolute_path, type);
        }

        more_menu->exec(parent_list_widget_item_button->mapToGlobal(more_button->pos()));
    }
}
