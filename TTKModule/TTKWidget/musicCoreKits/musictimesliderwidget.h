#ifndef MUSICTIMESLIDERWIDGET_H
#define MUSICTIMESLIDERWIDGET_H

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

class MusicGifLabelWidget;
class MusicMovingLabelSlider;

/*! @brief The class of the time slider widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTimeSliderWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicTimeSliderWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicTimeSliderWidget();

    /*!
     * Set input connection.
     */
    void setInputModule(QObject *object) const;
    /*!
     * Set current play state.
     */
    void setCurrentPlayState(bool state);

    /*!
     * Set current play index value.
     */
    void setValue(qint64 value) const;
    /*!
     * Set current play range from min and max.
     */
    void setRange(int min, int max);

public Q_SLOTS:
    /*!
     * Current slider move at pos.
     */
    void sliderMovedAt(int pos) const;
    /*!
     * Set slider style diff by diff enhanced effect.
     */
    void setSliderStyleByType(int type);

private:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;

    MusicGifLabelWidget *m_label;
    MusicMovingLabelSlider *m_slider;

};

#endif // MUSICTIMESLIDERWIDGET_H
