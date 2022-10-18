#include "serverrunner.h"
#include "logger.h"
#include "commandprocessor.h"

#include <QtWebSockets/QWebSocket>

ServerRunner* ServerRunner::m_instance = nullptr;
QPointer<QWebSocketServer> ServerRunner::m_web_socket_server = nullptr;
QTimer* ServerRunner::m_socket_watcher_timer = nullptr;
QVector<QWebSocket*> ServerRunner::m_connected_sockets;

ServerRunner *ServerRunner::Instance()
{
    if(m_instance)
    {
        return m_instance;
    }

    return new ServerRunner();
}

int ServerRunner::ConnectedClientsCount()
{
    return m_connected_sockets.count();
}

void ServerRunner::StartServer()
{
    StopServer();

    m_web_socket_server = new QWebSocketServer("Test_server", QWebSocketServer::SslMode::NonSecureMode);
    m_web_socket_server->listen(QHostAddress::SpecialAddress::Any, 7777);

    connect(m_web_socket_server, &QWebSocketServer::newConnection, Instance(), &ServerRunner::NewSocketConnected);

    m_socket_watcher_timer = new QTimer(Instance());
    m_socket_watcher_timer->setInterval(300000); // 5 min
    m_socket_watcher_timer->setSingleShot(false);
    connect(m_socket_watcher_timer, &QTimer::timeout, Instance(), &ServerRunner::WatchConnectedSockets);

    m_socket_watcher_timer->start();

    Logger::LogInfo("Server started");
}

void ServerRunner::StopServer()
{
    if(m_web_socket_server)
    {
        m_socket_watcher_timer->stop();
        m_socket_watcher_timer->deleteLater();

        m_web_socket_server->deleteLater();
        Logger::LogInfo("Server stopped");
    }
}

void ServerRunner::NewSocketConnected() const
{
    auto* new_client_socket = m_web_socket_server->nextPendingConnection();
    new_client_socket->setProperty("is_responsive", true);

    connect(new_client_socket, &QWebSocket::textMessageReceived, Instance(), &ServerRunner::ProcessSocketCommand);
    connect(new_client_socket, &QWebSocket::disconnected, this, &ServerRunner::SocketDisconnected);
    connect(new_client_socket, &QWebSocket::pong, new_client_socket, [=] { new_client_socket->setProperty("is_responsive", true); });

    m_connected_sockets.append(new_client_socket);

    Logger::LogInfo("Client connected");

}

void ServerRunner::SocketDisconnected() const
{
    auto* disconnected_socket = qobject_cast<QWebSocket*>(sender());
    disconnected_socket->deleteLater();

    m_connected_sockets.removeOne(disconnected_socket);

    Logger::LogInfo("Client disconnected");
}

void ServerRunner::ProcessSocketCommand(const QString &message)
{
    Logger::LogDebug("Message received: " + message);

    auto* client_socket = qobject_cast<QWebSocket*>(sender());

    if(client_socket)
    {
        // Execute command and return results
        const auto command_response_object = CommandProcessor::ExecuteCommand(message);
        const auto response_json_document = QJsonDocument(command_response_object);

        client_socket->sendTextMessage(response_json_document.toJson());
    }
}

void ServerRunner::WatchConnectedSockets()
{
    Logger::LogDebug("Checking sockets");

    QVector<QWebSocket*> sockets_to_delete;

    for(auto* connected_socket : m_connected_sockets)
    {
        const auto is_socket_responsive = connected_socket->property("is_responsive").toBool();
        if(is_socket_responsive)
        {
            connected_socket->setProperty("is_responsive", false);
            connected_socket->ping();
        }
        else
        {
            sockets_to_delete.append(connected_socket);
        }
    }

    for(auto* socket_to_delete : sockets_to_delete)
    {
        m_connected_sockets.removeOne(socket_to_delete);
        socket_to_delete->deleteLater();

        Logger::LogInfo("Client connection lost");
    }
}
