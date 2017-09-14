#ifndef MUSICLRCCONTAINERFORWALLPAPER_H
#define MUSICLRCCONTAINERFORWALLPAPER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclrccontainer.h"

class MusicLayoutAnimationWidget;
class MusicDesktopWallpaperThread;

/*! @brief The class of the wall paper lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainerForWallpaper : public MusicLrcContainer
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcContainerForWallpaper(QWidget *parent = 0);

    virtual ~MusicLrcContainerForWallpaper();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start timer clock to draw lrc.
     */
    virtual void startTimerClock() override;
    /*!
     * Stop timer clock to draw lrc.
     */
    virtual void stopLrcMask() override;
    /*!
     * Set setting parameter.
     */
    virtual void setSettingParameter() override;

    /*!
     * Set lrc analysis model.
     */
    void setLrcAnalysisModel(MusicLrcAnalysis *analysis);
    /*!
     * Update current lrc by given time.
     */
    void updateCurrentLrc(qint64 time);

public Q_SLOTS:
    /*!
     * Change current lrc linear color.
     */
    void changeCurrentLrcColor();

private Q_SLOTS:
    /*!
     * Animation finished.
     */
    void updateAnimationLrc();

protected:
    /*!
     * Init current lrc when the first show.
     */
    void initCurrentLrc(const QString &str);
    /*!
     * Set per lrc line style sheet by index and size and transparent.
     */
    void setItemStyleSheet(int index, int size, int transparent);

    int m_animationFreshTime;
    MusicLayoutAnimationWidget *m_layoutWidget;
    MusicDesktopWallpaperThread *m_wallThread;

};

#endif // MUSICLRCCONTAINERFORWALLPAPER_H
