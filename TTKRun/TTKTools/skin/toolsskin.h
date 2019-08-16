#ifndef TOOLSSKIN_H
#define TOOLSSKIN_H

#include <QWidget>

namespace Ui {
class ToolsSkin;
}

class ToolsSkin : public QWidget
{
    Q_OBJECT
public:
    explicit ToolsSkin(QWidget *parent = nullptr);
    ~ToolsSkin();

private Q_SLOTS:
    void imageClicked();
    void paletteClicked();
    void readClicked();
    void writeClicked();

protected:
    Ui::ToolsSkin *m_ui;
    QPixmap m_pixmap;

};

#endif // TOOLSSKIN_H
