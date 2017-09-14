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
    /*!
     * Object contsructor.
     */
    explicit MusicPhotoGrabItem(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set border rect.
     */
    virtual void setBorderRect(const QRect &rect) override;

};



/*! @brief The class of the photo grab label.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPhotoGrabLabel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPhotoGrabLabel(QWidget *parent = 0);

    ~MusicPhotoGrabLabel();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set song image path.
     */
    void setImagePath(const QString &path);
    /*!
     * Get changed pixmap.
     */
    QPixmap pixmap();

Q_SIGNALS:
    /*!
     * Send intersected pixmap if intersected flag set.
     */
    void intersectedPixmap(const QPixmap &pix);

public Q_SLOTS:
    /*!
     * Current rect changed.
     */
    void rectChanged();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    float m_ratio;
    QString m_path;
    QRect m_imgRect;
    MusicPhotoGrabItem *m_grabItem;

};

#endif // MUSICPHOTOGRABLABEL_H
