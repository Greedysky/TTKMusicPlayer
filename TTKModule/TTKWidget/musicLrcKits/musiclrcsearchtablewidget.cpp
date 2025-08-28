#include "musiclrcsearchtablewidget.h"
#include "musictoastlabel.h"
#include "musicdownloadqueryfactory.h"

MusicLrcSearchTableWidget::MusicLrcSearchTableWidget(QWidget *parent)
    : MusicItemSearchTableWidget(parent)
{
    setColumnCount(7);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 30);
    headerView->resizeSection(1, 213);
    headerView->resizeSection(2, 181);
    headerView->resizeSection(3, 55);
    headerView->resizeSection(4, 24);
    headerView->resizeSection(5, 24);
    headerView->resizeSection(6, 24);
}

MusicLrcSearchTableWidget::~MusicLrcSearchTableWidget()
{
    removeItems();
}

void MusicLrcSearchTableWidget::startToSearchByText(const QString &text)
{
    if(!G_NETWORK_PTR->isOnline())   //no network connection
    {
        removeItems();
        return;
    }

    MusicItemSearchTableWidget::startToSearchByText(text);
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), SIGNAL(resolvedSuccess()));

    m_networkRequest->setQueryType(MusicAbstractQueryRequest::QueryType::Lrc);
    m_networkRequest->startToSearch(text);
}

void MusicLrcSearchTableWidget::downloadQueryResult(int row)
{
    if(!isValid(row))
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    const TTK::MusicSongInformationList &songInfos = m_networkRequest->items();
    ///download lrc
    MusicAbstractDownLoadRequest *req = G_DOWNLOAD_QUERY_PTR->makeLrcRequest(songInfos[row].m_lrcUrl, TTK::String::lrcDirPrefix() + m_networkRequest->queryValue() + LRC_FILE, this);
    connect(req, SIGNAL(downLoadDataChanged(QString)), SIGNAL(lrcDownloadStateChanged(QString)));
    req->startToRequest();
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
        case 6: downloadQueryResult(row); break;
        default: break;
    }
}

void MusicLrcSearchTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0)
    {
        return;
    }

    downloadQueryResult(row);
}

void MusicLrcSearchTableWidget::removeItems()
{
    MusicItemSearchTableWidget::removeItems();
    setColumnCount(7);
}

void MusicLrcSearchTableWidget::createResultItem(const MusicResultInfoItem &result)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerView = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(result.m_songName);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 20));
    item->setForeground(QColor(TTK::UI::Color02));
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip(result.m_artistName);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(2) - 20));
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
    item->setIcon(QIcon(":/contextMenu/btn_download"));
    setItem(count, 6, item);
}

void MusicLrcSearchTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicItemSearchTableWidget::contextMenuEvent(event);

    QMenu menu(this);
    createContextMenu(menu);
    menu.exec(QCursor::pos());
}
