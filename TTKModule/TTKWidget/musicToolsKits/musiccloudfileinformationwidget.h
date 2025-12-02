#ifndef MUSICCLOUDFILEINFORMATIONWIDGET_H
#define MUSICCLOUDFILEINFORMATIONWIDGET_H

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

#include "musicabstractmovedialog.h"

struct QSyncDataItem;

namespace Ui {
class MusicCloudFileInformationWidget;
}

/*! @brief The class of the cloud file information widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicCloudFileInformationWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicCloudFileInformationWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicCloudFileInformationWidget();

    /*!
     * Set media file song information.
     */
    void setFileInformation(QSyncDataItem *data);

private:
    Ui::MusicCloudFileInformationWidget *m_ui;

};

#endif // MUSICCLOUDFILEINFORMATIONWIDGET_H
