#include "musictoolmenuwidget.h"

#include <QWidgetAction>

MusicToolMenuWidget::MusicToolMenuWidget(QWidget *parent)
    : QToolButton(parent)
{
    setCursor(Qt::PointingHandCursor);

    m_menu = new QMenu(this);
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

QString MusicToolMenuWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicToolMenuWidget::popupMenu()
{
    QPoint pos = mapToGlobal(QPoint(0, 0));
    pos.setY(pos.y() - m_containWidget->height() - 10);
    pos.setX(pos.x() - (m_containWidget->width() - width())/2);

    m_menu->exec(pos);
}
