#ifndef MUSICSONGSSUMMARIZIED_H
#define MUSICSONGSSUMMARIZIED_H

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

    void musicSongsFileNameAndPath(const MStringLists &, const MStringLists &);
    void setMusicSongsSearchedFileName(const MIntList&);
    void importOtherMusicSongs(const QStringList&);
    inline QStringList& getMusicSongsFileName(int index) { return m_musicFileNameList[index];}
    inline QStringList& getMusicSongsFilePath(int index) { return m_musicFilePathList[index];}
    inline MStringLists& getMusicAllSongsFileName() { return m_musicFileNameList;}
    inline MStringLists& getMusicAllSongsFilePath() { return m_musicFilePathList;}
    inline int getCurrentPlayToolIndex() const { return m_currentIndexs;}
    void clearAllLists();
    void currentMusicSongTreeIndexChanged(int index);
    void selectRow(int);
    void setTimerLabel(const QString&);
    void updateArtPicture();
    QString itemText(int,int) const;
    void setPlaybackMode(MusicObject::SongPlayType);

signals:
    void musicPlay(int,int);
    void deleteItemAt(MIntList);
    void updatePlayLists(const QString&);
    void showCurrentSong(int);
    void updateMediaLists(const QStringList&,int);

public slots:
    void setCurrentIndex();
    void addMusicSongToLovestListAt(int);
    void addNetMusicSongToList(const QString&);
    void setDeleteItemAt(const MIntList&,bool);
    void addNewItem();
    void deleteItem();
    void changItemName();
    void setChangItemName(const QString&);
    void currentIndexChanged(int);
    void currentTextChanged(int,const QString&);
    void musicOpenFileDir(int index);
    void musicPlay(int);
    void setMusicSongInformation(int, QMusicSong&);
    void setMusicSongFileInformation(int, QString&, QString&, bool = true);
    void setMusicIndexSwaped(int,int,int,QStringList&);

protected:
    void changeItemIcon();
    virtual void contextMenuEvent(QContextMenuEvent *event);

    QList<MusicSongsListWidget*> m_mainSongLists;
    MStringLists m_musicFileNameList;
    MStringLists m_musicFilePathList;
    int m_currentIndexs;
    int m_renameIndex;
    MusicSongsToolItemRenamedWidget* m_renameLine;

};

#endif // MUSICSONGSSUMMARIZIED_H
