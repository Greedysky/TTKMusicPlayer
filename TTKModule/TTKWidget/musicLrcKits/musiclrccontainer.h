#ifndef MUSICLRCCONTAINER_H
#define MUSICLRCCONTAINER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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
 ***************************************************************************/

#include "musiclrcmanager.h"
#include "musiclrcanalysis.h"
#include "musicsettingmanager.h"

class MusicLrcSearchWidget;

#define LRC_WALLPAPER_TPYE     "WALLPAPER"
#define LRC_INTERIOR_TPYE      "INTERIOR"
#define LRC_DESKTOP_TPYE       "DESKTOP"
#define LRC_DESKTOP_PREFIX     "D"

/*! @brief The class of the lrc container base.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcContainer : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcContainer)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcContainer(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcContainer();

    /*!
     * Start timer clock to draw lrc.
     * Subclass should implement this function.
     */
    virtual void startDrawLrc() = 0;
    /*!
     * Stop timer clock to draw lrc.
     * Subclass should implement this function.
     */
    virtual void stopDrawLrc() = 0;
    /*!
     * Set setting parameter.
     */
    virtual void applyParameter();

    /*!
     * Set linear gradient color.
     */
    void setLinearGradientColor(MusicLrcColor::Color color);
    /*!
     * Set linear gradient color.
     */
    void setLinearGradientColor(const MusicLrcColor &color);
    /*!
     * Set current song name.
     */
    inline void setCurrentSongName(const QString &name) { m_currentSongName = name; }
    /*!
     * Set current play total time.
     */
    void setCurrentTime(qint64 time, qint64 total);
    /*!
     * Get current play total time.
     */
    qint64 totalTime() const;

    /*!
     * Set lrc analysis model.
     */
    inline void setLrcAnalysisModel(MusicLrcAnalysis *analysis)  { m_lrcAnalysis = analysis; }
    /*!
     * Get lrc analysis model.
     */
    inline MusicLrcAnalysis* lrcAnalysisModel() const { return m_lrcAnalysis; }

Q_SIGNALS:
    /*!
     * The current lrc should update.
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

protected:
    /*!
     * Clear all music lrc manager.
     */
    void clearAllMusicLRCManager();

    bool m_linkLocalLrc;
    qint64 m_currentTime, m_totalTime;
    QString m_currentSongName, m_containerType;
    MusicLrcAnalysis *m_lrcAnalysis;
    QList<MusicLrcManager*> m_lrcManagers;
    MusicLrcSearchWidget *m_lrcSearchWidget;

};

#endif // MUSICLRCCONTAINER_H
