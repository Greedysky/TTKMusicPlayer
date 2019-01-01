#ifndef MUSICLRCCONTAINERFORWALLPAPER_H
#define MUSICLRCCONTAINERFORWALLPAPER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musiclrccontainer.h"

class MusicVLayoutAnimationWidget;
class MusicDesktopWallpaperThread;
class MusicTransitionAnimationLabel;

/*! @brief The class of the wall paper lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainerForWallpaper : public MusicLrcContainer
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcContainerForWallpaper)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcContainerForWallpaper(QWidget *parent = nullptr);

    virtual ~MusicLrcContainerForWallpaper();

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
    /*!
     * Update current lrc by given text.
     */
    void updateCurrentLrc(const QString &text);
    /*!
     * Strat now.
     */
    void start(bool immediate);

public Q_SLOTS:
    /*!
     * Change current lrc linear color.
     */
    void changeCurrentLrcColor();
    /*!
     * Update background pixmap.
     */
    void updateBackground(const QPixmap &pix);

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
    MusicVLayoutAnimationWidget *m_layoutWidget;
    MusicDesktopWallpaperThread *m_wallThread;
    MusicTransitionAnimationLabel *m_background;

};

#endif // MUSICLRCCONTAINERFORWALLPAPER_H
