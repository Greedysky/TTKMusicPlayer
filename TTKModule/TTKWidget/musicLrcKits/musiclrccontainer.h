#ifndef MUSICLRCCONTAINER_H
#define MUSICLRCCONTAINER_H

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

#include "musiclrcmanager.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicsettingmanager.h"

class MusicLrcAnalysis;
class MusicLrcSearchWidget;

#define LRC_WALLPAPER_TPYE     "WALLPAPER"
#define LRC_INLINE_TPYE        "INLINE"
#define LRC_DESKTOP_TPYE       "DESKTOP"
#define LRC_DESKTOP_PREFIX     "D"

/*! @brief The class of the lrc container base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainer : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcContainer)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcContainer(QWidget *parent = nullptr);

    virtual ~MusicLrcContainer();

    /*!
     * Start timer clock to draw lrc.
     * Subclass should implement this function.
     */
    virtual void startTimerClock() = 0;
    /*!
     * Stop timer clock to draw lrc.
     * Subclass should implement this function.
     */
    virtual void stopLrcMask() = 0;
    /*!
     * Set linear gradient color.
     */
    void setLinearGradientColor(MusicLrcColor::LrcColorType lrcColorType);
    /*!
     * Set linear gradient color.
     */
    void setLinearGradientColor(const MusicLrcColor &color);
    /*!
     * Set setting parameter.
     */
    virtual void setSettingParameter();

    /*!
     * Set current song name.
     */
    inline QString getCurrentSongName() const { return m_currentSongName;}
    /*!
     * Set current song name.
     */
    inline void setCurrentSongName(const QString &name) { m_currentSongName = name;}
    /*!
     * Set current play total time.
     */
    void setCurrentTime(qint64 time, qint64 total);
    /*!
     * Get current play total time.
     */
    qint64 getTotalTime() const;

    /*!
     * Set lrc analysis model.
     */
    inline void setLrcAnalysisModel(MusicLrcAnalysis *analysis)  { m_lrcAnalysis = analysis;}
    /*!
     * Get lrc analysis model.
     */
    inline MusicLrcAnalysis* getLrcAnalysisModel() const { return m_lrcAnalysis;}

Q_SIGNALS:
    /*!
     * The current lrc should update, emit it.
     */
    void currentLrcUpdated();
    /*!
     * Change current setting widget to color setting widget.
     */
    void changeCurrentLrcColorCustom();
    /*!
     * Change current setting widget to setting widget.
     */
    void changeCurrentLrcColorSetting();
    /*!
     * Mask linear gradient color changed.
     */
    void maskLinearGradientColorChanged();
    /*!
     * Linear gradient color changed.
     */
    void linearGradientColorChanged();

public Q_SLOTS:
    /*!
     * Show current setting widget.
     */
    void currentLrcCustom();
    /*!
     * Change current lrc linear color by action index.
     */
    void changeCurrentLrcColor(QAction *action);
    /*!
     * Change current lrc linear color by index.
     */
    void changeCurrentLrcColor(int index);
    /*!
     * Show search lrcs widget.
     */
    void searchMusicLrcs();
    /*!
     * Show making lrcs widget.
     */
    void showLrcMakedWidget();
    /*!
     * Link current lrc state changed.
     */
    void linkLrcStateChanged();
    /*!
     * Show making error lrcs widget.
     */
    void showLrcErrorWidget();

protected:
    /*!
     * Clear all music lrc manager.
     */
    void clearAllMusicLRCManager();
    /*!
     * Set setting parameter by diff type.
     */
    void setSettingParameter(const QString &t);

    bool m_linkLocalLrc;
    qint64 m_currentTime, m_totalTime;
    QString m_currentSongName, m_containerType;
    MusicLrcAnalysis *m_lrcAnalysis;
    QList<MusicLrcManager*> m_musicLrcContainer;
    MusicLrcSearchWidget *m_musicLrcSearchWidget;

};

#endif // MUSICLRCCONTAINER_H
