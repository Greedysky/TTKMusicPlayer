#include "musiclrccontainer.h"
#include "musiclrcsearchwidget.h"
#include "musiclrcmakerwidget.h"
#include <QActionGroup>

MusicLrcContainer::MusicLrcContainer(QWidget *parent)
    : QWidget(parent)
{
    m_musicLrcSearchWidget = nullptr;
    m_makerWidget = nullptr;
}

MusicLrcContainer::~MusicLrcContainer()
{
    delete m_musicLrcSearchWidget;
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
    group->addAction(menu.addAction(QIcon(":/color/origin"), tr("origin")));
    group->addAction(menu.addAction(QIcon(":/color/red"), tr("red")));
    group->addAction(menu.addAction(QIcon(":/color/orange"), tr("orange")));
    group->addAction(menu.addAction(QIcon(":/color/yellow"), tr("yellow")));
    group->addAction(menu.addAction(QIcon(":/color/green"), tr("green")));
    group->addAction(menu.addAction(QIcon(":/color/blue"), tr("blue")));
    group->addAction(menu.addAction(QIcon(":/color/indigo"), tr("indigo")));
    group->addAction(menu.addAction(QIcon(":/color/purple"), tr("purple")));
    group->addAction(menu.addAction(QIcon(":/color/white"), tr("white")));
    group->addAction(menu.addAction(QIcon(":/color/black"), tr("black")));
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
    setLinearGradientColor(static_cast<MusicLRCManager::LrcColorType>(index));
}

void MusicLrcContainer::setLinearGradientColor(MusicLRCManager::LrcColorType lrcColorType) const
{
    for(int i=0; i<m_musicLrcContainer.count(); ++i)
    {
        switch(lrcColorType)
        {
            case MusicLRCManager::Origin:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Origin);break;
            case MusicLRCManager::Red:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Red);break;
            case MusicLRCManager::Orange:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Orange);break;
            case MusicLRCManager::Yellow:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Yellow);break;
            case MusicLRCManager::Green:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Green);break;
            case MusicLRCManager::Blue:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Blue);break;
            case MusicLRCManager::Indigo:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Indigo);break;
            case MusicLRCManager::Purple:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Purple);break;
            case MusicLRCManager::White:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_White);break;
            case MusicLRCManager::Black:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Black);break;
            default:break;
        }
    }

    M_SETTING->setValue( (m_containerType == "DESKTOP") ? MusicSettingManager::DLrcColorChoiced :
                                                          MusicSettingManager::LrcColorChoiced, lrcColorType);
}

void MusicLrcContainer::setSettingParameter()
{
    setSettingParameter(m_containerType == "DESKTOP" ? "D" : QString() );
}

void MusicLrcContainer::setSettingParameter(const QString &t) const
{
    for(int i=0; i<m_musicLrcContainer.count(); ++i)
    {
        m_musicLrcContainer[i]->setFontFamily(M_SETTING->value(t + "LrcFamilyChoiced").toInt());
        m_musicLrcContainer[i]->setFontType(M_SETTING->value(t + "LrcTypeChoiced").toInt());
        m_musicLrcContainer[i]->setFontTransparent(M_SETTING->value(t + "LrcColorTransChoiced").toInt());
        m_musicLrcContainer[i]->setLrcFontSize((MusicLRCManager::LrcSizeTable)(M_SETTING->value(t + "LrcSizeChoiced").toInt()));
    }
    if(M_SETTING->value(t + "LrcColorChoiced").toInt() != -1)
    {
        setLinearGradientColor((MusicLRCManager::LrcColorType)M_SETTING->value(t + "LrcColorChoiced").toInt());
        setMaskLinearGradientColor();
    }
    else
    {
        for(int i=0; i<m_musicLrcContainer.count(); ++i)
        {
            m_musicLrcContainer[i]->setLinearGradientColor(M_SETTING->value(t + "LrcBgColorChoiced").value<QColor>());
        }
        setMaskLinearGradientColor(M_SETTING->value(t + "LrcFgColorChoiced").value<QColor>());
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
