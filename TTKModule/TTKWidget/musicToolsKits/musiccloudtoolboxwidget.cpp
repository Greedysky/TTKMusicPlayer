#include "musiccloudtoolboxwidget.h"
#include "musiccloudtablewidget.h"

MusicCloudToolBoxWidget::MusicCloudToolBoxWidget(QWidget *parent)
    : MusicFunctionToolBoxWidget(parent)
{
    MusicCloudUploadTableWidget *uploadTable = new MusicCloudUploadTableWidget(this);
    m_songItems << MusicSongItem();
    createWidgetItem(uploadTable, tr("UploadFailed"), 0);

    MusicCloudDownloadTableWidget *downloadTable = new MusicCloudDownloadTableWidget(this);
    m_songItems << MusicSongItem();
    createWidgetItem(downloadTable, tr("Download"), 1);

    connect(uploadTable, SIGNAL(updateItemTitle(int)), SLOT(updateItemTitle(int)));
    connect(downloadTable, SIGNAL(updateItemTitle(int)), SLOT(updateItemTitle(int)));
}

MusicCloudToolBoxWidget::~MusicCloudToolBoxWidget()
{
    while(!m_songItems.isEmpty())
    {
        delete m_songItems.last().m_itemObject;
        m_songItems.pop_back();
    }
}

void MusicCloudToolBoxWidget::updateItemTitle(int index)
{
    if(index < 0 || index >= m_songItems.count())
    {
        return;
    }

    const MusicSongItem *item = &m_songItems[index];
    setTitle(item->m_itemObject, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

void MusicCloudToolBoxWidget::createWidgetItem(MusicDownloadAbstractTableWidget *w, const QString &text, int index)
{
    MusicSongItem *item = &m_songItems.last();
    item->m_itemName = text;
    item->m_itemIndex = index;
    item->m_itemObject = w;
    addItem(w, item->m_itemName);

    w->setParentToolIndex(item->m_itemIndex);
    w->setSongsFileName(&item->m_songs);

    setTitle(w, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
}

MusicFunctionToolBoxWidgetItem *MusicCloudToolBoxWidget::createItem(QWidget *item, const QString &text)
{
    MusicFunctionToolBoxWidgetItem *it = new MusicNormalToolBoxWidgetItem(m_itemIndexRaise, text, this);
    it->addItem(item);
    it->setItemExpand(true);
    return it;
}
