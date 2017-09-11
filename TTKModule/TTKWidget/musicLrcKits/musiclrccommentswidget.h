#ifndef MUSICLRCCOMMENTSWIDGET_H
#define MUSICLRCCOMMENTSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicdownloadcommentsthread.h"

class QTextEdit;
class MusicPagingWidgetObject;

/*! @brief The class of the song comment item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcCommentsItem : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLrcCommentsItem(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    ~MusicLrcCommentsItem();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void createSearchedItems(const MusicSongCommentItem &comments);
    /*!
     * Create the current song comment.
     */

private Q_SLOTS:
    void iconDataDownloadFinished(const QByteArray &data);
    /*!
     * Icon data download finished.
     */

protected:
    QTextEdit *m_userCommit;
    QLabel *m_userName, *m_timerLabel, *m_iconLabel, *m_starLabel;

};


/*! @brief The class of the song comment widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcCommentsWidget : public QLabel
{
    Q_OBJECT
public:
    explicit MusicLrcCommentsWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicLrcCommentsWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setCurrentSongName(const QString &name);
    /*!
     * Set current song name.
     */

public Q_SLOTS:
    void createSearchedItems(const MusicSongCommentItem &comments);
    /*!
     * Create the current song comment.
     */
    void buttonClicked(int index);
    /*!
     * Paging widget button has changed.
     */
    void createEMOJILabelWidget();
    /*!
     * Create emoji label widget.
     */
    void currentEMOJIchanged(const QString &data);
    /*!
     * Current EMOJI changed.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    /*!
     * Override the widget event.
     */
    void initLabel(const QString &name, int total);
    /*!
     * Int current comment label text.
     */
    void deleteCommentsItems();
    /*!
     * Delete comments items.
     */
    void createPagingWidget();
    /*!
     * Create paging items.
     */

    QTextEdit *m_messageEdit;
    QLabel *m_topLabel, *m_commentsLabel;
    QWidget *m_messageComments;
    MusicDownLoadCommentsThread *m_commentsThread;
    QList<MusicLrcCommentsItem*> m_commentsItems;
    MusicPagingWidgetObject *m_pagingWidgetObject;

};

#endif // MUSICLRCCOMMENTSWIDGET_H
