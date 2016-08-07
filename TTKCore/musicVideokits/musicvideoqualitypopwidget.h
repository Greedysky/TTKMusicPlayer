#ifndef MUSICVIDEOQUALITYPOPWIDGET_H
#define MUSICVIDEOQUALITYPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musictoolmenuwidget.h"

/*! @brief The class of the video quality popup widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoQualityPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicVideoQualityPopWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicVideoQualityPopWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setQualityActionState();
    /*!
     * Set quality action state enable or disable.
     */
    void setQualityText(const QString &url);
    /*!
     * Set current media quality text.
     */

Q_SIGNALS:
    void getMusicMvInfo(MusicObject::MusicSongAttributes &data);
    /*!
     * Get music mv information data.
     */
    void mvURLChanged(const QString &data);
    /*!
     * Set current media url by selected quality.
     */

public Q_SLOTS:
    void movieQualityChoiced(QAction *action);
    /*!
     * Movie quality choiced by index.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
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

    QActionGroup *m_actionGroup;

};

#endif // MUSICVIDEOQUALITYPOPWIDGET_H
