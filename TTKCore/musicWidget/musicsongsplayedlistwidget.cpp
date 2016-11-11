#include "musicsongsplayedlistwidget.h"

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

    setStyleSheet(MusicUIObject::MTableWidgetStyle01);
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
    m_songLists = songs;
    updateSongsFileName(*m_songLists);
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
    if(index < 0)
    {
        return;
    }
    QTableWidget::selectRow(index);
    m_playRowIndex = index;
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
        it->setText((*m_songLists)[m_previousColorRow].m_song.getMusicTime());
    }

    ///draw new table item state
    if((it = item(row, 2)) != nullptr)
    {
        it->setIcon(QIcon(":/tiny/btn_unloved_hover"));
    }
    if((it = item(row, 3)) != nullptr)
    {
        it->setIcon(QIcon(":/tiny/btn_delete_hover"));
    }
    if((it = item(row, 4)) != nullptr)
    {
        it->setText(QString());
        it->setIcon(QIcon(":/tiny/btn_more_hover"));
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
    //the playing widget allow deleting
//    if(row == m_playRowIndex)
//    {
//        return;
//    }

    switch(column)
    {
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
    m_songLists->removeAt( index );
    emit updateCountLabel();
}

void MusicSongsPlayedListWidget::musicOpenFileDir()
{

}

void MusicSongsPlayedListWidget::musicFileInformation()
{

}

void MusicSongsPlayedListWidget::musicSongMovieFound()
{

}

void MusicSongsPlayedListWidget::musicAlbumFoundWidget()
{

}

void MusicSongsPlayedListWidget::musicSimilarFoundWidget()
{

}

void MusicSongsPlayedListWidget::musicSongSharedWidget()
{

}

void MusicSongsPlayedListWidget::musicSongTransferWidget()
{

}

void MusicSongsPlayedListWidget::musicSongDownload()
{

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
//    emit isSearchFileListEmpty(empty);
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
