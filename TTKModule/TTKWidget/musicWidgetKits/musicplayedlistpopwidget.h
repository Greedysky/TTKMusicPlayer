#ifndef MUSICPLAYEDLISTPOPWIDGET_H
#define MUSICPLAYEDLISTPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsong.h"
#include "musictoolmenuwidget.h"

class QLabel;
class QScrollArea;
class MusicPlayedlist;
class MusicSongsListPlayedTableWidget;

#define PairItem(a, b) std::pair<int, int>(a, b);
typedef QList< std::pair<int, int> > PairList;

/*! @brief The class of the played list pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlayedListPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPlayedListPopWidget(QWidget *parent = 0);

    virtual ~MusicPlayedListPopWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Get class object instance.
     */
    static MusicPlayedListPopWidget *instance();

    /*!
     * Set current play list.
     */
    void setPlaylist(MusicPlayedlist *playlist);
    /*!
     * Get current play list.
     */
    MusicPlayedlist *playlist() const;

    /*!
     * Clear music data list.
     */
    void clear();
    /*!
     * Get current played list.
     */
    void resetToolIndex(const PairList &indexs);

    /*!
     * Remove music from data list.
     */
    void remove(int index);
    /*!
     * Remove music from data list.
     */
    void remove(int toolIndex, const QString &path);
    /*!
     * Remove music from data list.
     */
    void remove(int toolIndex, const MusicSong &song);

    /*!
     * Append music to data list.
     */
    void append(int toolIndex, const MusicSong &song);
    /*!
     * Append music to data list.
     */
    void append(const MusicSongs &song);

    /*!
     * Insert music after played music index.
     */
    void insert(int toolIndex, const MusicSong &song);
    /*!
     * Insert music by given index.
     */
    void insert(int toolIndex, int index, const MusicSong &song);

    /*!
     * Set current select played music index.
     */
    void setCurrentIndex();
    /*!
     * Set current select played music index.
     */
    void setCurrentIndex(int toolIndex, const MusicSong &song);
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();

public Q_SLOTS:
    /*!
     * To popup menu.
     */
    void popupMenu();
    /*!
     * Delete item from list at current row.
     */
    void setDeleteItemAt(int index);
    /*!
     * Delete all items from list.
     */
    void setDeleteItemAll();
    /*!
     * List cell double clicked.
     */
    void cellDoubleClicked(int row, int column);

protected:
    /*!
     * Create all widget in layout.
     */
    void initWidget();
    /*!
     * Create container widget.
     */
    QWidget *createContainerWidget();
    /*!
     * Update songs file names.
     */
    void updateSongsFileName();
    /*!
     * Set play list count.
     */
    void setPlayListCount(int count);
    /*!
     * Set play state to pause or stop.
     */
    void setPlayEmpty();

    QList<QLabel*> m_labels;
    MusicPlayedlist *m_playlist;
    MusicSongs m_songLists;
    QScrollArea *m_scrollArea;
    MusicSongsListPlayedTableWidget *m_playedListWidget;

    static MusicPlayedListPopWidget *m_instance;
};

#endif // MUSICPLAYEDLISTPOPWIDGET_H
