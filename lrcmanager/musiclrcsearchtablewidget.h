#ifndef MUSICLRCSEARCHTABLEWIDGET_H
#define MUSICLRCSEARCHTABLEWIDGET_H

#include "core/musicdownloadquerythread.h"
#include "core/musictablequerywidget.h"

class MUSIC_EXPORT MusicLrcSearchTableWidget : public MusicTableQueryWidget
{
    Q_OBJECT
public:
    explicit MusicLrcSearchTableWidget(QWidget *parent = 0);
    virtual ~MusicLrcSearchTableWidget();

    void startSearchQuery(const QString&);
    inline void setCurrentSongName(const QString& name){ m_currentSongName = name;}

signals:
    void lrcDownloadStateChanged(const QString&);

public slots:
    void clearAllItems();
    void creatSearchedItems(const QString&, const QString&, const QString&);
    void itemDoubleClicked(int row, int column);
    virtual void listCellClicked(int, int){}

protected:
    void musicDownloadLocal(int);
    QString m_currentSongName;

};

#endif // MUSICLRCSEARCHTABLEWIDGET_H
