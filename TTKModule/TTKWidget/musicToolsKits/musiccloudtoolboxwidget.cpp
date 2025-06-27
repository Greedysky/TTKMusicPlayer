#include "musiccloudtoolboxwidget.h"
#include "musiccloudtablewidget.h"

MusicCloudToolBoxWidget::MusicCloudToolBoxWidget(QWidget *parent)
    : MusicFunctionToolBoxWidget(parent)
{
    MusicCloudUploadTableWidget *uploadTable = new MusicCloudUploadTableWidget(this);
    m_items << MusicSongItem();
    createWidgetItem(uploadTable, tr("Failed"), 0);

    MusicCloudDownloadTableWidget *downloadTable = new MusicCloudDownloadTableWidget(this);
    m_items << MusicSongItem();
    createWidgetItem(downloadTable, tr("Download"), 1);

    connect(uploadTable, SIGNAL(updateItemTitle(int)), SLOT(updateItemTitle(int)));
    connect(downloadTable, SIGNAL(updateItemTitle(int)), SLOT(updateItemTitle(int)));
}

MusicCloudToolBoxWidget::~MusicCloudToolBoxWidget()
{
    while(!m_items.isEmpty())
    {
        delete m_items.back().m_itemWidget;
        m_items.pop_back();
    }
}

void MusicCloudToolBoxWidget::updateItemTitle(int index)
{
    if(index < 0 || index >= m_items.count())
    {
        return;
    }

    const MusicSongItem *item = &m_items[index];
    setTitle(item->m_itemWidget, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

void MusicCloudToolBoxWidget::createWidgetItem(MusicAbstractDownloadTableWidget *widget, const QString &text, int index)
{
    MusicSongItem *item = &m_items.back();
    item->m_itemName = text;
    item->m_itemIndex = index;
    item->m_itemWidget = widget;
    addCellItem(widget, item->m_itemName);

    widget->setPlaylistRow(item->m_itemIndex);
    widget->setSongsList(&item->m_songs);

    setTitle(widget, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

MusicFunctionToolBoxWidgetItem *MusicCloudToolBoxWidget::initialItem(QWidget *item, const QString &text)
{
    MusicFunctionToolBoxWidgetItem *it = new MusicNormalToolBoxWidgetItem(m_itemIndexRaise, text, this);
    it->addCellItem(item);
    it->setExpand(true);
    return it;
}
