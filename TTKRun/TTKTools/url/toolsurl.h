#ifndef TOOLSURL_H
#define TOOLSURL_H

#include <QWidget>

namespace Ui {
class ToolsUrl;
}

class ToolsUrl : public QWidget
{
    Q_OBJECT
public:
    explicit ToolsUrl(QWidget *parent = nullptr);
    ~ToolsUrl();

private Q_SLOTS:
    void encodeClicked();
    void decodeClicked();

protected:
    Ui::ToolsUrl *m_ui;

};

#endif // TOOLSURL_H
