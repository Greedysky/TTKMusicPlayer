#ifndef MUSICVIDEOPLAYWIDGET_H
#define MUSICVIDEOPLAYWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include "ttkabstractmovewidget.h"
#include "musicvideosearchtablewidget.h"

class QLabel;
class QPushButton;
class QToolButton;
class QStackedWidget;
class QParallelAnimationGroup;
class MusicVideoView;
class MusicVideoFloatWidget;
class MusicItemQueryEdit;

/*! @brief The class of the video play widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicVideoPlayWidget : public TTKAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVideoPlayWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicVideoPlayWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicVideoPlayWidget();

    /*!
     * Set the window is popup or not.
     */
    void popupMode(bool popup);
    /*!
     * Get the window is popup or not.
     */
    bool isPopupMode() const;

    /*!
     * Resize window bound by resize called.
     */
    void resizeWindow();
    /*!
     * Resize geometry bound by resize called.
     */
    void resizeGeometry(bool resize);
    /*!
     * Resize geometry bound by resize called.
     */
    void resizeGeometry(int width, int height);

Q_SIGNALS:
    /*!
     * Popup button state changed.
     */
    void popupButtonClicked(bool popup);
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
    void startToSearchByID(const QString &name);
    /*!
     * Set current media url by selected quality.
     */
    void mediaUrlChanged(const QString &url);
    /*!
     * Set current media name and url to play.
     */
    void mediaUrlPathChanged(const MusicVideoItem &item);

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

private Q_SLOTS:
    /*!
     * Leave Timeout.
     */
    void leaveTimeout();

private:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;
    virtual void enterEvent(QtEnterEvent *event) override final;
    virtual void leaveEvent(QEvent *event) override final;
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
    QPushButton *m_closeButton;
    QStackedWidget *m_stackedWidget;
    MusicVideoItem m_videoItem;
    MusicVideoView *m_videoView;
    MusicVideoSearchTableWidget *m_videoTable;
    MusicItemQueryEdit *m_searchEdit;
    MusicVideoFloatWidget *m_videoFloatWidget;

};

#endif //MUSICVIDEOPLAYWIDGET_H

