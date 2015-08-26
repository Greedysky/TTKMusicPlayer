#ifndef MUSICSONGSEARCHONLINEWIDGET_H
#define MUSICSONGSEARCHONLINEWIDGET_H

#include "musicdownloadquerythread.h"
#include "musictablequerywidget.h"

class MusicDataDownloadThread;
class MusicTextDownLoadThread;

class MUSIC_WIDGET_EXPORT MusicSongSearchOnlineWidget : public MusicTableQueryWidget
{
    Q_OBJECT
public:
    explicit MusicSongSearchOnlineWidget(QWidget *parent = 0);
    virtual ~MusicSongSearchOnlineWidget();

    void startSearchQuery(const QString &text);

signals:
    void musicBgDownloadFinished();

public slots:
    void listCellClicked(int row, int column);
    void clearAllItems();
    void creatSearchedItems(const QString &songname,
                            const QString &artistname,
                            const QString &time);
    void itemDoubleClicked(int row, int column);

protected:
    void addSearchMusicToPlayList(int row);
    void musicDownloadLocal(int row);

};

#endif // MUSICSONGSEARCHONLINEWIDGET_H
