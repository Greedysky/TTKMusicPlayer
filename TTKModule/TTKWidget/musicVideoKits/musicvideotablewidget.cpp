#include "musicvideotablewidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicdownloadwidget.h"
#include "musicitemdelegate.h"
#include "musicgiflabelwidget.h"
#include "musictime.h"

MusicVideoTableWidget::MusicVideoTableWidget(QWidget *parent)
    : MusicQueryItemTableWidget(parent)
{
    setColumnCount(9);
    resizeWindow(0);

    MusicTime::timeSRand();
    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicVideoTableWidget::~MusicVideoTableWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    clearAllItems();
}

QString MusicVideoTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicVideoTableWidget::startSearchQuery(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())
    {   //no network connection
        clearAllItems();
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    MusicDownLoadQueryThreadAbstract *d = M_DOWNLOAD_QUERY_PTR->getMovieThread(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput( d );
    ////////////////////////////////////////////////////////////////////////////////////
    m_loadingLabel->show();
    m_loadingLabel->start();
    m_downLoadManager->startToSearch(MusicDownLoadQueryThreadAbstract::MovieQuery, text);
}

void MusicVideoTableWidget::startSearchSingleQuery(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())
    {   //no network connection
        clearAllItems();
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    MusicDownLoadQueryThreadAbstract *d = M_DOWNLOAD_QUERY_PTR->getMovieThread(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput( d );
    ////////////////////////////////////////////////////////////////////////////////////
    m_loadingLabel->show();
    m_loadingLabel->start();
    m_downLoadManager->setQueryType(MusicDownLoadQueryThreadAbstract::MovieQuery);
    m_downLoadManager->startToSingleSearch(text);
}

void MusicVideoTableWidget::musicDownloadLocal(int row)
{
    if(row < 0 || (row >= rowCount() - 1))
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    downloadLocalMovie(row);
}

void MusicVideoTableWidget::resizeWindow(int delta)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 306 + delta*0.5);
    headerview->resizeSection(2, 159 + delta*0.5);
    headerview->resizeSection(3, 55);
    headerview->resizeSection(4, 24);
    headerview->resizeSection(5, 24);
    headerview->resizeSection(6, 24);
    headerview->resizeSection(7, 24);
    headerview->resizeSection(8, 24);

    //resize row
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight,
                                                   headerview->sectionSize(1) - 5));
        it = item(i, 2);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight,
                                                   headerview->sectionSize(2) - 5));
    }
}

void MusicVideoTableWidget::listCellEntered(int row, int column)
{
    MusicQueryItemTableWidget::listCellEntered(row, column);
    if(column == 6 || column == 7 || column == 8)
    {
       setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
       unsetCursor();
    }
}

void MusicVideoTableWidget::listCellClicked(int row, int column)
{
    MusicQueryItemTableWidget::listCellClicked(row, column);
    switch(column)
    {
        case 6:
        case 7:
            itemDoubleClicked(row, 999);
            break;
        case 8:
            musicDownloadLocal(row);
            break;
        default:break;
    }
}

void MusicVideoTableWidget::clearAllItems()
{
    MusicQueryItemTableWidget::clearAllItems();
    setColumnCount(9);
}

void MusicVideoTableWidget::createSearchedItems(const MusicSearchedItem &songItem)
{
    int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerview = horizontalHeader();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_songName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight,
                                                 headerview->sectionSize(1) - 5));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_singerName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight,
                                                 headerview->sectionSize(2) - 5));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 2, item);

                      item = new QTableWidgetItem(songItem.m_time);
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(randToGetStrength()));
    setItem(count, 4, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/tiny/lb_server_type"));
    item->setToolTip(songItem.m_type);
    setItem(count, 5, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/contextMenu/btn_mv"));
    setItem(count, 6, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/contextMenu/btn_audition"));
    setItem(count, 7, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/contextMenu/btn_download"));
    setItem(count, 8, item);
}

void MusicVideoTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0 || (row >= rowCount() - 1))
    {
        return;
    }

    MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    MusicObject::MusicSongAttributes attrs = musicSongInfos[row].m_songAttrs;
    if(!attrs.isEmpty())
    {
        MusicObject::MusicSongAttribute attr = attrs.first();
        QString url = attr.m_multiParts ? attr.m_url.split(STRING_SPLITER).first() : attr.m_url;
        emit mvURLNameChanged(item(row, 2)->toolTip() + " - " + item(row, 1)->toolTip(), url);
    }
}

void MusicVideoTableWidget::getMusicMvInfo(MusicObject::MusicSongAttributes &data)
{
    if(!m_downLoadManager)
    {
        return;
    }
    MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    data = (!musicSongInfos.isEmpty() && m_previousClickRow != -1) ?
             musicSongInfos[m_previousClickRow].m_songAttrs : MusicObject::MusicSongAttributes();
}

void MusicVideoTableWidget::downloadLocalFromControl()
{
    if( m_previousClickRow != -1 && currentRow() != -1)
    {
        downloadLocalMovie(currentRow());
    }
}

void MusicVideoTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryItemTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    createContextMenu(rightClickMenu);

    rightClickMenu.exec(QCursor::pos());
}

void MusicVideoTableWidget::downloadLocalMovie(int row)
{
    MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row < 0 || row >= musicSongInfos.count())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], MusicDownLoadQueryThreadAbstract::MovieQuery);
    download->show();
}
