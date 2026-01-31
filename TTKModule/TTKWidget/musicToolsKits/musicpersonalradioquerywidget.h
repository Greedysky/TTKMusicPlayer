#ifndef MUSICPERSONALRADIOQUERYWIDGET_H
#define MUSICPERSONALRADIOQUERYWIDGET_H

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

#include "musicabstractitemquerywidget.h"

/*! @brief The class of the personal radio query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPersonalRadioQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicPersonalRadioQueryWidget(QWidget *parent = nullptr);

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeGeometry() override final;

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void queryAllFinished();

private:
    /*!
     * Set media playlist item.
     */
    void setResultDataItem(const MusicResultDataItem &item);

};

#endif // MUSICPERSONALRADIOQUERYWIDGET_H
