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

QString MusicLrcContainer::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcContainer::setLinearGradientColor(MusicLRCColor::LrcColorType lrcColorType)
{
    MusicLRCColor cl = MusicLRCManager::mapIndexToColor(lrcColorType);
    setLinearGradientColor(cl);
}

void MusicLrcContainer::setLinearGradientColor(const MusicLRCColor &color)
{
    foreach(MusicLRCManager *manager, m_musicLrcContainer)
    {
        manager->setLinearGradientColor(color);
    }

    M_SETTING_PTR->setValue( (m_containerType == "DESKTOP") ? MusicSettingManager::DLrcColorChoiced :
                                                              MusicSettingManager::LrcColorChoiced, color.m_index);
    emit linearGradientColorChanged();
}

void MusicLrcContainer::setSettingParameter()
{
    setSettingParameter(m_containerType == "DESKTOP" ? "D" : QString());
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
    emit changeCurrentLrcColorCustom();
    emit changeCurrentLrcColorSetting();
}

void MusicLrcContainer::changeCurrentLrcColor(QAction *action)
{
    switch(action->data().toInt())
    {
        case 0:  setLinearGradientColor(MusicLRCColor::IYellow); break;
        case 1:  setLinearGradientColor(MusicLRCColor::IBlue); break;
        case 2:  setLinearGradientColor(MusicLRCColor::IGray); break;
        case 3:  setLinearGradientColor(MusicLRCColor::IPink); break;
        case 4:  setLinearGradientColor(MusicLRCColor::IGreen); break;
        case 5:  setLinearGradientColor(MusicLRCColor::IRed); break;
        case 6:  setLinearGradientColor(MusicLRCColor::IPurple); break;
        case 7:  setLinearGradientColor(MusicLRCColor::IOrange); break;
        case 8:  setLinearGradientColor(MusicLRCColor::IIndigo); break;
        case 9:  setLinearGradientColor(MusicLRCColor::DWhite); break;
        case 10: setLinearGradientColor(MusicLRCColor::DBlue); break;
        case 11: setLinearGradientColor(MusicLRCColor::DRed); break;
        case 12: setLinearGradientColor(MusicLRCColor::DBlack); break;
        case 13: setLinearGradientColor(MusicLRCColor::DYellow); break;
        case 14: setLinearGradientColor(MusicLRCColor::DPurple); break;
        case 15: setLinearGradientColor(MusicLRCColor::DGreen); break;
        default: break;
    }
}

void MusicLrcContainer::changeCurrentLrcColor(int index)
{
    setLinearGradientColor(MStatic_cast(MusicLRCColor::LrcColorType, index));
}

void MusicLrcContainer::searchMusicLrcs()
{
    delete m_musicLrcSearchWidget;
    m_musicLrcSearchWidget = new MusicLrcSearchWidget(this);
    m_musicLrcSearchWidget->setCurrentSongName(m_currentSongName);
    m_musicLrcSearchWidget->exec();
}

void MusicLrcContainer::theCurrentLrcMaked()
{
    MusicLrcMakerWidget *w = new MusicLrcMakerWidget(this);
    w->setCurrentSongName(m_currentSongName);
    w->durationChanged(m_totalTime);
    w->show();
}

void MusicLrcContainer::theLinkLrcChanged()
{
    m_linkLocalLrc = !m_linkLocalLrc;
    foreach(MusicLRCManager *w, m_musicLrcContainer)
    {
        w->setVisible( m_linkLocalLrc );
    }
}

void MusicLrcContainer::theCurrentLrcError()
{
    MusicLrcErrorWidget(this).exec();
}

void MusicLrcContainer::clearAllMusicLRCManager()
{
    while(!m_musicLrcContainer.isEmpty())
    {
        delete m_musicLrcContainer.takeLast();
    }
}

void MusicLrcContainer::setSettingParameter(const QString &t)
{
    foreach(MusicLRCManager *manager, m_musicLrcContainer)
    {
        manager->setFontFamily(M_SETTING_PTR->value(t + "LrcFamilyChoiced").toInt());
        manager->setFontType(M_SETTING_PTR->value(t + "LrcTypeChoiced").toInt());
        manager->setFontTransparent(M_SETTING_PTR->value(t + "LrcColorTransChoiced").toInt());
        manager->setLrcFontSize(M_SETTING_PTR->value(t + "LrcSizeChoiced").toInt());
    }
    if(M_SETTING_PTR->value(t + "LrcColorChoiced").toInt() != -1)
    {
        MusicLRCColor::LrcColorType index = MStatic_cast(MusicLRCColor::LrcColorType, M_SETTING_PTR->value(t + "LrcColorChoiced").toInt());
        setLinearGradientColor(index);
    }
    else
    {
        MusicLRCColor cl(MusicUtils::String::readColorConfig(M_SETTING_PTR->value(t + "LrcFgColorChoiced").toString()),
                         MusicUtils::String::readColorConfig(M_SETTING_PTR->value(t + "LrcBgColorChoiced").toString()));
        setLinearGradientColor(cl);
    }
}
