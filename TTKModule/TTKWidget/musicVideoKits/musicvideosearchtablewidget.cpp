#include "musicvideosearchtablewidget.h"
#include "musicdownloadqueryfactory.h"
#include "musictoastlabel.h"
#include "musicconnectionpool.h"
#include "musicdownloadwidget.h"
#include "musicitemdelegate.h"
#include "musicgiflabelwidget.h"

MusicVideoSearchTableWidget::MusicVideoSearchTableWidget(QWidget *parent)
    : MusicItemSearchTableWidget(parent)
{
    setColumnCount(9);
    resizeWindow(0);

    m_singleRadioMode = false;

    MusicTime::initRandom();
    G_CONNECTION_PTR->setValue(className(), this);
}

MusicVideoSearchTableWidget::~MusicVideoSearchTableWidget()
{
    G_CONNECTION_PTR->removeValue(className());
    clearAllItems();
}

void MusicVideoSearchTableWidget::startSearchQuery(const QString &text)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        clearAllItems();
        return;
    }
    //
    MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeMovieRequest(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput(d);
    //
    m_singleRadioMode = false;
    m_loadingLabel->run(true);
    m_networkRequest->startToSearch(MusicAbstractQueryRequest::MovieQuery, text);
}

void MusicVideoSearchTableWidget::startSearchSingleQuery(const QString &text)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        clearAllItems();
        return;
    }
    //
    MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeMovieRequest(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput(d);
    //
    m_singleRadioMode = false;
    m_loadingLabel->run(true);
    m_networkRequest->setQueryType(MusicAbstractQueryRequest::MovieQuery);
    m_networkRequest->startToSingleSearch(text);
}

void MusicVideoSearchTableWidget::startSearchSingleQuery(const QVariant &data)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        clearAllItems();
        return;
    }
    //
    MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeMovieRequest(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput(d);
    //
    m_singleRadioMode = true;
    d->setMusicSongInfoList({data.value<MusicObject::MusicSongInformation>()});
}

void MusicVideoSearchTableWidget::musicDownloadLocal(int row)
{
    if(!m_singleRadioMode)
    {
        if(row < 0 || (row >= rowCount() - 1))
        {
            MusicToastLabel::popup(tr("Please select one item first!"));
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

void MusicVideoSearchTableWidget::itemCellEntered(int row, int column)
{
    MusicItemSearchTableWidget::itemCellEntered(row, column);
    if(column == 6 || column == 7 || column == 8)
    {
       setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
       unsetCursor();
    }
}

void MusicVideoSearchTableWidget::itemCellClicked(int row, int column)
{
    MusicItemSearchTableWidget::itemCellClicked(row, column);
    switch(column)
    {
        case 6:
        case 7: itemDoubleClicked(row, DEFAULT_HIGH_LEVEL); break;
        case 8: musicDownloadLocal(row); break;
        default:break;
    }
}

void MusicVideoSearchTableWidget::clearAllItems()
{
    MusicItemSearchTableWidget::clearAllItems();
    setColumnCount(9);
}

void MusicVideoSearchTableWidget::createSearchedItem(const MusicSearchedItem &songItem)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerview = horizontalHeader();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, Qt::Unchecked);
#if TTK_QT_VERSION_CHECK(5,13,0)
    item->setBackground(m_backgroundColor);
#else
    item->setBackgroundColor(m_backgroundColor);
#endif
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_songName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 5));
#if TTK_QT_VERSION_CHECK(5,13,0)
    item->setForeground(QColor(100, 100, 100));
#else
    item->setTextColor(QColor(100, 100, 100));
#endif
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_singerName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 5));
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

    const MusicObject::MusicSongInformation &musicSongInfo = m_networkRequest->musicSongInfoList()[row];
    const MusicObject::MusicSongPropertyList &props = musicSongInfo.m_songProps;
    if(!props.isEmpty())
    {
        const MusicObject::MusicSongProperty &prop = props.front();
        MusicVideoItem data;
        data.m_name = item(row, 2)->toolTip() + " - " + item(row, 1)->toolTip();
        data.m_url = prop.m_url;
        data.m_id = musicSongInfo.m_songId;
        data.m_server = m_networkRequest->queryServer();
        Q_EMIT mediaUrlPathChanged(data);
    }
}

void MusicVideoSearchTableWidget::musicMediaInfo(MusicObject::MusicSongPropertyList &props)
{
    if(!m_networkRequest)
    {
        return;
    }

    const int row = !m_singleRadioMode ? m_previousClickRow : 0;
    const MusicObject::MusicSongInformationList musicSongInfos(m_networkRequest->musicSongInfoList());
    props = (!musicSongInfos.isEmpty() && row != -1) ? musicSongInfos[row].m_songProps : MusicObject::MusicSongPropertyList();
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
    MusicItemSearchTableWidget::contextMenuEvent(event);
    QMenu menu(this);
    createContextMenu(menu);

    menu.exec(QCursor::pos());
}

void MusicVideoSearchTableWidget::downloadLocalMovie(int row)
{
    const MusicObject::MusicSongInformationList musicSongInfos(m_networkRequest->musicSongInfoList());
    if(row < 0 || row >= musicSongInfos.count())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], MusicAbstractQueryRequest::MovieQuery);
    download->show();
}
