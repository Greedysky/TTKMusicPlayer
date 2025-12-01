#include "musicplaybackmodepopwidget.h"
#include "musicfunctionuiobject.h"
#include "musicapplication.h"

MusicPlayBackModePopWidget::MusicPlayBackModePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();
}

void MusicPlayBackModePopWidget::setPlaybackMode(TTK::PlayMode mode)
{
    switch(mode)
    {
        case TTK::PlayMode::Order:
        {
            setStyleSheet(TTK::UI::BtnOrder);
            setToolTip(tr("Order Play"));
            break;
        }
        case TTK::PlayMode::Random:
        {
            setStyleSheet(TTK::UI::BtnRandom);
            setToolTip(tr("Random Play"));
            break;
        }
        case TTK::PlayMode::ListLoop:
        {
            setStyleSheet(TTK::UI::BtnListLoop);
            setToolTip(tr("List Cycle"));
            break;
        }
        case TTK::PlayMode::OneLoop:
        {
            setStyleSheet(TTK::UI::BtnOneLoop);
            setToolTip(tr("Single Cycle"));
            break;
        }
        case TTK::PlayMode::Once:
        {
            setStyleSheet(TTK::UI::BtnOnce);
            setToolTip(tr("Play Once"));
            break;
        }
        default: break;
    }
}

void MusicPlayBackModePopWidget::initialize()
{
    setTranslucentBackground();

    m_containWidget->setFixedSize(110, 160);
    m_menu->removeAction(m_menu->actions().first());
    m_menu->addAction(QIcon(":/functions/btn_once_hover"), tr("Play Once"), MusicApplication::instance(), SLOT(playOnce()));
    m_menu->addAction(QIcon(":/functions/btn_one_loop_hover"), tr("Single Cycle"), MusicApplication::instance(), SLOT(playOneLoop()));
    m_menu->addAction(QIcon(":/functions/btn_order_hover"), tr("Order Play"), MusicApplication::instance(), SLOT(playOrder()));
    m_menu->addAction(QIcon(":/functions/btn_list_loop_hover"), tr("List Cycle"), MusicApplication::instance(), SLOT(playlistLoop()));
    m_menu->addAction(QIcon(":/functions/btn_random_hover"), tr("Random Play"), MusicApplication::instance(), SLOT(playRandom()));
}
