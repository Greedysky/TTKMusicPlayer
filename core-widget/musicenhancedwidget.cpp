#include "musicenhancedwidget.h"
#include "musicuiobject.h"

#include <QMenu>
#include <QWidgetAction>

MusicEnhancedWidget::MusicEnhancedWidget(QWidget *parent)
    : QToolButton(parent)
{
    initWidget();
    setStyleSheet("background:black");
}

void MusicEnhancedWidget::initWidget()
{
    m_menu = new QMenu(this);
    m_menu->setStyleSheet(MusicUIObject::MMenuStyle02);
    QWidgetAction *actionWidget = new QWidgetAction(m_menu);

    QWidget *containWidget = new QWidget(m_menu);
    containWidget->resize(200, 200);
    actionWidget->setDefaultWidget(containWidget);
    m_menu->addAction(actionWidget);
    setMenu(m_menu);
    setPopupMode(QToolButton::InstantPopup);
}
