#ifndef REMOTEFUNCTIONSLISTWIDGET_H
#define REMOTEFUNCTIONSLISTWIDGET_H

#include "remotefunctionbasewidget.h"

namespace Ui {
class RemoteFunctionsListWidget;
}

class RemoteFunctionsListWidget : public RemoteFunctionBaseWidget
{
    Q_OBJECT

public:
    explicit RemoteFunctionsListWidget(QWebSocket* active_socket, QWidget *parent = nullptr);
    ~RemoteFunctionsListWidget();

private:
    Ui::RemoteFunctionsListWidget *ui;

    void InitializeUi();
};

#endif // REMOTEFUNCTIONSLISTWIDGET_H
