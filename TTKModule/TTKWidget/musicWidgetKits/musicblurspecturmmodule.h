#ifndef MUSICBLURSPECTURMMODULE_H
#define MUSICBLURSPECTURMMODULE_H

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
#include <QBoxLayout>
#include "musicglobaldefine.h"

/*! @brief The class of the blur spectrum object.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBlurSpecturmModule : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicBlurSpecturmModule(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicBlurSpecturmModule();

    /*!
     * Set ripple spectrum show or not.
     */
    void setVisible(bool v);
    /*!
     * Set ripple spectrum show.
     */
    void show();
    /*!
     * Set ripple spectrum hide.
     */
    void close();
    /*!
     * Update ripple spectrum paramters.
     */
    void update(bool up);
    /*!
     * Init ripple spectrum paramters.
     */
    void initialize(QVBoxLayout *layout, QWidget *widget);

private:
    /*!
     * Remove ripple spectrum object.
     */
    void removeSpectrum();

    QVBoxLayout *m_topAreaLayout;
    QWidget *m_topAreaWidget, *m_visualWidget;

};

#endif // MUSICBLURSPECTURMMODULE_H
