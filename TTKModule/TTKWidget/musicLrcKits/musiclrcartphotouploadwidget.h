#ifndef MUSICLRCARTPHOTOUPLOADWIDGET_H
#define MUSICLRCARTPHOTOUPLOADWIDGET_H

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

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicLrcArtPhotoUploadWidget;
}

/*! @brief The class of the lrc art photo upload.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcArtPhotoUploadWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcArtPhotoUploadWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcArtPhotoUploadWidget(QWidget *parent = nullptr);

    virtual ~MusicLrcArtPhotoUploadWidget();

public Q_SLOTS:
    /*!
     * Delta value changed.
     */
    void deltaValueChanged(float v);
    /*!
     * Select button clicked.
     */
    void selectButtonClicked();
    /*!
     * Upload button clicked.
     */
    void uploadButtonClicked();
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicLrcArtPhotoUploadWidget *m_ui;

};

#endif // MUSICLRCARTPHOTOUPLOADWIDGET_H
