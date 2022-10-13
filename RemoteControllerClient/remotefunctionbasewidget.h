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
    GET_FILE_CONTENT,
    DELETE_SELECTED_FILE
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
    QJsonObject CreateSimpleCommand(CommandType command) const;
    QJsonObject ParseResponse(const QString& response) const;
};

#endif // REMOTEFUNCTIONBASEWIDGET_H
