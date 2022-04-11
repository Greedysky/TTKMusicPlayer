#include "musiclrcsearchtablewidget.h"
#include "musictoastlabel.h"
#include "musicitemdelegate.h"
#include "musicdownloadqueryfactory.h"
#include "musicgiflabelwidget.h"

MusicLrcSearchTableWidget::MusicLrcSearchTableWidget(QWidget *parent)
    : MusicItemSearchTableWidget(parent)
{
    setColumnCount(7);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 213);
    headerview->resizeSection(2, 181);
    headerview->resizeSection(3, 55);
    headerview->resizeSection(4, 24);
    headerview->resizeSection(5, 24);
    headerview->resizeSection(6, 24);
}

MusicLrcSearchTableWidget::~MusicLrcSearchTableWidget()
{
    clearAllItems();
}

void MusicLrcSearchTableWidget::startSearchQuery(const QString &text)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        clearAllItems();
        return;
    }

    MusicItemSearchTableWidget::startSearchQuery(text);
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), SIGNAL(resolvedSuccess()));
    m_loadingLabel->run(true);
    m_networkRequest->startToSearch(MusicAbstractQueryRequest::LrcQuery, text);
}

void MusicLrcSearchTableWidget::musicDownloadLocal(int row)
{
    if(row < 0 || (row >= rowCount() - 1))
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
        return;
    }

    const MusicObject::MusicSongInformationList musicSongInfos(m_networkRequest->musicSongInfoList());
    ///download lrc
    MusicAbstractDownLoadRequest *d = G_DOWNLOAD_QUERY_PTR->makeLrcRequest(musicSongInfos[row].m_lrcUrl,
                                      MusicUtils::String::lrcPrefix() + m_networkRequest->queryValue() + LRC_FILE,
                                      MusicObject::DownloadLrc, this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SIGNAL(lrcDownloadStateChanged(QString)));
    d->startToDownload();
}

void MusicLrcSearchTableWidget::itemCellEntered(int row, int column)
{
    MusicItemSearchTableWidget::itemCellEntered(row, column);
    if(column == 6)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }
}

void MusicLrcSearchTableWidget::itemCellClicked(int row, int column)
{
    MusicItemSearchTableWidget::itemCellClicked(row, column);
    switch(column)
    {
        case 6: musicDownloadLocal(row); break;
        default: break;
    }
}

void MusicLrcSearchTableWidget::clearAllItems()
{
    MusicItemSearchTableWidget::clearAllItems();
    setColumnCount(7);
}

void MusicLrcSearchTableWidget::createSearchedItem(const MusicSearchedItem &songItem)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, Qt::Unchecked);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_songName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
#if TTK_QT_VERSION_CHECK(5,13,0)
    item->setForeground(QColor(100, 100, 100));
#else
    item->setTextColor(QColor(100, 100, 100));
#endif
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_singerName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 20));
#if TTK_QT_VERSION_CHECK(5,13,0)
    item->setForeground(QColor(100, 100, 100));
#else
    item->setTextColor(QColor(100, 100, 100));
#endif
    setItem(count, 2, item);

                      item = new QTableWidgetItem(songItem.m_duration);
#if TTK_QT_VERSION_CHECK(5,13,0)
    item->setForeground(QColor(100, 100, 100));
#else
    item->setTextColor(QColor(100, 100, 100));
#endif
    setItem(count, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(randSimulation()));
    setItem(count, 4, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/tiny/lb_server_type"));
    item->setToolTip(songItem.m_type);
    setItem(count, 5, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/contextMenu/btn_download"));
    setItem(count, 6, item);
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
    MusicItemSearchTableWidget::contextMenuEvent(event);

    QMenu menu(this);
    createContextMenu(menu);
    menu.exec(QCursor::pos());
}
