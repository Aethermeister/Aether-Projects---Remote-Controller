#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtCore/QPointer>
#include <QtWebSockets/QWebSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* Instance();

    void ShowConnectionList();
    void ShowRemoteFunctionsList(QWebSocket* new_socket = nullptr);
    void ShowSystemFunctionsList();
    void ShowFilesList();

private:
    Ui::MainWindow *ui;

    static MainWindow* m_instance;
    QPointer<QWebSocket> m_active_socket;

    void DeleteActualContent();
};
#endif // MAINWINDOW_H
