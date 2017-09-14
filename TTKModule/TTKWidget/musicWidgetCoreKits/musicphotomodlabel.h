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
    /*!
     * Object contsructor.
     */
    explicit MusicPhotoModLabel(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
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
