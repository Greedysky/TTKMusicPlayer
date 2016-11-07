#ifndef MUSICPLAYEDLISTWIDGET_H
#define MUSICPLAYEDLISTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictoolmenuwidget.h"

class QLabel;
class MusicSongsPlayedListWidget;

/*! @brief The class of the played list widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlayedListWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicPlayedListWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicPlayedListWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setPlayListCount(int count);
    /*!
     * Set play list count.
     */

public Q_SLOTS:
    void popupMenu();
    /*!
     * To popup menu.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

    QList<QLabel*> m_labels;
    MusicSongsPlayedListWidget *m_musicPlayedListWidget;

};

#endif // MUSICPLAYEDLISTWIDGET_H
