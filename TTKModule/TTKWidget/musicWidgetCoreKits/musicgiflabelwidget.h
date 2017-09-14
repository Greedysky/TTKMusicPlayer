#ifndef MUSICGIFLABELWIDGET_H
#define MUSICGIFLABELWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicglobaldefine.h"

class QTimer;

/*! @brief The class of the float gif label widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicGifLabelWidget : public QLabel
{
    Q_OBJECT
public:
    enum Type
    {
        Gif_Ballon_White,           /*!< gif ballon white*/
        Gif_Cicle_Blue,             /*!< gif cicle blue*/
        Gif_Rice_Font_White,        /*!< gif rice font white*/
        Gif_Rice_Font_Black,        /*!< gif rice font black*/
        Gif_Rice_Font_Black_Big,    /*!< gif rice font black big*/
        Gif_Hourglass_White,        /*!< gif hourglass white*/
        Gif_Radio_Blue,             /*!< gif radio blue*/
        Gif_Check_Blue,             /*!< gif check blue*/
        Gif_Record_red,             /*!< gif record red*/
        Gif_Close_White             /*!< gif close white*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicGifLabelWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    explicit MusicGifLabelWidget(Type type, QWidget *parent = 0);

    ~MusicGifLabelWidget();

    /*!
     * Set the gif type.
     */
    void setType(Type type);
    /*!
     * Get the gif type.
     */
    Type getType() const;

    /*!
     * Set the gif interval.
     */
    void setInterval(int value);

    /*!
     * Get the gif interval.
     */
    int getInterval() const;
    /*!
     * Set the gif infinited mode.
     */
    void setInfinited(bool s);
    /*!
     * Get the gif infinited mode.
     */
    bool getInfinited() const;

    /*!
     * Start the gif.
     */
    void start();
    /*!
     * Stop the gif.
     */
    void stop();
    /*!
     * Get current running state.
     */
    inline bool isRunning() const { return m_isRunning; }

public Q_SLOTS:
    /*!
     * Change the current gif index.
     */
    void timeout();

protected:
    /*!
     * Infinited mode check.
     */
    bool infinitedModeCheck();

    int m_index;
    Type m_type;
    QTimer *m_timer;
    bool m_isRunning, m_infinited;

};

#endif // MUSICGIFLABELWIDGET_H
