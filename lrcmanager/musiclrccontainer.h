#ifndef MUSICLRCCONTAINER_H
#define MUSICLRCCONTAINER_H

#include <QWidget>
#include "musiclrcmanager.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicsettingmanager.h"

class MusicLrcSearchWidget;
class MusicLrcMakerWidget;

class MUSIC_WIDGET_EXPORT MusicLrcContainer : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLrcContainer(QWidget *parent = 0);
    virtual ~MusicLrcContainer();

    virtual void startTimerClock() = 0;
    virtual void stopLrcMask() = 0;
    virtual void setMaskLinearGradientColor(QColor = CL_Mask) = 0;
    virtual void setSettingParameter();
    void setLinearGradientColor(LrcColorType lrcColorType);
    inline void setCurrentSongName(const QString &name)
            { m_currentSongName = name;}
    void setCurrentPosition(qint64 pos);

signals:
    void theCurrentLrcUpdated();
    void changeCurrentLrcColorCustom();
    void changeCurrentLrcColorSetting();

public slots:
    void currentLrcCustom();
    void changeCurrentLrcColor(QAction *action);
    void changeCurrentLrcColor(int index);
    void searchMusicLrcs();
    void theCurrentLrcMaked();

protected:
    void createColorMenu(QMenu &menu);
    void clearAllMusicLRCManager();
    void setSettingParameter(const QString &t);

    QString m_currentSongName;
    QList<MusicLRCManager*> m_musicLrcContainer;
    QString m_containerType;
    MusicLrcSearchWidget *m_musicLrcSearchWidget;
    MusicLrcMakerWidget *m_makerWidget;

};

#endif // MUSICLRCCONTAINER_H
