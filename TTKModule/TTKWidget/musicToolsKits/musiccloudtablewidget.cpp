#include "musiccloudtablewidget.h"
#include "musicitemdelegate.h"
#include "musicconnectionpool.h"

#include <QScrollBar>

MusicCloudDownloadTableWidget::MusicCloudDownloadTableWidget(QWidget *parent)
    : MusicDownloadAbstractTableWidget(parent)
{
    M_CONNECTION_PTR->setValue(getClassName(), this);

    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 168);
    headerview->resizeSection(2, 83);
    headerview->resizeSection(3, 50);

    m_type = MusicNetwork::CloudDownload;
    setItemDelegateForColumn(2, m_delegate);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    MusicUtils::Widget::setTransparent(this, 0);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);

    musicSongsFileName();
}

MusicCloudDownloadTableWidget::~MusicCloudDownloadTableWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
}

void MusicCloudDownloadTableWidget::createItem(int index, const MusicDownloadRecord &record)
{
    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip( record.m_name );
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, 100);
    setItem(index, 2, item);

                      item = new QTableWidgetItem( record.m_size );
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setData(MUSIC_TIMES_ROLE, record.m_time);
    setItem(index, 3, item);

    m_musicSongs->append(MusicSong(record.m_path));
    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
}


MusicCloudUploadTableWidget::MusicCloudUploadTableWidget(QWidget *parent)
    : MusicDownloadAbstractTableWidget(parent)
{
    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 168);
    headerview->resizeSection(2, 83);
    headerview->resizeSection(3, 50);

    m_type = MusicNetwork::CloudUpload;
    setItemDelegateForColumn(2, m_delegate);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    MusicUtils::Widget::setTransparent(this, 0);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);

    musicSongsFileName();
}

void MusicCloudUploadTableWidget::createItem(int index, const MusicDownloadRecord &record)
{
    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip( record.m_name );
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, 100);
    setItem(index, 2, item);

                      item = new QTableWidgetItem( record.m_size );
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setData(MUSIC_TIMES_ROLE, record.m_time);
    setItem(index, 3, item);

    m_musicSongs->append(MusicSong(record.m_path));
    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
}
