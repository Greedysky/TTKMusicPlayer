#include "musiccloudtablewidget.h"
#include "musicconnectionpool.h"
#include "musicitemdelegate.h"
#include "musiccoreutils.h"
#include "musicmessagebox.h"

#include <QScrollBar>
#include <QContextMenuEvent>

MusicCloudUploadTableWidget::MusicCloudUploadTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 300);
    headerview->resizeSection(2, 111);
    headerview->resizeSection(3, 50);

    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03.arg(50));

    m_progressBarDelegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(2, m_progressBarDelegate);

}

MusicCloudUploadTableWidget::~MusicCloudUploadTableWidget()
{
    delete m_progressBarDelegate;
    clear();
}

QString MusicCloudUploadTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudUploadTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}



MusicCloudDownloadTableWidget::MusicCloudDownloadTableWidget(QWidget *parent)
    : MusicSongsListAbstractTableWidget(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 25);
    headerview->resizeSection(2, 275);
    headerview->resizeSection(3, 111);
    headerview->resizeSection(4, 50);

    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03.arg(50));

    m_progressBarDelegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(3, m_progressBarDelegate);

    musicSongsFileName();

    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicCloudDownloadTableWidget::~MusicCloudDownloadTableWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_musicSongs;
    delete m_progressBarDelegate;
    clear();

    MusicDownloadRecordConfigManager xml(MusicDownloadRecordConfigManager::Cloud, this);
    xml.writeDownloadConfig(m_musicRecords);
}

QString MusicCloudDownloadTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudDownloadTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicCloudDownloadTableWidget::setDeleteItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if( message.exec() || rowCount() == 0 )
    {
       return;
    }

    MusicObject::MIntSet deletedRow; //if selected multi rows
    foreach(QTableWidgetItem *item, selectedItems())
    {
        deletedRow.insert(item->row());
    }

    MusicObject::MIntList deleteList = deletedRow.toList();
    qSort(deleteList);
    for(int i=deleteList.count() - 1; i>=0; --i)
    {
        int index = deleteList[i];
        removeRow(index); //Delete the current row
        m_musicRecords.removeAt(index);
    }
}

void MusicCloudDownloadTableWidget::downloadProgressChanged(float percent, const QString &total, qint64 time)
{
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 4);
        if(it && it->data(MUSIC_TIMES_ROLE).toLongLong() == time)
        {
            item(i, 3)->setData(MUSIC_PROCS_ROLE, percent);
            item(i, 4)->setText( total );

            if(percent == 100)
            {
                m_musicRecords[i].m_size = total;
            }
            break;
        }
    }
}

void MusicCloudDownloadTableWidget::createDownloadItem(const QString &name, qint64 time)
{
    setRowCount( rowCount() + 1);
    QString musicName = name;
    musicName.remove(MusicUtils::Core::musicPrefix()).chop(4);

    MusicDownloadRecord record;
    record.m_name = musicName;
    record.m_path = QFileInfo(name).absoluteFilePath();
    record.m_size = "0.0M";
    m_musicRecords << record;

    createItem(rowCount() - 1, record, time);
}

void MusicCloudDownloadTableWidget::musicSongsFileName()
{
    MusicDownloadRecordConfigManager xml(MusicDownloadRecordConfigManager::Cloud, this);
    if(!xml.readDownloadXMLConfig())
    {
        return;
    }
    xml.readDownloadConfig(m_musicRecords);

    m_musicSongs = new MusicSongs;
    setRowCount(m_musicRecords.count()); //reset row count
    for(int i=0; i<m_musicRecords.count(); i++)
    {
        createItem(i, m_musicRecords[i], DEFAULT_INDEX_LEVEL1);
    }
}

void MusicCloudDownloadTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicSongsListAbstractTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);

    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay"), this, SLOT(musicPlayClicked()));
    rightClickMenu.addAction(tr("downloadMore..."), this, SLOT(musicSongDownload()));
    rightClickMenu.addSeparator();

    createMoreMenu(&rightClickMenu);

    bool empty = !m_musicSongs->isEmpty();
    rightClickMenu.addAction(tr("musicInfo..."), this, SLOT(musicFileInformation()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_localFile"), tr("openFileDir"), this, SLOT(musicOpenFileDir()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("ablum"), this, SLOT(musicAlbumFoundWidget()));
    rightClickMenu.addSeparator();

    rightClickMenu.addAction(QIcon(":/contextMenu/btn_delete"), tr("delete"), this, SLOT(setDeleteItemAt()))->setEnabled(empty);
    rightClickMenu.addAction(tr("deleteAll"), this, SLOT(setDeleteItemAll()))->setEnabled(empty);

    rightClickMenu.exec(QCursor::pos());
    event->accept();
}

void MusicCloudDownloadTableWidget::createItem(int index, const MusicDownloadRecord &record, qint64 time)
{
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/tiny/lb_right"));
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::Widget::elidedText(font(), record.m_name, Qt::ElideRight, 260));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip( record.m_name );
    setItem(index, 2, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, 100);
    setItem(index, 3, item);

                      item = new QTableWidgetItem( record.m_size );
    item->setTextAlignment(Qt::AlignCenter);
    item->setData(MUSIC_TIMES_ROLE, time);
    setItem(index, 4, item);

    m_musicSongs->append(MusicSong(record.m_path));
}
