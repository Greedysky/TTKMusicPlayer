#ifndef MUSICVIDEOTABLEWIDGET_H
#define MUSICVIDEOTABLEWIDGET_H

#include "musicdownloadquerythread.h"
#include "musictablequerywidget.h"

class MUSIC_WIDGET_EXPORT MusicVideoTableWidget : public MusicTableQueryWidget
{
    Q_OBJECT
public:
    explicit MusicVideoTableWidget(QWidget *parent = 0);
    virtual ~MusicVideoTableWidget();

    void startSearchQuery(const QString &text);

signals:
    void mvURLChanged(const QString &data);

public slots:
    void listCellClicked(int row, int col);
    void clearAllItems();
    void creatSearchedItems(const QString &songname,
                            const QString &artistname,
                            const QString &time);
    void itemDoubleClicked(int row, int column);

protected:
    QString randToGetStrength() const;
    void musicDownloadLocal(int row);

    QString m_currentSongName;

};

#endif // MUSICVIDEOTABLEWIDGET_H
