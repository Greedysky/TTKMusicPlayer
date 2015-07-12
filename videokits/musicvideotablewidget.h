#ifndef MUSICVIDEOTABLEWIDGET_H
#define MUSICVIDEOTABLEWIDGET_H

#include "../core/musicdownloadmanagerthread.h"
#include "../core/musictablewidgetabstract.h"

class MusicLrcDownLoadThread;

class MUSIC_EXPORT MusicVideoTableWidget : public MusicTableWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicVideoTableWidget(QWidget *parent = 0);
    virtual ~MusicVideoTableWidget();

    void startSearchMV(const QString&);
signals:
    void mvURLChanged(const QString&);

public slots:
    void listCellClicked(int row,int col);
    void clearAllItems();
    void creatSearchedItems(const QString&, const QString&, const QString&);
    void itemDoubleClicked(int row, int column);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    QString randToGetStrength();
    void musicDownloadLocal(int);

    MusicDownLoadManagerThread* m_downLoadManager;
    QString m_currentSongName;

};

#endif // MUSICVIDEOTABLEWIDGET_H
