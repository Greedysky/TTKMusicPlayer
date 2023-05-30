#ifndef MUSICWEBRADIOVIEW_H
#define MUSICWEBRADIOVIEW_H

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

#include <QWidget>
#include "musicglobaldefine.h"

/*! @brief The class of the web radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebRadioView : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebRadioView)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebRadioView(QWidget *parent = nullptr);
    ~MusicWebRadioView();

public Q_SLOTS:
    /*!
     * Open FM Radio window has clicked.
     */
    void openFMRadioWindow();
    /*!
     * Open TK Radio window has clicked.
     */
    void openTKRadioWindow();
    /*!
     * Open DJ Radio window has clicked.
     */
    void openDJRadioWindow();
    /*!
     * Open MV Radio window has clicked.
     */
    void openMVRadioWindow();

private:
    QWidget *m_radio;

};

#endif // MUSICWEBRADIOVIEW_H
