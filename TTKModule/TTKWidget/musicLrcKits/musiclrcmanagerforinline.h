#ifndef MUSICLRCMANAGERFORINLINE_H
#define MUSICLRCMANAGERFORINLINE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclrcmanager.h"

#define LRC_PER_WIDTH 670

/*! @brief The class of the inline lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcManagerForInline : public MusicLrcManager
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcManagerForInline(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set lrc per width.
     */
    inline void setLrcPerWidth(int width) { m_lrcPerWidth = width + LRC_PER_WIDTH;}
    /*!
     * Set adjust font size.
     */
    inline void setFontSize(int size) { m_gradientFontSize = size;}
    /*!
     * Set adjust transparent by value.
     */
    inline void setTransparent(int tran) { m_gradientTransparent = tran;}
    /*!
     * Set center current lrc on or not.
     */
    inline void setCenterOnLrc(bool status) { m_centerOnLrc = status;}

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    int m_gradientFontSize;
    int m_gradientTransparent;
    bool m_centerOnLrc;

};

#endif // MUSICLRCMANAGERFORINLINE_H
