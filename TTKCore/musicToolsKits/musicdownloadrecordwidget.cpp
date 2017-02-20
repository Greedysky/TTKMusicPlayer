#include "musicdownloadrecordwidget.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicitemdelegate.h"
#include "musicsongssummariziedwidget.h"
#include "musiccoreutils.h"

#include <QScrollBar>
#include <QContextMenuEvent>

MusicDownloadRecordWidget::MusicDownloadRecordWidget(QWidget *parent)
    : MusicSongsListAbstractTableWidget(parent)
{
    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 170);
    headerview->resizeSection(2, 83);
    headerview->resizeSection(3, 50);

    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03.arg(50));

    m_delegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(2, m_delegate);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(listCellDoubleClicked(int,int)));
    musicSongsFileName();

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummariziedWidget::getClassName());
}

MusicDownloadRecordWidget::~MusicDownloadRecordWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_musicSongs;
    delete m_delegate;
    clearAllItems();

    MusicDownloadRecordConfigManager xml(MusicDownloadRecordConfigManager::Normal, this);
    xml.writeDownloadConfig(m_musicRecords);
}

QString MusicDownloadRecordWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownloadRecordWidget::musicSongsFileName()
{
    MusicDownloadRecordConfigManager xml(MusicDownloadRecordConfigManager::Normal, this);
    if(!xml.readDownloadXMLConfig())
    {
        return;
    }
    xml.readDownloadConfig(m_musicRecords);

    m_musicSongs = new MusicSongs;
    setRowCount(m_loadRecordCount = m_musicRecords.count()); //reset row count

    for(int i=0; i<m_musicRecords.count(); i++)
    {
        createItem(i, m_musicRecords[i], DEFAULT_INDEX_LEVEL1);
    }
}

void MusicDownloadRecordWidget::clearAllItems()
{
    //Remove all the original item
    MusicSongsListAbstractTableWidget::clear();
    setColumnCount(4);
}

void MusicDownloadRecordWidget::musicPlay()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    QString path = m_musicRecords[currentRow()].m_path;
    emit addSongToPlay(QStringList( QFile::exists(path) ? path : QString() ));
}

void MusicDownloadRecordWidget::setDeleteItemAt()
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
        --m_loadRecordCount;
    }
}

void MusicDownloadRecordWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicDownloadRecordWidget::listCellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    musicPlay();
}

void MusicDownloadRecordWidget::downloadProgressChanged(float percent, const QString &total, qint64 time)
{
    for(int i=m_loadRecordCount; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 3);
        if(it && it->data(MUSIC_TIMES_ROLE).toLongLong() == time)
        {
            item(i, 2)->setData(MUSIC_PROCS_ROLE, percent);
            item(i, 3)->setText( total );

            if(percent == 100)
            {
                m_musicRecords[i].m_size = total;
            }
            break;
        }
    }
}

void MusicDownloadRecordWidget::createDownloadItem(const QString &name, qint64 time)
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

void MusicDownloadRecordWidget::contextMenuEvent(QContextMenuEvent *event)
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

void MusicDownloadRecordWidget::createItem(int index, const MusicDownloadRecord &record, qint64 time)
{
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::Widget::elidedText(font(), record.m_name, Qt::ElideRight, 160));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip( record.m_name );
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, 100);
    setItem(index, 2, item);

                      item = new QTableWidgetItem( record.m_size );
    item->setTextAlignment(Qt::AlignCenter);
    item->setData(MUSIC_TIMES_ROLE, time);
    setItem(index, 3, item);

    m_musicSongs->append(MusicSong(record.m_path));
}
