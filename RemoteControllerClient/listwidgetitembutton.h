#ifndef LISTWIDGETITEMBUTTON_H
#define LISTWIDGETITEMBUTTON_H

#include <QPushButton>
#include <QtCore/QMap>

enum class ButtonTextAlignment
{
    CENTER,
    LEFT,
    RIGHT
};

class ListWidgetItemButton : public QPushButton
{
    Q_OBJECT
public:
    ListWidgetItemButton(QWidget* parent = nullptr);
    ListWidgetItemButton(const QString& text, QWidget* parent = nullptr);
    virtual ~ListWidgetItemButton() = default;

    void SetTextAlignment(ButtonTextAlignment alignment);
    void SetMoreButtonVisibility(bool visible);
    QPushButton* MoreButton() const;

private:
    static QMap<ButtonTextAlignment, QString> m_button_text_alignments;

    void InitializeUi();

    QPushButton* m_more_button = nullptr;
};

#endif // LISTWIDGETITEMBUTTON_H
