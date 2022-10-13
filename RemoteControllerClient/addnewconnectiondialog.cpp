#include "addnewconnectiondialog.h"
#include "ui_addnewconnectiondialog.h"

AddNewConnectionDialog::AddNewConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewConnectionDialog)
{
    ui->setupUi(this);

    InitializeConnections();
}

AddNewConnectionDialog::~AddNewConnectionDialog()
{
    delete ui;
}

QString AddNewConnectionDialog::DisplayName() const
{
    return ui->m_name_line_edit->text();
}

QString AddNewConnectionDialog::IPAddress() const
{
    return ui->m_ip_line_edit->text();
}

QString AddNewConnectionDialog::Port() const
{
    return ui->m_port_line_edit->text();
}

void AddNewConnectionDialog::InitializeConnections() const
{
    connect(ui->m_cancel_btn, &QPushButton::clicked, this, &AddNewConnectionDialog::reject);
    connect(ui->m_add_btn, &QPushButton::clicked, this, &AddNewConnectionDialog::accept);
}
