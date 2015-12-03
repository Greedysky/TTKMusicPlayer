#ifndef MUSICSONGSSUMMARIZIED_H
#define MUSICSONGSSUMMARIZIED_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QToolBox>
#include <QContextMenuEvent>
#include "musicsong.h"
#include "musiclibexportglobal.h"
#include "musicobject.h"

class MusicSongsListWidget;
class QTableWidgetItem;
class MusicSongsToolItemRenamedWidget;

class MUSIC_TOOL_EXPORT MusicSongsSummarizied : public QToolBox
{
    Q_OBJECT
public:
    explicit MusicSongsSummarizied(QWidget *parent = 0);
    virtual ~MusicSongsSummarizied();

    void setMusicLists(const MusicSongsList &names);
    void setMusicSongsSearchedFileName(const MIntList &fileIndexs);
    void importOtherMusicSongs(const QStringList &filelist);

    QStringList getMusicSongsFileName(int index) const;
    QStringList getMusicSongsFilePath(int index) const;
    inline const MusicSongsList& getMusicLists() const  { return m_musicFileNames;}

    inline int getCurrentPlayToolIndex() const { return m_currentIndexs;}
    void currentMusicSongTreeIndexChanged(int index);
    void selectRow(int index);
    void setTimerLabel(const QString &time) const;
    void setPlaybackMode(MusicObject::SongPlayType mode) const;

signals:
    void deleteItemAt(MIntList list);
    void updatePlayLists(const QString &list);
    void showCurrentSong(int index);
    void updateMediaLists(const QStringList &list, int index);

public slots:
    void addNewItem();
    void deleteItem();
    void changItemName();
    void setCurrentIndex();
    void currentIndexChanged(int index);
    void addMusicSongToLovestListAt(int row);
    void addNetMusicSongToList(const QString &name, const QString &time,
                               const QString &format);
    void setDeleteItemAt(const MIntList &index, bool fileRemove);
    void setMusicIndexSwaped(int before, int after, int play, QStringList &list);
    void isCurrentIndexs(bool &state);
    void setChangItemName(const QString &name);
    void setTransparent(int alpha);
    void setMusicPlayCount(int index);

protected:
    void changeItemIcon();
    void clearAllLists();
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    QList<MusicSongsListWidget*> m_mainSongLists;
    MusicSongsList m_musicFileNames;
    int m_currentIndexs;
    int m_renameIndex;
    MusicSongsToolItemRenamedWidget *m_renameLine;

};

#endif // MUSICSONGSSUMMARIZIED_H
