#ifndef MUSICADVANCEDSEARCHEDWIDGET_H
#define MUSICADVANCEDSEARCHEDWIDGET_H

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

#include <QTabWidget>
#include "musicglobaldefine.h"

class MusicLocalSongSearchEdit;

/*! @brief The class of the advance search widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAdvancedSearchedWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAdvancedSearchedWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicAdvancedSearchedWidget(QWidget *parent = nullptr);

    virtual ~MusicAdvancedSearchedWidget();

private Q_SLOTS:
    /*!
     * Search button clicked.
     */
    void searchButtonClicked();

protected:
    /*!
     * Create search pair widget.
     */
    QWidget *createSearchPairWidget(MusicLocalSongSearchEdit **input);
    /*!
     * Update placeholder text.
     */
    void updateServerPlaceholderText();
    /*!
     * Get search key.
     */
    QString getSearchedKeyWork(int type, const QString &url);
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    QTabWidget *m_tabWidget;
    MusicLocalSongSearchEdit *m_songEdit, *m_artistEdit;
    MusicLocalSongSearchEdit *m_albumEdit, *m_playlistEdit, *m_movieEdit;

};

#endif // MUSICADVANCEDSEARCHEDWIDGET_H
