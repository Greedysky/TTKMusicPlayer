#ifndef MUSICVOLUMEPOPWIDGET_H
#define MUSICVOLUMEPOPWIDGET_H

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

#include "musictoolmenuwidget.h"

class MusicClickedSlider;

/*! @brief The class of the volume popup widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicVolumePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVolumePopWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicVolumePopWidget(QWidget *parent = nullptr);

    virtual ~MusicVolumePopWidget();

    /*!
     * Set slider value.
     */
    void setValue(int value);
    /*!
     * Get slider value.
     */
    int value() const;

Q_SIGNALS:
    /*!
     * Current play volume changed.
     */
    void musicVolumeChanged(int volume);

public Q_SLOTS:
    /*!
     * Timer ot reset show slider flag.
     */
    void timeToResetFlag();

protected:
    /*!
     * Override the widget event.
     */
    virtual void leaveEvent(QEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    /*!
     * Create all widget in layout.
     */
    void initWidget();

    bool m_menuShown;
    MusicClickedSlider *m_volumeSlider;

};

#endif // MUSICVOLUMEPOPWIDGET_H
