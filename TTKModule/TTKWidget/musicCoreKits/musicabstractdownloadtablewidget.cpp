#include "musicabstractdownloadtablewidget.h"
#include "musicsongscontainerwidget.h"
#include "musicdownloadmanager.h"
#include "musicmessagebox.h"

MusicAbstractDownloadTableWidget::MusicAbstractDownloadTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent),
      m_type(TTK::Record::Null)
{
    m_progressBarDelegate = new TTKProgressBarItemDelegate(this);
    m_progressBarDelegate->setStyleSheet(TTK::UI::ProgressBar01);
    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
}

MusicAbstractDownloadTableWidget::~MusicAbstractDownloadTableWidget()
{
    MusicDownloadRecordConfigManager manager;
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

    MusicDownloadRecordConfigManager manager;
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
        G_DOWNLOAD_MANAGER_PTR->reconnectNetworkData(MusicDownLoadPairData(v.addTimeStr().toULongLong(), this, m_type));
    }
}

void MusicAbstractDownloadTableWidget::removeItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(!message.exec() || rowCount() == 0)
    {
       return;
    }

    const TTKIntList deletedList(selectedIndexList());
    if(deletedList.isEmpty())
    {
        return;
    }

    for(int i = deletedList.count() - 1; i >= 0; --i)
    {
        const int index = deletedList[i];
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

    if(!isValid())
    {
        return;
    }

    const QString &path = m_songs->at(currentRow()).path();
    MusicSongsContainerWidget::instance()->addSongToPlaylist(QFile::exists(path) ? QStringList(path) : QStringList());
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
    QFileInfo fin(name);
    setRowCount(rowCount() + 1);

    MusicSong record;
    record.setName(fin.completeBaseName());
    record.setPath(fin.absoluteFilePath());
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

    const bool status = !(m_songs->isEmpty() || TTK::String::isNetworkUrl(currentSongPath()));
    menu.addAction(tr("Song Info..."), this, SLOT(showFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_local_file"), tr("Open File Dir"), this, SLOT(openFileDir()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("Ablum"), this, SLOT(showAlbumQueryWidget()));
    menu.addSeparator();

    menu.addAction(QIcon(":/contextMenu/btn_delete"), tr("Delete"), this, SLOT(removeItemAt()))->setEnabled(status);
    menu.addAction(tr("Delete All"), this, SLOT(clearItems()))->setEnabled(status);

    menu.exec(QCursor::pos());
}
