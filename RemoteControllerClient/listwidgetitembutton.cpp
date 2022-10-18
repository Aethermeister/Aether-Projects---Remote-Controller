#include "listwidgetitembutton.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>

QMap<ButtonTextAlignment, QString> ListWidgetItemButton::m_button_text_alignments
{
    { ButtonTextAlignment::CENTER, "center" },
    { ButtonTextAlignment::RIGHT, "right" },
    { ButtonTextAlignment::LEFT, "left" }
};

ListWidgetItemButton::ListWidgetItemButton(QWidget* parent) : QPushButton(parent)
{
    InitializeUi();
}

ListWidgetItemButton::ListWidgetItemButton(const QString& text, QWidget* parent) : QPushButton(text, parent)
{
    InitializeUi();
}

void ListWidgetItemButton::SetTextAlignment(ButtonTextAlignment alignment)
{
    setStyleSheet(QString("text-align: %0;").arg(m_button_text_alignments.value(alignment)));
}

void ListWidgetItemButton::SetMoreButtonVisibility(bool visible)
{
    m_more_button->setVisible(visible);
}

QPushButton *ListWidgetItemButton::MoreButton() const
{
    return m_more_button;
}

void ListWidgetItemButton::InitializeUi()
{
    m_more_button = new QPushButton(this);
    m_more_button->setProperty("type", "menu");
    m_more_button->setFlat(true);
    m_more_button->setFixedSize(25, 25);
    m_more_button->setIcon(QIcon("://Images/baseline_more_vert_white_24dp.png"));
    m_more_button->setIconSize({ 25, 25 });

    auto* spacer_item = new QSpacerItem(10, 10, QSizePolicy::Expanding);

    auto* button_layout = new QHBoxLayout(this);
    button_layout->addItem(spacer_item);
    button_layout->addWidget(m_more_button);

    setLayout(button_layout);
}
