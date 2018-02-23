#include "musicdownloadabstracttablewidget.h"
#include "musicsongssummariziedwidget.h"
#include "musicdownloadmanager.h"
#include "musicconnectionpool.h"
#include "musicitemdelegate.h"
#include "musicmessagebox.h"
#include "musiccoreutils.h"

MusicDownloadAbstractTableWidget::MusicDownloadAbstractTableWidget(QWidget *parent)
    : MusicSongsListAbstractTableWidget(parent)
{
    m_delegate = new MusicProgressBarDelegate(this);

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummariziedWidget::getClassName());

    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(listCellDoubleClicked(int,int)));
}

MusicDownloadAbstractTableWidget::~MusicDownloadAbstractTableWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_musicSongs;
    delete m_delegate;
    clear();

    MusicDownloadRecordConfigManager xml(m_type, this);
    xml.writeDownloadConfig(m_musicRecords);
}

QString MusicDownloadAbstractTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownloadAbstractTableWidget::musicSongsFileName()
{
    MusicDownloadRecordConfigManager xml(m_type, this);
    if(!xml.readDownloadXMLConfig())
    {
        return;
    }
    xml.readDownloadConfig(m_musicRecords);

    m_musicSongs = new MusicSongs;
    setRowCount(m_musicRecords.count()); //reset row count

    for(int i=0; i<m_musicRecords.count(); i++)
    {
        MusicDownloadRecord *r = &m_musicRecords[i];
        createItem(i, *r);
        M_DOWNLOAD_MANAGER_PTR->reconnectMusicDownload(MusicDownLoadPair(r->m_time.toULongLong(), this));
    }
}

void MusicDownloadAbstractTableWidget::musicPlay()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    QString path = m_musicRecords[currentRow()].m_path;
    emit addSongToPlay(QStringList( QFile::exists(path) ? path : QString() ));
}

void MusicDownloadAbstractTableWidget::setDeleteItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if( !message.exec() || rowCount() == 0 )
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
        m_musicSongs->removeAt(index);
    }
}

void MusicDownloadAbstractTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicDownloadAbstractTableWidget::listCellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    musicPlay();
}

void MusicDownloadAbstractTableWidget::downloadProgressChanged(float percent, const QString &total, qint64 time)
{
    bool nor = (m_type == MusicDownloadRecordConfigManager::Normal);
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, nor ? 3 : 4);
        if(it && it->data(MUSIC_TIMES_ROLE).toLongLong() == time)
        {
            item(i, nor ? 2 : 3)->setData(MUSIC_PROCS_ROLE, percent);
            item(i, nor ? 3 : 4)->setText( total );

            m_musicRecords[i].m_size = total;
            break;
        }
    }
}

void MusicDownloadAbstractTableWidget::createDownloadItem(const QString &name, qint64 time)
{
    setRowCount(rowCount() + 1);
    QString musicName = name;
    musicName.remove(MusicUtils::Core::musicPrefix()).chop(4);

    MusicDownloadRecord record;
    record.m_name = musicName;
    record.m_path = QFileInfo(name).absoluteFilePath();
    record.m_size = "0.00M";
    record.m_time = QString::number(time);
    m_musicRecords << record;

    createItem(rowCount() - 1, record);
}

void MusicDownloadAbstractTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
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
}
