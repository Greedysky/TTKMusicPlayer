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

    bool getWindowToTop() const {return m_setWindowToTop;}
    /*!
     * Get current window is to top.
     */
#if defined(Q_OS_WIN)
#  ifdef MUSIC_QT_5
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
    void musicSetEqualizer();
    /*!
     * Show set equalizer widget.
     */

Q_SIGNALS:
    void getCurrentPlayList(QStringList &list);
    /*!
     * Get current play lists.
     */
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
    void musicToolSetsParameter();
    /*!
     * Timer parameter changed.
     */
    void musicDevicesLinuxChanged(bool state);
    /*!
     * Detect mobile devices on linux.
     */

protected:
    bool m_setWindowToTop;
    QPropertyAnimation *m_animation;
    QWidget *m_supperClass;
    MusicTimerAutoObject *m_musicTimerAutoObj;
    MusicMobileDevicesWidget *m_mobileDevices;
    MusicMobileDevicesThread *m_mobileDevicesLinux;

};

#endif // MUSICAPPLICATIONOBJECT_H
