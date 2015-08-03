#include "musiclrccontainer.h"
#include "musiclrcsearchwidget.h"
#include "musiclrcmakerwidget.h"

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
        m_musicLrcSearchWidget = new MusicLrcSearchWidget(this);
    m_musicLrcSearchWidget->setCurrentSongName(m_currentSongName);
    m_musicLrcSearchWidget->show();
}

void MusicLrcContainer::createColorMenu(QMenu& menu)
{
    menu.addAction(QIcon(":/color/origin"),
                             tr("origin"),this,SLOT(changeCurrentLrcColorOrigin()));
    menu.addAction(QIcon(":/color/red"),
                             tr("red"),this,SLOT(changeCurrentLrcColorRed()));
    menu.addAction(QIcon(":/color/orange"),
                             tr("orange"),this,SLOT(changeCurrentLrcColorOrange()));
    menu.addAction(QIcon(":/color/yellow"),
                             tr("yellow"),this,SLOT(changeCurrentLrcColorYellow()));
    menu.addAction(QIcon(":/color/green"),
                             tr("green"),this,SLOT(changeCurrentLrcColorGreen()));
    menu.addAction(QIcon(":/color/blue"),
                             tr("blue"),this,SLOT(changeCurrentLrcColorBlue()));
    menu.addAction(QIcon(":/color/indigo"),
                             tr("indigo"),this,SLOT(changeCurrentLrcColorIndigo()));
    menu.addAction(QIcon(":/color/purple"),
                             tr("purple"),this,SLOT(changeCurrentLrcColorPurple()));
    menu.addAction(QIcon(":/color/white"),
                             tr("white"),this,SLOT(changeCurrentLrcColorWhite()));
    menu.addAction(QIcon(":/color/black"),
                             tr("black"),this,SLOT(changeCurrentLrcColorBlack()));
    menu.addSeparator();
    menu.addAction(tr("custom"),this,SLOT(currentLrcCustom()));
}

void MusicLrcContainer::setLinearGradientColor(LrcColorTable::LrcColorType lrcColorType)
{
    for(int i=0; i<m_musicLrcContainer.count(); ++i)
    {
        switch(lrcColorType)
        {
            case LrcColorTable::Origin:
            m_musicLrcContainer[i]->setLinearGradientColor(LrcColorTable::CL_Origin);break;
            case LrcColorTable::Red:
            m_musicLrcContainer[i]->setLinearGradientColor(LrcColorTable::CL_Red);break;
            case LrcColorTable::Orange:
            m_musicLrcContainer[i]->setLinearGradientColor(LrcColorTable::CL_Orange);break;
            case LrcColorTable::Yellow:
            m_musicLrcContainer[i]->setLinearGradientColor(LrcColorTable::CL_Yellow);break;
            case LrcColorTable::Green:
            m_musicLrcContainer[i]->setLinearGradientColor(LrcColorTable::CL_Green);break;
            case LrcColorTable::Blue:
            m_musicLrcContainer[i]->setLinearGradientColor(LrcColorTable::CL_Blue);break;
            case LrcColorTable::Indigo:
            m_musicLrcContainer[i]->setLinearGradientColor(LrcColorTable::CL_Indigo);break;
            case LrcColorTable::Purple:
            m_musicLrcContainer[i]->setLinearGradientColor(LrcColorTable::CL_Purple);break;
            case LrcColorTable::White:
            m_musicLrcContainer[i]->setLinearGradientColor(LrcColorTable::CL_White);break;
            case LrcColorTable::Black:
            m_musicLrcContainer[i]->setLinearGradientColor(LrcColorTable::CL_Black);break;
            default:break;
        }
    }
    if(m_containerType == "DESKTOP")
        M_SETTING.setValue(MusicSettingManager::DLrcColorChoiced, lrcColorType);
    else
        M_SETTING.setValue(MusicSettingManager::LrcColorChoiced, lrcColorType);
}

void MusicLrcContainer::setSettingParameter()
{
    if(m_containerType == "DESKTOP")
        setSettingParameter("D");
    else
        setSettingParameter("");
}

void MusicLrcContainer::setSettingParameter(const QString& t)
{
    for(int i=0; i<m_musicLrcContainer.count(); ++i)
    {
        m_musicLrcContainer[i]->setFontFamily(M_SETTING.value(t + "LrcFamilyChoiced").toInt());
        m_musicLrcContainer[i]->setFontType(M_SETTING.value(t + "LrcTypeChoiced").toInt());
        m_musicLrcContainer[i]->setFontTransparent(M_SETTING.value(t + "LrcColorTransChoiced").toInt());
        m_musicLrcContainer[i]->setLrcFontSize((LrcSizeTable)(M_SETTING.value(t + "LrcSizeChoiced").toInt()));
    }
    if(M_SETTING.value(t + "LrcColorChoiced").toInt() != -1)
    {
        setLinearGradientColor((LrcColorTable::LrcColorType)M_SETTING.value(t + "LrcColorChoiced").toInt());
        setMaskLinearGradientColor();
    }
    else
    {
        for(int i=0; i<m_musicLrcContainer.count(); ++i)
            m_musicLrcContainer[i]->setLinearGradientColor(M_SETTING.value(t + "LrcBgColorChoiced").value<QColor>());
        setMaskLinearGradientColor(M_SETTING.value(t + "LrcFgColorChoiced").value<QColor>());
    }
}


void MusicLrcContainer::theCurrentLrcMaked()
{
    if(m_makerWidget == NULL)
        m_makerWidget = new MusicLrcMakerWidget(this);
    m_makerWidget->setCurrentSongName(m_currentSongName);
    m_makerWidget->show();
}

void MusicLrcContainer::setCurrentPosition(qint64 pos)
{
    if(m_makerWidget)
        m_makerWidget->setCurrentPosition(pos);
}
