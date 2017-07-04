#ifndef MUSICLRCMANAGER_H
#define MUSICLRCMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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

#define LRC_PER_TIME        30
#define LRC_COLOR_OFFSET    9

typedef struct MUSIC_LRC_EXPORT MusicLRCColor
{
    enum LrcColorType
    {
        Null = -1,   ///*color null*/
        IYellow,     ///*color yellow*/
        IBlue,       ///*color blue*/
        IGray,       ///*color gray*/
        IPink,       ///*color pink*/
        IGreen,      ///*color green*/
        IRed,        ///*color red*/
        IPurple,     ///*color purple*/
        IOrange,     ///*color orange*/
        IIndigo,     ///*color indigo*/

        DWhite,      ///*color white*/
        DBlue,       ///*color blue*/
        DRed,        ///*color red*/
        DBlack,      ///*color black*/
        DYellow,     ///*color yellow*/
        DPurple,     ///*color purple*/
        DGreen,      ///*color green*/

    };

    QList<QColor> m_fgColor;
    QList<QColor> m_bgColor;
    MusicLRCColor::LrcColorType m_index;

    MusicLRCColor() { }

    MusicLRCColor(const QList<QColor> &fg, const QList<QColor> &bg,
                  MusicLRCColor::LrcColorType index = MusicLRCColor::Null)
    {
        m_fgColor = fg;
        m_bgColor = bg;
        m_index = index;
    }

    bool custum() const
    {
        return m_index == MusicLRCColor::Null;
    }

}MusicLRCColor;

/*! @brief The class of the lrc manager base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLRCManager : public QLabel
{
    Q_OBJECT
public:
    explicit MusicLRCManager(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLRCManager();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    static MusicLRCColor mapIndexToColor(MusicLRCColor::LrcColorType index);
    /*!
     * Map index to color.
     */

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
    void setLinearGradientColor(const MusicLRCColor &color);
    /*!
     * Set linear gradient color.
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
     * Set lrc font transparent by given value.
     */
    inline void setSpeedLevel(int l) { m_speedLevel = l;}
    /*!
     * Set lrc font speed level by given value.
     */

    void setSelfGeometry(const QPoint &point);
    /*!
     * Set self geometry by point.
     */
    void setSelfGeometry(int x, int y);
    /*!
     * Set self geometry by x and y.
     */
    int x() const;
    /*!
     * Get self geometry x.
     */
    int y() const;
    /*!
     * Get self geometry y.
     */
    void setX(int x);
    /*!
     * Set self geometry x.
     */
    void setY(int y);
    /*!
     * Set self geometry y.
     */
    void reset();
    /*!
     * Reset to origin state.
     */
    void setLrcFontSize(int size);
    /*!
     * Set adjust font szie by value.
     */
    inline int getFirstFontSize() const { return m_font.pointSize(); }
    /*!
     * Get current font szie.
     */

public Q_SLOTS:
    void setUpdateMask();
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

    int m_lrcPerWidth, m_transparent, m_speedLevel;
    QPoint m_geometry;

};

#endif // MUSICLRCMANAGER_H
