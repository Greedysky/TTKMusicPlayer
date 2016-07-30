#include "musicplaybackmodewidget.h"
#include "musicttkuiobject.h"
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
    switch( mode )
    {
        case MusicObject::MC_PlayOrder:
                setStyleSheet(MusicTTKUIObject::MKGBtnOrder);
                setToolTip(tr("OrderPlay"));
                break;
        case MusicObject::MC_PlayRandom:
                setStyleSheet(MusicTTKUIObject::MKGBtnRandom);
                setToolTip(tr("RandomPlay"));
                break;
        case MusicObject::MC_PlayListLoop:
                setStyleSheet(MusicTTKUIObject::MKGBtnListLoop);
                setToolTip(tr("ListCycle"));
                break;
        case MusicObject::MC_PlayOneLoop:
                setStyleSheet(MusicTTKUIObject::MKGBtnOneLoop);
                setToolTip(tr("SingleCycle"));
                break;
        case MusicObject::MC_PlayOnce:
                setStyleSheet(MusicTTKUIObject::MKGBtnOnce);
                setToolTip(tr("PlayOnce"));
                break;
        default:
                break;
    }
}

void MusicPlayBackModeWidget::initWidget(QWidget *parent)
{
    m_menu->setWindowFlags(m_menu->windowFlags() | Qt::FramelessWindowHint);
    m_menu->setAttribute(Qt::WA_TranslucentBackground);
    m_menu->setStyleSheet(MusicUIObject::MMenuStyle03);

    m_containWidget->setFixedSize(140, 160);
    m_menu->removeAction(m_menu->actions().first());
    m_menu->addAction(QIcon(":/functions/btn_once_hover"), tr("PlayOnce"), parent, SLOT(musicPlayItemOnce()));
    m_menu->addAction(QIcon(":/functions/btn_oneLoop_hover"), tr("SingleCycle"), parent, SLOT(musicPlayOneLoop()));
    m_menu->addAction(QIcon(":/functions/btn_order_hover"), tr("OrderPlay"), parent, SLOT(musicPlayOrder()));
    m_menu->addAction(QIcon(":/functions/btn_listLoop_hover"), tr("ListCycle"), parent, SLOT(musicPlayListLoop()));
    m_menu->addAction(QIcon(":/functions/btn_random_hover"), tr("RandomPlay"), parent, SLOT(musicPlayRandom()));
}
