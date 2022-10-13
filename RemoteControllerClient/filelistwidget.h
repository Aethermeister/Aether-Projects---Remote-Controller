#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include "remotefunctionbasewidget.h"

namespace Ui {
class FileListWidget;
}

class FileListWidget : public RemoteFunctionBaseWidget
{
    Q_OBJECT

public:
    explicit FileListWidget(QWebSocket* active_socket, QWidget *parent = nullptr);
    ~FileListWidget();

private:
    Ui::FileListWidget *ui;

    void ListFiles(bool is_drives_root, const QJsonArray& entries);
    void CreateDrivesButton();
    void CreateEntryButton(const QJsonObject& entry_object);

private slots:
    void ShowMoreMenu();
};

#endif // FILELISTWIDGET_H
