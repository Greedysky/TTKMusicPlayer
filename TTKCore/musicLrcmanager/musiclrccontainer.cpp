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

void MusicLrcContainer::setLinearGradientColor(MusicLRCManager::LrcColorType lrcColorType) const
{
    foreach(MusicLRCManager *manager, m_musicLrcContainer)
    {
        switch(lrcColorType)
        {
            case MusicLRCManager::Origin:
                manager->setLinearGradientColor(QList<QColor>() << CL_Origin << CL_White << CL_Origin);break;
            case MusicLRCManager::Red:
                manager->setLinearGradientColor(QList<QColor>() << CL_Red << CL_White << CL_Red);break;
            case MusicLRCManager::Orange:
                manager->setLinearGradientColor(QList<QColor>() << CL_Orange << CL_White << CL_Orange);break;
            case MusicLRCManager::Yellow:
                manager->setLinearGradientColor(QList<QColor>() << CL_Yellow << CL_White << CL_Yellow);break;
            case MusicLRCManager::Green:
                manager->setLinearGradientColor(QList<QColor>() << CL_Green << CL_White << CL_Green);break;
            case MusicLRCManager::Blue:
                manager->setLinearGradientColor(QList<QColor>() << CL_Blue << CL_White << CL_Blue);break;
            case MusicLRCManager::Indigo:
                manager->setLinearGradientColor(QList<QColor>() << CL_Indigo << CL_White << CL_Indigo);break;
            case MusicLRCManager::Purple:
                manager->setLinearGradientColor(QList<QColor>() << CL_Purple << CL_White << CL_Purple);break;
            case MusicLRCManager::White:
                manager->setLinearGradientColor(QList<QColor>() << CL_White << CL_White << CL_White);break;
            case MusicLRCManager::Black:
                manager->setLinearGradientColor(QList<QColor>() << CL_Black << CL_White << CL_Black);break;
            default:break;
        }
    }
    M_SETTING_PTR->setValue( (m_containerType == "DESKTOP") ? MusicSettingManager::DLrcColorChoiced :
                                                              MusicSettingManager::LrcColorChoiced, lrcColorType);
}

void MusicLrcContainer::setSettingParameter()
{
    setSettingParameter(m_containerType == "DESKTOP" ? "D" : QString() );
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
        case 0: setLinearGradientColor(MusicLRCManager::Origin); break;
        case 1: setLinearGradientColor(MusicLRCManager::Red); break;
        case 2: setLinearGradientColor(MusicLRCManager::Orange); break;
        case 3: setLinearGradientColor(MusicLRCManager::Yellow); break;
        case 4: setLinearGradientColor(MusicLRCManager::Green); break;
        case 5: setLinearGradientColor(MusicLRCManager::Blue); break;
        case 6: setLinearGradientColor(MusicLRCManager::Indigo); break;
        case 7: setLinearGradientColor(MusicLRCManager::Purple); break;
        case 8: setLinearGradientColor(MusicLRCManager::White); break;
        case 9: setLinearGradientColor(MusicLRCManager::Black); break;
        default: break;
    }
}

void MusicLrcContainer::changeCurrentLrcColor(int index)
{
    setLinearGradientColor(MStatic_cast(MusicLRCManager::LrcColorType, index));
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

void MusicLrcContainer::createColorMenu(QMenu &menu)
{
    QActionGroup *group = new QActionGroup(this);
    group->addAction(menu.addAction(QIcon(":/color/lb_origin"), tr("origin")))->setData(0);
    group->addAction(menu.addAction(QIcon(":/color/lb_red"), tr("red")))->setData(1);
    group->addAction(menu.addAction(QIcon(":/color/lb_orange"), tr("orange")))->setData(2);
    group->addAction(menu.addAction(QIcon(":/color/lb_yellow"), tr("yellow")))->setData(3);
    group->addAction(menu.addAction(QIcon(":/color/lb_green"), tr("green")))->setData(4);
    group->addAction(menu.addAction(QIcon(":/color/lb_blue"), tr("blue")))->setData(5);
    group->addAction(menu.addAction(QIcon(":/color/lb_indigo"), tr("indigo")))->setData(6);
    group->addAction(menu.addAction(QIcon(":/color/lb_purple"), tr("purple")))->setData(7);
    group->addAction(menu.addAction(QIcon(":/color/lb_white"), tr("white")))->setData(8);
    group->addAction(menu.addAction(QIcon(":/color/lb_black"), tr("black")))->setData(9);
    connect(group, SIGNAL(triggered(QAction*)), SLOT(changeCurrentLrcColor(QAction*)));
    menu.addSeparator();
    menu.addAction(tr("custom"), this, SLOT(currentLrcCustom()));
}

void MusicLrcContainer::clearAllMusicLRCManager()
{
    while(!m_musicLrcContainer.isEmpty())
    {
        delete m_musicLrcContainer.takeLast();
    }
}

void MusicLrcContainer::setSettingParameter(const QString &t) const
{
    foreach(MusicLRCManager *manager, m_musicLrcContainer)
    {
        manager->setFontFamily(M_SETTING_PTR->value(t + "LrcFamilyChoiced").toInt());
        manager->setFontType(M_SETTING_PTR->value(t + "LrcTypeChoiced").toInt());
        manager->setFontTransparent(M_SETTING_PTR->value(t + "LrcColorTransChoiced").toInt());
        manager->setLrcFontSize((MusicLRCManager::LrcSizeTable)(M_SETTING_PTR->value(t + "LrcSizeChoiced").toInt()));
    }
    if(M_SETTING_PTR->value(t + "LrcColorChoiced").toInt() != -1)
    {
        setLinearGradientColor((MusicLRCManager::LrcColorType)M_SETTING_PTR->value(t + "LrcColorChoiced").toInt());
        setMaskLinearGradientColor( QList<QColor>() << CL_Mask << CL_White << CL_Mask );
    }
    else
    {
        foreach(MusicLRCManager *manager, m_musicLrcContainer)
        {
            manager->setLinearGradientColor(MusicUtils::String::readColorConfig(M_SETTING_PTR->value(t + "LrcBgColorChoiced").toString()));
        }
        setMaskLinearGradientColor(MusicUtils::String::readColorConfig(M_SETTING_PTR->value(t + "LrcFgColorChoiced").toString()));
    }
}
