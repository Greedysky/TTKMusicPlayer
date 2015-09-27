#include "musiclrcsearchtablewidget.h"
#include "musictextdownloadthread.h"
#include "musicmessagebox.h"

MusicLrcSearchTableWidget::MusicLrcSearchTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,30);
    headerview->resizeSection(1,175);
    headerview->resizeSection(2,148);
    headerview->resizeSection(3,50);
    headerview->resizeSection(4,24);
    setTransparent(255);
}

MusicLrcSearchTableWidget::~MusicLrcSearchTableWidget()
{
    clearAllItems();
}

void MusicLrcSearchTableWidget::startSearchQuery(const QString &text)
{
    m_currentSongName = text;
    m_downLoadManager->startSearchSong(MusicQuery, text);
}

void MusicLrcSearchTableWidget::clearAllItems()
{
    MusicAbstractTableWidget::clear();
    setColumnCount(5);
}

void MusicLrcSearchTableWidget::creatSearchedItems(const QString &songname,
                        const QString &artistname, const QString &time)
{
    int count;
    setRowCount(count = m_downLoadManager->getSongIdIndex());

    setStyleSheet(MusicUIObject::MTableWidgetStyle01 + \
                  MusicUIObject::MScrollBarStyle01);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(Qt::DisplayRole, false);
    setItem(count - 1, 0, item);

                      item = new QTableWidgetItem(songname);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(songname);
    setItem(count - 1, 1, item);

                      item = new QTableWidgetItem(artistname);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(artistname);
    setItem(count - 1, 2, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 3, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/radio/collect")),"");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 4, item);
}

void MusicLrcSearchTableWidget::musicDownloadLocal(int row)
{
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    MStringLists musicSongInfo(m_downLoadManager->getMusicSongInfo());

    MusicTextDownLoadThread* lrcDownload = new MusicTextDownLoadThread(musicSongInfo[row][1],
                             LRC_DOWNLOAD + m_currentSongName + LRC_FILE, Download_Lrc, this);
    connect(lrcDownload, SIGNAL(musicDownLoadFinished(QString)),
                         SIGNAL(lrcDownloadStateChanged(QString)));
    lrcDownload->startToDownload();
}

void MusicLrcSearchTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0)
    {
        return;
    }
    musicDownloadLocal(row);
}

void MusicLrcSearchTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    createContextMenu(rightClickMenu);

    rightClickMenu.exec(QCursor::pos());
}
