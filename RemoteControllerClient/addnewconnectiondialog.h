#ifndef ADDNEWCONNECTIONDIALOG_H
#define ADDNEWCONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class AddNewConnectionDialog;
}

class AddNewConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewConnectionDialog(QWidget *parent = nullptr);
    ~AddNewConnectionDialog();

    QString DisplayName() const;
    QString IPAddress() const;
    QString Port() const;

private:
    Ui::AddNewConnectionDialog *ui;

    void InitializeConnections() const;
};

#endif // ADDNEWCONNECTIONDIALOG_H
