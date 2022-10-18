#ifndef SERVERRUNNER_H
#define SERVERRUNNER_H

#include <QObject>
#include <QtCore/QPointer>
#include <QtCore/QTimer>
#include <QtWebSockets/QWebSocketServer>

class ServerRunner : public QObject
{
    Q_OBJECT
public:
    explicit ServerRunner() = default;
    virtual ~ServerRunner() = default;

    static ServerRunner* Instance();
    static int ConnectedClientsCount();

    static void StartServer();
    static void StopServer();

private:
    static ServerRunner* m_instance;
    static QPointer<QWebSocketServer> m_web_socket_server;

    static QTimer* m_socket_watcher_timer;
    static QVector<QWebSocket*> m_connected_sockets;

private slots:
    void NewSocketConnected() const;
    void SocketDisconnected() const;
    void ProcessSocketCommand(const QString& message);

    void WatchConnectedSockets();
};

#endif // SERVERRUNNER_H
