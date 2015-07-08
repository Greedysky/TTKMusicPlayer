#ifndef MUSICLRCSEARCHTABLEWIDGET_H
#define MUSICLRCSEARCHTABLEWIDGET_H

#include "core/musicdownloadmanagerthread.h"
#include "core/musictablewidgetabstract.h"

class MusicLrcDownLoadThread;

class MUSIC_EXPORT MusicLrcSearchTableWidget : public MusicTableWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicLrcSearchTableWidget(QWidget *parent = 0);
    virtual ~MusicLrcSearchTableWidget();

    void startSearchLrc(const QString&);
    void musicDownloadLocal(int);

signals:
    void lrcDownloadStateChanged(const QString&);

public slots:
    void clearAllItems();
    void creatSearchedItems(const QString&, const QString&, double);
    void itemDoubleClicked(int row, int column);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

    MusicDownLoadManagerThread* m_downLoadManager;
    int m_songItemIndex;

};

#endif // MUSICLRCSEARCHTABLEWIDGET_H
