#include "musicdownloadabstracttablewidget.h"
#include "musicsongssummariziedwidget.h"
#include "musicdownloadmanager.h"
#include "musicconnectionpool.h"
#include "musicitemdelegate.h"
#include "musicmessagebox.h"
#include "musicstringutils.h"

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
    MusicDownloadRecordConfigManager xml(m_type, this);
    xml.writeDownloadData(*m_musicSongs);
    clear();

    delete m_delegate;
}

void MusicDownloadAbstractTableWidget::updateSongsFileName(const MusicSongs &songs)
{
    Q_UNUSED(songs);
    MusicDownloadRecordConfigManager xml(m_type, this);
    if(!xml.readConfig())
    {
        return;
    }
    xml.readDownloadData(*m_musicSongs);

    setRowCount(m_musicSongs->count()); //reset row count

    for(int i=0; i<m_musicSongs->count(); ++i)
    {
        MusicSong *song = &((*m_musicSongs)[i]);
        createItem(i, *song);
        M_DOWNLOAD_MANAGER_PTR->reconnectMusicDownload(MusicDownLoadPair(song->getMusicAddTimeStr().toULongLong(), this, m_type));
    }
}

void MusicDownloadAbstractTableWidget::musicPlay()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    const QString &path = (*m_musicSongs)[currentRow()].getMusicPath();
    emit addSongToPlay(QStringList( QFile::exists(path) ? path : QString() ));
}

void MusicDownloadAbstractTableWidget::setDeleteItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(!message.exec() || rowCount() == 0)
    {
       return;
    }

    const MIntList deleteList(getMultiIndexSet());

    for(int i=deleteList.count() - 1; i>=0; --i)
    {
        const int index = deleteList[i];
        removeRow(index); //Delete the current row
        m_musicSongs->removeAt(index);
    }
    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
    emit updateItemTitle(m_parentToolIndex);
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
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 3);
        if(it && it->data(MUSIC_TIMES_ROLE).toLongLong() == time)
        {
            item(i, 2)->setData(MUSIC_PROCS_ROLE, percent);
            item(i, 3)->setText( total );

            (*m_musicSongs)[i].setMusicSizeStr(total);
            break;
        }
    }
}

void MusicDownloadAbstractTableWidget::createDownloadItem(const QString &name, qint64 time)
{
    setRowCount(rowCount() + 1);
    QString musicName = name;
    musicName.remove(MusicUtils::String::musicPrefix()).chop(4);

    MusicSong record;
    record.setMusicName(musicName);
    record.setMusicPath(QFileInfo(name).absoluteFilePath());
    record.setMusicSizeStr("0.00M");
    record.setMusicAddTimeStr(QString::number(time));
    m_musicSongs->append(record);

    createItem(rowCount() - 1, record);
    emit updateItemTitle(m_parentToolIndex);
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

    const bool empty = !m_musicSongs->isEmpty();
    rightClickMenu.addAction(tr("musicInfo..."), this, SLOT(musicFileInformation()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_localFile"), tr("openFileDir"), this, SLOT(musicOpenFileDir()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("ablum"), this, SLOT(musicAlbumFoundWidget()));
    rightClickMenu.addSeparator();

    rightClickMenu.addAction(QIcon(":/contextMenu/btn_delete"), tr("delete"), this, SLOT(setDeleteItemAt()))->setEnabled(empty);
    rightClickMenu.addAction(tr("deleteAll"), this, SLOT(setDeleteItemAll()))->setEnabled(empty);

    rightClickMenu.exec(QCursor::pos());
}
