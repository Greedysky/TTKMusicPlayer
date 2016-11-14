#include "musicsongsplayedlistwidget.h"
#include "musicfileinformationwidget.h"
#include "musicsongsharingwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicrightareawidget.h"
#include "musicdownloadwidget.h"
#include "musiccoreutils.h"
#include "musicmessagebox.h"

#include <qmath.h>
#include <QMenu>
#include <QScrollBar>
#include <QContextMenuEvent>

MusicSongsPlayedListWidget::MusicSongsPlayedListWidget(QWidget *parent)
    : MusicSlowMovingTableWidget(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setColumnCount(5);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 20);
    headerview->resizeSection(1, 197);
    headerview->resizeSection(2, 25);
    headerview->resizeSection(3, 25);
    headerview->resizeSection(4, 45);

    setStyleSheet(MusicUIObject::MTableWidgetStyle01 + MusicUIObject::MScrollBarStyle02);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle02);
    setMovedScrollBar( verticalScrollBar() );

    m_playRowIndex = -1;
}

MusicSongsPlayedListWidget::~MusicSongsPlayedListWidget()
{

}

QString MusicSongsPlayedListWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsPlayedListWidget::setSongsFileName(MusicPlayedSongs *songs)
{
    m_musicSongs = songs;
    updateSongsFileName(*m_musicSongs);
}

void MusicSongsPlayedListWidget::updateSongsFileName(const MusicPlayedSongs &songs)
{
    int count = rowCount();
    setRowCount(songs.count());    //reset row count
    for(int i=count; i<songs.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);
                          item = new QTableWidgetItem;
        item->setToolTip(songs[i].m_song.getMusicName());
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, 182));
        item->setTextColor(QColor(50, 50, 50));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);
                          item = new QTableWidgetItem;
        setItem(i, 2, item);
                          item = new QTableWidgetItem;
        setItem(i, 3, item);
                          item = new QTableWidgetItem(songs[i].m_song.getMusicTime());
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 4, item);
    }
}

void MusicSongsPlayedListWidget::selectRow(int index)
{
    if(index < 0 || rowCount() < 0)
    {
        return;
    }

    if(-1 < m_playRowIndex && m_playRowIndex < rowCount())
    {
        item(m_playRowIndex, 1)->setForeground(QColor(0, 0, 0));
    }

    QTableWidget::selectRow(index);
    m_playRowIndex = index;
    verticalScrollBar()->setSliderPosition( ceil(height()*1.0/rowCount())*index );

    item(m_playRowIndex, 1)->setForeground(QColor(0, 191, 255));
}

void MusicSongsPlayedListWidget::selectPlayedRow()
{
    selectRow(m_playRowIndex);
}

void MusicSongsPlayedListWidget::listCellEntered(int row, int column)
{
    ///clear previous table item state
    QTableWidgetItem *it = item(m_previousColorRow, 2);
    if(it != nullptr)
    {
        it->setIcon(QIcon());
    }
    it = item(m_previousColorRow, 3);
    if(it != nullptr)
    {
        it->setIcon(QIcon());
    }
    it = item(m_previousColorRow, 4);
    if(it != nullptr)
    {
        it->setIcon(QIcon());
        it->setText((*m_musicSongs)[m_previousColorRow].m_song.getMusicTime());
    }

    ///draw new table item state
    if((it = item(row, 2)) != nullptr)
    {
        it->setIcon(QIcon(":/contextMenu/btn_download"));
    }
    if((it = item(row, 3)) != nullptr)
    {
        it->setIcon(QIcon(":/tiny/btn_delete_normal"));
    }
    if((it = item(row, 4)) != nullptr)
    {
        it->setText(QString());
        it->setIcon(QIcon(":/tiny/btn_more_normal"));
    }

    if(column == 2 || column == 3 || column == 4)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }
    MusicSlowMovingTableWidget::listCellEntered(row, column);
}

void MusicSongsPlayedListWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    switch(column)
    {
        case 2:
            musicSongDownload();
            break;
        case 3:
            setDeleteItemAt();
            break;
        case 4:
            createMoreMenu();
            break;
        default:
            break;
    }
}

void MusicSongsPlayedListWidget::musicPlayClicked()
{

}

void MusicSongsPlayedListWidget::setDeleteItemAt()
{
    int index = currentRow();
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    removeRow( index );
    m_musicSongs->removeAt( index );
    emit updateCountLabel();
}

void MusicSongsPlayedListWidget::musicOpenFileDir()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    QString path = !m_musicSongs->isEmpty() ? m_musicSongs->at(currentRow()).m_song.getMusicPath() : QString();
    if(!MusicUtils::Core::openUrl(QFileInfo(path).absoluteFilePath(), true))
    {
        MusicMessageBox message;
        message.setText(tr("The origin one does not exist!"));
        message.exec();
    }
}

void MusicSongsPlayedListWidget::musicFileInformation()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicFileInformationWidget file;
    file.setFileInformation( getCurrentSongPath() );
    file.exec();
}

void MusicSongsPlayedListWidget::musicSongMovieFound()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicVideoButtonSearched( getCurrentSongName() );
}

void MusicSongsPlayedListWidget::musicAlbumFoundWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicAlbumFound( getCurrentSongName() );
}

void MusicSongsPlayedListWidget::musicSimilarFoundWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicSimilarFound( getCurrentSongName() );
}

void MusicSongsPlayedListWidget::musicSongSharedWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicSongSharingWidget shareWidget;
    shareWidget.setSongName( getCurrentSongName() );
    shareWidget.exec();
}

void MusicSongsPlayedListWidget::musicSongTransferWidget()
{
    if(m_musicSongs->isEmpty())
    {
        return;
    }

    MusicConnectTransferWidget transferWidget;
    transferWidget.redirectToCurrentSong(m_musicSongs->first().m_toolIndex, currentRow());
    transferWidget.exec();
}

void MusicSongsPlayedListWidget::musicSongDownload()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget;
    download->setSongName(getCurrentSongName(), MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();
}

void MusicSongsPlayedListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicSlowMovingTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);

    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay"), this, SLOT(musicPlayClicked()));
    rightClickMenu.addAction(tr("downloadMore..."), this, SLOT(musicSongDownload()));
    rightClickMenu.addSeparator();

    QMenu *addMenu = rightClickMenu.addMenu(QIcon(":/contextMenu/btn_add"), tr("addToList"));
    addMenu->addAction(tr("musicCloud"));

    rightClickMenu.addAction(QIcon(":/contextMenu/btn_mobile"), tr("songToMobile"), this, SLOT(musicSongTransferWidget()));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_ring"), tr("ringToMobile"), this, SLOT(musicSongTransferWidget()));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_similar"), tr("similar"), this, SLOT(musicSimilarFoundWidget()));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_share"), tr("songShare"), this, SLOT(musicSongSharedWidget()));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_kmicro"), tr("KMicro"));

    bool empty = true;
    rightClickMenu.addAction(tr("musicInfo..."), this, SLOT(musicFileInformation()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_localFile"), tr("openFileDir"), this, SLOT(musicOpenFileDir()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("ablum"), this, SLOT(musicAlbumFoundWidget()));
    rightClickMenu.addSeparator();

    rightClickMenu.addAction(QIcon(":/contextMenu/btn_delete"), tr("delete"), this, SLOT(setDeleteItemAt()))->setEnabled(empty);
    rightClickMenu.addSeparator();

    rightClickMenu.exec(QCursor::pos());
    event->accept();
}

void MusicSongsPlayedListWidget::createMoreMenu()
{
    QMenu menu;
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);

    QMenu *addMenu = menu.addMenu(QIcon(":/contextMenu/btn_add"), tr("addToList"));
    addMenu->addAction(tr("musicCloud"));

    menu.addAction(QIcon(":/contextMenu/btn_mobile"), tr("songToMobile"), this, SLOT(musicSongTransferWidget()));
    menu.addAction(QIcon(":/contextMenu/btn_ring"), tr("ringToMobile"), this, SLOT(musicSongTransferWidget()));
    menu.addAction(QIcon(":/contextMenu/btn_similar"), tr("similar"), this, SLOT(musicSimilarFoundWidget()));
    menu.addAction(QIcon(":/contextMenu/btn_share"), tr("songShare"), this, SLOT(musicSongSharedWidget()));
    menu.addAction(QIcon(":/contextMenu/btn_kmicro"), tr("KMicro"));

    menu.exec(QCursor::pos());
}

QString MusicSongsPlayedListWidget::getCurrentSongPath() const
{
    if(rowCount() == 0 || currentRow() < 0 )
    {
        return QString();
    }

    return getSongPath( currentRow() );
}

QString MusicSongsPlayedListWidget::getSongPath(int index) const
{
    return !m_musicSongs->isEmpty() ? m_musicSongs->at(index).m_song.getMusicPath().trimmed() : QString();
}

QString MusicSongsPlayedListWidget::getCurrentSongName() const
{
    if(rowCount() == 0 || currentRow() < 0 )
    {
        return QString();
    }

    return getSongName( currentRow() );
}

QString MusicSongsPlayedListWidget::getSongName(int index) const
{
    return !m_musicSongs->isEmpty() ? m_musicSongs->at(index).m_song.getMusicName().trimmed() : QString();
}
