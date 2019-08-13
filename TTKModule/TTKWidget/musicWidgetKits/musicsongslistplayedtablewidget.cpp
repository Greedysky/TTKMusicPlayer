#include "musicsongslistplayedtablewidget.h"
#include "musicsongsharingwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicsongslistplayedwidget.h"
#include "musicrightareawidget.h"
#include "musicdownloadwidget.h"
#include "musiccoreutils.h"
#include "musicmessagebox.h"

#include <qmath.h>
#include <QScrollBar>

MusicSongsListPlayedTableWidget::MusicSongsListPlayedTableWidget(QWidget *parent)
    : MusicSongsListAbstractTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setColumnCount(5);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 20);
    headerview->resizeSection(1, 197);
    headerview->resizeSection(2, 25);
    headerview->resizeSection(3, 25);
    headerview->resizeSection(4, 45);

    m_playRowIndex = -1;
    m_hasParentToolIndex = false;
    m_musicSongsPlayWidget = nullptr;

    MusicUtils::Widget::setTransparent(this, 0xFF);
}

MusicSongsListPlayedTableWidget::~MusicSongsListPlayedTableWidget()
{
    clearAllItems();
    delete m_musicSongsPlayWidget;
}

void MusicSongsListPlayedTableWidget::clearPlayLaterState()
{
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 0);
        if(it)
        {
            it->setIcon(QIcon());
        }
    }
}

void MusicSongsListPlayedTableWidget::setPlayLaterState(int row)
{
    if(row < 0 || row >= rowCount())
    {
        return;
    }

    QTableWidgetItem *it = item(row, 0);
    if(it)
    {
        it->setIcon(QIcon(":/tiny/lb_playlater"));
    }
}

void MusicSongsListPlayedTableWidget::updateSongsFileName(const MusicSongs &songs)
{
    const int count = rowCount();
    setRowCount(songs.count());

    QHeaderView *headerview = horizontalHeader();
    for(int i=count; i<songs.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(songs[i].getMusicName());
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 15));
        item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        setItem(i, 1, item);
                          item = new QTableWidgetItem;
        setItem(i, 2, item);
                          item = new QTableWidgetItem;
        setItem(i, 3, item);

                          item = new QTableWidgetItem(songs[i].getMusicPlayTime());
        item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 4, item);
    }

    setFixedHeight( qMax(365, allRowsHeight()) );
}

void MusicSongsListPlayedTableWidget::clearAllItems()
{
    if(m_playRowIndex < 0)
    {
        return;
    }

    //Remove play widget
    removeCellWidget(m_playRowIndex, 0);

    delete m_musicSongsPlayWidget;
    m_musicSongsPlayWidget = nullptr;

    m_playRowIndex = -1;
    //Remove all the original item
    MusicSongsListAbstractTableWidget::clear();
    setColumnCount(5);
}

void MusicSongsListPlayedTableWidget::selectRow(int index)
{
    if(index < 0 || rowCount() < 0)
    {
        return;
    }
    MusicSongsListAbstractTableWidget::selectRow(index);

    replacePlayWidgetRow();
    for(int i=0; i<columnCount(); ++i)
    {
        delete takeItem(index, i);
    }

    const QString &name = !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicName() : QString();

    m_musicSongsPlayWidget = new MusicSongsListPlayedWidget(index, this);
    m_musicSongsPlayWidget->setParameter(name);

    setSpan(index, 0, 1, 5);
    setCellWidget(index, 0, m_musicSongsPlayWidget);
    m_playRowIndex = index;

    setFixedHeight( qMax(365, allRowsHeight()) );

    if(m_scrollBar)
    {
        m_scrollBar->setSliderPosition(index*30);
    }
}

void MusicSongsListPlayedTableWidget::selectPlayedRow()
{
    selectRow(m_playRowIndex);
}

void MusicSongsListPlayedTableWidget::replacePlayWidgetRow()
{
    if(m_playRowIndex >= rowCount() || m_playRowIndex < 0)
    {
        return;
    }

    const QString &name = !m_musicSongs->isEmpty() ? m_musicSongs->at(m_playRowIndex).getMusicName() : QString();

    removeCellWidget(m_playRowIndex, 0);
    delete takeItem(m_playRowIndex, 0);
    clearSpans();

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(m_playRowIndex, 0, item);

    item = new QTableWidgetItem;
    item->setToolTip(name);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 15));
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    setItem(m_playRowIndex, 1, item);
    setItem(m_playRowIndex, 2, new QTableWidgetItem);
    setItem(m_playRowIndex, 3, new QTableWidgetItem);

    item = new QTableWidgetItem( (*m_musicSongs)[m_playRowIndex].getMusicPlayTime() );
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(m_playRowIndex, 4, item);

    delete m_musicSongsPlayWidget;
    m_musicSongsPlayWidget = nullptr;

    setFixedHeight( qMax(365, allRowsHeight()) );
}

void MusicSongsListPlayedTableWidget::listCellEntered(int row, int column)
{
    ///clear previous table item state
    QTableWidgetItem *it = item(m_previousColorRow, 2);
    if(it)
    {
        it->setIcon(QIcon());
    }

    it = item(m_previousColorRow, 3);
    if(it)
    {
        it->setIcon(QIcon());
    }

    it = item(m_previousColorRow, 4);
    if(it)
    {
        it->setIcon(QIcon());
        it->setText((*m_musicSongs)[m_previousColorRow].getMusicPlayTime());
    }

    ///draw new table item state
    if((it = item(row, 2)))
    {
        it->setIcon(QIcon(":/contextMenu/btn_download"));
    }
    if((it = item(row, 3)))
    {
        it->setIcon(QIcon(":/tiny/btn_delete_normal"));
    }
    if((it = item(row, 4)))
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
    if(row == m_playRowIndex)
    {
        return;
    }

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
    const int index = currentRow();
    if(rowCount() == 0 || index < 0)
    {
        return;
    }

    replacePlayWidgetRow();

    if(index < m_playRowIndex)
    {
        --m_playRowIndex;
    }

    removeRow( index );
    m_musicSongs->removeAt( index );

    setFixedHeight( qMax(365, allRowsHeight()) );

    emit setDeleteItemAt(index);
}

void MusicSongsListPlayedTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu rightClickMenu(this);

    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay"), this, SLOT(musicPlayClicked()));
    rightClickMenu.addAction(tr("downloadMore..."), this, SLOT(musicSongDownload()));
    rightClickMenu.addSeparator();

    createMoreMenu(&rightClickMenu);

    const bool empty = true;
    rightClickMenu.addAction(tr("musicInfo..."), this, SLOT(musicFileInformation()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_localFile"), tr("openFileDir"), this, SLOT(musicOpenFileDir()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("ablum"), this, SLOT(musicAlbumFoundWidget()));
    rightClickMenu.addSeparator();

    rightClickMenu.addAction(QIcon(":/contextMenu/btn_delete"), tr("delete"), this, SLOT(setDeleteItemAt()))->setEnabled(empty);
    rightClickMenu.addSeparator();

    rightClickMenu.exec(QCursor::pos());
}
