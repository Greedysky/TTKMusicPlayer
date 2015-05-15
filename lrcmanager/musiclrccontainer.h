#ifndef MUSICLRCCONTAINER_H
#define MUSICLRCCONTAINER_H

#include <QWidget>
#include <QSettings>
#include "musiclrcmanager.h"
#include "../core/musicobject.h"

class MUSIC_EXPORT MusicLrcContainer : public QWidget
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

protected:
    void createColorMenu(QMenu&);
    void clearAllMusicLRCManager();
    void setSettingParameter(const QString& t);

    QList<MusicLRCManager*> m_musicLrcContainer;
    QString m_containerType;

};

#endif // MUSICLRCCONTAINER_H
