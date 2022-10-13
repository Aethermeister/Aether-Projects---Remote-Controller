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
    { CommandType::GET_FILE_CONTENT, "get_file_content" },
    { CommandType::DELETE_SELECTED_FILE, "delete_selected_file" }
};

RemoteFunctionBaseWidget::RemoteFunctionBaseWidget(QWebSocket *active_socket, QWidget *parent)
    : QWidget{parent}, m_active_socket(active_socket)
{
    connect(m_active_socket, &QWebSocket::textMessageReceived, this, [=] (const QString& message) {
        qDebug() << message;

        const auto response_object = ParseResponse(message);
        const auto response_command = response_object.value("command").toString();

        if(CommandTypes.key(response_command) == CommandType::RESPONSE_NOTIFICATION)
        {
            QMessageBox::information(this, "Response", response_object.value("message").toString());
        }
    });
}

void RemoteFunctionBaseWidget::SendCommand(const QJsonObject &command_object)
{
    const QJsonDocument command_json_document(command_object);
    m_active_socket->sendTextMessage(command_json_document.toJson());
}

QJsonObject RemoteFunctionBaseWidget::CreateSimpleCommand(CommandType command) const
{
    QJsonObject command_object;
    command_object["command"] = CommandTypes.value(command);

    return command_object;
}

QJsonObject RemoteFunctionBaseWidget::ParseResponse(const QString &response) const
{
    QJsonDocument response_document = QJsonDocument::fromJson(response.toUtf8());
    return response_document.object();
}
