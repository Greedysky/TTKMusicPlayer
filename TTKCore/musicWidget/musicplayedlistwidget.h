#ifndef MUSICPLAYEDLISTWIDGET_H
#define MUSICPLAYEDLISTWIDGET_H

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
class MusicSongsListPlayedTableWidget;

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

    void clear();
    /*!
     * Clear music data list.
     */
    void resetToolIndex(const QList< std::pair<int, int> > &indexs);
    /*!
     * Get current played list.
     */

    QStringList getPlayedList() const;
    /*!
     * Get current played list.
     */

    void remove(int toolIndex, const MusicSong &song);
    /*!
     * Remove music from data list.
     */
    void remove(int toolIndex, const MusicSongs &songs);
    /*!
     * Remove given music list from data list.
     */
    void append(int toolIndex, const MusicSong &song);
    /*!
     * Append music to data list.
     */
    void append(int toolIndex, const MusicSongs &songs);
    /*!
     * Append music list to data list.
     */
    void insert(int toolIndex, const MusicSong &song);
    /*!
     * Insert music after played music index.
     */
    void insert(int toolIndex, int index, const MusicSong &song);
    /*!
     * Insert music by given index.
     */

    void setCurrentIndex(const QString &path);
    /*!
     * Set current select played music index.
     */
    void resizeWindow();
    /*!
     * Resize window bound by widgte resize called.
     */

public Q_SLOTS:
    void popupMenu();
    /*!
     * To popup menu.
     */
    void setDeleteItemAt();
    /*!
     * Delete item from list at current row.
     */
    void setDeleteItemAll();
    /*!
     * Delete all items from list.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */
    QWidget *createContainerWidget();
    /*!
     * Create container widget.
     */
    void updateSongsFileName();
    /*!
     * Update songs file names.
     */
    void setPlayListCount(int count);
    /*!
     * Set play list count.
     */

    QList<QLabel*> m_labels;
    MusicSongs m_songLists;
    MusicSongsListPlayedTableWidget *m_playedListWidget;

    static MusicPlayedListWidget *m_instance;
};

#endif // MUSICPLAYEDLISTWIDGET_H
