#include "musiccloudtablewidget.h"
#include "musicitemdelegate.h"

#include <QScrollBar>

MusicCloudUploadTableWidget::MusicCloudUploadTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 300);
    headerview->resizeSection(2, 111);
    headerview->resizeSection(3, 50);

    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle05.arg(50));

    m_progressBarDelegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(2, m_progressBarDelegate);

}

MusicCloudUploadTableWidget::~MusicCloudUploadTableWidget()
{
    delete m_progressBarDelegate;
    clear();
}

void MusicCloudUploadTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}



MusicCloudDownloadTableWidget::MusicCloudDownloadTableWidget(QWidget *parent)
    : MusicDownloadAbstractTableWidget(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 25);
    headerview->resizeSection(2, 275);
    headerview->resizeSection(3, 111);
    headerview->resizeSection(4, 50);

    m_type = MusicDownloadRecordConfigManager::Cloud;
    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle05.arg(50));
    setItemDelegateForColumn(3, m_delegate);

    musicSongsFileName();
}

void MusicCloudDownloadTableWidget::createItem(int index, const MusicDownloadRecord &record)
{
    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/tiny/lb_right"));
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setToolTip( record.m_name );
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 20));
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 2, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, 100);
    setItem(index, 3, item);

                      item = new QTableWidgetItem( record.m_size );
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item->setData(MUSIC_TIMES_ROLE, record.m_time);
    setItem(index, 4, item);

    m_musicSongs->append(MusicSong(record.m_path));
}
