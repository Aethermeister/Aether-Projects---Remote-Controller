#include "commandprocessor.h"
#include "logger.h"
#include "serverrunner.h"
#include <windows.h>

#include <QtCore/QDir>

QMap<QString, CommandType> CommandProcessor::CommandTypes
{
    { "response_notification", CommandType::RESPONSE_NOTIFICATION },

    { "get_client_count", CommandType::GET_CLIENT_COUNT },

    { "lock_workstation", CommandType::LOCK_WORKSTATION },

    { "shutdown", CommandType::SHUTDOWN_SYSTEM },
    { "cancel_shutdown", CommandType::CANCEL_SHUTDOWN },

    { "restart", CommandType::RESTART_SYSTEM },
    { "cancel_restart", CommandType::CANCEL_RESTART },

    { "get_folder_content", CommandType::GET_FOLDER_CONTENT },
    { "response_folder_content", CommandType::RESPONSE_FOLDER_CONTENT },
    { "refresh_folder_content", CommandType::REFRESH_FOLDER_CONTENT },

    { "get_file_content", CommandType::GET_FILE_CONTENT },
    { "response_file_content", CommandType::RESPONSE_FILE_CONTENT },
    { "download_file", CommandType::DOWNLOAD_FILE },
    { "response_download_file", CommandType::RESPONSE_DOWNLOAD_FILE },

    { "delete_selected_file", CommandType::DELETE_SELECTED_FILE },
    { "delete_selected_folder", CommandType::DELETE_SELECTED_FOLDER }
};

QJsonObject CommandProcessor::ExecuteCommand(const QString& command_string)
{
    QJsonObject response_object;

    const auto json_command = QJsonDocument::fromJson(command_string.toUtf8());
    if(json_command.isNull())
    {
        const QString message = "Invalid command syntax";
        Logger::LogDebug(message);

        response_object["command"] = CommandTypes.key(CommandType::RESPONSE_NOTIFICATION);
        response_object["message"] = message;

        return response_object;
    }

    const auto json_command_object = json_command.object();
    const auto command_type = CommandTypes.value(json_command_object.value("command").toString(), CommandType::INVALID);

    switch (command_type)
    {
    case CommandType::GET_CLIENT_COUNT:
        GetClientCount(&response_object);
        break;

    case CommandType::LOCK_WORKSTATION:
        LockWorkstation(&response_object);
        break;

    case CommandType::SHUTDOWN_SYSTEM:
        ShutdownSystem(json_command_object, &response_object);
        break;
    case CommandType::CANCEL_SHUTDOWN:
        CancelShutdown(&response_object);
        break;

    case CommandType::RESTART_SYSTEM:
        RestartSystem(json_command_object, &response_object);
        break;
    case CommandType::CANCEL_RESTART:
        CancelRestart(&response_object);
        break;

    case CommandType::GET_FOLDER_CONTENT:
        GetFolderContent(json_command_object, &response_object);
        break;
    case CommandType::GET_FILE_CONTENT:
    case CommandType::DOWNLOAD_FILE:
        SendSelectedFileContent(json_command_object, &response_object, command_type);
        break;

    case CommandType::DELETE_SELECTED_FILE:
        DeleteSelectedFile(json_command_object, &response_object);
        break;
    case CommandType::DELETE_SELECTED_FOLDER:
        DeleteSelectedFolder(json_command_object, &response_object);
        break;

    case CommandType::INVALID:
    default:
        response_object["command"] = CommandTypes.key(CommandType::RESPONSE_NOTIFICATION);
        response_object["message"] = "Unknown command";
        break;
    }

    return response_object;
}

void CommandProcessor::AddCommandToResponse(QJsonObject* response_object, const QString& additional_response_command)
{
    auto response_command = response_object->value("command").toString();

    if(!response_command.isEmpty())
    {
        response_command += "|";
    }

    response_command += additional_response_command;
    response_object->insert("command", response_command);
}

void CommandProcessor::GetClientCount(QJsonObject* response_object)
{
    response_object->insert("command",CommandTypes.key(CommandType::RESPONSE_NOTIFICATION));
    response_object->insert("message", QString("Currently connected clients: %0").arg(ServerRunner::ConnectedClientsCount()));
}

void CommandProcessor::LockWorkstation(QJsonObject* response_object)
{
    response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_NOTIFICATION));

    if(LockWorkStation())
    {
        response_object->insert("message", "Workstation locked");
    }
    else
    {
        response_object->insert("message", QString::number(GetLastError()));
    }
}

void CommandProcessor::ShutdownSystem(const QJsonObject& command_object, QJsonObject* response_object)
{
    const auto shutdown_timeout = command_object.value("timeout").toInt(60);

    system(QString("shutdown /s /t %0").arg(shutdown_timeout).toStdString().c_str());

    response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_NOTIFICATION));
    response_object->insert("message", QString("System set to shutdown in %0 minute(s)").arg(shutdown_timeout / 60));
}

void CommandProcessor::CancelShutdown(QJsonObject* response_object)
{
    system("shutdown /a");

    response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_NOTIFICATION));
    response_object->insert("message", "System shutdown cancelled");
}

void CommandProcessor::RestartSystem(const QJsonObject &command_object, QJsonObject *response_object)
{
    const auto restart_timeout = command_object.value("timeout").toInt(60);

    system(QString("shutdown /r /t %0").arg(restart_timeout).toStdString().c_str());

    response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_NOTIFICATION));
    response_object->insert("message", QString("System set to restart in %0 minute(s)").arg(restart_timeout / 60));
}

void CommandProcessor::CancelRestart(QJsonObject *response_object)
{
    system("shutdown /a");

    response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_NOTIFICATION));
    response_object->insert("message", "System restart cancelled");
}

void CommandProcessor::GetFolderContent(const QJsonObject& command_object, QJsonObject* response_object)
{
    response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_FOLDER_CONTENT));

    const auto root_folder = command_object.value("root_folder").toString();
    response_object->insert("current_path", root_folder);

    QJsonArray entry_array;

    if(root_folder.isEmpty() || !QDir(root_folder).exists())
    {
        response_object->insert("drives_root", true);

        for(const auto& drive_entry : QDir::drives())
        {
            QJsonObject entry_object;
            entry_object["name"] = drive_entry.absoluteFilePath();
            entry_object["absolute_path"] = drive_entry.absoluteFilePath();
            entry_object["type"] = "drive";
            entry_array.append(entry_object);
        }
    }
    else
    {
        for(const auto& entry : QDir(root_folder).entryInfoList(QDir::Filter::Dirs | QDir::Filter::Files | QDir::Filter::Hidden | QDir::Filter::System | QDir::Filter::NoDot))
        {
            QJsonObject entry_object;
            entry_object["name"] = entry.fileName();
            entry_object["absolute_path"] = entry.absoluteFilePath();
            entry_object["type"] = entry.isDir() ? "dir" : "file";
            entry_array.append(entry_object);
        }
    }

    response_object->insert("entries", entry_array);
}

void CommandProcessor::SendSelectedFileContent(const QJsonObject& command_object, QJsonObject* response_object, CommandType original_command)
{
    const auto file_path = command_object.value("absolute_path").toString();
    QFile file(file_path);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            const auto file_content = file.readAll();
            const auto filename = QFileInfo(file_path).fileName();

            if(original_command == CommandType::GET_FILE_CONTENT)
            {
                response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_FILE_CONTENT));
            }
            else if(original_command == CommandType::DOWNLOAD_FILE)
            {
                response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_DOWNLOAD_FILE));
            }

            response_object->insert("filename", filename);
            response_object->insert("file_content", QString(file_content));
        }
        else
        {
            response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_NOTIFICATION));
            response_object->insert("message", "Selected file could not be opened");
        }
    }
    else
    {
        response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_NOTIFICATION));
        response_object->insert("message", "Selected file does not exist");
    }
}

void CommandProcessor::DeleteSelectedFile(const QJsonObject& command_object, QJsonObject* response_object)
{
    response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_NOTIFICATION));

    const auto file_path = command_object.value("absolute_path").toString();
    if(QFile::exists(file_path))
    {
        if(QFile::remove(file_path))
        {
            response_object->insert("message", "Selected file has been removed");
            AddCommandToResponse(response_object, CommandTypes.key(CommandType::REFRESH_FOLDER_CONTENT));
        }
        else
        {
            response_object->insert("message", "Selected file could not be removed");
        }
    }
    else
    {
        response_object->insert("message", "Selected file does not exist");
    }
}

void CommandProcessor::DeleteSelectedFolder(const QJsonObject& command_object, QJsonObject* response_object)
{
    response_object->insert("command", CommandTypes.key(CommandType::RESPONSE_NOTIFICATION));

    const auto folder_path = command_object.value("absolute_path").toString();
    QDir folder_to_delete(folder_path);
    if(folder_to_delete.exists())
    {
        if(folder_to_delete.removeRecursively())
        {
            response_object->insert("message", "Selected folder has been removed");
            AddCommandToResponse(response_object, CommandTypes.key(CommandType::REFRESH_FOLDER_CONTENT));
        }
        else
        {
            response_object->insert("message", "Selected folder could not be removed");
        }
    }
    else
    {
        response_object->insert("message", "Selected folder does not exist");
    }
}
