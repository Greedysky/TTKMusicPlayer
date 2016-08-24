#ifndef MUSICLRCMANAGERFORDESKTOP_H
#define MUSICLRCMANAGERFORDESKTOP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclrcmanager.h"

/*! @brief The class of the desktop lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLRCManagerForDesktop : public MusicLRCManager
{
    Q_OBJECT
public:
    explicit MusicLRCManagerForDesktop(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setSelfGeometry(const QPoint &point);
    /*!
     * Set self geometry by point.
     */
    void setSelfGeometry(int x, int y);
    /*!
     * Set self geometry by x and y.
     */
    int x() const;
    /*!
     * Get self geometry x.
     */
    int y() const;
    /*!
     * Get self geometry y.
     */
    void resetOrigin();
    /*!
     * Reset to origin state.
     */
    void setLrcFontSize(int size);
    /*!
     * Set adjust font szie by value.
     */
    inline int getFirstFontSize() const { return m_font.pointSize(); }
    /*!
     * Get current font szie.
     */

};


/*! @brief The class of the desktop horizontal lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLRCManagerHorizontalDesktop : public MusicLRCManagerForDesktop
{
    Q_OBJECT
public:
    explicit MusicLRCManagerHorizontalDesktop(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */
};


/*! @brief The class of the desktop vertical lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLRCManagerVerticalDesktop : public MusicLRCManagerForDesktop
{
    Q_OBJECT
public:
    explicit MusicLRCManagerVerticalDesktop(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */
};

#endif // MUSICLRCMANAGERFORDESKTOP_H
