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
class MusicSongsListPlayedTableWidget;

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
     * Clear music data list.
     */
    void clear();
    /*!
     * Get current played list.
     */
    void resetToolIndex(const QList< std::pair<int, int> > &indexs);

    /*!
     * Get current played list.
     */
    QStringList getPlayedList() const;

    /*!
     * Remove music from data list.
     */
    void remove(int toolIndex, const MusicSong &song);
    /*!
     * Remove given music list from data list.
     */
    void remove(int toolIndex, const MusicSongs &songs);
    /*!
     * Append music to data list.
     */
    void append(int toolIndex, const MusicSong &song);
    /*!
     * Append music list to data list.
     */
    void append(int toolIndex, const MusicSongs &songs);
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
    void setCurrentIndex(const QString &path);
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
    void setDeleteItemAt();
    /*!
     * Delete all items from list.
     */
    void setDeleteItemAll();

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

    QList<QLabel*> m_labels;
    MusicSongs m_songLists;
    QScrollArea *m_scrollArea;
    MusicSongsListPlayedTableWidget *m_playedListWidget;

    static MusicPlayedListPopWidget *m_instance;
};

#endif // MUSICPLAYEDLISTPOPWIDGET_H
