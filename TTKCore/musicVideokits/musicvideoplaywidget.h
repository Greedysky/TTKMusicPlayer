#ifndef MUSICVIDEOPLAYWIDGET_H
#define MUSICVIDEOPLAYWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovewidget.h"

class QLabel;
class QPushButton;
class QStackedWidget;
class MusicVideoView;
class MusicVideoTableWidget;
class MusicLocalSongSearchEdit;

/*! @brief The class of the video play widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoPlayWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    MusicVideoPlayWidget(bool popup, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicVideoPlayWidget();

    void resizeWindow(bool resize);
    /*!
     * Resize widget size or not.
     */
    void setObjectToClose(QObject *object);
    /*!
     * Set object to close current widget.
     */

public Q_SLOTS:
    void backButtonClicked();
    /*!
     * Stack backward button clicked.
     */
    void afterButtonClicked();
    /*!
     * Stack forward button clicked.
     */
    void searchButtonClicked();
    /*!
     * Search button clicked.
     */
    void videoResearchButtonSearched(const QString &name);
    /*!
     * Video research button searched by name.
     */
    void mvURLChanged(const QString &data);
    /*!
     * Set current media url by selected quality.
     */
    void mvURLNameChanged(const QString &name, const QString &data);
    /*!
     * Set current media name and url to play.
     */

protected:
    QWidget *m_topWidget;
    QLabel *m_textLabel;
    QPushButton *m_backButton, *m_afterButton;
    QPushButton *m_searchButton, *m_closeButton;
    QStackedWidget *m_stackedWidget;
    MusicVideoView *m_videoView;
    MusicVideoTableWidget *m_videoTable;
    MusicLocalSongSearchEdit *m_searchEdit;

};

#endif //MUSICVIDEOPLAYWIDGET_H

