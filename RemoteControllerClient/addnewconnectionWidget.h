#ifndef ADDNEWCONNECTIONWIDGET_H
#define ADDNEWCONNECTIONWIDGET_H

#include "connecteddevicemanager.h"

#include <QWidget>

namespace Ui {
class AddNewConnectionWidget;
}

class AddNewConnectionWidget : public QWidget, public ConnectedDeviceManager
{
    Q_OBJECT

public:
    explicit AddNewConnectionWidget(QWidget *parent = nullptr);
    ~AddNewConnectionWidget();

    QString DisplayName() const;
    QString IPAddress() const;
    QString Port() const;

private:
    Ui::AddNewConnectionWidget *ui;

    void InitializeConnections();
};

#endif // ADDNEWCONNECTIONWIDGET_H
