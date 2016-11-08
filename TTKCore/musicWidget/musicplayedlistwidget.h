#ifndef MUSICPLAYEDLISTWIDGET_H
#define MUSICPLAYEDLISTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsong.h"
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
    static MusicPlayedListWidget *instance();
    /*!
     * Get class object instance.
     */

    void remove(const QString &path);
    void remove(const QStringList &paths);

    void append(const MusicSong &song);
    void append(const MusicSongs &songs);

    void resizeWindow();
    /*!
     * Resize window bound by widgte resize called.
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
    QWidget *createContainerWidget();
    void setPlayListCount(int count);
    /*!
     * Set play list count.
     */

    MusicSongs m_songLists;
    QList<QLabel*> m_labels;
    MusicSongsPlayedListWidget *m_playedListWidget;

    static MusicPlayedListWidget *m_instance;
};

#endif // MUSICPLAYEDLISTWIDGET_H
