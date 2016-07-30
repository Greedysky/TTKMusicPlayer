#ifndef MUSICVIDEOCONTROL_H
#define MUSICVIDEOCONTROL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include <QWidget>
#include "musicuiobject.h"
#include "musicobject.h"
#include "musicglobaldefine.h"
#include "musicnumberdefine.h"

class QLabel;
class QToolButton;
class QPushButton;
class MusicMovingLabelSlider;

/*! @brief The class of the video controller.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoControl : public QWidget
{
    Q_OBJECT
public:
    explicit MusicVideoControl(bool popup, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicVideoControl();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setValue(qint64 position) const;
    /*!
     * Set current video play value.
     */
    void durationChanged(qint64 duration) const;
    /*!
     * Set current video duration.
     */
    void setButtonStyle(bool style) const;
    /*!
     * Set current button style.
     */
    void mediaChanged(const QString &url);
    /*!
     * Set current media url.
     */
    inline bool isPopup() const { return m_widgetPopup;}
    /*!
     * Check widget is popup or not.
     */
    void setFixedSize(int w, int h);
    /*!
     * Resize width bound by given width and height.
     */

Q_SIGNALS:
    void musicVideoSetPopup(bool popup);
    /*!
     * Set video widget popop or not.
     */
    void musicVideoFullscreen(bool full);
    /*!
     * Set video widget fullScreen or not.
     */
    void getMusicMvInfo(MusicObject::MusicSongAttributes &data);
    /*!
     * Get music mv information data.
     */
    void downloadLocalByControl();
    /*!
     * Download current mv by controller.
     */
    void mvURLChanged(const QString &data);
    /*!
     * Set current media url by selected quality.
     */
    void sliderValueChanged(int value);
    /*!
     * Slider value changed at value.
     */

    void addBarrageChanged(const QString &string);
    /*!
     * Add barrage text to mv.
     */
    void pushBarrageChanged(bool on);
    /*!
     * Open barrage on or not.
     */
    void barrageSizeButtonChanged(int size);
    /*!
     * Change barrage size button clicked by size.
     */
    void barrageColorButtonChanged(const QColor &color);
    /*!
     * Change barrage color button clicked by color.
     */

public Q_SLOTS:
    void show();
    /*!
     * Override show function.
     */
    void insideButtonClicked();
    /*!
     * Inside button clicked.
     */
    void fullButtonClicked();
    /*!
     * Full button clicked.
     */
    void movieQualityChoiced(QAction *action);
    /*!
     * Movie quality choiced by index.
     */
    void pushBarrageClicked();
    /*!
     * Push barrage clicked.
     */
    void barrageSizeButtonClicked(int index);
    /*!
     * Barrage size button clicked by index.
     */
    void barrageColorButtonClicked(int index);
    /*!
     * Barrage color button clicked by index.
     */

protected:
    void setQualityActionState();
    /*!
     * Set quality action state enable or disable.
     */
    QString findMVUrlByBitrate(int bitrate);
    /*!
     * Find mv url by given bitrate.
     */
    int findMVBitrateByUrl(const QString &url);
    /*!
     * Find mv bitrate by given url.
     */
    bool findExistByBitrate(int bitrate);
    /*!
     * Check given bitrate is exist or not.
     */
    QWidget* createBarrageWidget();
    /*!
     * Create barrage widget.
     */
    QPushButton* createBarrageSizeButton(int index);
    /*!
     * Create barrage size button by index.
     */
    QPushButton* createBarrageColorButton(int index);
    /*!
     * Create barrage color button by index.
     */

    bool m_widgetPopup;
    QMenu m_popupQuality, m_popupBarrage;
    MusicMovingLabelSlider *m_timeSlider;
    QToolButton *m_volumeButton;
    QPushButton *m_playButton, *m_inSideButton, *m_fullButton;
    QPushButton *m_qualityButton, *m_downloadButton;
    QAction *m_mvSd, *m_mvHd, *m_mvSq;

    QPushButton *m_pushBarrage;
    bool m_pushBarrageOn;

};

#endif // MUSICVIDEOCONTROL_H
