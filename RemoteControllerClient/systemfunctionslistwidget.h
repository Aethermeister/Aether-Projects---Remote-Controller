#ifndef SYSTEMFUNCTIONSLISTWIDGET_H
#define SYSTEMFUNCTIONSLISTWIDGET_H

#include "remotefunctionbasewidget.h"

namespace Ui {
class SystemFunctionsListWidget;
}

class SystemFunctionsListWidget : public RemoteFunctionBaseWidget
{
    Q_OBJECT

public:
    explicit SystemFunctionsListWidget(QWebSocket* active_socket, QWidget *parent = nullptr);
    ~SystemFunctionsListWidget();

private:
    Ui::SystemFunctionsListWidget *ui;

    void InitializeUi();
    void CreateClientCountButton();
    void CreateLockWorkstationButton();
    void CreateShutdownButton();
    QAction* CreateShutdownActionWithTimeout(const QString& text, int timeout);
    void CreateRestartButton();
    QAction* CreateRestartActionWithTimeout(const QString& text, int timeout);

private slots:
    void ShutdownWithTimeout();
    void RestartWithTimeout();
};

#endif // SYSTEMFUNCTIONSLISTWIDGET_H
