#include "remotefunctionbasewidget.h"

#include <QtCore/QJsonDocument>
#include <QtWidgets/QMessageBox>

QMap<CommandType, QString> RemoteFunctionBaseWidget::CommandTypes
{
    { CommandType::RESPONSE_NOTIFICATION, "response_notification" },

    { CommandType::GET_CLIENT_COUNT, "get_client_count" },

    { CommandType::LOCK_WORKSTATION, "lock_workstation" },

    { CommandType::SHUTDOWN_SYSTEM, "shutdown" },
    { CommandType::CANCEL_SHUTDOWN, "cancel_shutdown" },

    { CommandType::RESTART_SYSTEM, "restart" },
    { CommandType::CANCEL_RESTART, "cancel_restart" },

    { CommandType::GET_FOLDER_CONTENT, "get_folder_content" },
    { CommandType::RESPONSE_FOLDER_CONTENT, "response_folder_content" },
    { CommandType::REFRESH_FOLDER_CONTENT, "refresh_folder_content" },

    { CommandType::GET_FILE_CONTENT, "get_file_content" },
    { CommandType::RESPONSE_FILE_CONTENT, "response_file_content" },
    { CommandType::DOWNLOAD_FILE, "download_file" },
    { CommandType::RESPONSE_DOWNLOAD_FILE, "response_download_file" },

    { CommandType::DELETE_SELECTED_FILE, "delete_selected_file" },
    { CommandType::DELETE_SELECTED_FOLDER, "delete_selected_folder" }
};

RemoteFunctionBaseWidget::RemoteFunctionBaseWidget(QWebSocket* active_socket, QWidget* parent)
    : QWidget{parent}, m_active_socket(active_socket)
{
    connect(m_active_socket, &QWebSocket::textMessageReceived, this, [=] (const QString& message) {
        qDebug().noquote() << message;

        QStringList response_commands;
        const auto response_object = ParseResponse(message, &response_commands);

        if(response_commands.contains(CommandTypes.value(CommandType::RESPONSE_NOTIFICATION)))
        {
            QMessageBox::information(this, "Response", response_object.value("message").toString());
        }
    });
}

void RemoteFunctionBaseWidget::SendCommand(const QJsonObject& command_object)
{
    const QJsonDocument command_json_document(command_object);
    m_active_socket->sendTextMessage(command_json_document.toJson());
}

QJsonObject RemoteFunctionBaseWidget::CreateCommand(CommandType command, const QMap<QString, QVariant>& properties) const
{
    QJsonObject command_object;
    command_object.insert("command", CommandTypes.value(command));

    for(auto it = properties.begin(); it != properties.end(); ++it)
    {
        command_object.insert(it.key(), it.value().toJsonValue());
    }

    return command_object;
}

QJsonObject RemoteFunctionBaseWidget::ParseResponse(const QString& response, QStringList* response_commands) const
{
    QJsonDocument response_document = QJsonDocument::fromJson(response.toUtf8());
    const auto response_object = response_document.object();

    const auto raw_commands = response_object.value("command").toString();
    *response_commands = raw_commands.split("|");


    return response_object;
}
