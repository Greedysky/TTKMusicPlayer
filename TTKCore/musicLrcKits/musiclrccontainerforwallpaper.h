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

class MusicLayoutAnimation;
class MusicDesktopWallpaperThread;

/*! @brief The class of the wall paper lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainerForWallpaper : public MusicLrcContainer
{
    Q_OBJECT
public:
    explicit MusicLrcContainerForWallpaper(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcContainerForWallpaper();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startTimerClock() override;
    /*!
     * Start timer clock to draw lrc.
     */
    virtual void stopLrcMask() override;
    /*!
     * Stop timer clock to draw lrc.
     */
    virtual void setSettingParameter() override;
    /*!
     * Set setting parameter.
     */

    void setLrcAnalysisModel(MusicLrcAnalysis *analysis);
    /*!
     * Set lrc analysis model.
     */
    void updateCurrentLrc(qint64 time);
    /*!
     * Update current lrc by given time.
     */

public Q_SLOTS:
    void changeCurrentLrcColor();
    /*!
     * Change current lrc linear color.
     */

private Q_SLOTS:
    void updateAnimationLrc();
    /*!
     * Animation finished.
     */

protected:
    void initCurrentLrc(const QString &str);
    /*!
     * Init current lrc when the first show.
     */
    void setItemStyleSheet(int index, int size, int transparent);
    /*!
     * Set per lrc line style sheet by index and size and transparent.
     */

    int m_animationFreshTime;
    MusicLayoutAnimation *m_layoutWidget;
    MusicDesktopWallpaperThread *m_wallThread;
};

#endif // MUSICLRCCONTAINERFORWALLPAPER_H
