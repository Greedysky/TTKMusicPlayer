#include "musiclrccontainer.h"
#include "musiclrcsearchwidget.h"
#include "musiclrcmakerwidget.h"
#include <QActionGroup>

MusicLrcContainer::MusicLrcContainer(QWidget *parent) :
    QWidget(parent)
{
    m_musicLrcSearchWidget = NULL;
    m_makerWidget = NULL;
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
    if(m_musicLrcSearchWidget == NULL)
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
    if(text == tr("origin")) setLinearGradientColor(Origin);
    else if (text == tr("origin")) setLinearGradientColor(Red);
    else if (text == tr("red")) setLinearGradientColor(Orange);
    else if (text == tr("yellow")) setLinearGradientColor(Yellow);
    else if (text == tr("green")) setLinearGradientColor(Green);
    else if (text == tr("blue")) setLinearGradientColor(Blue);
    else if (text == tr("indigo")) setLinearGradientColor(Indigo);
    else if (text == tr("purple")) setLinearGradientColor(Purple);
    else if (text == tr("white")) setLinearGradientColor(White);
    else if (text == tr("black")) setLinearGradientColor(Black);
}

void MusicLrcContainer::changeCurrentLrcColor(int index)
{
    setLinearGradientColor(static_cast<LrcColorType>(index));
}

void MusicLrcContainer::setLinearGradientColor(LrcColorType lrcColorType) const
{
    for(int i=0; i<m_musicLrcContainer.count(); ++i)
    {
        switch(lrcColorType)
        {
            case Origin:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Origin);break;
            case Red:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Red);break;
            case Orange:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Orange);break;
            case Yellow:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Yellow);break;
            case Green:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Green);break;
            case Blue:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Blue);break;
            case Indigo:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Indigo);break;
            case Purple:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Purple);break;
            case White:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_White);break;
            case Black:
                m_musicLrcContainer[i]->setLinearGradientColor(CL_Black);break;
            default:break;
        }
    }
    if(m_containerType == "DESKTOP")
        M_SETTING->setValue(MusicSettingManager::DLrcColorChoiced, lrcColorType);
    else
        M_SETTING->setValue(MusicSettingManager::LrcColorChoiced, lrcColorType);
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
        m_musicLrcContainer[i]->setLrcFontSize((LrcSizeTable)(M_SETTING->value(t + "LrcSizeChoiced").toInt()));
    }
    if(M_SETTING->value(t + "LrcColorChoiced").toInt() != -1)
    {
        setLinearGradientColor((LrcColorType)M_SETTING->value(t + "LrcColorChoiced").toInt());
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
    if(m_makerWidget == NULL)
    {
        m_makerWidget = new MusicLrcMakerWidget(this);
    }
    m_makerWidget->setCurrentSongName(m_currentSongName);
    m_makerWidget->show();
}

void MusicLrcContainer::setCurrentPosition(qint64 pos) const
{
    if(m_makerWidget)
    {
        m_makerWidget->setCurrentPosition(pos);
    }
}
