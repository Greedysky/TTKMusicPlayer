#include "musicabstractdownloadtablewidget.h"
#include "musicsongssummariziedwidget.h"
#include "musicdownloadmanager.h"
#include "musicconnectionpool.h"
#include "musicmessagebox.h"

MusicAbstractDownloadTableWidget::MusicAbstractDownloadTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent),
      m_type(TTK::Record::Null)
{
    m_progressBarDelegate = new TTKProgressBarItemDelegate(this);
    m_progressBarDelegate->setStyleSheet(TTK::UI::ProgressBar01);
    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->connect(className(), MusicSongsSummariziedWidget::className());
}

MusicAbstractDownloadTableWidget::~MusicAbstractDownloadTableWidget()
{
    G_CONNECTION_PTR->removeValue(this);
    MusicDownloadRecordConfigManager manager(this);
    if(!manager.load(TTK::toString(m_type)))
    {
        return;
    }

    manager.writeBuffer(*m_songs);
    removeItems();

    delete m_progressBarDelegate;
}

void MusicAbstractDownloadTableWidget::updateSongsList(const MusicSongList &songs)
{
    Q_UNUSED(songs);

    MusicDownloadRecordConfigManager manager(this);
    if(!manager.fromFile(TTK::toString(m_type)))
    {
        return;
    }

    manager.readBuffer(*m_songs);
    setRowCount(m_songs->count()); //reset row count

    for(int i = 0; i < m_songs->count(); ++i)
    {
        const MusicSong &v = m_songs->at(i);
        addCellItem(i, v);
        G_DOWNLOAD_MANAGER_PTR->reconnectMusicDownload(MusicDownLoadPairData(v.addTimeStr().toULongLong(), this, m_type));
    }
}

void MusicAbstractDownloadTableWidget::playSong()
{
    if(!isValid())
    {
        return;
    }

    const QString &path = m_songs->at(currentRow()).path();
    Q_EMIT addSongToPlaylist(QStringList(QFile::exists(path) ? path : QString()));
}

void MusicAbstractDownloadTableWidget::removeItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(!message.exec() || rowCount() == 0)
    {
       return;
    }

    const TTKIntList deleteList(multiSelectedIndexList());
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
    Q_EMIT updateItemTitle(m_playlistRow);
}

void MusicAbstractDownloadTableWidget::itemDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    playSong();
}

void MusicAbstractDownloadTableWidget::downloadProgressChanged(float percent, const QString &total, qint64 time)
{
    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 3);
        if(it && it->data(TTK_TIME_ROLE).toLongLong() == time)
        {
            item(i, 2)->setData(TTK_PROGRESS_ROLE, percent);
            item(i, 3)->setText(total);

            (*m_songs)[i].setSizeStr(total);
            break;
        }
    }
}

void MusicAbstractDownloadTableWidget::createDownloadItem(const QString &name, qint64 time)
{
    QString musicName = name;
    musicName.remove(TTK::String::musicDirPrefix()).chop(4);
    setRowCount(rowCount() + 1);

    MusicSong record;
    record.setName(musicName);
    record.setPath(QFileInfo(name).absoluteFilePath());
    record.setSizeStr("0.00M");
    record.setAddTimeStr(QString::number(time));
    m_songs->append(record);

    addCellItem(rowCount() - 1, record);
    Q_EMIT updateItemTitle(m_playlistRow);
}

void MusicAbstractDownloadTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu menu(this);

    menu.setStyleSheet(TTK::UI::MenuStyle02);
    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"), this, SLOT(playClicked()));
    menu.addAction(tr("Download More..."), this, SLOT(showDownloadWidget()));
    menu.addSeparator();

    createMoreMenu(&menu);

    const bool status = !m_songs->isEmpty();
    menu.addAction(tr("Song Info..."), this, SLOT(showFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_local_file"), tr("Open File Dir"), this, SLOT(openFileDir()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("Ablum"), this, SLOT(showAlbumQueryWidget()));
    menu.addSeparator();

    menu.addAction(QIcon(":/contextMenu/btn_delete"), tr("Delete"), this, SLOT(removeItemAt()))->setEnabled(status);
    menu.addAction(tr("Delete All"), this, SLOT(removeItemAll()))->setEnabled(status);

    menu.exec(QCursor::pos());
}
