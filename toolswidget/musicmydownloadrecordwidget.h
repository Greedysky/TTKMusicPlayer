#ifndef MUSICMYDOWNLOADRECORDWIDGET_H
#define MUSICMYDOWNLOADRECORDWIDGET_H

#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include "../core/musiclibexportglobal.h"
#include "../core/musictablewidgetabstract.h"

class MUSIC_EXPORT MusicMyDownloadRecordWidget :
        public MusicTableWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicMyDownloadRecordWidget(QWidget *parent = 0);
    virtual ~MusicMyDownloadRecordWidget();

    void musicSongsFileName();
    void clearAllItems();

signals:
    void musicPlay(const QStringList&);

public slots:
    void musicPlay();
    void setDeleteItemAt();
    void setDeleteItemAll();
    void listCellClicked(int row, int column);
    void listCellDoubleClicked(int row, int column);
    void musicOpenFileDir();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

    QStringList m_musicFileNameList;
    QStringList m_musicFilePathList;

};

#endif // MUSICMYDOWNLOADRECORDWIDGET_H
