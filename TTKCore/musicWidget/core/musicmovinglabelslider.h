#ifndef MUSICMOVINGLABELSLIDER_H
#define MUSICMOVINGLABELSLIDER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QSlider>
#include <QLabel>
#include "musicglobaldefine.h"

class MUSIC_WIDGET_EXPORT MusicMovingLabelSlider : public QSlider
{
    Q_OBJECT
public:
    explicit MusicMovingLabelSlider(QWidget *parent = 0);
    explicit MusicMovingLabelSlider(Qt::Orientation orientation,
                                    QWidget *parent = 0);
    ~MusicMovingLabelSlider();

    inline bool isMoving() const { return m_isMoving;}
    inline void setMoving(bool m) { m_isMoving = m;}

Q_SIGNALS:
    void sliderReleasedAt(int pos);

public Q_SLOTS:
    void setValue(int value);

private Q_SLOTS:
    void sliderMovedChanged();
    void sliderReleasedChanged();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    QPoint limitLableGeometry(int x, int y, int z);

    bool m_isMoving;
    QLabel *m_textLabel;
    Qt::Orientation m_orientation;

};

#endif // MUSICMOVINGLABELSLIDER_H
