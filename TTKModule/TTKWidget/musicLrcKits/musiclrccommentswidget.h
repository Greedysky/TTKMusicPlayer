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
    /*!
     * Object contsructor.
     */
    explicit MusicLrcCommentsItem(QWidget *parent = 0);

    ~MusicLrcCommentsItem();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Create the current song comment.
     */
    void createSearchedItems(const MusicSongCommentItem &comments);

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
class MUSIC_LRC_EXPORT MusicLrcCommentsWidget : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcCommentsWidget(QWidget *parent = 0);

    ~MusicLrcCommentsWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current song name.
     */
    void setCurrentSongName(const QString &name);

public Q_SLOTS:
    /*!
     * Create the current song comment.
     */
    void createSearchedItems(const MusicSongCommentItem &comments);
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
    virtual void wheelEvent(QWheelEvent *event) override;
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

    QTextEdit *m_messageEdit;
    QLabel *m_topLabel, *m_commentsLabel;
    QWidget *m_messageComments;
    MusicDownLoadCommentsThread *m_commentsThread;
    QList<MusicLrcCommentsItem*> m_commentsItems;
    MusicPagingWidgetObject *m_pagingWidgetObject;

};

#endif // MUSICLRCCOMMENTSWIDGET_H
