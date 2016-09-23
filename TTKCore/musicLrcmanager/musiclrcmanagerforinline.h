#ifndef MUSICLRCMANAGERFORINLINE_H
#define MUSICLRCMANAGERFORINLINE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclrcmanager.h"

#define LRC_PER_WIDTH 670

/*! @brief The class of the inline lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLRCManagerForInline : public MusicLRCManager
{
    Q_OBJECT
public:
    explicit MusicLRCManagerForInline(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    inline void setLrcPerWidth(int width) { m_lrcPerWidth = width + LRC_PER_WIDTH;}
    /*!
     * Set lrc per width.
     */
    inline void setFontSize(int size) { m_gradientFontSize = size;}
    /*!
     * Set adjust font size.
     */
    inline void setTransparent(int tran) { m_gradientTransparent = tran;}
    /*!
     * Set adjust transparent by value.
     */
    inline void setCenterOnLrc(bool status) { m_centerOnLrc = status;}
    /*!
     * Set center current lrc on or not.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    int m_gradientFontSize;
    int m_gradientTransparent;
    bool m_centerOnLrc;

};

#endif // MUSICLRCMANAGERFORINLINE_H
