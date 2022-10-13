#ifndef CONNECTIONLISTWIDGET_H
#define CONNECTIONLISTWIDGET_H

#include <QWidget>

namespace Ui {
class ConnectionListWidget;
}

class ConnectionListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionListWidget(QWidget *parent = nullptr);
    ~ConnectionListWidget();

private:
    Ui::ConnectionListWidget *ui;

    void InitializeConnections() const;

private slots:
    void ShowAddNewConnectionDialog();
    void ConnectToComputer();
};

#endif // CONNECTIONLISTWIDGET_H
