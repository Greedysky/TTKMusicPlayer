#include "musiclrcsearchtablewidget.h"
#include "musictextdownloadthread.h"
#include "musicmessagebox.h"
#include "musicitemdelegate.h"

MusicLrcSearchTableWidget::MusicLrcSearchTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 175);
    headerview->resizeSection(2, 148);
    headerview->resizeSection(3, 50);
    headerview->resizeSection(4, 24);
    setTransparent(255);
}

MusicLrcSearchTableWidget::~MusicLrcSearchTableWidget()
{
    clearAllItems();
}

void MusicLrcSearchTableWidget::startSearchQuery(const QString &text)
{
    if(!M_NETWORK->isOnline())
    {   //no network connection
        emit showDownLoadInfoFor(MusicObject::DisConnection);
        return;
    }
    m_currentSongName = text;
    m_downLoadManager->startSearchSong(MusicDownLoadQueryThread::LrcQuery, text);
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

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    setItem(count - 1, 0, item);

                      item = new QTableWidgetItem;
    item->setText(QFontMetrics(font()).elidedText(songname, Qt::ElideRight, 170));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(songname);
    setItem(count - 1, 1, item);

                      item = new QTableWidgetItem;
    item->setText(QFontMetrics(font()).elidedText(artistname, Qt::ElideRight, 144));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(artistname);
    setItem(count - 1, 2, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/radio/collect")));
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
    MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());

    MusicTextDownLoadThread* lrcDownload = new MusicTextDownLoadThread(musicSongInfos[row].m_lrcUrl,
                             LRC_DOWNLOAD_AL + m_currentSongName + LRC_FILE,
                             MusicDownLoadThreadAbstract::Download_Lrc, this);
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
