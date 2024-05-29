#include "musictoolmenuwidget.h"

#include <QWidgetAction>

MusicToolMenu::MusicToolMenu(QWidget *parent)
    : QMenu(parent)
{

}

void MusicToolMenu::showEvent(QShowEvent *event)
{
    QMenu::showEvent(event);
    Q_EMIT windowStateChanged(true);
}

void MusicToolMenu::hideEvent(QHideEvent *event)
{
    QMenu::hideEvent(event);
    Q_EMIT windowStateChanged(false);
}



MusicToolMenuWidget::MusicToolMenuWidget(QWidget *parent)
    : QToolButton(parent)
{
    setCursor(Qt::PointingHandCursor);

    m_menu = new MusicToolMenu(this);
    QWidgetAction *actionWidget = new QWidgetAction(m_menu);
    m_containWidget = new QWidget(m_menu);

    actionWidget->setDefaultWidget(m_containWidget);
    m_menu->addAction(actionWidget);

    connect(this, SIGNAL(clicked()), SLOT(popupMenu()));
}

MusicToolMenuWidget::~MusicToolMenuWidget()
{
    delete m_containWidget;
    delete m_menu;
}

void MusicToolMenuWidget::setTranslucentBackground()
{
    m_menu->setWindowFlags(m_menu->windowFlags() | Qt::FramelessWindowHint);
    m_menu->setAttribute(Qt::WA_TranslucentBackground);
    m_menu->setStyleSheet(TTK::UI::MenuStyle04);
}

void MusicToolMenuWidget::popupMenu()
{
    QPoint pos = mapToGlobal(QPoint(0, 0));
    pos.setY(pos.y() - m_containWidget->height() - 10);
    pos.setX(pos.x() - (m_containWidget->width() - width()) / 2);

    m_menu->exec(pos);
}
