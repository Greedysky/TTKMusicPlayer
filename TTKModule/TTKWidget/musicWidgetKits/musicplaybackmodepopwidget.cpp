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
    switch( mode )
    {
        case MusicObject::PM_PlayOrder:
                setStyleSheet(MusicUIObject::MKGBtnOrder);
                setToolTip(tr("OrderPlay"));
                break;
        case MusicObject::PM_PlayRandom:
                setStyleSheet(MusicUIObject::MKGBtnRandom);
                setToolTip(tr("RandomPlay"));
                break;
        case MusicObject::PM_PlaylistLoop:
                setStyleSheet(MusicUIObject::MKGBtnListLoop);
                setToolTip(tr("ListCycle"));
                break;
        case MusicObject::PM_PlayOneLoop:
                setStyleSheet(MusicUIObject::MKGBtnOneLoop);
                setToolTip(tr("SingleCycle"));
                break;
        case MusicObject::PM_PlayOnce:
                setStyleSheet(MusicUIObject::MKGBtnOnce);
                setToolTip(tr("PlayOnce"));
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
    m_menu->addAction(QIcon(":/functions/btn_once_hover"), tr("PlayOnce"), MusicApplication::instance(), SLOT(musicPlayItemOnce()));
    m_menu->addAction(QIcon(":/functions/btn_oneLoop_hover"), tr("SingleCycle"), MusicApplication::instance(), SLOT(musicPlayOneLoop()));
    m_menu->addAction(QIcon(":/functions/btn_order_hover"), tr("OrderPlay"), MusicApplication::instance(), SLOT(musicPlayOrder()));
    m_menu->addAction(QIcon(":/functions/btn_listLoop_hover"), tr("ListCycle"), MusicApplication::instance(), SLOT(musicPlaylistLoop()));
    m_menu->addAction(QIcon(":/functions/btn_random_hover"), tr("RandomPlay"), MusicApplication::instance(), SLOT(musicPlayRandom()));
}
