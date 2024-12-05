#ifndef MUSICOPENFILEWIDGET_H
#define MUSICOPENFILEWIDGET_H

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

/*! @brief The class of the open file widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicOpenFileWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicOpenFileWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicOpenFileWidget(QWidget *parent = nullptr);

    /*!
     * Adjust the widget rect in the parent widget.
     */
    void adjustWidgetRect(int w, int h);

Q_SIGNALS:
    /*!
     * Upload file button clicked.
     */
    void uploadFileClicked();
    /*!
     * Upload dir button clicked.
     */
    void uploadDirClicked();

};

#endif // MUSICOPENFILEWIDGET_H
