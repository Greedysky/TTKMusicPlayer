#ifndef MUSICFILEINFORMATIONWIDGET_H
#define MUSICFILEINFORMATIONWIDGET_H

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

#include "musicwidgetheaders.h"
#include "musicabstractmovedialog.h"

class MusicCoreMPlayer;

namespace Ui {
class MusicFileInformationWidget;
}

/*! @brief The class of the file information widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFileInformationWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicFileInformationWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicFileInformationWidget();

    /*!
     * Set media file song information.
     */
    void initialize(const QString &path);

public Q_SLOTS:
    /*!
     * Open the media at local path.
     */
    void openFileDir();
    /*!
     * Open the selected user pixmap.
     */
    void openImageFileDir();
    /*!
     * Delete album image.
     */
    void deleteAlbumImage();
    /*!
     * Save album image.
     */
    void saveAlbumImage();
    /*!
     * Dynamic album image.
     */
    void openDynamicImage();
    /*!
     * Current tab index changed.
     */
    void currentTabChanged(int index);
    /*!
     * Download data from net finished.
     */
    void downloadDataFinished(const QString &bytes);
    /*!
     * Media modify tag start.
     */
    void editTag();
    /*!
     * Media modify tag save.
     */
    void saveTag();

private:
    /*!
     * Override the widget event.
     */
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    /*!
     * Eet editLine enable.
     */
    void setEditLineEnabled(bool enabled);

    Ui::MusicFileInformationWidget *m_ui;
    QString m_path, m_imagePath;
    bool m_deleteImage;
    QWidget *m_container;
    MusicCoreMPlayer *m_player;

};

#endif // MUSICFILEINFORMATIONWIDGET_H
