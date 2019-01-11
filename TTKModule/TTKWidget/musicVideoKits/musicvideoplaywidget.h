#ifndef MUSICVIDEOPLAYWIDGET_H
#define MUSICVIDEOPLAYWIDGET_H

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

#include "musicabstractmovewidget.h"
#include "musicvideosearchtablewidget.h"

class QLabel;
class QPushButton;
class QToolButton;
class QStackedWidget;
class QParallelAnimationGroup;
class MusicVideoView;
class MusicVideoFloatWidget;
class MusicLocalSongSearchEdit;

/*! @brief The class of the video play widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoPlayWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVideoPlayWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicVideoPlayWidget(QWidget *parent = nullptr);

    ~MusicVideoPlayWidget();

    /*!
     * Set the window is popup or not.
     */
    void popup(bool popup);
    /*!
     * Get the window is popup or not.
     */
    bool isPopup() const;
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();
    /*!
     * Resize widget size or not.
     */
    void resizeWindow(bool resize);
    /*!
     * Resize widget size or not.
     */
    void resizeWindow(int width, int height);
    /*!
     * Get search text in search line.
     */
    QString getSearchText() const;

Q_SIGNALS:
    /*!
     * Fresh button state changed.
     */
    void freshButtonClicked(bool popup);
    /*!
     * Fullscreen button clicked.
     */
    void fullscreenButtonClicked(bool popup);

public Q_SLOTS:
    /*!
     * Switch to search table.
     */
    void switchToSearchTable();
    /*!
     * Switch to play view.
     */
    void switchToPlayView();
    /*!
     * Search button clicked.
     */
    void searchButtonClicked();
    /*!
     * Video research button searched by name.
     */
    void videoResearchButtonSearched(const QString &name);
    /*!
     * Video research button searched by name.
     */
    void videoResearchButtonSearched(const QVariant &data);
    /*!
     * Video search query by given id.
     */
    void startSearchSingleQuery(const QString &name);
    /*!
     * Set current media url by selected quality.
     */
    void mediaUrlChanged(const QString &url);
    /*!
     * Set current media name and url to play.
     */
    void mediaUrlNameChanged(const MusicVideoItem &item);

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

private Q_SLOTS:
    /*!
     * Leave Timeout.
     */
    void leaveTimeout();

protected:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Set current title text(song name).
     */
    void setTitleText(const QString &text);
    /*!
     * Start to turn on animation.
     */
    void start(int topst, int topend, int ctrlst, int ctrlend);

    QTimer *m_leaverTimer;
    QParallelAnimationGroup *m_leaverAnimation;

    QWidget *m_topWidget;
    QLabel *m_textLabel;
    QToolButton *m_backButton;
    QPushButton *m_searchButton, *m_closeButton;
    QStackedWidget *m_stackedWidget;
    MusicVideoItem m_videoItem;
    MusicVideoView *m_videoView;
    MusicVideoSearchTableWidget *m_videoTable;
    MusicLocalSongSearchEdit *m_searchEdit;
    MusicVideoFloatWidget *m_videoFloatWidget;

};

#endif //MUSICVIDEOPLAYWIDGET_H

