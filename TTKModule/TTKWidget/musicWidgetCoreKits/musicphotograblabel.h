#ifndef MUSICPHOTOGRABLABEL_H
#define MUSICPHOTOGRABLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicresizegrabitemwidget.h"

/*! @brief The class of the photo grab label item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPhotoGrabItem : public MusicResizeGrabItemWidget
{
    Q_OBJECT
public:
    explicit MusicPhotoGrabItem(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void setBorderRect(const QRect &rect) override;
    /*!
     * Set border rect.
     */

};



/*! @brief The class of the photo grab label.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPhotoGrabLabel : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPhotoGrabLabel(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicPhotoGrabLabel();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setImagePath(const QString &path);
    /*!
     * Set song image path.
     */
    QPixmap pixmap();
    /*!
     * Get changed pixmap.
     */

Q_SIGNALS:
    void intersectedPixmap(const QPixmap &pix);
    /*!
     * Send intersected pixmap if intersected flag set.
     */

public Q_SLOTS:
    void rectChanged();
    /*!
     * Current rect changed.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    float m_ratio;
    QString m_path;
    QRect m_imgRect;
    MusicPhotoGrabItem *m_grabItem;

};

#endif // MUSICPHOTOGRABLABEL_H
