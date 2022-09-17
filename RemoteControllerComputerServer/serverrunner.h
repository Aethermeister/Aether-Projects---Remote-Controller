#ifndef SERVERRUNNER_H
#define SERVERRUNNER_H

#include <QObject>
#include <QtCore/QPointer>
#include <QtWebSockets/QWebSocketServer>

class ServerRunner : public QObject
{
    Q_OBJECT
public:
    explicit ServerRunner() = default;
    virtual ~ServerRunner() = default;

    static ServerRunner* Instance();

    static void StartServer();
    static void StopServer();

private:
    static ServerRunner* m_instance;
    static QPointer<QWebSocketServer> m_web_socket_server;

private slots:
    void NewSocketConnected() const;
    void SocketDisconnected() const;
    void ProcessSocketCommand(const QString& message);
};

#endif // SERVERRUNNER_H
