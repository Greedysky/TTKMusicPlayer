#ifndef MUSICPHOTOGRABLABEL_H
#define MUSICPHOTOGRABLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#include "musicresizegrabitemwidget.h"

/*! @brief The class of the photo grab label item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPhotoGrabItem : public MusicResizeGrabItemWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPhotoGrabItem)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPhotoGrabItem(QWidget *parent = nullptr);

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
    TTK_DECLARE_MODULE(MusicPhotoGrabLabel)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPhotoGrabLabel(QWidget *parent = nullptr);

    ~MusicPhotoGrabLabel();

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
