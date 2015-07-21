#ifndef MUSICVIDEOTABLEWIDGET_H
#define MUSICVIDEOTABLEWIDGET_H

#include "../core/musicdownloadquerythread.h"
#include "../core/musictablequerywidget.h"

class MUSIC_EXPORT MusicVideoTableWidget : public MusicTableQueryWidget
{
    Q_OBJECT
public:
    explicit MusicVideoTableWidget(QWidget *parent = 0);
    virtual ~MusicVideoTableWidget();

    void startSearchQuery(const QString&);

signals:
    void mvURLChanged(const QString&);

public slots:
    void listCellClicked(int row,int col);
    void clearAllItems();
    void creatSearchedItems(const QString&, const QString&, const QString&);
    void itemDoubleClicked(int row, int column);

protected:
    QString randToGetStrength();
    void musicDownloadLocal(int);
    QString m_currentSongName;

};

#endif // MUSICVIDEOTABLEWIDGET_H
