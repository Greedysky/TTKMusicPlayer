#include "musicmorefunctionspopwidget.h"
#include "musicuiobject.h"

MusicMoreFunctionsPopWidget::MusicMoreFunctionsPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();
}

QString MusicMoreFunctionsPopWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicMoreFunctionsPopWidget::initWidget()
{
    m_menu->setWindowFlags(m_menu->windowFlags() | Qt::FramelessWindowHint);
    m_menu->setAttribute(Qt::WA_TranslucentBackground);
    m_menu->setStyleSheet(MusicUIObject::MMenuStyle03);

    m_containWidget->setFixedSize(140, 160);
    m_menu->removeAction(m_menu->actions().first());
    m_menu->addAction(QIcon(":/functions/btn_message_hover"), tr("message"));
    m_menu->addAction(QIcon(":/functions/btn_similar_hover"), tr("similar"));
    m_menu->addAction(QIcon(":/functions/btn_ablum_hover"), tr("ablum"));
    m_menu->addAction(QIcon(":/functions/btn_share_hover"), tr("share"));
    m_menu->addAction(QIcon(":/functions/btn_mv_hover"), tr("showMv"));
}
