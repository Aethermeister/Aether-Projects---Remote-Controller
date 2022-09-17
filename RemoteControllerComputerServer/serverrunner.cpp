#include "serverrunner.h"
#include "logger.h"

#include <QtWebSockets/QWebSocket>

ServerRunner* ServerRunner::m_instance = nullptr;
QPointer<QWebSocketServer> ServerRunner::m_web_socket_server = nullptr;

ServerRunner *ServerRunner::Instance()
{
    if(m_instance)
    {
        return m_instance;
    }

    return new ServerRunner();
}

void ServerRunner::StartServer()
{
    StopServer();

    qDebug() << "========================= Server started =========================";
    m_web_socket_server = new QWebSocketServer("Test_server", QWebSocketServer::SslMode::NonSecureMode);
    m_web_socket_server->listen(QHostAddress::SpecialAddress::Any, 7777);

    connect(m_web_socket_server, &QWebSocketServer::newConnection, Instance(), &ServerRunner::NewSocketConnected);
}

void ServerRunner::StopServer()
{
    if(m_web_socket_server)
    {
        m_web_socket_server->deleteLater();
        qDebug() << "========================= Server stopped =========================";
    }
}

void ServerRunner::NewSocketConnected() const
{
    auto* new_client_socket = m_web_socket_server->nextPendingConnection();
    connect(new_client_socket, &QWebSocket::textMessageReceived, Instance(), &ServerRunner::ProcessSocketCommand);
    connect(new_client_socket, &QWebSocket::disconnected, this, &ServerRunner::SocketDisconnected);

    Logger::LogDebug("Client socket connected");
}

void ServerRunner::SocketDisconnected() const
{
    auto* disconnected_socket = qobject_cast<QWebSocket*>(sender());
    disconnected_socket->deleteLater();

    Logger::LogDebug("Client socket disconnected");
}

void ServerRunner::ProcessSocketCommand(const QString &message)
{
    Logger::LogDebug("Message received: " + message);

    auto* client_socket = qobject_cast<QWebSocket*>(sender());

    if(client_socket)
    {
        // Execute command and return results
        client_socket->sendTextMessage("Got it");
    }
}
