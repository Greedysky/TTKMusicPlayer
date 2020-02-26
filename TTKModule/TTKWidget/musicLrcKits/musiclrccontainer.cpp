#include "musiclrccontainer.h"
#include "musiclrcsearchwidget.h"
#include "musiclrcmakerwidget.h"
#include "musiclrcerrorwidget.h"
#include "musicstringutils.h"

#include <QActionGroup>

MusicLrcContainer::MusicLrcContainer(QWidget *parent)
    : QWidget(parent)
{
    m_currentTime = 0;
    m_totalTime = 0;
    m_linkLocalLrc = true;
    m_lrcAnalysis = nullptr;
    m_musicLrcSearchWidget = nullptr;
}

MusicLrcContainer::~MusicLrcContainer()
{
    delete m_musicLrcSearchWidget;
}

void MusicLrcContainer::setLinearGradientColor(MusicLrcColor::LrcColorType lrcColorType)
{
    const MusicLrcColor &cl = MusicLrcColor::mapIndexToColor(lrcColorType);
    setLinearGradientColor(cl);
}

void MusicLrcContainer::setLinearGradientColor(const MusicLrcColor &color)
{
    foreach(MusicLrcManager *manager, m_musicLrcContainer)
    {
        manager->setLinearGradientColor(color);
    }

    M_SETTING_PTR->setValue( (m_containerType == LRC_DESKTOP_TPYE) ? MusicSettingManager::DLrcColor : MusicSettingManager::LrcColor, color.m_index);
    Q_EMIT linearGradientColorChanged();
}

void MusicLrcContainer::applySettingParameter()
{
    applySettingParameter(m_containerType == LRC_DESKTOP_TPYE ? LRC_DESKTOP_PREFIX : QString());
}

void MusicLrcContainer::setCurrentTime(qint64 time, qint64 total)
{
    m_currentTime = time;
    m_totalTime = total;
}

qint64 MusicLrcContainer::getTotalTime() const
{
    return m_totalTime;
}

void MusicLrcContainer::currentLrcCustom()
{
    Q_EMIT changeCurrentLrcColorCustom();
    Q_EMIT changeCurrentLrcColorSetting();
}

void MusicLrcContainer::changeCurrentLrcColor(QAction *action)
{
    switch(action->data().toInt())
    {
        case 0:  setLinearGradientColor(MusicLrcColor::IYellow); break;
        case 1:  setLinearGradientColor(MusicLrcColor::IBlue); break;
        case 2:  setLinearGradientColor(MusicLrcColor::IGray); break;
        case 3:  setLinearGradientColor(MusicLrcColor::IPink); break;
        case 4:  setLinearGradientColor(MusicLrcColor::IGreen); break;
        case 5:  setLinearGradientColor(MusicLrcColor::IRed); break;
        case 6:  setLinearGradientColor(MusicLrcColor::IPurple); break;
        case 7:  setLinearGradientColor(MusicLrcColor::IOrange); break;
        case 8:  setLinearGradientColor(MusicLrcColor::IIndigo); break;
        case 9:  setLinearGradientColor(MusicLrcColor::DWhite); break;
        case 10: setLinearGradientColor(MusicLrcColor::DBlue); break;
        case 11: setLinearGradientColor(MusicLrcColor::DRed); break;
        case 12: setLinearGradientColor(MusicLrcColor::DBlack); break;
        case 13: setLinearGradientColor(MusicLrcColor::DYellow); break;
        case 14: setLinearGradientColor(MusicLrcColor::DPurple); break;
        case 15: setLinearGradientColor(MusicLrcColor::DGreen); break;
        default: break;
    }
}

void MusicLrcContainer::changeCurrentLrcColor(int index)
{
    setLinearGradientColor(TTKStatic_cast(MusicLrcColor::LrcColorType, index));
}

void MusicLrcContainer::searchMusicLrcs()
{
    delete m_musicLrcSearchWidget;
    m_musicLrcSearchWidget = new MusicLrcSearchWidget(this);
    m_musicLrcSearchWidget->setCurrentSongName(m_currentSongName);
    m_musicLrcSearchWidget->exec();
}

void MusicLrcContainer::showLrcMakedWidget()
{
    MusicLrcMakerWidget *w = new MusicLrcMakerWidget(this);
    w->setCurrentSongName(m_currentSongName);
    w->durationChanged(m_totalTime);
    w->show();
}

void MusicLrcContainer::linkLrcStateChanged()
{
    m_linkLocalLrc = !m_linkLocalLrc;
    foreach(MusicLrcManager *w, m_musicLrcContainer)
    {
        w->setVisible( m_linkLocalLrc );
    }
}

void MusicLrcContainer::showLrcErrorWidget()
{
    MusicLrcErrorWidget(this).exec();
}

void MusicLrcContainer::clearAllMusicLRCManager()
{
    qDeleteAll(m_musicLrcContainer);
    m_musicLrcContainer.clear();
}

void MusicLrcContainer::applySettingParameter(const QString &t)
{
    foreach(MusicLrcManager *manager, m_musicLrcContainer)
    {
        manager->setFontFamily(M_SETTING_PTR->value(t + "LrcFamily").toInt());
        manager->setFontType(M_SETTING_PTR->value(t + "LrcType").toInt());
        manager->setFontTransparent(M_SETTING_PTR->value(t + "LrcColorTrans").toInt());
        manager->setLrcFontSize(M_SETTING_PTR->value(t + "LrcSize").toInt());
    }
    if(M_SETTING_PTR->value(t + "LrcColor").toInt() != -1)
    {
        const MusicLrcColor::LrcColorType index = TTKStatic_cast(MusicLrcColor::LrcColorType, M_SETTING_PTR->value(t + "LrcColor").toInt());
        setLinearGradientColor(index);
    }
    else
    {
        const MusicLrcColor cl(MusicUtils::String::readColorConfig(M_SETTING_PTR->value(t + "LrcFrontgroundColor").toString()),
                               MusicUtils::String::readColorConfig(M_SETTING_PTR->value(t + "LrcBackgroundColor").toString()));
        setLinearGradientColor(cl);
    }
}
