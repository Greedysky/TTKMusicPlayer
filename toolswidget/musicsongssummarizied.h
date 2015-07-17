#ifndef MUSICSONGSSUMMARIZIED_H
#define MUSICSONGSSUMMARIZIED_H

#include <QToolBox>
#include <QScrollBar>
#include <QContextMenuEvent>
#include "../core/musicsonginformation.h"
#include "../core/musiclibexportglobal.h"
#include "../core/musicobject.h"

class MusicSongsListWidget;
class QTableWidgetItem;
class MusicSongsToolItemRenamedWidget;

class MUSIC_EXPORT MusicSongsSummarizied : public QToolBox
{
    Q_OBJECT
public:
    explicit MusicSongsSummarizied(QWidget *parent = 0);
    virtual ~MusicSongsSummarizied();

    void musicSongsFileNameAndPath(const QList<QStringList>&,const QList<QStringList>&);
    void setMusicSongsSearchedFileName(const QList<int>&);
    void importOtherMusicSongs(const QStringList&);
    inline QStringList& getMusicSongsFileName(int index) { return m_musicFileNameList[index];}
    inline QStringList& getMusicSongsFilePath(int index) { return m_musicFilePathList[index];}
    inline QList<QStringList>& getMusicAllSongsFileName() { return m_musicFileNameList;}
    inline QList<QStringList>& getMusicAllSongsFilePath() { return m_musicFilePathList;}
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
    void deleteItemAt(QList<int>);
    void updatePlayLists(const QString&);
    void showCurrentSong(int);
    void updateMediaLists(const QStringList&,int);

public slots:
    void setCurrentIndex();
    void addMusicSongToLovestListAt(int);
    void addNetMusicSongToList(const QString&);
    void setDeleteItemAt(const QList<int>&,bool);
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
    QList<QStringList> m_musicFileNameList;
    QList<QStringList> m_musicFilePathList;
    int m_currentIndexs;
    int m_renameIndex;
    MusicSongsToolItemRenamedWidget* m_renameLine;

};

#endif // MUSICSONGSSUMMARIZIED_H
