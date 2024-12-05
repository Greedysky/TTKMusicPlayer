#ifndef MUSICMARQUEEWIDGET_H
#define MUSICMARQUEEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include <QWidget>
#include "musicglobaldefine.h"

/*! @brief The class of the marquee widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicMarqueeWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicMarqueeWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicMarqueeWidget(QWidget *parent = nullptr);

    /*!
     * Set effect on resize.
     */
    inline void setEffectOnResize(bool on) { m_effectOnResize = on; }
    /*!
     * Set marquee text.
     */
    void setText(const QString &newText);
    /*!
     * Get marquee text.
     */
    inline QString text() const { return m_text.trimmed(); }
    /*!
     * Override size hint.
     */
    virtual QSize sizeHint() const override;

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;
    virtual void showEvent(QShowEvent *event) override final;
    virtual void hideEvent(QHideEvent *event) override final;
    virtual void timerEvent(QTimerEvent *event) override final;
    virtual void resizeEvent(QResizeEvent *event) override final;

    QString m_text;
    int m_offset,m_timerId;
    bool m_effectOnResize;

};

#endif // MUSICMARQUEEWIDGET_H
