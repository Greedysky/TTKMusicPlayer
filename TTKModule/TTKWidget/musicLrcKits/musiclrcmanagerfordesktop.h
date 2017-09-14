#ifndef MUSICLRCMANAGERFORDESKTOP_H
#define MUSICLRCMANAGERFORDESKTOP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclrcmanager.h"

/*! @brief The class of the desktop lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcManagerForDesktop : public MusicLrcManager
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcManagerForDesktop(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

};


/*! @brief The class of the desktop horizontal lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcManagerHorizontalDesktop : public MusicLrcManagerForDesktop
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcManagerHorizontalDesktop(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

};


/*! @brief The class of the desktop vertical lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcManagerVerticalDesktop : public MusicLrcManagerForDesktop
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcManagerVerticalDesktop(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

};

#endif // MUSICLRCMANAGERFORDESKTOP_H
