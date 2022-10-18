#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include "remotefunctionbasewidget.h"

#include <QtWidgets/QMenu>

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

    static QMap<QString, QString> EntryTypeIcons;

    QString m_last_used_folder_path;
    QString GetLastUsedFolderPath();
    void SaveLastUsedFolderPath();

    void ListFiles(bool is_drives_root, const QJsonArray& entries);
    void CreateDrivesButton();
    void CreateEntryButton(const QJsonObject& entry_object);

    void CreateContentAction(QMenu* menu, const QString& absolute_path);
    void CreateDownloadAction(QMenu* menu, const QString& absolute_path);
    void CreateDeleteAction(QMenu* menu, const QString& absolute_path, const QString& type);

    void CreateDownloadedFile(const QString& filename, const QString& file_content);

private slots:
    void ShowMoreMenu();
};

#endif // FILELISTWIDGET_H
