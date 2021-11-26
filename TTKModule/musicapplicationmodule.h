#ifndef MUSICAPPLICATIONMODULE_H
#define MUSICAPPLICATIONMODULE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#include <QPropertyAnimation>
#include "musicglobaldefine.h"

class QDeviceWatcher;
class MusicTimerAutoModule;
class MusicMobileDevicesWidget;
class MusicSourceUpdateRequest;
class MusicDownloadCounterPVRequest;
class MusicScreenSaverBackgroundWidget;

/*! @brief The class of the app object widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicApplicationModule : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicApplicationModule)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicApplicationModule(QObject *parent = nullptr);
    ~MusicApplicationModule();

    /*!
     * Get class object instance.
     */
    static MusicApplicationModule *instance();
    /*!
     * Get current window is to top.
     */
    inline bool getWindowToTop() const { return m_setWindowToTop; }

    /*!
     * Is lasted version.
     */
    bool isLastedVersion() const;
    /*!
     * load network settings parameters.
     */
    void loadNetWorkSetting();
    /*!
     * Apply settings parameters.
     */
    void applySettingParameter();
    /*!
     * Window close animation.
     */
    void windowCloseAnimation();
    /*!
     * Soure update check.
     */
    void soureUpdateCheck();

    /*!
     * Side animation by on.
     */
    void sideAnimationByOn();
    /*!
     * Side animation by off.
     */
    void sideAnimationByOff();
    /*!
     * Side animation reset.
     */
    void sideAnimationReset();

Q_SIGNALS:
    /*!
     * Set enhanced music config changed.
     */
    void enhancedMusicChanged(int type);

public Q_SLOTS:
    /*!
     * Window close animation finished.
     */
    void windowCloseAnimationFinished();
    /*!
     * Show about us widget.
     */
    void musicAboutUs();
    /*!
     * Show bug report view.
     */
    void musicBugReportView();
    /*!
     * Show version update widget.
     */
    void musicVersionUpdate();
    /*!
     * Show timer widget.
     */
    void musicTimerWidget();
    /*!
     * Show spectrum widget.
     */
    void musicSpectrumWidget();
    /*!
     * Set current window to top.
     */
    void musicSetWindowToTop();
    /*!
     * Reset current window geometry.
     */
    void musicResetWindow();
    /*!
     * Tools sets parameter changed.
     */
    void musicToolSetsParameter();
    /*!
     * Detect mobile devices name changed.
     */
    void musicDeviceNameChanged(const QString &name);
    /*!
     * Detect mobile devices changed.
     */
    void musicDeviceChanged(bool state);
    /*!
     * Show set equalizer widget.
     */
    void musicSetEqualizer();
    /*!
     * Show set sound effect widget.
     */
    void musicSetSoundEffect();
    /*!
     * Sound effect changed.
     */
    void musicEffectChanged();

protected:
    /*!
     * Close current equalizer.
     */
    bool closeCurrentEqualizer();

    bool m_setWindowToTop;
    bool m_leftSideByOn, m_rightSideByOn;
    QPropertyAnimation *m_quitAnimation, *m_sideAnimation;
    MusicTimerAutoModule *m_MusicTimerAutoModule;
    MusicMobileDevicesWidget *m_mobileDeviceWidget;
    QDeviceWatcher *m_deviceWatcher;
    QWidget *m_quitContainer;

    MusicSourceUpdateRequest *m_sourceUpdatehread;
    MusicDownloadCounterPVRequest *m_counterPVThread;
    MusicScreenSaverBackgroundWidget *m_screenSaverWidget;

    static MusicApplicationModule *m_instance;

};

#endif // MUSICAPPLICATIONMODULE_H
