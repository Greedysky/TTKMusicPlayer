#ifndef MUSICPHOTOMODLABEL_H
#define MUSICPHOTOMODLABEL_H

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

#include <QWidget>
#include "musicglobaldefine.h"

#define PIX_HEIGHT 16
#define PIX_WIDTH  16

/*! @brief The class of the photo modify label.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicPhotoModLabel : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPhotoModLabel)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPhotoModLabel(QWidget *parent = nullptr);

    /*!
     * Set song image path.
     */
    void setImagePath(const QString &path);
    /*!
     * Set save song image path.
     */
    void saveImagePath(const QString &path) const;

Q_SIGNALS:
    /*!
     * Delta value changed.
     */
    void deltaValueChanged(float v);

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    QString m_path;
    QPixmap m_showPix;
    float m_ratio;
    bool m_picMoved;
    int m_width, m_height, m_originWidth;
    QPoint m_pressedPos, m_deltaPos, m_imagePos;

};

#endif // MUSICPHOTOMODLABEL_H
