#ifndef MUSICLRCCONTAINER_H
#define MUSICLRCCONTAINER_H

#include <QWidget>
#include <QSettings>
#include "musiclrcmanager.h"
#include "musicobject.h"

class MusicLrcSearchWidget;

class MUSIC_WIDGET_EXPORT MusicLrcContainer : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLrcContainer(QWidget *parent = 0);
    virtual ~MusicLrcContainer();

    virtual void startTimerClock() = 0;
    virtual void stopLrcMask() = 0;
    virtual void setMaskLinearGradientColor(QColor = LrcColorTable::CL_Mask) = 0;
    virtual void setSettingParameter();
    void setLinearGradientColor(LrcColorTable::LrcColorType);
    inline void setCurrentSongName(const QString& name){ m_currentSongName = name;}

signals:
    void theCurrentLrcUpdated();
    void changeCurrentLrcColorCustom();
    void changeCurrentLrcColorSetting();

public slots:
    void currentLrcCustom();
    void changeCurrentLrcColorOrigin(){setLinearGradientColor(LrcColorTable::Origin);}
    void changeCurrentLrcColorRed(){setLinearGradientColor(LrcColorTable::Red);}
    void changeCurrentLrcColorOrange(){setLinearGradientColor(LrcColorTable::Orange);}
    void changeCurrentLrcColorYellow(){setLinearGradientColor(LrcColorTable::Yellow);}
    void changeCurrentLrcColorGreen(){setLinearGradientColor(LrcColorTable::Green);}
    void changeCurrentLrcColorBlue(){setLinearGradientColor(LrcColorTable::Blue);}
    void changeCurrentLrcColorIndigo(){setLinearGradientColor(LrcColorTable::Indigo);}
    void changeCurrentLrcColorPurple(){setLinearGradientColor(LrcColorTable::Purple);}
    void changeCurrentLrcColorWhite(){setLinearGradientColor(LrcColorTable::White);}
    void changeCurrentLrcColorBlack(){setLinearGradientColor(LrcColorTable::Black);}
    void searchMusicLrcs();
    void theCurrentLrcMaked();

protected:
    void createColorMenu(QMenu&);
    void clearAllMusicLRCManager();
    void setSettingParameter(const QString& t);

    QString m_currentSongName;
    QList<MusicLRCManager*> m_musicLrcContainer;
    QString m_containerType;
    MusicLrcSearchWidget *m_musicLrcSearchWidget;

};

#endif // MUSICLRCCONTAINER_H
