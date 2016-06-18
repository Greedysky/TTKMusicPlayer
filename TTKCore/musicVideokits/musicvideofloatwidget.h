#ifndef MUSICVIDEOFLOATWIDGET_H
#define MUSICVIDEOFLOATWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfloatabstractwidget.h"

class QPushButton;

/*! @brief The class of the video float widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicVideoFloatWidget : public MusicFloatAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicVideoFloatWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicVideoFloatWidget();

    virtual void resizeWidth(int width) override;
    /*!
     * Resize width bound by given width.
     */

protected:
    QPushButton *m_more, *m_update, *m_search;
    QPushButton *m_wallp, *m_photo;

};

#endif // MUSICVIDEOFLOATWIDGET_H
