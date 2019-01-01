#include "musicdownloadrecordwidget.h"
#include "musicitemdelegate.h"
#include "musicwidgetheaders.h"
#include "musicconnectionpool.h"

MusicDownloadRecordTableWidget::MusicDownloadRecordTableWidget(QWidget *parent)
    : MusicDownloadAbstractTableWidget(parent)
{
    M_CONNECTION_PTR->setValue(getClassName(), this);

    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 168);
    headerview->resizeSection(2, 83);
    headerview->resizeSection(3, 50);

    m_type = MusicObject::RecordNormalDownload;
    setItemDelegateForColumn(2, m_delegate);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    MusicUtils::Widget::setTransparent(this, 0);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);
}

MusicDownloadRecordTableWidget::~MusicDownloadRecordTableWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
}

void MusicDownloadRecordTableWidget::createItem(int index, const MusicSong &record)
{
    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip( record.getMusicName() );
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, 100);
    setItem(index, 2, item);

                      item = new QTableWidgetItem( record.getMusicSizeStr() );
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setData(MUSIC_TIMES_ROLE, record.getMusicAddTimeStr());
    setItem(index, 3, item);

    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
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
        delete m_songItems.last().m_itemObject;
        m_songItems.pop_back();
    }
}

void MusicDownloadToolBoxWidget::updateItemTitle(int index)
{
    if(index == 0)
    {
        const MusicSongItem *item = &m_songItems[index];
        setTitle(item->m_itemObject, QString("%1[%2]").arg(item->m_itemName).arg(item->m_songs.count()));
    }
}

void MusicDownloadToolBoxWidget::createWidgetItem(MusicDownloadAbstractTableWidget *w, const QString &text, int index)
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

MusicFunctionToolBoxWidgetItem *MusicDownloadToolBoxWidget::createItem(QWidget *item, const QString &text)
{
    MusicFunctionToolBoxWidgetItem *it = new MusicNormalToolBoxWidgetItem(m_itemIndexRaise, text, this);
    it->addItem(item);
    it->setItemExpand(true);
    return it;
}
