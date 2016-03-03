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
class QPropertyAnimation;

class MUSIC_GUI_EXPORT MusicApplicationObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicApplicationObject(QObject *parent = 0);
    ~MusicApplicationObject();

    bool getWindowToTop() const {return m_setWindowToTop;}
#if defined(Q_OS_WIN)
#  ifdef MUSIC_QT_5
    void nativeEvent(const QByteArray &eventType,
                     void *message, long *result);
#  else
    void winEvent(MSG *message, long *result);
#  endif
#endif
    void getParameterSetting();
    void windowStartAnimationOpacity();
    void windowCloseAnimationOpacity();

Q_SIGNALS:
    void getCurrentPlayList(QStringList &list);
    /*!
     * Get current play lists.
     */
    void enhancedMusicChanged(int type);

public Q_SLOTS:
    void musicAboutUs();
    void musicAudioRecorder();
    void musicTimerWidget();
    void musicSetWindowToTop();
    void musicToolSetsParameter();
    /*!
     * Timer parameter changed.
     */
    void musicSetEqualizer();

protected:
    bool m_setWindowToTop;
    QPropertyAnimation *m_animation;
    QWidget *m_supperClass;
    MusicTimerAutoObject *m_musicTimerAutoObj;
    MusicMobileDevicesWidget *m_mobileDevices;

};

#endif // MUSICAPPLICATIONOBJECT_H
