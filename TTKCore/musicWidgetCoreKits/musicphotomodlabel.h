#ifndef MUSICPHOTOMODLABEL_H
#define MUSICPHOTOMODLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
public:
    explicit MusicPhotoModLabel(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setImagePath(const QString &path);
    /*!
     * Set song image path.
     */
    void saveImagePath(const QString &path) const;
    /*!
     * Set save song image path.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    /*!
     * Override the widget event.
     */

    QString m_path;
    QPixmap m_showPix;
    int m_width, m_height;
    float m_ratio;
    bool m_picMoved;
    QPoint m_pressedPos, m_deltaPos, m_imagePos;

};

#endif // MUSICPHOTOMODLABEL_H
