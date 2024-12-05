#ifndef MUSICVIDEOFLOATWIDGET_H
#define MUSICVIDEOFLOATWIDGET_H

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

#include "musicabstractfloatwidget.h"

class QPushButton;

/*! @brief The class of the video float widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicVideoFloatWidget : public MusicAbstractFloatWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVideoFloatWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicVideoFloatWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicVideoFloatWidget();

    /*!
     * Resize geometry bound by resize called.
     */
    void resizeGeometry(int width, int height);

    /*!
     * Set the window is popup or not.
     */
    bool isPopupMode() const;
    /*!
     * Get the window is popup or not.
     */
    void popupMode(bool popup);
    /*!
     * Set the window is fullscreen or not.
     */
    bool isFullscreenMode() const;
    /*!
     * Set the window is fullscreen or not.
     */
    void fullscreenMode(bool full);

Q_SIGNALS:
    /*!
     * Search button clicked.
     */
    void searchButtonClicked();
    /*!
     * Popup button clicked.
     */
    void popupButtonClicked();
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

private:
    QPushButton *m_search, *m_popup, *m_fullscreen;
    QPushButton *m_download, *m_share;

};

#endif // MUSICVIDEOFLOATWIDGET_H
