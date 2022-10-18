#ifndef CONNECTIONLISTWIDGET_H
#define CONNECTIONLISTWIDGET_H

#include "connecteddevicemanager.h"

#include <QWidget>

namespace Ui {
class ConnectionListWidget;
}

class ConnectionListWidget : public QWidget, public ConnectedDeviceManager
{
    Q_OBJECT

public:
    explicit ConnectionListWidget(QWidget *parent = nullptr);
    ~ConnectionListWidget();

private:
    Ui::ConnectionListWidget *ui;

    void InitializeUi();
    void ListDevices();
    void CreateAddButton();

private slots:
    void ConnectToComputer();
};

#endif // CONNECTIONLISTWIDGET_H
