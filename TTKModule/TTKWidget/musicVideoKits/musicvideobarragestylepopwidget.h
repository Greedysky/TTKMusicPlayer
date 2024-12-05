#ifndef MUSICVIDEOBARRAGESTYLEPOPWIDGET_H
#define MUSICVIDEOBARRAGESTYLEPOPWIDGET_H

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

#include "musictoolmenuwidget.h"

/*! @brief The class of the video varrage style popup widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicVideoBarrageStylePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVideoBarrageStylePopWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicVideoBarrageStylePopWidget(QWidget *MusicToolMenuWidget = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicVideoBarrageStylePopWidget();

    /*!
     * Get barrage size.
     */
    int barrageSize() const;
    /*!
     * Get barrage color.
     */
    QColor barrageColor() const;

public Q_SLOTS:
    /*!
     * Barrage size button clicked by index.
     */
    void barrageSizeButtonClicked(int index);
    /*!
     * Barrage color button clicked by index.
     */
    void barrageColorButtonClicked(int index);

private:
    /*!
     * Create all widget in layout.
     */
    void initialize();

    int m_barrageSize;
    QColor m_barrageColor;
    QButtonGroup *m_sizeGroup;
    QButtonGroup *m_colorGroup;

};

#endif // MUSICVIDEOBARRAGESTYLEPOPWIDGET_H
