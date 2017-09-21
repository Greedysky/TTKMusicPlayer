#ifndef MUSICPLAYEDLIST_H
#define MUSICPLAYEDLIST_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the music played item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_CORE_EXPORT MusicPlayedItem
{
    int m_toolIndex;
    QString m_path;

    MusicPlayedItem()
    {
        m_toolIndex = -1;
    }

    MusicPlayedItem(int index, const QString &path)
    {
        m_toolIndex = index;
        m_path = path;
    }

    bool isValid() const
    {
        return m_toolIndex != -1 && !m_path.isEmpty();
    }

    bool operator== (const MusicPlayedItem &other) const
    {
        return m_toolIndex == other.m_toolIndex && m_path == other.m_path;
    }
}MusicPlayedItem;
MUSIC_DECLARE_LISTS(MusicPlayedItem)


/*! @brief The class of the music playedlist.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicPlayedlist : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlayedlist(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Get current play mode.
     */
    MusicObject::PlayMode playbackMode() const;
    /*!
     * Set current play mode.
     */
    void setPlaybackMode(MusicObject::PlayMode mode);

    /*!
     * Map item index at container.
     */
    int mapItemIndex(const MusicPlayedItem &item) const;
    /*!
     * Get current play index.
     */
    int currentIndex() const;
    /*!
     * Get current play item.
     */
    MusicPlayedItem currentItem() const;
    /*!
     * Get current play music media path.
     */
    QString currentMediaString() const;

    /*!
     * Get all music media path.
     */
    MusicPlayedItems mediaListConst() const;
    /*!
     * Get all music media path.
     */
    MusicPlayedItems *mediaList();
    /*!
     * Get current medias count.
     */
    int mediaCount() const;
    /*!
     * Check current medias is empty.
     */
    bool isEmpty() const;
    /*!
     * Clear current medias.
     */
    bool clear();
    /*!
     * Find item by index and content.
     */
    int find(int toolIndex, const QString &content, int from = 0);

    /*!
     * Add music media, not append remember.
     */
    void addMedia(int toolIndex, const QString &content);
    /*!
     * Add music media list, not append remember.
     */
    void addMedia(int toolIndex, const QStringList &items);
    /*!
     * Add music media list, not append remember.
     */
    void addMedia(const MusicPlayedItem &item);
    /*!
     * Add music media list, not append remember.
     */
    void addMedia(const MusicPlayedItems &items);

    /*!
     * Append music medias.
     */
    void appendMedia(int toolIndex, const QString &content);
    /*!
     * Append music medias.
     */
    void appendMedia(int toolIndex, const QStringList &items);
    /*!
     * Append music media.
     */
    void appendMedia(const MusicPlayedItem &item);
    /*!
     * Append music medias.
     */
    void appendMedia(const MusicPlayedItems &items);

    /*!
     * Get later music media path.
     */
    MusicPlayedItems laterListConst() const;
    /*!
     * Insert music media by index and content.
     */
    void insertLaterMedia(int toolIndex, const QString &content);
    /*!
     * Remove music all media.
     */
    void laterListClear();

    /*!
     * Remove music media from current medias by index pos.
     */
    bool removeMedia(int pos);
    /*!
     * Remove music media from current medias by index pos.
     */
    int removeMedia(int toolIndex, const QString &content);

Q_SIGNALS:
    /*!
     * Current play index changed emit.
     */
    void currentIndexChanged(int index);
    /*!
     * Current play mode changed emit.
     */
    void playbackModeChanged(MusicObject::PlayMode mode);

public Q_SLOTS:
    /*!
     * Set current play index.
     */
    void setCurrentIndex(int index = DEFAULT_INDEX_LEVEL1);
    /*!
     * Set current play index.
     */
    void setCurrentIndex(int toolIndex, const QString &path);

protected:
    int m_currentIndex;
    MusicPlayedItems m_mediaList;
    MusicPlayedItems m_laterMediaList;
    MusicObject::PlayMode m_playbackMode;

};

#endif // MUSICPLAYEDLIST_H
