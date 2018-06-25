#include "musiccloudtoolboxwidget.h"
#include "musiccloudtablewidget.h"

MusicCloudToolBoxWidget::MusicCloudToolBoxWidget(QWidget *parent)
    : MusicFunctionToolBoxWidget(parent)
{
    m_uploadTable = new MusicCloudUploadTableWidget(this);
    m_songItems << MusicSongItem();
    createWidgetItem(m_uploadTable, tr("UploadFailed"), 0);

    m_downloadTable = new MusicCloudDownloadTableWidget(this);
    m_songItems << MusicSongItem();
    createWidgetItem(m_downloadTable, tr("Download"), 1);

    connect(m_uploadTable, SIGNAL(updateItemTitle(int)), SLOT(updateItemTitle(int)));
    connect(m_downloadTable, SIGNAL(updateItemTitle(int)), SLOT(updateItemTitle(int)));
}

MusicCloudToolBoxWidget::~MusicCloudToolBoxWidget()
{
    delete m_uploadTable;
    delete m_downloadTable;
}

void MusicCloudToolBoxWidget::updateItemTitle(int index)
{
    if(index == 0)
    {
        MusicSongItem *item = &m_songItems[index];
        setTitle(m_uploadTable, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
    }
    else if(index == 1)
    {
        MusicSongItem *item = &m_songItems[index];
        setTitle(m_downloadTable, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
    }
}

void MusicCloudToolBoxWidget::createWidgetItem(MusicDownloadAbstractTableWidget *w, const QString &text, int index)
{
    MusicSongItem *item = &m_songItems.last();
    item->m_itemName = text;
    item->m_itemIndex = index;
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
