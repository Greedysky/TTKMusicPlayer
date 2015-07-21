#ifndef MUSICSONGSEARCHONLINEWIDGET_H
#define MUSICSONGSEARCHONLINEWIDGET_H

#include "core/musicdownloadquerythread.h"
#include "core/musictablequerywidget.h"

class MusicDataDownloadThread;
class MusicTextDownLoadThread;

class MUSIC_EXPORT MusicSongSearchOnlineWidget : public MusicTableQueryWidget
{
    Q_OBJECT
public:
    explicit MusicSongSearchOnlineWidget(QWidget *parent = 0);
    virtual ~MusicSongSearchOnlineWidget();

    void startSearchQuery(const QString&);
    void addSearchMusicToPlayList(int);
    void musicDownloadLocal(int);

signals:
    void musicBgDownloadFinished();

public slots:
    void listCellClicked(int,int);
    void clearAllItems();
    void creatSearchedItems(const QString&, const QString&, const QString&);
    void itemDoubleClicked(int row, int column);

};

#endif // MUSICSONGSEARCHONLINEWIDGET_H
