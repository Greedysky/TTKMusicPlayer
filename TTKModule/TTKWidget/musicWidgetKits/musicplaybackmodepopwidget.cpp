#include "musicplaybackmodepopwidget.h"
#include "musicfunctionuiobject.h"
#include "musicapplication.h"

MusicPlayBackModePopWidget::MusicPlayBackModePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();
}

void MusicPlayBackModePopWidget::setPlaybackMode(MusicObject::PlayMode mode)
{
    switch(mode)
    {
        case MusicObject::PlayMode::Order:
        {
            setStyleSheet(MusicUIObject::BtnOrder);
            setToolTip(tr("Order Play"));
            break;
        }
        case MusicObject::PlayMode::Random:
        {
            setStyleSheet(MusicUIObject::BtnRandom);
            setToolTip(tr("Random Play"));
            break;
        }
        case MusicObject::PlayMode::ListLoop:
        {
            setStyleSheet(MusicUIObject::BtnListLoop);
            setToolTip(tr("List Cycle"));
            break;
        }
        case MusicObject::PlayMode::OneLoop:
        {
            setStyleSheet(MusicUIObject::BtnOneLoop);
            setToolTip(tr("Single Cycle"));
            break;
        }
        case MusicObject::PlayMode::Once:
        {
            setStyleSheet(MusicUIObject::BtnOnce);
            setToolTip(tr("Play Once"));
            break;
        }
        default: break;
    }
}

void MusicPlayBackModePopWidget::initialize()
{
    setTranslucentBackground();
    m_containWidget->setFixedSize(140, 160);
    m_menu->removeAction(m_menu->actions().front());
    m_menu->addAction(QIcon(":/functions/btn_once_hover"), tr("Play Once"), MusicApplication::instance(), SLOT(musicPlayOnce()));
    m_menu->addAction(QIcon(":/functions/btn_one_loop_hover"), tr("Single Cycle"), MusicApplication::instance(), SLOT(musicPlayOneLoop()));
    m_menu->addAction(QIcon(":/functions/btn_order_hover"), tr("Order Play"), MusicApplication::instance(), SLOT(musicPlayOrder()));
    m_menu->addAction(QIcon(":/functions/btn_list_loop_hover"), tr("List Cycle"), MusicApplication::instance(), SLOT(musicPlaylistLoop()));
    m_menu->addAction(QIcon(":/functions/btn_random_hover"), tr("Random Play"), MusicApplication::instance(), SLOT(musicPlayRandom()));
}
