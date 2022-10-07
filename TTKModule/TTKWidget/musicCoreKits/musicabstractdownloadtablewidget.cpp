#include "musicabstractdownloadtablewidget.h"
#include "musicsongssummariziedwidget.h"
#include "musicdownloadmanager.h"
#include "musicconnectionpool.h"
#include "musicitemdelegate.h"
#include "musicmessagebox.h"

MusicAbstractDownloadTableWidget::MusicAbstractDownloadTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent),
      m_type(MusicObject::Record::Null)
{
    m_progressBarDelegate = new MusicProgressBarDelegate(this);
    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->poolConnect(className(), MusicSongsSummariziedWidget::className());
}

MusicAbstractDownloadTableWidget::~MusicAbstractDownloadTableWidget()
{
    G_CONNECTION_PTR->removeValue(className());
    MusicDownloadRecordConfigManager xml(m_type, this);
    xml.writeBuffer(*m_songs);
    removeItems();

    delete m_progressBarDelegate;
}

void MusicAbstractDownloadTableWidget::updateSongsList(const MusicSongList &songs)
{
    Q_UNUSED(songs);

    MusicDownloadRecordConfigManager xml(m_type, this);
    if(!xml.fromFile())
    {
        return;
    }
    xml.readBuffer(*m_songs);

    setRowCount(m_songs->count()); //reset row count

    for(int i = 0; i < m_songs->count(); ++i)
    {
        MusicSong *song = &(*m_songs)[i];
        addItem(i, *song);
        G_DOWNLOAD_MANAGER_PTR->reconnectMusicDownload(MusicDownLoadPairData(song->addTimeStr().toULongLong(), this, m_type));
    }
}

void MusicAbstractDownloadTableWidget::musicPlay()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    const QString &path = m_songs->at(currentRow()).path();
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
        m_songs->removeAt(index);
    }

    setFixedHeight(totalHeight());
    Q_EMIT updateItemTitle(m_toolIndex);
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

            (*m_songs)[i].setSizeStr(total);
            break;
        }
    }
}

void MusicAbstractDownloadTableWidget::createDownloadItem(const QString &name, qint64 time)
{
    setRowCount(rowCount() + 1);
    QString musicName = name;
    musicName.remove(MusicUtils::String::musicDirPrefix()).chop(4);

    MusicSong record;
    record.setName(musicName);
    record.setPath(QFileInfo(name).absoluteFilePath());
    record.setSizeStr("0.00M");
    record.setAddTimeStr(QString::number(time));
    m_songs->append(record);

    addItem(rowCount() - 1, record);
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

    const bool status = !m_songs->isEmpty();
    menu.addAction(tr("Song Info..."), this, SLOT(musicFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_localFile"), tr("Open File Dir"), this, SLOT(musicOpenFileDir()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("Ablum"), this, SLOT(musicAlbumQueryWidget()));
    menu.addSeparator();

    menu.addAction(QIcon(":/contextMenu/btn_delete"), tr("Delete"), this, SLOT(setDeleteItemAt()))->setEnabled(status);
    menu.addAction(tr("Delete All"), this, SLOT(setDeleteItemAll()))->setEnabled(status);

    menu.exec(QCursor::pos());
}
