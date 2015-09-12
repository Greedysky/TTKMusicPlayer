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
#include <QScrollBar>
#include <QContextMenuEvent>
#include "musicsonginformation.h"
#include "musiclibexportglobal.h"
#include "musicobject.h"

class MusicSongsListWidget;
class QTableWidgetItem;
class MusicSongsToolItemRenamedWidget;

class MUSIC_WIDGET_EXPORT MusicSongsSummarizied : public QToolBox
{
    Q_OBJECT
public:
    explicit MusicSongsSummarizied(QWidget *parent = 0);
    virtual ~MusicSongsSummarizied();

    void musicSongsFileNameAndPath(const MStringLists &names, const MStringLists &urls);
    void setMusicSongsSearchedFileName(const MIntList &fileIndexs);
    void importOtherMusicSongs(const QStringList &filelist);
    inline QStringList& getMusicSongsFileName(int index) { return m_musicFileNameList[index];}
    inline QStringList& getMusicSongsFilePath(int index) { return m_musicFilePathList[index];}
    inline MStringLists& getMusicAllSongsFileName() { return m_musicFileNameList;}
    inline MStringLists& getMusicAllSongsFilePath() { return m_musicFilePathList;}
    inline int getCurrentPlayToolIndex() const { return m_currentIndexs;}
    void clearAllLists();
    void currentMusicSongTreeIndexChanged(int index);
    void selectRow(int index);
    void setTimerLabel(const QString &time) const;
    QString itemText(int row, int col) const;
    void setPlaybackMode(MusicObject::SongPlayType mode) const;

signals:
    void musicPlay(int row, int col);
    void deleteItemAt(MIntList list);
    void updatePlayLists(const QString &list);
    void showCurrentSong(int index);
    void updateMediaLists(const QStringList &list, int index);

public slots:
    void setCurrentIndex();
    void addMusicSongToLovestListAt(int row);
    void addNetMusicSongToList(const QString &name);
    void setDeleteItemAt(const MIntList &index, bool fileRemove);
    void addNewItem();
    void deleteItem();
    void changItemName();
    void setChangItemName(const QString &name);
    void currentIndexChanged(int index);
    void currentTextChanged(int index, const QString &text);
    void musicOpenFileDir(int index);
    void musicPlay(int index);
    void setMusicSongInformation(int row, QMusicSong &song);
    void setMusicSongFileInformation(int row, QString &name,
                                     QString &path, bool st = true);
    void setMusicIndexSwaped(int before, int after, int play,
                             QStringList &list);
    void updateArtPicture();
    void setTransparent(int alpha);

protected:
    void changeItemIcon();
    virtual void contextMenuEvent(QContextMenuEvent *event);

    QList<MusicSongsListWidget*> m_mainSongLists;
    MStringLists m_musicFileNameList;
    MStringLists m_musicFilePathList;
    int m_currentIndexs;
    int m_renameIndex;
    MusicSongsToolItemRenamedWidget *m_renameLine;

};

#endif // MUSICSONGSSUMMARIZIED_H
