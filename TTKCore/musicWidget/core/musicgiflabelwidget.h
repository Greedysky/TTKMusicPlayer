#ifndef MUSICGIFLABELWIDGET_H
#define MUSICGIFLABELWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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
    enum Type{
        Gif_Ballon_White,           ///gif ballon white
        Gif_Cicle_Blue,             ///gif cicle blue
        Gif_Rice_Font_White,        ///gif rice font white
        Gif_Rice_Font_Black,        ///gif rice font black
        Gif_Rice_Font_Black_Big,    ///gif rice font black big
        Gif_Hourglass_White,        ///gif hourglass white
        Gif_Radio_Blue              ///gif radio blue
    };
    explicit MusicGifLabelWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    explicit MusicGifLabelWidget(Type type, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicGifLabelWidget();

    void setType(Type type);
    /*!
     * Set the gif type.
     */
    Type getType() const;
    /*!
     * Get the gif type.
     */

    void setInterval(int value);
    /*!
     * Set the gif interval.
     */
    int getInterval() const;
    /*!
     * Get the gif interval.
     */

    void start();
    /*!
     * Start the gif.
     */
    void stop();
    /*!
     * Stop the gif.
     */

public Q_SLOTS:
    void timeout();
    /*!
     * Change the current gif index.
     */

protected:
    int m_index;
    Type m_type;
    QTimer *m_timer;

};

#endif // MUSICGIFLABELWIDGET_H
