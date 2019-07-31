#ifndef MUSICCOMMENTSWIDGET_H
#define MUSICCOMMENTSWIDGET_H

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

#include <QLabel>
#include "musicdownloadcommentsthread.h"

class QTextEdit;
class MusicPagingWidgetObject;

/*! @brief The class of the song comment item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicCommentsItem : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCommentsItem)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicCommentsItem(QWidget *parent = nullptr);

    ~MusicCommentsItem();

    /*!
     * Create the current song comment.
     */
    void createSearchedItem(const MusicResultsItem &comments);

private Q_SLOTS:
    /*!
     * Icon data download finished.
     */
    void iconDataDownloadFinished(const QByteArray &data);

protected:
    QTextEdit *m_userCommit;
    QLabel *m_userName, *m_timerLabel, *m_iconLabel, *m_starLabel;

};


/*! @brief The class of the song comment widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicCommentsWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCommentsWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicCommentsWidget(QWidget *parent = nullptr);

    ~MusicCommentsWidget();

    /*!
     * Init widget.
     */
    virtual void initWidget(bool isPain);
    /*!
     * Set current song name.
     */
    void setCurrentSongName(const QString &name);

public Q_SLOTS:
    /*!
     * Create the current song comment.
     */
    void createSearchedItem(const MusicResultsItem &comments);
    /*!
     * Paging widget button has changed.
     */
    void buttonClicked(int index);
    /*!
     * Create emoji label widget.
     */
    void createEMOJILabelWidget();
    /*!
     * Current EMOJI changed.
     */
    void currentEMOJIchanged(const QString &data);

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    /*!
     * Int current comment label text.
     */
    void initLabel(const QString &name, int total);
    /*!
     * Delete comments items.
     */
    void deleteCommentsItems();
    /*!
     * Create paging items.
     */
    void createPagingWidget();

    bool m_isPain;
    QTextEdit *m_messageEdit;
    QLabel *m_topLabel, *m_commentsLabel;
    QWidget *m_messageComments;
    MusicDownLoadCommentsThread *m_commentsThread;
    QList<MusicCommentsItem*> m_commentsItems;
    MusicPagingWidgetObject *m_pagingWidgetObject;

};

#endif // MUSICCOMMENTSWIDGET_H
