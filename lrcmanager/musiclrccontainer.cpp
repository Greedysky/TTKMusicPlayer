#include "musiclrccontainer.h"

MusicLrcContainer::MusicLrcContainer(QWidget *parent) :
    QWidget(parent)
{

}

MusicLrcContainer::~MusicLrcContainer()
{

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

void MusicLrcContainer::createColorMenu(QMenu& menu)
{
    menu.addAction(QIcon(QPixmap(":/color/origin")),
                             tr("origin"),this,SLOT(changeCurrentLrcColorOrigin()));
    menu.addAction(QIcon(QPixmap(":/color/red")),
                             tr("red"),this,SLOT(changeCurrentLrcColorRed()));
    menu.addAction(QIcon(QPixmap(":/color/orange")),
                             tr("orange"),this,SLOT(changeCurrentLrcColorOrange()));
    menu.addAction(QIcon(QPixmap(":/color/yellow")),
                             tr("yellow"),this,SLOT(changeCurrentLrcColorYellow()));
    menu.addAction(QIcon(QPixmap(":/color/green")),
                             tr("green"),this,SLOT(changeCurrentLrcColorGreen()));
    menu.addAction(QIcon(QPixmap(":/color/blue")),
                             tr("blue"),this,SLOT(changeCurrentLrcColorBlue()));
    menu.addAction(QIcon(QPixmap(":/color/indigo")),
                             tr("indigo"),this,SLOT(changeCurrentLrcColorIndigo()));
    menu.addAction(QIcon(QPixmap(":/color/purple")),
                             tr("purple"),this,SLOT(changeCurrentLrcColorPurple()));
    menu.addAction(QIcon(QPixmap(":/color/white")),
                             tr("white"),this,SLOT(changeCurrentLrcColorWhite()));
    menu.addAction(QIcon(QPixmap(":/color/black")),
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
        QSettings().setValue("DLRCCOLORCHOICED",lrcColorType);
    else
        QSettings().setValue("LRCCOLORCHOICED",lrcColorType);
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
    QSettings para;
    for(int i=0; i<m_musicLrcContainer.count(); ++i)
    {
        m_musicLrcContainer[i]->setFontFamily(para.value(t + "LRCFAMILYCHOICED").toInt());
        m_musicLrcContainer[i]->setFontType(para.value(t + "LRCTYPECHOICED").toInt());
        m_musicLrcContainer[i]->setFontTransparent(para.value(t + "LRCCOLORTRANSCHOICED").toInt());
        m_musicLrcContainer[i]->setLrcFontSize((LrcSizeTable)(para.value(t + "LRCSIZECHOICED").toInt()));
    }
    if(para.value(t + "LRCCOLORCHOICED").toInt() != -1)
    {
        setLinearGradientColor((LrcColorTable::LrcColorType)para.value(t + "LRCCOLORCHOICED").toInt());
        setMaskLinearGradientColor();
    }
    else
    {
        for(int i=0; i<m_musicLrcContainer.count(); ++i)
            m_musicLrcContainer[i]->setLinearGradientColor(para.value(t + "LRCBGCOLORCHOICED").value<QColor>());
        setMaskLinearGradientColor(para.value(t + "LRCFGCOLORCHOICED").value<QColor>());
    }
}
