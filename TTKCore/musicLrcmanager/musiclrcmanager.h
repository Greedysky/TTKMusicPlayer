#ifndef MUSICLRCMANAGER_H
#define MUSICLRCMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QMouseEvent>
#include <QMenu>
#include <QTimer>
#include <QAction>
#include <QPainter>
#include "musicglobaldefine.h"

#define LRC_PER_TIME 30

const QColor CL_Origin = QColor(14,179,255);
const QColor CL_Red    = QColor(214,51,44);
const QColor CL_Orange = QColor(230,130,52);
const QColor CL_Yellow = QColor(243,209,0);
const QColor CL_Green  = QColor(62,164,140);
const QColor CL_Blue   = QColor(76,147,193);
const QColor CL_Indigo = QColor(29,237,235);
const QColor CL_Purple = QColor(156,115,155);
const QColor CL_White  = QColor(255,255,255);
const QColor CL_Black  = QColor(0,0,0);
const QColor CL_Mask   = QColor(222,54,4);

/*! @brief The class of the lrc manager base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLRCManager : public QLabel
{
    Q_OBJECT
public:
    enum LrcColorType
    {
        Origin, ///*color origin*/
        Red,    ///*color red*/
        Orange, ///*color orange*/
        Yellow, ///*color yellow*/
        Green,  ///*color green*/
        Blue,   ///*color blue*/
        Indigo, ///*color indigo*/
        Purple, ///*color purple*/
        White,  ///*color white*/
        Black,  ///*color black*/
        Mask    ///*color mask*/
    };

    enum LrcSizeTable
    {
        Smaller = 13,   ///*lrc size smaller*/
        Small = 14,     ///*lrc size small*/
        Middle = 15,    ///*lrc size middle*/
        Big = 16,       ///*lrc size big*/
        Bigger = 17     ///*lrc size bigger*/
    };

    explicit MusicLRCManager(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLRCManager();

    void startTimerClock();
    /*!
     * Start timer clock to draw lrc.
     */
    void startLrcMask(qint64 intervaltime);
    /*!
     * Start timer clock to draw lrc mask.
     */
    void stopLrcMask();
    /*!
     * Stop timer clock to draw lrc mask.
     */
    void setLinearGradientColor(QColor = CL_Origin);
    /*!
     * Set linear gradient color.
     */
    void setMaskLinearGradientColor(QColor = CL_Mask);
    /*!
     * Set mask linear gradient color.
     */
    void setLrcFontSize(LrcSizeTable size);
    /*!
     * Set lrc font size by given size type.
     */
    void setFontFamily(int index);
    /*!
     * Set lrc font family by given type.
     */
    void setFontType(int type);
    /*!
     * Set lrc font type by given type.
     */
    inline void setFontTransparent(int t) { m_transparent = t;}
    /*!
     * Set lrc font transparent by given valu.e
     */

Q_SIGNALS:

public Q_SLOTS:
    void setTimeOut();
    /*!
     * Time out to calculate lrc mask line length.
     */
    void setText(const QString &str);
    /*!
     * Override the setTtext function.
     */

protected:
    QLinearGradient m_linearGradient, m_maskLinearGradient;
    QFont m_font;
    QTimer *m_timer;
    qreal m_lrcMaskWidth, m_lrcMaskWidthInterval, m_intervalCount;

    int m_lrcPerWidth, m_transparent, m_speedLeve;
    QPoint m_geometry;

};

#endif // MUSICLRCMANAGER_H
