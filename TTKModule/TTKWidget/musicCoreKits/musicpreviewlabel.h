#ifndef MUSICPREVIEWLABEL_H
#define MUSICPREVIEWLABEL_H

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

/*! @brief The class of the lrc color preview label.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicColorPreviewLabel : public MusicClickedLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicColorPreviewLabel)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicColorPreviewLabel(QWidget *parent = nullptr);

    /*!
     * Set linear gradient colors.
     */
    void setColors(const QList<QColor> &colors);
    /*!
     * Get linear gradient colors.
     */
    QList<QColor> getColors() const;
    /*!
     * Get linear gradient color.
     */
    QColor getColor() const;

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    QList<QColor> m_gradientColors;
    QLinearGradient m_linearGradient;

};

/*! @brief The class of the label previewed.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_WIDGET_EXPORT MusicPreviewLabelItem
{
    QString m_family;
    int m_size;
    int m_type;
    QList<QColor> m_frontground;
    QList<QColor> m_background;
}MusicPreviewLabelItem;

/*! @brief The class of the lrc setting preview label.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPreviewLabel : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPreviewLabel)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPreviewLabel(QWidget *parent = nullptr);

    /*!
     * Set linear gradient item.
     */
    void setLinearGradient(const MusicPreviewLabelItem &item);
    /*!
     * Set linear gradient and front and back.
     */
    void setLinearGradient(const QList<QColor> &front, const QList<QColor> &back);
    /*!
     * Set current transparent.
     */
    void setTransparent(int trans) { m_transparent = trans;}

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    QFont m_font;
    QLinearGradient m_linearGradient;
    QLinearGradient m_maskLinearGradient;
    int m_transparent;

};

#endif // MUSICPREVIEWLABEL_H
