#ifndef MUSICPLAYLISTFOUNDINFOWIDGET_H
#define MUSICPLAYLISTFOUNDINFOWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include "musicfoundabstractwidget.h"
#include "musicdownloadquerythreadabstract.h"

class QPushButton;
class QStackedWidget;
class MusicPlaylistFoundTableWidget;
class MusicPlaylistFoundCommentsWidget;

/*! @brief The class of the playlist music found info widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlaylistFoundInfoWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPlaylistFoundInfoWidget(QWidget *parent = 0);

    virtual ~MusicPlaylistFoundInfoWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow();

    /*!
     * Set current id to search founds.
     */
    virtual void setSongNameById(const QString &id) override;

    /*!
     * Set music playlist item.
     */
    void setMusicPlaylistItem(const MusicPlaylistItem &item, QObject *obj);
    /*!
     * Set network query input.
     */
    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);

public Q_SLOTS:
    /*!
     * Set current container index.
     */
    void setCurrentIndex(int index);
    /*!
     * Query all quality musics is finished.
     */
    void queryAllFinished();
    /*!
     * Query all quality musics is finished.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Play all button clicked now.
     */
    void playAllButtonClicked();
    /*!
     * Share button clicked now.
     */
    void shareButtonClicked();
    /*!
     * Play button clicked now.
     */
    void playButtonClicked();
    /*!
     * Download button clicked now.
     */
    void downloadButtonClicked();
    /*!
     * Add button clicked now.
     */
    void addButtonClicked();

protected:
    /*!
     * Init the first widget.
     */
    void initFirstWidget();
    /*!
     * Init the second widget.
     */
    void initSecondWidget();
    /*!
     * Init the third widget.
     */
    void initThirdWidget();

    QLabel *m_iconLabel, *m_infoLabel;
    QStackedWidget *m_container;
    QPushButton *m_songButton;
    MusicPlaylistFoundTableWidget *m_playlistTableWidget;
    MusicPlaylistFoundCommentsWidget *m_commentsWidget;

};

#endif // MUSICPLAYLISTFOUNDINFOWIDGET_H
