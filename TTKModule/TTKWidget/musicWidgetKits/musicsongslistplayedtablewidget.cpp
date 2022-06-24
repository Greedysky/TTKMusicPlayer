#include "musicsongslistplayedtablewidget.h"
#include "musicsongsharingwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicsongslistplayedwidget.h"
#include "musicrightareawidget.h"
#include "musicdownloadwidget.h"

#include <qmath.h>

MusicSongsListPlayedTableWidget::MusicSongsListPlayedTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setColumnCount(5);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 20);
    headerview->resizeSection(1, 197);
    headerview->resizeSection(2, 25);
    headerview->resizeSection(3, 25);
    headerview->resizeSection(4, 45);

    m_hasParentToolIndex = false;
    m_songsPlayWidget = nullptr;

    MusicUtils::Widget::setTransparent(this, 0xFF);
}

MusicSongsListPlayedTableWidget::~MusicSongsListPlayedTableWidget()
{
    clearAllItems();
    delete m_songsPlayWidget;
}

void MusicSongsListPlayedTableWidget::updateSongsFileName(const MusicSongList &songs)
{
    const int count = rowCount();
    setRowCount(songs.count());

    QHeaderView *headerview = horizontalHeader();
    for(int i = count; i < songs.count(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(songs[i].name());
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 15));  
        QtItemSetForegroundColor(item, MusicUIObject::MQSSColor01);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        setItem(i, 1, item);
                          item = new QTableWidgetItem;
        setItem(i, 2, item);
                          item = new QTableWidgetItem;
        setItem(i, 3, item);

                          item = new QTableWidgetItem(songs[i].playTime());
        QtItemSetForegroundColor(item, MusicUIObject::MQSSColor01);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 4, item);
    }

    setFixedHeight(qMax(365, totalHeight()));
}

void MusicSongsListPlayedTableWidget::selectRow(int index)
{
    if(index < 0 || rowCount() < 0)
    {
        return;
    }
    MusicAbstractSongsListTableWidget::selectRow(index);

    adjustPlayWidgetRow();
    for(int i = 0; i < columnCount(); ++i)
    {
        delete takeItem(index, i);
    }

    const QString &name = !m_songs->isEmpty() ? m_songs->at(index).name() : QString();

    m_songsPlayWidget = new MusicSongsListPlayedWidget(index, this);
    m_songsPlayWidget->setParameter(name);

    setSpan(index, 0, 1, 5);
    setCellWidget(index, 0, m_songsPlayWidget);
    m_playRowIndex = index;

    setFixedHeight(qMax(365, totalHeight()));

    if(m_scrollBar)
    {
        m_scrollBar->setSliderPosition(index * 30);
    }
}

void MusicSongsListPlayedTableWidget::clearPlayQueueState()
{
    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 0);
        if(it)
        {
            it->setIcon(QIcon());
        }
    }
}

void MusicSongsListPlayedTableWidget::setPlayQueueState(int row)
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

void MusicSongsListPlayedTableWidget::clearAllItems()
{
    if(m_playRowIndex < 0)
    {
        return;
    }

    //Remove play widget
    removeCellWidget(m_playRowIndex, 0);

    delete m_songsPlayWidget;
    m_songsPlayWidget = nullptr;

    m_playRowIndex = -1;
    //Remove all the original item
    MusicAbstractSongsListTableWidget::clear();
    setColumnCount(5);
}

void MusicSongsListPlayedTableWidget::selectPlayedRow()
{
    selectRow(m_playRowIndex);
}

void MusicSongsListPlayedTableWidget::adjustPlayWidgetRow()
{
    if(m_playRowIndex >= rowCount() || m_playRowIndex < 0)
    {
        return;
    }

    const QString &name = !m_songs->isEmpty() ? m_songs->at(m_playRowIndex).name() : QString();

    removeCellWidget(m_playRowIndex, 0);
    delete takeItem(m_playRowIndex, 0);
    clearSpans();

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(m_playRowIndex, 0, item);

    item = new QTableWidgetItem;
    item->setToolTip(name);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 15));
    QtItemSetForegroundColor(item, MusicUIObject::MQSSColor01);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    setItem(m_playRowIndex, 1, item);
    setItem(m_playRowIndex, 2, new QTableWidgetItem);
    setItem(m_playRowIndex, 3, new QTableWidgetItem);

    item = new QTableWidgetItem((*m_songs)[m_playRowIndex].playTime());
    QtItemSetForegroundColor(item, MusicUIObject::MQSSColor01);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(m_playRowIndex, 4, item);

    delete m_songsPlayWidget;
    m_songsPlayWidget = nullptr;

    //m_playRowIndex = -1;
    //just fix table widget size hint
    setFixedHeight(qMax(365, totalHeight()));
}

void MusicSongsListPlayedTableWidget::itemCellEntered(int row, int column)
{
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
        it->setText((*m_songs)[m_previousColorRow].playTime());
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

    MusicAbstractSongsListTableWidget::itemCellEntered(row, column);
}

void MusicSongsListPlayedTableWidget::itemCellClicked(int row, int column)
{
    if(row == m_playRowIndex)
    {
        return;
    }

    switch(column)
    {
        case 2: musicSongDownload(); break;
        case 3: setDeleteItemAt(); break;
        case 4:
        {
            QMenu menu(this);
            createMoreMenu(&menu);
            menu.exec(QCursor::pos());
            break;
        }
        default: break;
    }
}

void MusicSongsListPlayedTableWidget::setDeleteItemAt()
{
    const TTKIntList deleteList(multiSelectedIndex());
    if(deleteList.isEmpty())
    {
        return;
    }

    if(deleteList.contains(m_playRowIndex) || deleteList[0] < m_playRowIndex)
    {
        adjustPlayWidgetRow();
    }

    for(int i = deleteList.count() - 1; i >= 0; --i)
    {
        const int index = deleteList[i];
        removeRow(index);
        m_songs->removeAt(index);
    }

    //just fix table widget size hint
    setFixedHeight(qMax(365, totalHeight()));
    Q_EMIT deleteItemAt(deleteList);
}

void MusicSongsListPlayedTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);
    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"), this, SLOT(musicPlayClicked()));
    menu.addAction(tr("Download More..."), this, SLOT(musicSongDownload()));
    menu.addSeparator();

    createMoreMenu(&menu);

    const bool status = m_toolIndex != MUSIC_NETWORK_LIST;
    menu.addAction(tr("Song Info..."), this, SLOT(musicFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_localFile"), tr("Open File Dir"), this, SLOT(musicOpenFileDir()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("Ablum"), this, SLOT(musicAlbumQueryWidget()));
    menu.addSeparator();

    menu.addAction(QIcon(":/contextMenu/btn_delete"), tr("Delete"), this, SLOT(setDeleteItemAt()));
    menu.addSeparator();

    menu.exec(QCursor::pos());
}
