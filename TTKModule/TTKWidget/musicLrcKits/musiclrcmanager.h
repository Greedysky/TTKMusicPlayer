#ifndef MUSICLRCMANAGER_H
#define MUSICLRCMANAGER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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
 ***************************************************************************/

#include <QTimer>
#include <QAction>
#include <QMouseEvent>
#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"

#define LRC_PER_TIME        30
#define LRC_COLOR_OFFSET    9

/*! @brief The class of the lrc color.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcColor
{
public:
    enum class Color
    {
        Null = -1,   /*!< color null */
        IYellow,     /*!< color yellow */
        IBlue,       /*!< color blue */
        IGray,       /*!< color gray */
        IPink,       /*!< color pink */
        IGreen,      /*!< color green */
        IRed,        /*!< color red */
        IPurple,     /*!< color purple */
        IOrange,     /*!< color orange */
        IIndigo,     /*!< color indigo */

        DWhite,      /*!< color white */
        DBlue,       /*!< color blue */
        DRed,        /*!< color red */
        DBlack,      /*!< color black */
        DYellow,     /*!< color yellow */
        DPurple,     /*!< color purple */
        DGreen       /*!< color green */
    };

    /*!
     * Object constructor.
     */
    MusicLrcColor();
    MusicLrcColor(const QList<QColor> &front, const QList<QColor> &back, Color index = Color::Null);

    /*!
     * Map index to color.
     */
    static MusicLrcColor mapIndexToColor(Color index);
    /*!
     * Transform colors string to color list.
     */
    static QList<QColor> readColorConfig(const QString &value);
    /*!
     * Transform color list to colors string.
     */
    static QString writeColorConfig(const QList<QColor> &colors);

    QList<QColor> m_frontColor;
    QList<QColor> m_backColor;
    MusicLrcColor::Color m_index;

};


/*! @brief The class of the lrc manager base.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcManager : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcManager)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcManager(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcManager();

    /*!
     * Start timer clock to draw lrc.
     */
    void startDrawLrc();
    /*!
     * Start timer clock to draw lrc mask.
     */
    void startDrawLrcMask(qint64 intervaltime);
    /*!
     * Stop timer clock to draw lrc mask.
     */
    void stopDrawLrc();

    /*!
     * Set linear gradient color.
     */
    void setLinearGradientColor(const MusicLrcColor &color);
    /*!
     * Set lrc font family by given type.
     */
    void setFontFamily(int index);
    /*!
     * Set lrc font type by given type.
     */
    void setFontType(int type);
    /*!
     * Set lrc font transparent by given value.
     */
    inline void setFontTransparent(int t) { m_transparent = t; }
    /*!
     * Set lrc font speed level by given value.
     */
    inline void setSpeedLevel(int l) { m_speedLevel = l; }

    /*!
     * Set self position by point.
     */
    void setSelfPosition(const QPoint &point);
    /*!
     * Set self position by x and y.
     */
    void setSelfPosition(int x, int y);
    /*!
     * Get self position x.
     */
    int x() const;
    /*!
     * Get self position y.
     */
    int y() const;
    /*!
     * Set self position x.
     */
    void setX(int x);
    /*!
     * Set self position y.
     */
    void setY(int y);
    /*!
     * Reset to origin state.
     */
    void reset();
    /*!
     * Set adjust font szie by value.
     */
    void setLrcFontSize(int size);
    /*!
     * Get current font szie.
     */
    inline int lrcFontSize() const { return m_font.pointSize(); }

public Q_SLOTS:
    /*!
     * Time out to calculate lrc mask line length.
     */
    void setUpdateMask();
    /*!
     * Override the setTtext function.
     */
    void setText(const QString &str);

protected:
    QFont m_font;
    QTimer *m_timer;
    QLinearGradient m_linearGradient, m_maskLinearGradient;
    float m_lrcMaskWidth, m_lrcMaskWidthInterval, m_intervalCount;

    int m_lrcPerWidth, m_transparent, m_speedLevel;
    QPoint m_position;

};

#endif // MUSICLRCMANAGER_H
