#include "musicdownloadrecordwidget.h"
#include "musicconnectionpool.h"
#include "musicwidgetheaders.h"

MusicDownloadRecordTableWidget::MusicDownloadRecordTableWidget(QWidget *parent)
    : MusicAbstractDownloadTableWidget(parent)
{
    setColumnCount(4);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 10);
    headerView->resizeSection(1, 168);
    headerView->resizeSection(2, 83);
    headerView->resizeSection(3, 50);

    m_type = TTK::Record::NormalDownload;
    setItemDelegateForColumn(2, m_progressBarDelegate);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    TTK::Widget::setTransparent(this, 0);
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);

    G_CONNECTION_PTR->setValue(className(), this);
}

MusicDownloadRecordTableWidget::~MusicDownloadRecordTableWidget()
{
    G_CONNECTION_PTR->removeValue(this);
}

void MusicDownloadRecordTableWidget::addCellItem(int index, const MusicSong &record)
{
    QHeaderView *headerView = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(record.name());
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 20));
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setData(TTK_PROGRESS_ROLE, 100);
    setItem(index, 2, item);

                      item = new QTableWidgetItem(record.sizeStr());
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignRight| Qt::AlignVCenter);
    item->setData(TTK_TIME_ROLE, record.addTimeStr());
    setItem(index, 3, item);

    setFixedHeight(totalHeight());
}



MusicDownloadToolBoxWidget::MusicDownloadToolBoxWidget(QWidget *parent)
    : MusicFunctionToolBoxWidget(parent)
{
    MusicDownloadRecordTableWidget *downloadTable = new MusicDownloadRecordTableWidget(this);
    m_songItems << MusicSongItem();
    createWidgetItem(downloadTable, tr("Download"), 0);

    connect(downloadTable, SIGNAL(updateItemTitle(int)), SLOT(updateItemTitle(int)));
}

MusicDownloadToolBoxWidget::~MusicDownloadToolBoxWidget()
{
    while(!m_songItems.isEmpty())
    {
        delete m_songItems.back().m_itemWidget;
        m_songItems.pop_back();
    }
}

void MusicDownloadToolBoxWidget::updateItemTitle(int index)
{
    if(index == 0)
    {
        const MusicSongItem *item = &m_songItems[index];
        setTitle(item->m_itemWidget, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
    }
}

void MusicDownloadToolBoxWidget::createWidgetItem(MusicAbstractDownloadTableWidget *widget, const QString &text, int index)
{
    MusicSongItem *item = &m_songItems.back();
    item->m_itemName = text;
    item->m_itemIndex = index;
    item->m_itemWidget = widget;
    addCellItem(widget, item->m_itemName);

    widget->setPlaylistRow(item->m_itemIndex);
    widget->setSongsList(&item->m_songs);

    setTitle(widget, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

MusicFunctionToolBoxWidgetItem *MusicDownloadToolBoxWidget::initialItem(QWidget *item, const QString &text)
{
    MusicFunctionToolBoxWidgetItem *it = new MusicNormalToolBoxWidgetItem(m_itemIndexRaise, text, this);
    it->addCellItem(item);
    it->setExpand(true);
    return it;
}
