#ifndef MUSICPHOTOGRABLABEL_H
#define MUSICPHOTOGRABLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

/*! @brief The class of the photo grab label item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPhotoGrabItem : public QWidget
{
    Q_OBJECT
public:
    enum Direction
    {
        Direction_No = 0,                                           ///*no direction*/
        Direction_Left = 1,                                         ///*left direction*/
        Direction_Top = 2,                                          ///*top direction*/
        Direction_Right = 4,                                        ///*right direction*/
        Direction_Bottom = 8,                                       ///*bottom direction*/
        Direction_LeftTop = Direction_Left + Direction_Top,         ///*left top direction*/
        Direction_LeftBottom = Direction_Left + Direction_Bottom,   ///*left bottom direction*/
        Direction_RightTop = Direction_Right + Direction_Top,       ///*right top direction*/
        Direction_RightBottom = Direction_Right + Direction_Bottom  ///*right bottom direction*/
    };
    explicit MusicPhotoGrabItem(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setBorderRect(const QRect &rect);
    /*!
     * Set border rect.
     */

Q_SIGNALS:
    void rectChanged();
    /*!
     * Current rect changed.
     */

private Q_SLOTS:
    void onMouseChange(int x,int y);
    /*!
     * On mouse changed.
     */

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    /*!
     * Override the widget event.
     */

    Direction getRegion(const QPoint &cursor);
    /*!
     * Get region rect.
     */

private:
    bool m_isPressed;
    Direction m_direction;
    QPoint m_originPoint, m_movePos;
    QPolygon m_listMarker;
    QRect m_currentRect, m_borderRect;

};



/*! @brief The class of the photo grab label.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPhotoGrabLabel : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPhotoGrabLabel(QWidget *parent = nullptr);
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
