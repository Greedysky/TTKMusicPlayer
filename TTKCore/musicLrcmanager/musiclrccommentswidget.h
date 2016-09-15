#ifndef MUSICLRCCOMMENTSWIDGET_H
#define MUSICLRCCOMMENTSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicglobaldefine.h"

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

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    QLabel *m_topLabel;
    QString m_currentSongName;

};

#endif // MUSICLRCCOMMENTSWIDGET_H
