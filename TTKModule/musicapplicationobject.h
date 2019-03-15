#ifndef MUSICAPPLICATIONOBJECT_H
#define MUSICAPPLICATIONOBJECT_H

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

#include "musicobject.h"
#include "musicglobaldefine.h"

class QDeviceWatcher;
class QPropertyAnimation;
class MusicTimerAutoObject;
class MusicMobileDevicesWidget;
class MusicDownloadCounterPVThread;
class MusicDownloadQNConfighread;

/*! @brief The class of the app object widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_GUI_EXPORT MusicApplicationObject : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicApplicationObject)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicApplicationObject(QObject *parent = nullptr);

    ~MusicApplicationObject();

    /*!
     * Get class object instance.
     */
    static MusicApplicationObject *instance();
    /*!
     * Get current window is to top.
     */
    inline bool getWindowToTop() const { return m_setWindowToTop; }

    /*!
     * load network settings parameters.
     */
    void loadNetWorkSetting();
    /*!
     * Get settings parameters.
     */
    void getParameterSetting();
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
    /*!
     * Something temp clean up.
     */
    void cleanUp();

    bool m_setWindowToTop;
    bool m_leftSideByOn, m_rightSideByOn;
    QPropertyAnimation *m_quitAnimation, *m_sideAnimation;
    MusicTimerAutoObject *m_musicTimerAutoObj;
    MusicMobileDevicesWidget *m_mobileDeviceWidget;
    QDeviceWatcher *m_deviceWatcher;
    QWidget *m_quitContainer;
    MusicDownloadCounterPVThread *m_counterPVThread;
    MusicDownloadQNConfighread *m_qnConfigThread;

    static MusicApplicationObject *m_instance;
};

#endif // MUSICAPPLICATIONOBJECT_H
