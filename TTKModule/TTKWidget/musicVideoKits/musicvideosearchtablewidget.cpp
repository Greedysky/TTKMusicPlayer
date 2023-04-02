#include "musicvideosearchtablewidget.h"
#include "musicdownloadqueryfactory.h"
#include "musictoastlabel.h"
#include "musicconnectionpool.h"
#include "musicdownloadwidget.h"
#include "musicitemdelegate.h"
#include "musicgiflabelwidget.h"

MusicVideoSearchTableWidget::MusicVideoSearchTableWidget(QWidget *parent)
    : MusicItemSearchTableWidget(parent),
      m_singleRadioMode(false)
{
    setColumnCount(9);
    resizeSection(0);

    TTK::initRandom();

    G_CONNECTION_PTR->setValue(className(), this);
}

MusicVideoSearchTableWidget::~MusicVideoSearchTableWidget()
{
    G_CONNECTION_PTR->removeValue(this);
    removeItems();
}

void MusicVideoSearchTableWidget::startSearchQuery(const QString &text)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        removeItems();
        return;
    }
    //
    MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeMovieRequest(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput(d);
    //
    m_singleRadioMode = false;
    m_loadingLabel->run(true);
    m_networkRequest->startToSearch(MusicAbstractQueryRequest::QueryType::Movie, text);
}

void MusicVideoSearchTableWidget::musicDownloadLocal(int row)
{
    if(!m_singleRadioMode)
    {
        if(row < 0 || (row >= rowCount() - 1))
        {
            MusicToastLabel::popup(tr("Please select one item first"));
            return;
        }
        downloadLocalMovie(row);
    }
    else
    {
        downloadLocalMovie(0);
    }
}

void MusicVideoSearchTableWidget::startSearchSingleQuery(const QString &text)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        removeItems();
        return;
    }
    //
    MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeMovieRequest(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput(d);
    //
    m_singleRadioMode = false;
    m_loadingLabel->run(true);
    m_networkRequest->setQueryType(MusicAbstractQueryRequest::QueryType::Movie);
    m_networkRequest->startToSingleSearch(text);
}

void MusicVideoSearchTableWidget::startSearchSingleQuery(const QVariant &data)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        removeItems();
        return;
    }
    //
    MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeMovieRequest(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput(d);
    //
    m_singleRadioMode = true;
    d->setSongInfoList({data.value<TTK::MusicSongInformation>()});
}

void MusicVideoSearchTableWidget::resizeSection(int delta)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 306 + delta * 0.5);
    headerview->resizeSection(2, 159 + delta * 0.5);
    headerview->resizeSection(3, 55);
    headerview->resizeSection(4, 24);
    headerview->resizeSection(5, 24);
    headerview->resizeSection(6, 24);
    headerview->resizeSection(7, 24);
    headerview->resizeSection(8, 24);

    //resize row
    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(TTK::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 5));

        it = item(i, 2);
        it->setText(TTK::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 5));
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
        case 7: itemDoubleClicked(row, TTK_HIGH_LEVEL); break;
        case 8: musicDownloadLocal(row); break;
        default:break;
    }
}

void MusicVideoSearchTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0 || (row >= rowCount() - 1))
    {
        return;
    }

    const TTK::MusicSongInformation &info = m_networkRequest->songInfoList()[row];
    const TTK::MusicSongPropertyList &props = info.m_songProps;
    if(!props.isEmpty())
    {
        const TTK::MusicSongProperty &prop = props.front();
        MusicVideoItem data;
        data.m_name = item(row, 2)->toolTip() + " - " + item(row, 1)->toolTip();
        data.m_url = prop.m_url;
        data.m_id = info.m_songId;
        data.m_server = m_networkRequest->queryServer();
        Q_EMIT mediaUrlPathChanged(data);
    }
}

void MusicVideoSearchTableWidget::removeItems()
{
    MusicItemSearchTableWidget::removeItems();
    setColumnCount(9);
}

void MusicVideoSearchTableWidget::createSearchedItem(const MusicResultInfoItem &songItem)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
    item->setBackground(m_backgroundColor);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_songName);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 5));
    item->setForeground(QColor(TTK::UI::Color02));
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_singerName);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 5));
    item->setForeground(QColor(TTK::UI::Color02));
    setItem(count, 2, item);

                      item = new QTableWidgetItem(songItem.m_duration);
    item->setForeground(QColor(TTK::UI::Color02));
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

void MusicVideoSearchTableWidget::musicMediaInfo(TTK::MusicSongPropertyList &props)
{
    if(!m_networkRequest)
    {
        return;
    }

    const int row = !m_singleRadioMode ? m_previousClickRow : 0;
    const TTK::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    props = (!songInfos.isEmpty() && row != -1) ? songInfos[row].m_songProps : TTK::MusicSongPropertyList();
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
    const TTK::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    if(row < 0 || row >= songInfos.count())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(songInfos[row], MusicAbstractQueryRequest::QueryType::Movie);
    download->show();
}
