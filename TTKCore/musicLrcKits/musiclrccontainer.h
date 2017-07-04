#ifndef MUSICLRCCONTAINER_H
#define MUSICLRCCONTAINER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclrcmanager.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicsettingmanager.h"

class MusicLrcAnalysis;
class MusicLrcSearchWidget;

/*! @brief The class of the lrc container base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainer : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLrcContainer(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcContainer();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startTimerClock() = 0;
    /*!
     * Start timer clock to draw lrc.
     * Subclass should implement this function.
     */
    virtual void stopLrcMask() = 0;
    /*!
     * Stop timer clock to draw lrc.
     * Subclass should implement this function.
     */
    void setLinearGradientColor(MusicLRCColor::LrcColorType lrcColorType);
    /*!
     * Set linear gradient color.
     */
    void setLinearGradientColor(const MusicLRCColor &color);
    /*!
     * Set linear gradient color.
     */
    virtual void setSettingParameter();
    /*!
     * Set setting parameter.
     */

    inline QString getCurrentSongName() const { return m_currentSongName;}
    /*!
     * Set current song name.
     */
    inline void setCurrentSongName(const QString &name) { m_currentSongName = name;}
    /*!
     * Set current song name.
     */
    void setCurrentTime(qint64 time, qint64 total);
    /*!
     * Set current play total time.
     */
    qint64 getTotalTime() const;
    /*!
     * Get current play total time.
     */

    inline void setLrcAnalysisModel(MusicLrcAnalysis *analysis)  { m_lrcAnalysis = analysis;}
    /*!
     * Set lrc analysis model.
     */
    inline MusicLrcAnalysis* getLrcAnalysisModel() const { return m_lrcAnalysis;}
    /*!
     * Get lrc analysis model.
     */

Q_SIGNALS:
    void theCurrentLrcUpdated();
    /*!
     * The current lrc should update, emit it.
     */
    void changeCurrentLrcColorCustom();
    /*!
     * Change current setting widget to color setting widget.
     */
    void changeCurrentLrcColorSetting();
    /*!
     * Change current setting widget to setting widget.
     */
    void maskLinearGradientColorChanged();
    /*!
     * Mask linear gradient color changed.
     */
    void linearGradientColorChanged();
    /*!
     * Linear gradient color changed.
     */

public Q_SLOTS:
    void currentLrcCustom();
    /*!
     * Show current setting widget.
     */
    void changeCurrentLrcColor(QAction *action);
    /*!
     * Change current lrc linear color by action index.
     */
    void changeCurrentLrcColor(int index);
    /*!
     * Change current lrc linear color by index.
     */
    void searchMusicLrcs();
    /*!
     * Show search lrcs widget.
     */
    void theCurrentLrcMaked();
    /*!
     * Show making lrcs widget.
     */
    void theLinkLrcChanged();
    /*!
     * Link current lrc state changed.
     */
    void theCurrentLrcError();
    /*!
     * Show making error lrcs widget.
     */

protected:
    void clearAllMusicLRCManager();
    /*!
     * Clear all music lrc manager.
     */
    void setSettingParameter(const QString &t);
    /*!
     * Set setting parameter by diff type.
     */

    bool m_linkLocalLrc;
    qint64 m_currentTime, m_totalTime;
    QString m_currentSongName, m_containerType;
    MusicLrcAnalysis *m_lrcAnalysis;
    QList<MusicLRCManager*> m_musicLrcContainer;
    MusicLrcSearchWidget *m_musicLrcSearchWidget;

};

#endif // MUSICLRCCONTAINER_H
