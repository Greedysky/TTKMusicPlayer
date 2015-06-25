#ifndef MUSICSONGSEARCHONLINEWIDGET_H
#define MUSICSONGSEARCHONLINEWIDGET_H

#include <QTableWidget>
#include <QHeaderView>
#include "core/musicdownloadmanagerthread.h"
#include "core/musiclibexportglobal.h"
#include "core/musictablewidgetabstract.h"

class MusicSongDownloadThread;
class MusicLrcDownLoadThread;

class MUSIC_EXPORT MusicSongSearchOnlineWidget :
        public MusicTableWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicSongSearchOnlineWidget(QWidget *parent = 0);
    virtual ~MusicSongSearchOnlineWidget();

    void startSearchSong(const QString&);
    void addSearchMusicToPlayList(int);
    void musicDownloadLocal(int);

signals:
    void showDownLoadInfoFinished(const QString&);
    void MuiscSongToPlayListChanged(const QString& name);
    void showDownLoadInfoFor(DownLoadType);

public slots:
    void listCellClicked(int,int);
    void clearAllItems();
    void creatSearchedItems(const QString&, const QString&, double);
    void itemDoubleClicked(int row, int column);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

    MusicDownLoadManagerThread* m_downLoadManager;
    int m_songItemIndex; //The index of the true QTableWiget item count

};

#endif // MUSICSONGSEARCHONLINEWIDGET_H
