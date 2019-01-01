#ifndef MUSICVIDEOFLOATWIDGET_H
#define MUSICVIDEOFLOATWIDGET_H

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

#include "musicfloatabstractwidget.h"

class QPushButton;

/*! @brief The class of the video float widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoFloatWidget : public MusicFloatAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVideoFloatWidget)
public:
    enum Type
    {
        SearchType,     /*!< search text type*/
        FreshType,      /*!< fresh text type*/
        FullscreenType, /*!< fullscreen text type*/
        DownloadType ,  /*!< download text type*/
        ShareType       /*!< share text type*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicVideoFloatWidget(QWidget *parent = nullptr);

    virtual ~MusicVideoFloatWidget();

    /*!
     * Resize window bound by given width and height.
     */
    virtual void resizeWindow(int width, int height) override;

    /*!
     * Set tool button text by type.
     */
    void setText(Type type, const QString &text);
    /*!
     * Get tool button text by type.
     */
    QString getText(Type type) const;

Q_SIGNALS:
    /*!
     * Search button clicked.
     */
    void searchButtonClicked();
    /*!
     * Fresh button clicked.
     */
    void freshButtonClicked();
    /*!
     * Fullscreen button clicked.
     */
    void fullscreenButtonClicked();
    /*!
     * Download button clicked.
     */
    void downloadButtonClicked();
    /*!
     * Share button clicked.
     */
    void shareButtonClicked();

protected:
    QPushButton *m_search, *m_fresh, *m_fullscreen;
    QPushButton *m_download, *m_share;

};

#endif // MUSICVIDEOFLOATWIDGET_H
