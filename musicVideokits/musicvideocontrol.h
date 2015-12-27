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

class QLabel;
class QSlider;
class QToolButton;
class QPushButton;
class MusicMovingLabelSlider;

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

    void setFixedSize(int w, int h);

signals:
    void musicVideoSetPopup(bool popup);
    void musicVideoFullscreen(bool full);
    void getMusicMvInfo(MusicSongAttributes &data);
    void downloadLocalByControl();
    void mvURLChanged(const QString &data);
    void sliderValueChanged(int value);

    void pushBarrageChanged(bool on);
    void barrageSizeButtonChanged(int size);
    void barrageColorButtonChanged(const QColor &color);

public slots:
    void show();
    void inSideButtonClicked();
    void fullButtonClicked();
    void menuActionTriggered(QAction *action);
    void sliderReleased();

    void pushBarrageClicked();
    void barrageSizeButtonClicked(int index);
    void barrageColorButtonClicked(int index);

protected:
    void setQualityActionState();
    QString findMVUrlByBitrate(int bitrate);
    int findMVBitrateByUrl(const QString &url);
    bool findExsitByBitrate(int bitrate);
    QWidget* createBarrageWidget();
    QPushButton* createBarrageSizeButton(int index);
    QPushButton* createBarrageColorButton(int index);

    bool m_widgetPopup;
    QMenu m_popupVolumn, m_popupQuality, m_popupBarrage;
    MusicMovingLabelSlider *m_timeSlider;
    QSlider *m_volumnSlider;
    QToolButton *m_menuButton, *m_volumnButton;
    QPushButton *m_playButton, *m_inSideButton, *m_fullButton;
    QPushButton *m_qualityButton, *m_downloadButton;
    QAction *m_mvSd, *m_mvHd, *m_mvSq;

    QPushButton *m_pushBarrage;
    bool m_pushBarrageOn;

};

#endif // MUSICVIDEOCONTROL_H
