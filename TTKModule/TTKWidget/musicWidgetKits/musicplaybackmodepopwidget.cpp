#include "musicplaybackmodepopwidget.h"
#include "musicfunctionuiobject.h"
#include "musicapplication.h"
#include "musicuiobject.h"

MusicPlayBackModePopWidget::MusicPlayBackModePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();
}

void MusicPlayBackModePopWidget::setPlaybackMode(MusicObject::PlayMode mode)
{
    switch(mode)
    {
        case MusicObject::PM_PlayOrder:
                setStyleSheet(MusicUIObject::MQSSBtnOrder);
                setToolTip(tr("Order Play"));
                break;
        case MusicObject::PM_PlayRandom:
                setStyleSheet(MusicUIObject::MQSSBtnRandom);
                setToolTip(tr("Random Play"));
                break;
        case MusicObject::PM_PlaylistLoop:
                setStyleSheet(MusicUIObject::MQSSBtnListLoop);
                setToolTip(tr("List Cycle"));
                break;
        case MusicObject::PM_PlayOneLoop:
                setStyleSheet(MusicUIObject::MQSSBtnOneLoop);
                setToolTip(tr("Single Cycle"));
                break;
        case MusicObject::PM_PlayOnce:
                setStyleSheet(MusicUIObject::MQSSBtnOnce);
                setToolTip(tr("Play Once"));
                break;
        default:
                break;
    }
}

void MusicPlayBackModePopWidget::initWidget()
{
    setTranslucentBackground();
    m_containWidget->setFixedSize(140, 160);
    m_menu->removeAction(m_menu->actions().first());
    m_menu->addAction(QIcon(":/functions/btn_once_hover"), tr("Play Once"), MusicApplication::instance(), SLOT(musicPlayItemOnce()));
    m_menu->addAction(QIcon(":/functions/btn_oneLoop_hover"), tr("Single Cycle"), MusicApplication::instance(), SLOT(musicPlayOneLoop()));
    m_menu->addAction(QIcon(":/functions/btn_order_hover"), tr("Order Play"), MusicApplication::instance(), SLOT(musicPlayOrder()));
    m_menu->addAction(QIcon(":/functions/btn_listLoop_hover"), tr("List Cycle"), MusicApplication::instance(), SLOT(musicPlaylistLoop()));
    m_menu->addAction(QIcon(":/functions/btn_random_hover"), tr("Random Play"), MusicApplication::instance(), SLOT(musicPlayRandom()));
}
