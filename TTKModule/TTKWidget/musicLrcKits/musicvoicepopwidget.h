#ifndef MUSICVOICEPOPWIDGET_H
#define MUSICVOICEPOPWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include "musictoolmenuwidget.h"

class TTKClickedSlider;

/*! @brief The class of the volume popup widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicVoicePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicVoicePopWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicVoicePopWidget();

public Q_SLOTS:
    /*!
     * Timer ot reset show slider.
     */
    void timeToReset();
    /*!
     * Slider value changed.
     */
    void valueChanged(int value);
    /*!
     * Update voice remove mode.
     */
    void updateVoiceRemove(bool v = true);

private:
    /*!
     * Override the widget event.
     */
    virtual void leaveEvent(QEvent *event) override final;
    virtual void enterEvent(QtEnterEvent *event) override final;
    /*!
     * Init widget and parameter.
     */
    void initialize();

    bool m_menuShown;
    TTKClickedSlider *m_slider;

};

#endif // MUSICVOICEPOPWIDGET_H
