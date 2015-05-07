#ifndef MUSICMYDOWNLOADRECORDWIDGET_H
#define MUSICMYDOWNLOADRECORDWIDGET_H

#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include "../core/musiclibexportglobal.h"
#include "../core/musicobject.h"

class MUSIC_EXPORT MusicMyDownloadRecordWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit MusicMyDownloadRecordWidget(QWidget *parent = 0);
    virtual ~MusicMyDownloadRecordWidget();

    void setTransparent(int angle);
    void musicSongsFileName();
    void clearAllItems();
    void setRowColor(int row, const QColor& color);

signals:
    void musicPlay(const QStringList&);

public slots:
    void musicPlay();
    void setDeleteItemAt();
    void setDeleteItemAll();
    void listCellEntered(int row, int column);
    void listCellClicked(int row, int column);
    void listCellDoubleClicked(int row, int column);
    void musicOpenFileDir();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

    QColor m_defaultBkColor;
    int m_previousColorRow;
    QStringList m_musicFileNameList;
    QStringList m_musicFilePathList;

};

#endif // MUSICMYDOWNLOADRECORDWIDGET_H
