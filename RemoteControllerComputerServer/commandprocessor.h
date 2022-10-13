#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <QtCore/QString>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

enum class CommandType
{
    RESPONSE_NOTIFICATION,

    GET_CLIENT_COUNT,

    LOCK_WORKSTATION,

    SHUTDOWN_SYSTEM,
    CANCEL_SHUTDOWN,

    RESTART_SYSTEM,
    CANCEL_RESTART,

    GET_FOLDER_CONTENT,
    RESPONSE_FOLDER_CONTENT,
    DELETE_SELECTED_FILE,

    INVALID
};

class CommandProcessor
{
public:
    explicit CommandProcessor() = delete;
    virtual ~CommandProcessor() = delete;

    static QJsonObject ExecuteCommand(const QString& command_string);

private:
    static QMap<QString, CommandType> CommandTypes;

    static void GetClientCount(QJsonObject* response_object);

    static void LockWorkstation(QJsonObject* response_object);

    static void ShutdownSystem(const QJsonObject& command_object, QJsonObject* response_object);
    static void CancelShutdown(QJsonObject* response_object);

    static void RestartSystem(const QJsonObject& command_object, QJsonObject* response_object);
    static void CancelRestart(QJsonObject* response_object);

    static void GetFolderContent(const QJsonObject& command_object, QJsonObject* response_object);
    static void DeleteSelectedFile(const QJsonObject& command_object, QJsonObject* response_object);
};

#endif // COMMANDPROCESSOR_H
