#include "addnewconnectionwidget.h"
#include "ui_addnewconnectionwidget.h"

#include "mainwindow.h"

AddNewConnectionWidget::AddNewConnectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNewConnectionWidget)
{
    ui->setupUi(this);

    InitializeConnections();
}

AddNewConnectionWidget::~AddNewConnectionWidget()
{
    delete ui;
}

QString AddNewConnectionWidget::DisplayName() const
{
    return ui->m_display_name_line_edit->text();
}

QString AddNewConnectionWidget::IPAddress() const
{
    return ui->m_ip_line_edit->text();
}

QString AddNewConnectionWidget::Port() const
{
    return ui->m_port_line_edit->text();
}

void AddNewConnectionWidget::InitializeConnections()
{
    connect(ui->m_add_btn, &QPushButton::clicked, this, [=] {
        AddDevice({IPAddress(), Port(), DisplayName()});
        MainWindow::Instance()->ShowConnectionList();
    });

    connect(MainWindow::Instance(), &MainWindow::RequestBack, this, [=] {
        MainWindow::Instance()->ShowConnectionList();
    });
}
