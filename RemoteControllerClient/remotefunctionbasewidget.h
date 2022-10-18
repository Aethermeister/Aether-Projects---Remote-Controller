#ifndef REMOTEFUNCTIONBASEWIDGET_H
#define REMOTEFUNCTIONBASEWIDGET_H

#include <QWidget>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>
#include <QtWebSockets/QWebSocket>

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
    REFRESH_FOLDER_CONTENT,

    GET_FILE_CONTENT,
    RESPONSE_FILE_CONTENT,
    DOWNLOAD_FILE,
    RESPONSE_DOWNLOAD_FILE,

    DELETE_SELECTED_FILE,
    DELETE_SELECTED_FOLDER
};

class RemoteFunctionBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RemoteFunctionBaseWidget(QWebSocket* active_socket, QWidget *parent = nullptr);

protected:
    static QMap<CommandType, QString> CommandTypes;

    QWebSocket* m_active_socket;

    void SendCommand(const QJsonObject& command_object);
    QJsonObject CreateCommand(CommandType command, const QMap<QString, QVariant>& properties = QMap<QString, QVariant>()) const;
    QJsonObject ParseResponse(const QString& response, QStringList* response_commands) const;
};

#endif // REMOTEFUNCTIONBASEWIDGET_H
