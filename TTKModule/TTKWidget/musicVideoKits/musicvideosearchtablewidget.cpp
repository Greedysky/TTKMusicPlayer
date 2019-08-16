#include "musicvideosearchtablewidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicdownloadwidget.h"
#include "musicitemdelegate.h"
#include "musicgiflabelwidget.h"
#include "musictime.h"

MusicVideoSearchTableWidget::MusicVideoSearchTableWidget(QWidget *parent)
    : MusicQueryItemTableWidget(parent)
{
    setColumnCount(9);
    resizeWindow(0);

    viewport()->setStyleSheet(MusicUIObject::MBackgroundStyle02);

    m_defaultBkColor = Qt::black;
    m_singleRadioMode = false;

    MusicTime::timeSRand();
    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicVideoSearchTableWidget::~MusicVideoSearchTableWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    clearAllItems();
}

void MusicVideoSearchTableWidget::startSearchQuery(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())   //no network connection
    {
        clearAllItems();
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    //
    MusicDownLoadQueryThreadAbstract *d = M_DOWNLOAD_QUERY_PTR->getMovieThread(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput( d );
    //
    m_singleRadioMode = false;
    m_loadingLabel->run(true);
    m_downLoadManager->startToSearch(MusicDownLoadQueryThreadAbstract::MovieQuery, text);
}

void MusicVideoSearchTableWidget::startSearchSingleQuery(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())   //no network connection
    {
        clearAllItems();
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    //
    MusicDownLoadQueryThreadAbstract *d = M_DOWNLOAD_QUERY_PTR->getMovieThread(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput( d );
    //
    m_singleRadioMode = false;
    m_loadingLabel->run(true);
    m_downLoadManager->setQueryType(MusicDownLoadQueryThreadAbstract::MovieQuery);
    m_downLoadManager->startToSingleSearch(text);
}

void MusicVideoSearchTableWidget::startSearchSingleQuery(const QVariant &data)
{
    if(!M_NETWORK_PTR->isOnline())   //no network connection
    {
        clearAllItems();
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    //
    MusicDownLoadQueryThreadAbstract *d = M_DOWNLOAD_QUERY_PTR->getMovieThread(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput( d );
    //
    m_singleRadioMode = true;
    d->setMusicSongInfos(MusicObject::MusicSongInformations() << data.value<MusicObject::MusicSongInformation>());
}

void MusicVideoSearchTableWidget::musicDownloadLocal(int row)
{
    if(!m_singleRadioMode)
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
    else
    {
        downloadLocalMovie(0);
    }
}

void MusicVideoSearchTableWidget::resizeWindow(int delta)
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
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 5));

        it = item(i, 2);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 5));
    }
}

void MusicVideoSearchTableWidget::listCellEntered(int row, int column)
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

    QTableWidgetItem *it = item(row, 0);
    if(it)
    {
        it->setBackgroundColor(m_defaultBkColor);
    }
}

void MusicVideoSearchTableWidget::listCellClicked(int row, int column)
{
    MusicQueryItemTableWidget::listCellClicked(row, column);
    switch(column)
    {
        case 6:
        case 7:
            itemDoubleClicked(row, DEFAULT_LEVEL_HIGH);
            break;
        case 8:
            musicDownloadLocal(row);
            break;
        default:break;
    }
}

void MusicVideoSearchTableWidget::clearAllItems()
{
    MusicQueryItemTableWidget::clearAllItems();
    setColumnCount(9);
}

void MusicVideoSearchTableWidget::createSearchedItem(const MusicSearchedItem &songItem)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerview = horizontalHeader();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    item->setBackgroundColor(m_defaultBkColor);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_songName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 5));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_singerName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 5));
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
    item->setIcon(QIcon(":/video/btn_mv"));
    setItem(count, 6, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/video/btn_audition"));
    setItem(count, 7, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/video/btn_download"));
    setItem(count, 8, item);
}

void MusicVideoSearchTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0 || (row >= rowCount() - 1))
    {
        return;
    }

    const MusicObject::MusicSongInformation &musicSongInfo = m_downLoadManager->getMusicSongInfos()[row];
    const MusicObject::MusicSongAttributes &attrs = musicSongInfo.m_songAttrs;
    if(!attrs.isEmpty())
    {
        const MusicObject::MusicSongAttribute &attr = attrs.first();
        const QString &url = attr.m_multiPart ? attr.m_url.split(TTK_STR_SPLITER).first() : attr.m_url;
        MusicVideoItem data;
        data.m_name = item(row, 2)->toolTip() + " - " + item(row, 1)->toolTip();
        data.m_url = url;
        data.m_id = musicSongInfo.m_songId;
        data.m_server = m_downLoadManager->getQueryServer();
        emit mediaUrlNameChanged(data);
    }
}

void MusicVideoSearchTableWidget::getMusicMediaInfo(MusicObject::MusicSongAttributes &data)
{
    if(!m_downLoadManager)
    {
        return;
    }

    const int row = !m_singleRadioMode ? m_previousClickRow : 0;
    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    data = (!musicSongInfos.isEmpty() && row != -1) ? musicSongInfos[row].m_songAttrs : MusicObject::MusicSongAttributes();
}

void MusicVideoSearchTableWidget::downloadLocalFromControl()
{
    if(!m_singleRadioMode)
    {
        if(m_previousClickRow != -1 && currentRow() != -1)
        {
            downloadLocalMovie(currentRow());
        }
    }
    else
    {
        downloadLocalMovie(0);
    }
}

void MusicVideoSearchTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryItemTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    createContextMenu(rightClickMenu);

    rightClickMenu.exec(QCursor::pos());
}

void MusicVideoSearchTableWidget::downloadLocalMovie(int row)
{
    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row < 0 || row >= musicSongInfos.count())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], MusicDownLoadQueryThreadAbstract::MovieQuery);
    download->show();
}
