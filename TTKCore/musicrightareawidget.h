#ifndef MUSICRIGHTAREAWIDGET_H
#define MUSICRIGHTAREAWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

class MusicSettingWidget;
class MusicDownloadStatusLabel;
class MusicLrcContainerForDesktop;

namespace Ui {
    class MusicApplication;
}

/*! @brief The class of the app right area widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_GUI_EXPORT MusicRightAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicRightAreaWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicRightAreaWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setupUi(Ui::MusicApplication* ui);
    /*!
     * Set up app ui.
     */
    void stopLrcMask() const;
    /*!
     * Stop timer clock to draw lrc.
     */
    void startTimerClock() const;
    /*!
     * Start timer clock to draw lrc.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Set current play state button.
     */
    bool getDestopLrcVisible() const;
    /*!
     * Get destop lrc visible state.
     */
    void setInlineLrcVisible(bool status) const;
    /*!
     * Set inline lrc visible by string.
     */
    bool getInlineLrcVisible() const;
    /*!
     * Get inline lrc visible state.
     */
    void setSettingParameter() const;
    /*!
     * Set setting parameter.
     */
    void getParameterSetting() const;
    /*!
     * Get setting parameter.
     */
    bool checkSettingParameterValue() const;
    /*!
     * Check the setting has open inline or desktop lrc on or not.
     */
    void updateCurrentLrc(qint64 current, qint64 total, bool playStatus) const;
    /*!
     * Update current lrc by current time\total time and play state.
     */
    void loadCurrentSongLrc(const QString &name, const QString &path) const;
    /*!
     * Load current song lrc by name and path.
     */
    void setSongSpeedAndSlow(qint64 time) const;
    /*!
     * Set song speed and slow by given time.
     */
    void musicCheckHasLrcAlready() const;
    /*!
     * Check the current song already has lrc or not,
     * if not just download it.
     */
    void showSettingWidget() const;
    /*!
     * Show setting widget.
     */

Q_SIGNALS:
    void updateBgThemeDownload();
    /*!
     * Current background skin download.
     */
    void updateBackgroundTheme();
    /*!
     * Current background transparent changed.
     */
    void desktopLrcClosed();
    /*!
     * Set desktop lrc close state.
     */
    void lockDesktopLrc(bool lock);
    /*!
     * Lock or not current desktop lrc emit.
     */

public Q_SLOTS:
    void musicFunctionClicked(int index);
    /*!
     * Music function button clicked.
     */
    void musicLoadSongIndexWidget();
    /*!
     * Music load song index widget.
     */
    void deleteStackedFuncWidget();
    /*!
     * Delete current stacked widget.
     */
    void setDestopLrcVisible(bool visible) const;
    /*!
     * Set destop lrc visible or invisible.
     */
    void setWindowLockedChanged();
    /*!
     * Lock current desktop lrc state changed.
     */
    void songResearchButtonSearched(const QString &name);
    /*!
     * Music song research button searched by name.
     */
    void researchQueryByQuality(const QString &quality);
    /*!
     * Research query by quality it changed.
     */
    void musicVideoButtonSearched(const QString &name);
    /*!
     * Video button clicked by name to search.
     */
    void musicVideoSetPopup(bool popup);
    /*!
     * Set video widget popop or not.
     */
    void musicVideoFullscreen(bool full);
    /*!
     * Set video widget fullScreen or not.
     */
    void musicLrcDisplayAllButtonClicked();
    /*!
     * Lrc display all button clicked.
     */

protected:
    void musicButtonStyleClear(bool fore);
    /*!
     * Function button style clear.
     */

    QWidget *m_supperClass, *m_stackedFuncWidget;
    Ui::MusicApplication *m_ui;
    MusicSettingWidget *m_setting;
    MusicLrcContainerForDesktop *m_musiclrcfordesktop;
    MusicDownloadStatusLabel *m_downloadStatusLabel;

};

#endif // MUSICRIGHTAREAWIDGET_H
