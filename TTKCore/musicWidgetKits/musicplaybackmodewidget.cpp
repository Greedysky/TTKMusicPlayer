#include "musicplaybackmodewidget.h"
#include "musicfunctionuiobject.h"
#include "musicapplication.h"
#include "musicuiobject.h"

MusicPlayBackModeWidget::MusicPlayBackModeWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();
}

QString MusicPlayBackModeWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicPlayBackModeWidget::setPlaybackMode(MusicObject::SongPlayMode mode)
{
    switch( mode )
    {
        case MusicObject::MC_PlayOrder:
                setStyleSheet(MusicUIObject::MKGBtnOrder);
                setToolTip(tr("OrderPlay"));
                break;
        case MusicObject::MC_PlayRandom:
                setStyleSheet(MusicUIObject::MKGBtnRandom);
                setToolTip(tr("RandomPlay"));
                break;
        case MusicObject::MC_PlayListLoop:
                setStyleSheet(MusicUIObject::MKGBtnListLoop);
                setToolTip(tr("ListCycle"));
                break;
        case MusicObject::MC_PlayOneLoop:
                setStyleSheet(MusicUIObject::MKGBtnOneLoop);
                setToolTip(tr("SingleCycle"));
                break;
        case MusicObject::MC_PlayOnce:
                setStyleSheet(MusicUIObject::MKGBtnOnce);
                setToolTip(tr("PlayOnce"));
                break;
        default:
                break;
    }
}

void MusicPlayBackModeWidget::initWidget()
{
    setTranslucentBackground();
    m_containWidget->setFixedSize(140, 160);
    m_menu->removeAction(m_menu->actions().first());
    m_menu->addAction(QIcon(":/functions/btn_once_hover"), tr("PlayOnce"), MusicApplication::instance(), SLOT(musicPlayItemOnce()));
    m_menu->addAction(QIcon(":/functions/btn_oneLoop_hover"), tr("SingleCycle"), MusicApplication::instance(), SLOT(musicPlayOneLoop()));
    m_menu->addAction(QIcon(":/functions/btn_order_hover"), tr("OrderPlay"), MusicApplication::instance(), SLOT(musicPlayOrder()));
    m_menu->addAction(QIcon(":/functions/btn_listLoop_hover"), tr("ListCycle"), MusicApplication::instance(), SLOT(musicPlayListLoop()));
    m_menu->addAction(QIcon(":/functions/btn_random_hover"), tr("RandomPlay"), MusicApplication::instance(), SLOT(musicPlayRandom()));
}
