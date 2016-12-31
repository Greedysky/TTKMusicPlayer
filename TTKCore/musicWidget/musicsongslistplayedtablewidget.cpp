#include "musicsongslistplayedtablewidget.h"
#include "musicfileinformationwidget.h"
#include "musicsongsharingwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicrightareawidget.h"
#include "musicdownloadwidget.h"
#include "musiccoreutils.h"
#include "musicmessagebox.h"

#include <qmath.h>
#include <QScrollBar>
#include <QContextMenuEvent>

MusicSongsListPlayedTableWidget::MusicSongsListPlayedTableWidget(QWidget *parent)
    : MusicSongsListAbstractTableWidget(parent)
{
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
    m_hasParentToolIndex = false;
}

MusicSongsListPlayedTableWidget::~MusicSongsListPlayedTableWidget()
{

}

QString MusicSongsListPlayedTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsListPlayedTableWidget::updateSongsFileName(const MusicSongs &songs)
{
    int count = rowCount();
    setRowCount(songs.count());    //reset row count
    for(int i=count; i<songs.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);
                          item = new QTableWidgetItem;
        item->setToolTip(songs[i].getMusicName());
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, 182));
        item->setTextColor(QColor(50, 50, 50));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);
                          item = new QTableWidgetItem;
        setItem(i, 2, item);
                          item = new QTableWidgetItem;
        setItem(i, 3, item);
                          item = new QTableWidgetItem(songs[i].getMusicTime());
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 4, item);
    }
}

void MusicSongsListPlayedTableWidget::selectRow(int index)
{
    if(index < 0 || rowCount() < 0)
    {
        return;
    }

    if(-1 < m_playRowIndex && m_playRowIndex < rowCount())
    {
        item(m_playRowIndex, 1)->setForeground(QColor(0, 0, 0));
    }

    MusicSongsListAbstractTableWidget::selectRow(index);
    m_playRowIndex = index;
    verticalScrollBar()->setSliderPosition( ceil(height()*1.0/rowCount())*index );

    item(m_playRowIndex, 1)->setForeground(QColor(0, 191, 255));
}

void MusicSongsListPlayedTableWidget::selectPlayedRow()
{
    selectRow(m_playRowIndex);
}

void MusicSongsListPlayedTableWidget::listCellEntered(int row, int column)
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
        it->setText((*m_musicSongs)[m_previousColorRow].getMusicTime());
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
    MusicSongsListAbstractTableWidget::listCellEntered(row, column);
}

void MusicSongsListPlayedTableWidget::listCellClicked(int row, int column)
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
            {
                QMenu menu(this);
                createMoreMenu(&menu);
                menu.exec(QCursor::pos());
                break;
            }
        default:
            break;
    }
}

void MusicSongsListPlayedTableWidget::setDeleteItemAt()
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

void MusicSongsListPlayedTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicSongsListAbstractTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);

    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay"), this, SLOT(musicPlayClicked()));
    rightClickMenu.addAction(tr("downloadMore..."), this, SLOT(musicSongDownload()));
    rightClickMenu.addSeparator();

    createMoreMenu(&rightClickMenu);

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
