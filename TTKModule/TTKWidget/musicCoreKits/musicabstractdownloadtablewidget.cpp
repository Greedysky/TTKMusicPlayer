#include "musicabstractdownloadtablewidget.h"
#include "musicsongssummariziedwidget.h"
#include "musicdownloadmanager.h"
#include "musicconnectionpool.h"
#include "musicitemdelegate.h"
#include "musicmessagebox.h"
#include "musicstringutils.h"

MusicAbstractDownloadTableWidget::MusicAbstractDownloadTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent)
{
    m_delegate = new MusicProgressBarDelegate(this);

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->poolConnect(className(), MusicSongsSummariziedWidget::className());

    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
}

MusicAbstractDownloadTableWidget::~MusicAbstractDownloadTableWidget()
{
    G_CONNECTION_PTR->removeValue(className());
    MusicDownloadRecordConfigManager xml(m_type, this);
    xml.writeBuffer(*m_musicSongs);
    clear();

    delete m_delegate;
}

void MusicAbstractDownloadTableWidget::updateSongsFileName(const MusicSongList &songs)
{
    Q_UNUSED(songs);
    MusicDownloadRecordConfigManager xml(m_type, this);
    if(!xml.fromFile())
    {
        return;
    }
    xml.readBuffer(*m_musicSongs);

    setRowCount(m_musicSongs->count()); //reset row count

    for(int i = 0; i < m_musicSongs->count(); ++i)
    {
        MusicSong *song = &(*m_musicSongs)[i];
        createItem(i, *song);
        G_DOWNLOAD_MANAGER_PTR->reconnectMusicDownload(MusicDownLoadPairData(song->musicAddTimeStr().toULongLong(), this, m_type));
    }
}

void MusicAbstractDownloadTableWidget::musicPlay()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    const QString &path = m_musicSongs->at(currentRow()).musicPath();
    Q_EMIT addSongToPlaylist(QStringList(QFile::exists(path) ? path : QString()));
}

void MusicAbstractDownloadTableWidget::setDeleteItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(!message.exec() || rowCount() == 0)
    {
       return;
    }

    const TTKIntList deleteList(multiSelectedIndex());
    if(deleteList.isEmpty())
    {
        return;
    }

    for(int i = deleteList.count() - 1; i >= 0; --i)
    {
        const int index = deleteList[i];
        removeRow(index);
        m_musicSongs->removeAt(index);
    }
    //just fix table widget size hint
    setFixedHeight(totalHeight());
    Q_EMIT updateItemTitle(m_toolIndex);
}

void MusicAbstractDownloadTableWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicAbstractDownloadTableWidget::itemDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    musicPlay();
}

void MusicAbstractDownloadTableWidget::downloadProgressChanged(float percent, const QString &total, qint64 time)
{
    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 3);
        if(it && it->data(MUSIC_TIME_ROLE).toLongLong() == time)
        {
            item(i, 2)->setData(MUSIC_PROGRESS_ROLE, percent);
            item(i, 3)->setText(total);

            (*m_musicSongs)[i].setMusicSizeStr(total);
            break;
        }
    }
}

void MusicAbstractDownloadTableWidget::createDownloadItem(const QString &name, qint64 time)
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
    Q_EMIT updateItemTitle(m_toolIndex);
}

void MusicAbstractDownloadTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu menu(this);

    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);
    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"), this, SLOT(musicPlayClicked()));
    menu.addAction(tr("Download More..."), this, SLOT(musicSongDownload()));
    menu.addSeparator();

    createMoreMenu(&menu);

    const bool status = !m_musicSongs->isEmpty();
    menu.addAction(tr("Song Info..."), this, SLOT(musicFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_localFile"), tr("Open File Dir"), this, SLOT(musicOpenFileDir()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("Ablum"), this, SLOT(musicAlbumQueryWidget()));
    menu.addSeparator();

    menu.addAction(QIcon(":/contextMenu/btn_delete"), tr("Delete"), this, SLOT(setDeleteItemAt()))->setEnabled(status);
    menu.addAction(tr("Delete All"), this, SLOT(setDeleteItemAll()))->setEnabled(status);

    menu.exec(QCursor::pos());
}
