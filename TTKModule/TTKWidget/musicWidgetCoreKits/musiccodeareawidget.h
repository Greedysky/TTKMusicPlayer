#ifndef MUSICCODEAREAWIDGET_H
#define MUSICCODEAREAWIDGET_H

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

#include "musicclickedlabel.h"

/*! @brief The class of the label show code area widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicCodeAreaWidget : public MusicClickedLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCodeAreaWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicCodeAreaWidget(QWidget *parent = nullptr);

    /*!
     * Render the verify code picture.
     */
    void renderPicture();
    /*!
     * Get the verify code string.
     */
    inline QString getCheckCode() const { return m_sCode; }

    /*!
     * Set the verify code count length.
     */
    void setCodeCount(int nCount);
    /*!
     * Get the verify code count length.
     */
    inline int getCodeCount() const { return m_nCodeCount; }

    /*!
     * Set the noisy point count.
     */
    void setNoisyPointCount(int nCount);
    /*!
     * Get the noisy point count.
     */
    inline int getNoisyPointCount() const { return m_nNoisyPointCount; }

protected:
    /*!
     * Draw outline pie.
     */
    void drawOutline(QPainter &painter);
    /*!
     * Draw noisy point.
     */
    void drawNoisyPoint(QPainter &painter);
    /*!
     * Draw conversion.
     */
    void drawConversion(QPainter &painter);
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    int m_nNoisyPointCount, m_nConverseRotate;
    int m_nConverseScale, m_nCodeCount;
    QString m_sCode;
    QStringList m_slCodeRange;
    QList<QPainterPath> m_lCodePic;
    QList<Qt::GlobalColor> m_lCodeColor;

};

#endif // MUSICCODEAREAWIDGET_H
