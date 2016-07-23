#include "musicplaybackmodewidget.h"
#include "musickugouuiobject.h"
#include "musicuiobject.h"

MusicPlayBackModeWidget::MusicPlayBackModeWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{

}

QString MusicPlayBackModeWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicPlayBackModeWidget::setPlaybackMode(MusicObject::SongPlayType mode)
{
    QString style;
    switch( mode )
    {
        case MusicObject::MC_PlayOrder:
            style = MusicKuGouUIObject::MKGBtnOrder; break;
        case MusicObject::MC_PlayRandom:
            style = MusicKuGouUIObject::MKGBtnRandom; break;
        case MusicObject::MC_PlayListLoop:
            style = MusicKuGouUIObject::MKGBtnListLoop; break;
        case MusicObject::MC_PlayOneLoop:
            style = MusicKuGouUIObject::MKGBtnOneLoop; break;
        case MusicObject::MC_PlayOnce:
            style = MusicKuGouUIObject::MKGBtnOnce; break;
        default:break;
    }
    setStyleSheet(style);
}

void MusicPlayBackModeWidget::initWidget(QWidget *parent)
{
    m_menu->setWindowFlags(m_menu->windowFlags() | Qt::FramelessWindowHint);
    m_menu->setAttribute(Qt::WA_TranslucentBackground);
    QString MMenuStyle = " \
            QMenu {color: rgb(150,150,150); border:none; padding: 5px; background:rgba(0,0,0,240);}\
            QMenu::item {padding: 4px 25px 4px 30px;border: 1px solid transparent; }\
            QMenu::item:disabled {color: #666666;}\
            QMenu::item:selected { color: white; background:rgba(0,0,0,200);}\
            QMenu::separator {height: 1px;background: #BBBBBB;margin-top: 5px; margin-bottom: 5px;}";

    m_menu->setStyleSheet(MMenuStyle);

    m_containWidget->setFixedSize(120, 140);
    m_menu->removeAction(m_menu->actions().first());

    m_menu->addAction(QIcon(":/functions/btn_once_hover"), tr("once"), parent, SLOT(musicPlayItemOnce()));
    m_menu->addAction(QIcon(":/functions/btn_oneLoop_hover"), tr("once"), parent, SLOT(musicPlayOneLoop()));
    m_menu->addAction(QIcon(":/functions/btn_order_hover"), tr("once"), parent, SLOT(musicPlayOrder()));
    m_menu->addAction(QIcon(":/functions/btn_listLoop_hover"), tr("once"), parent, SLOT(musicPlayListLoop()));
    m_menu->addAction(QIcon(":/functions/btn_random_hover"), tr("random"), parent, SLOT(musicPlayRandom()));
}
