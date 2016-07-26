#include "musiclrccontainer.h"
#include "musiclrcsearchwidget.h"
#include "musiclrcmakerwidget.h"
#include "musiclrcerrorwidget.h"

#include <QActionGroup>

MusicLrcContainer::MusicLrcContainer(QWidget *parent)
    : QWidget(parent)
{
    m_linkLocalLrc = true;
    m_musicLrcSearchWidget = nullptr;
    m_makerWidget = nullptr;
    m_currentTime = 0;
}

MusicLrcContainer::~MusicLrcContainer()
{
    delete m_musicLrcSearchWidget;
}

QString MusicLrcContainer::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcContainer::clearAllMusicLRCManager()
{
    while(!m_musicLrcContainer.isEmpty())
    {
        delete m_musicLrcContainer.takeLast();
    }
}

void MusicLrcContainer::currentLrcCustom()
{
    emit changeCurrentLrcColorCustom();
    emit changeCurrentLrcColorSetting();
}

void MusicLrcContainer::searchMusicLrcs()
{
    if(m_musicLrcSearchWidget == nullptr)
    {
        m_musicLrcSearchWidget = new MusicLrcSearchWidget(this);
    }
    m_musicLrcSearchWidget->setCurrentSongName(m_currentSongName);
    m_musicLrcSearchWidget->exec();
}

void MusicLrcContainer::createColorMenu(QMenu &menu)
{
    QActionGroup *group = new QActionGroup(this);
    group->addAction(menu.addAction(QIcon(":/color/lb_origin"), tr("origin")));
    group->addAction(menu.addAction(QIcon(":/color/lb_red"), tr("red")));
    group->addAction(menu.addAction(QIcon(":/color/lb_orange"), tr("orange")));
    group->addAction(menu.addAction(QIcon(":/color/lb_yellow"), tr("yellow")));
    group->addAction(menu.addAction(QIcon(":/color/lb_green"), tr("green")));
    group->addAction(menu.addAction(QIcon(":/color/lb_blue"), tr("blue")));
    group->addAction(menu.addAction(QIcon(":/color/lb_indigo"), tr("indigo")));
    group->addAction(menu.addAction(QIcon(":/color/lb_purple"), tr("purple")));
    group->addAction(menu.addAction(QIcon(":/color/lb_white"), tr("white")));
    group->addAction(menu.addAction(QIcon(":/color/lb_black"), tr("black")));
    connect(group, SIGNAL(triggered(QAction*)), SLOT(changeCurrentLrcColor(QAction*)));
    menu.addSeparator();
    menu.addAction(tr("custom"), this, SLOT(currentLrcCustom()));
}

void MusicLrcContainer::changeCurrentLrcColor(QAction *action)
{
    QString text = action->text();
    if(text == tr("origin")) setLinearGradientColor(MusicLRCManager::Origin);
    else if (text == tr("origin")) setLinearGradientColor(MusicLRCManager::Red);
    else if (text == tr("red")) setLinearGradientColor(MusicLRCManager::Orange);
    else if (text == tr("yellow")) setLinearGradientColor(MusicLRCManager::Yellow);
    else if (text == tr("green")) setLinearGradientColor(MusicLRCManager::Green);
    else if (text == tr("blue")) setLinearGradientColor(MusicLRCManager::Blue);
    else if (text == tr("indigo")) setLinearGradientColor(MusicLRCManager::Indigo);
    else if (text == tr("purple")) setLinearGradientColor(MusicLRCManager::Purple);
    else if (text == tr("white")) setLinearGradientColor(MusicLRCManager::White);
    else if (text == tr("black")) setLinearGradientColor(MusicLRCManager::Black);
}

void MusicLrcContainer::changeCurrentLrcColor(int index)
{
    setLinearGradientColor(MStatic_cast(MusicLRCManager::LrcColorType, index));
}

void MusicLrcContainer::setLinearGradientColor(MusicLRCManager::LrcColorType lrcColorType) const
{
    foreach(MusicLRCManager *manager, m_musicLrcContainer)
    {
        switch(lrcColorType)
        {
            case MusicLRCManager::Origin:
                manager->setLinearGradientColor(CL_Origin);break;
            case MusicLRCManager::Red:
                manager->setLinearGradientColor(CL_Red);break;
            case MusicLRCManager::Orange:
                manager->setLinearGradientColor(CL_Orange);break;
            case MusicLRCManager::Yellow:
                manager->setLinearGradientColor(CL_Yellow);break;
            case MusicLRCManager::Green:
                manager->setLinearGradientColor(CL_Green);break;
            case MusicLRCManager::Blue:
                manager->setLinearGradientColor(CL_Blue);break;
            case MusicLRCManager::Indigo:
                manager->setLinearGradientColor(CL_Indigo);break;
            case MusicLRCManager::Purple:
                manager->setLinearGradientColor(CL_Purple);break;
            case MusicLRCManager::White:
                manager->setLinearGradientColor(CL_White);break;
            case MusicLRCManager::Black:
                manager->setLinearGradientColor(CL_Black);break;
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
        setMaskLinearGradientColor();
    }
    else
    {
        foreach(MusicLRCManager *manager, m_musicLrcContainer)
        {
            manager->setLinearGradientColor(M_SETTING_PTR->value(t + "LrcBgColorChoiced").value<QColor>());
        }
        setMaskLinearGradientColor(M_SETTING_PTR->value(t + "LrcFgColorChoiced").value<QColor>());
    }
}

void MusicLrcContainer::theCurrentLrcMaked()
{
    if(m_makerWidget == nullptr)
    {
        m_makerWidget = new MusicLrcMakerWidget(this);
    }
    m_makerWidget->setCurrentSongName(m_currentSongName);
    m_makerWidget->show();
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
