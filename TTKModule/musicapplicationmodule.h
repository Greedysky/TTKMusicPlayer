#ifndef MUSICAPPLICATIONMODULE_H
#define MUSICAPPLICATIONMODULE_H

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

#include <QPropertyAnimation>
#include "musicglobaldefine.h"

class QDeviceWatcher;
class MusicBackupModule;
class MusicTimerAutoModule;
class MusicMobileDevicesWidget;
class MusicCounterPVRequest;
class MusicSourceUpdateRequest;
class MusicScreenSaverBackgroundWidget;

namespace TTK
{
enum class Direction;
}

/*! @brief The class of the app object widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicApplicationModule : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicApplicationModule)
public:
    /*!
     * Object constructor.
     */
    explicit MusicApplicationModule(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicApplicationModule();

    /*!
     * Get class object instance.
     */
    static MusicApplicationModule *instance();

    /*!
     * Get current window is to top.
     */
    inline bool windowToTop() const { return m_setWindowToTop; }

    /*!
     * Clean up the system parameter.
    */
    void cleanup();
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
    void applyParameter();
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

Q_SIGNALS:
    /*!
     * Set enhanced music config changed.
     */
    void enhancedMusicChanged(int type);

public Q_SLOTS:
    /*!
     * Application quit.
     */
    void quit();
    /*!
     * Window close animation finished.
     */
    void windowCloseAnimationFinished();
    /*!
     * Show about widget.
     */
    void showAboutWidget();
    /*!
     * Show bug report view.
     */
    void showBugReportView();
    /*!
     * Show version update widget.
     */
    void showVersionWidget();
    /*!
     * Show timer widget.
     */
    void showTimerWidget();
    /*!
     * Show spectrum widget.
     */
    void showSpectrumWidget();
    /*!
     * Set current window to top.
     */
    void setWindowToTop();
    /*!
     * Reset current window geometry.
     */
    void resetWindowGeometry();
    /*!
     * Run timer auto module.
     */
    void runTimerAutoModule();
    /*!
     * Detect mobile devices name changed.
     */
    void deviceNameChanged(const QString &name);
    /*!
     * Detect mobile devices changed.
     */
    void deviceChanged(bool state);
    /*!
     * Show set equalizer widget.
     */
    void showEqualizerWidget();
    /*!
     * Show set sound effect widget.
     */
    void showSoundEffectWidget();
    /*!
     * Sound effect changed.
     */
    void soundEffectChanged();

private:
    /*!
     * Close current equalizer.
     */
    bool closeCurrentEqualizer();

    bool m_setWindowToTop;
    TTK::Direction m_direction;
    QPropertyAnimation *m_quitAnimation, *m_sideAnimation;
    MusicBackupModule *m_backupModule;
    MusicTimerAutoModule *m_timerAutoModule;
    MusicMobileDevicesWidget *m_mobileDeviceWidget;
    QDeviceWatcher *m_deviceWatcher;
    QWidget *m_quitContainer;

    MusicCounterPVRequest *m_counterPVThread;
    MusicSourceUpdateRequest *m_sourceUpdatehread;
    MusicScreenSaverBackgroundWidget *m_screenSaverWidget;

    static MusicApplicationModule *m_instance;

};

#endif // MUSICAPPLICATIONMODULE_H
