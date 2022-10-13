#ifndef CONNECTIONLISTWIDGETBUTTON_H
#define CONNECTIONLISTWIDGETBUTTON_H

#include <QtWidgets/QPushButton>
#include <QtWebSockets/QWebSocket>

namespace Ui {
class ConnectionListWidgetButton;
}

class ConnectionListWidgetButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ConnectionListWidgetButton(const QString& display_name, const QString& ip_address, const QString& port, QWidget *parent = nullptr);
    virtual ~ConnectionListWidgetButton();

    const QString& IPAddress() const;
    const QString& Port() const;

private:
    Ui::ConnectionListWidgetButton *ui;

    QString m_display_name;
    QString m_ip_address;
    QString m_port;

    QWebSocket* m_socket = nullptr;

    void InitializeUi();
};

#endif // CONNECTIONLISTWIDGETBUTTON_H
