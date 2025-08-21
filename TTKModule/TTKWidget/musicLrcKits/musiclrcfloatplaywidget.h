#ifndef MUSICLRCFLOATPLAYWIDGET_H
#define MUSICLRCFLOATPLAYWIDGET_H

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

#include "musicabstractfloatwidget.h"

class QToolButton;

/*! @brief The class of the lrc play float widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcFloatPlayWidget : public MusicAbstractFloatWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcFloatPlayWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcFloatPlayWidget();

    /*!
     * Resize geometry bound by given width and height.
     */
    void resizeGeometry(int width, int height);

public Q_SLOTS:
    /*!
     * Set current player to play or not.
     */
    void switchToPlayState();

private:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QtEnterEvent *event) override final;
    /*!
     * Set current play state icon.
     */
    void setCurrentPlayState() const;

    QToolButton *m_buttonPrevious, *m_buttonNext, *m_buttonKey;

};

#endif // MUSICLRCFLOATPLAYWIDGET_H
