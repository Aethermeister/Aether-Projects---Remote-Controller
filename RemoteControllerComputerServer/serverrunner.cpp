#include "serverrunner.h"
#include "logger.h"
#include "commandprocessor.h"

#include <QtWebSockets/QWebSocket>

ServerRunner* ServerRunner::m_instance = nullptr;
QPointer<QWebSocketServer> ServerRunner::m_web_socket_server = nullptr;
int ServerRunner::m_connected_clients_count = 0;

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
    return m_connected_clients_count;
}

void ServerRunner::StartServer()
{
    StopServer();

    Logger::LogInfo("Server started");
    m_web_socket_server = new QWebSocketServer("Test_server", QWebSocketServer::SslMode::NonSecureMode);
    m_web_socket_server->listen(QHostAddress::SpecialAddress::Any, 7777);

    connect(m_web_socket_server, &QWebSocketServer::newConnection, Instance(), &ServerRunner::NewSocketConnected);
}

void ServerRunner::StopServer()
{
    if(m_web_socket_server)
    {
        m_web_socket_server->deleteLater();
        Logger::LogInfo("Server stopped");
    }
}

void ServerRunner::NewSocketConnected() const
{
    auto* new_client_socket = m_web_socket_server->nextPendingConnection();
    connect(new_client_socket, &QWebSocket::textMessageReceived, Instance(), &ServerRunner::ProcessSocketCommand);
    connect(new_client_socket, &QWebSocket::disconnected, this, &ServerRunner::SocketDisconnected);

    ++m_connected_clients_count;

    Logger::LogInfo("Client connected");
}

void ServerRunner::SocketDisconnected() const
{
    auto* disconnected_socket = qobject_cast<QWebSocket*>(sender());
    disconnected_socket->deleteLater();

    --m_connected_clients_count;

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
