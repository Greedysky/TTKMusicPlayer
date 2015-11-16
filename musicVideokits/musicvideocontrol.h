#ifndef MUSICVIDEOCONTROL_H
#define MUSICVIDEOCONTROL_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include <QWidget>
#include "musicuiobject.h"
#include "musicobject.h"
#include "musiclibexportglobal.h"

class QSlider;
class QToolButton;
class QPushButton;

class MUSIC_VIDEO_EXPORT MusicVideoControl : public QWidget
{
    Q_OBJECT
public:
    explicit MusicVideoControl(bool popup, QWidget *parent = 0);
    ~MusicVideoControl();

    void setValue(qint64 position) const;
    void durationChanged(qint64 duration) const;
    void setButtonStyle(bool style) const;
    void mediaChanged(const QString &url);
    inline bool isPopup() const { return m_widgetPopup;}

signals:
    void musicVideoSetPopup(bool popup);
    void musicVideoFullscreen(bool full);
    void getMusicMvInfo(SongUrlFormats &data);
    void downloadLocalByQuality(int quality);
    void mvURLChanged(const QString &data);

public slots:
    void show();
    void inSideButtonClicked();
    void fullButtonClicked();
    void downloadButtonClicked();
    void menuActionTriggered(QAction *action);

protected:
    void setQualityActionState();

    bool m_widgetPopup;
    QMenu m_popupVolumn;
    QMenu m_popupQuality;
    QSlider *m_timeSlider, *m_volumnSlider;
    QToolButton *m_menuButton, *m_volumnButton;
    QPushButton *m_playButton, *m_inSideButton, *m_fullButton;
    QPushButton *m_qualityButton, *m_downloadButton;
    QAction *m_mvNormal, *m_mvHd;

};

#endif // MUSICVIDEOCONTROL_H
