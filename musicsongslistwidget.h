#ifndef MUSICSONGSLISTWIDGET_H
#define MUSICSONGSLISTWIDGET_H

#include <QMenu>
#include <QContextMenuEvent>
#include "core/musicsonginformation.h"
#include "core/musictablewidgetabstract.h"

class QAction;
class QTimer;
class MusicSongsListPlayWidget;
class MusicSongsListItemInformation;

class MUSIC_EXPORT MusicSongsListWidget :
        public MusicTableWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicSongsListWidget(QWidget *parent = 0);
    virtual ~MusicSongsListWidget();

    void musicSongsFileName(const QStringList& filenamelists);
    void clearAllItems();
    void selectRow(int index);
    inline void setPlaybackMode(MusicObject::SongPlayType type)
        { m_songplaymode = type;}
    void setTimerLabel(const QString& t);
    void replacePlayWidgetRow();

signals:
    void deleteItemAt(const QList<int>&,bool fileRemove);
    void MusicSongToLovestListAt(int);
    void currentTextChanged(int,const QString&);
    void mucellDoubleClicked(int,int);

    void musicPlay(int);
    void musicPlayOrder();
    void musicPlayRandom();
    void musicPlayListLoop();
    void musicPlayOneLoop();
    void musicPlayItemOnce();
    void musicAddNewFiles();
    void musicAddNewDir();
    void musicOpenFileDir(int);

    void getMusicSongInformation(int,QMusicSong&);
    void getMusicSongFileInformation(int, QString&, QString&, bool = true);
    void getMusicIndexSwaped(int,int,int,QStringList&);

public slots:
    void musicPlay();
    void setDeleteItemAt();
    void setDeleteItemAll();
    void setDeleteItemWithFile();
    void listCellEntered(int row, int column);
    void listCellClicked(int row, int column);
    void showTimeOut();
    void stayTimeOut();
    void addMusicSongToLovestListAt();
    void addPlaySongToLovestListAt();
    void setChangSongName();
    void musicOpenFileDir();
    void musicMakeRingWidget();
    void musicTransformWidget();
    void musicFileInformation();
    void setItemRenameFinished(const QString&);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);

    QTimer* m_timerShow;
    QTimer* m_timerStay;

    int m_playRowIndex;
    int m_dragStartIndex;
    bool m_mouseMoved;

    MusicSongsListItemInformation* m_musicSongsListItem;
    MusicSongsListPlayWidget* m_musicSongsPlayWidget;

    bool m_leftButtonPressed;
    bool m_renameActived;
    QTableWidgetItem* m_renameItem;
    bool m_deleteItemWithFile;
    MusicObject::SongPlayType m_songplaymode;

};

#endif // MUSICSONGSLISTWIDGET_H
