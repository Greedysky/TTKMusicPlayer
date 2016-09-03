#ifndef MUSICAPPLICATIONOBJECT_H
#define MUSICAPPLICATIONOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMimeData>
#include <QFileDialog>
#include "musicobject.h"
#include "musicglobaldefine.h"

class MusicTimerAutoObject;
class MusicMobileDevicesWidget;
class MusicMobileDevicesThread;
class QPropertyAnimation;

/*! @brief The class of the app object widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_GUI_EXPORT MusicApplicationObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicApplicationObject(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicApplicationObject();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    static MusicApplicationObject *instance();
    /*!
     * Get class object instance.
     */
    bool getWindowToTop() const {return m_setWindowToTop;}
    /*!
     * Get current window is to top.
     */
#if defined(Q_OS_WIN)
#  ifdef MUSIC_GREATER_NEW
    void nativeEvent(const QByteArray &eventType,
                     void *message, long *result);
    /*!
     * Native event.
     */
#  else
    void winEvent(MSG *message, long *result);
    /*!
     * Win event.
     */
#  endif
#endif
    void getParameterSetting();
    /*!
     * Get settings parameters.
     */
    void windowStartAnimationOpacity();
    /*!
     * Window start animation opacity.
     */
    void windowCloseAnimationOpacity();
    /*!
     * Window close animation opacity.
     */

Q_SIGNALS:
    void enhancedMusicChanged(int type);
    /*!
     * Set enhanced music config changed.
     */

public Q_SLOTS:
    void musicAboutUs();
    /*!
     * Show about us widget.
     */
    void musicVersionUpdate();
    /*!
     * Show version update widget.
     */
    void musicAudioRecorder();
    /*!
     * Show audio recorder widget.
     */
    void musicTimerWidget();
    /*!
     * Show timer widget.
     */
    void musicSetWindowToTop();
    /*!
     * Set current window to top.
     */
    void musicResetWindow();
    /*!
     * Reset current window geometry.
     */
    void musicToolSetsParameter();
    /*!
     * Timer parameter changed.
     */
    void musicDevicesLinuxChanged(bool state);
    /*!
     * Detect mobile devices on linux.
     */
    void musicSetEqualizer();
    /*!
     * Show set equalizer widget.
     */
    void musicSetSoundEffect();
    /*!
     * Show set sound effect widget.
     */

protected:
    bool closeCurrentEqualizer();
    /*!
     * Close current equalizer.
     */

    bool m_setWindowToTop;
    QPropertyAnimation *m_animation;
    MusicTimerAutoObject *m_musicTimerAutoObj;
    MusicMobileDevicesWidget *m_mobileDevices;
    MusicMobileDevicesThread *m_mobileDevicesLinux;

    static MusicApplicationObject *m_instance;
};

#endif // MUSICAPPLICATIONOBJECT_H
