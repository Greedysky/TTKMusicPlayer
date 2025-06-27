#include "musicvideosearchtablewidget.h"
#include "musicdownloadqueryfactory.h"
#include "musictoastlabel.h"
#include "musicconnectionpool.h"
#include "musicdownloadwidget.h"

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

void MusicVideoSearchTableWidget::startToSearchByText(const QString &text)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        removeItems();
        return;
    }

    setLoadingStatus(true);
    setQueryInput(G_DOWNLOAD_QUERY_PTR->makeMovieRequest(this));

    m_singleRadioMode = false;
    m_networkRequest->startToSearch(text);
}

void MusicVideoSearchTableWidget::downloadQueryResult(int row)
{
    if(!m_singleRadioMode)
    {
        if(!isValid(row))
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

void MusicVideoSearchTableWidget::startToSearchByID(const QString &text)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        removeItems();
        return;
    }

    setLoadingStatus(true);
    setQueryInput(G_DOWNLOAD_QUERY_PTR->makeMovieRequest(this));

    m_singleRadioMode = false;
    m_networkRequest->startToSearchByID(text);
}

void MusicVideoSearchTableWidget::startToSearchByID(const QVariant &data)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        removeItems();
        return;
    }

    setQueryInput(G_DOWNLOAD_QUERY_PTR->makeMovieRequest(this));

    m_singleRadioMode = true;
    m_networkRequest->setItems({data.value<TTK::MusicSongInformation>()});
}

void MusicVideoSearchTableWidget::resizeSection(int delta)
{
    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 30);
    headerView->resizeSection(1, 306 + delta * 0.5);
    headerView->resizeSection(2, 159 + delta * 0.5);
    headerView->resizeSection(3, 55);
    headerView->resizeSection(4, 24);
    headerView->resizeSection(5, 24);
    headerView->resizeSection(6, 24);
    headerView->resizeSection(7, 24);
    headerView->resizeSection(8, 24);

    //resize row
    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(TTK::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 5));

        it = item(i, 2);
        it->setText(TTK::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerView->sectionSize(2) - 5));
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
        case 8: downloadQueryResult(row); break;
        default:break;
    }
}

void MusicVideoSearchTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0 || row >= rowCount() - 1)
    {
        return;
    }

    const TTK::MusicSongInformation &info = m_networkRequest->items()[row];
    TTK::MusicSongPropertyList props = info.m_songProps;
    std::sort(props.begin(), props.end()); //to find out the min bitrate

    if(!props.isEmpty())
    {
        const TTK::MusicSongProperty &prop = props.front();
        MusicVideoItem data;
        data.m_name = TTK::generateSongName(item(row, 1)->toolTip(), item(row, 2)->toolTip());
        data.m_url = prop.m_url;
        data.m_id = info.m_songId;
        data.m_cover = info.m_coverUrl;
        data.m_server = m_networkRequest->queryServer();
        Q_EMIT mediaUrlPathChanged(data);
    }
}

void MusicVideoSearchTableWidget::removeItems()
{
    MusicItemSearchTableWidget::removeItems();
    setColumnCount(9);
}

void MusicVideoSearchTableWidget::createResultItem(const MusicResultInfoItem &result)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerView = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
    item->setBackground(m_backgroundColor);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(result.m_songName);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 5));
    item->setForeground(QColor(TTK::UI::Color02));
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(result.m_artistName);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(2) - 5));
    item->setForeground(QColor(TTK::UI::Color02));
    setItem(count, 2, item);

                      item = new QTableWidgetItem(result.m_duration);
    item->setForeground(QColor(TTK::UI::Color02));
    setItem(count, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(randSimulation()));
    setItem(count, 4, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/tiny/lb_server_type"));
    item->setToolTip(result.m_type);
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

void MusicVideoSearchTableWidget::queryMediaProps(TTK::MusicSongPropertyList &props)
{
    if(!m_networkRequest)
    {
        return;
    }

    const int row = !m_singleRadioMode ? m_previousClickRow : 0;
    const TTK::MusicSongInformationList &songInfos = m_networkRequest->items();
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
    if(!isValid(row))
    {
        return;
    }

    MusicDownloadWidget *widget = new MusicDownloadWidget(this);
    widget->initialize(m_networkRequest, row);
    widget->show();
}
