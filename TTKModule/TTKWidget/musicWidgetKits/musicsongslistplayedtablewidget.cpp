#include "musicsongslistplayedtablewidget.h"
#include "musicsongslistplayedwidget.h"

#include <qmath.h>

MusicSongsListPlayedTableWidget::MusicSongsListPlayedTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent),
      m_songsPlayWidget(nullptr)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnCount(5);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 20);
    headerView->resizeSection(1, 197);
    headerView->resizeSection(2, 25);
    headerView->resizeSection(3, 25);
    headerView->resizeSection(4, 45);

    TTK::Widget::setTransparent(this, 255);
}

MusicSongsListPlayedTableWidget::~MusicSongsListPlayedTableWidget()
{
    removeItems();
    delete m_songsPlayWidget;
}

void MusicSongsListPlayedTableWidget::updateSongsList(const MusicSongList &songs)
{
    const int count = rowCount();
    setRowCount(songs.count());

    QHeaderView *headerView = horizontalHeader();
    for(int i = count; i < songs.count(); ++i)
    {
        const MusicSong &v = songs[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(v.name());
        item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 15));
        item->setForeground(QColor(TTK::UI::Color01));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                          item = new QTableWidgetItem;
        setItem(i, 2, item);

                          item = new QTableWidgetItem;
        setItem(i, 3, item);

                          item = new QTableWidgetItem(v.duration());
        item->setForeground(QColor(TTK::UI::Color01));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 4, item);
    }

    setFixedHeight(qMax(365, totalRowHeight()));
}

void MusicSongsListPlayedTableWidget::selectRow(int index)
{
    if(index < 0 || rowCount() <= 0)
    {
        return;
    }

    MusicAbstractSongsListTableWidget::selectRow(index);

    adjustPlayWidgetRow();
    for(int i = 0; i < columnCount(); ++i)
    {
        delete takeItem(index, i);
    }

    const QString &name = songName(index);

    m_songsPlayWidget = new MusicSongsListPlayedWidget(index, this);
    m_songsPlayWidget->initialize(name);

    setSpan(index, 0, 1, 5);
    setCellWidget(index, 0, m_songsPlayWidget);
    m_playRowIndex = index;

    setFixedHeight(qMax(365, totalRowHeight()));

    if(scrollBar())
    {
        scrollBar()->setSliderPosition(index * 30);
    }
}

void MusicSongsListPlayedTableWidget::setQueueState(int index) const
{
    if(index < 0 || index >= rowCount())
    {
        return;
    }

    QTableWidgetItem *it = item(index, 0);
    if(it)
    {
        it->setIcon(QIcon(":/tiny/lb_playlater"));
    }
}

void MusicSongsListPlayedTableWidget::clearQueueState()
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

    const QString &name = songName(m_playRowIndex);

    removeCellWidget(m_playRowIndex, 0);
    delete takeItem(m_playRowIndex, 0);
    clearSpans();

    QHeaderView *headerView = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(m_playRowIndex, 0, item);

    item = new QTableWidgetItem;
    item->setToolTip(name);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 15));
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);

    setItem(m_playRowIndex, 1, item);
    setItem(m_playRowIndex, 2, new QTableWidgetItem);
    setItem(m_playRowIndex, 3, new QTableWidgetItem);

    item = new QTableWidgetItem((*m_songs)[m_playRowIndex].duration());
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(m_playRowIndex, 4, item);

    delete m_songsPlayWidget;
    m_songsPlayWidget = nullptr;

    setFixedHeight(qMax(365, totalRowHeight()));
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
        it->setText((*m_songs)[m_previousColorRow].duration());
    }

    ///draw new table item state
    if(it = item(row, 2))
    {
        it->setIcon(QIcon(":/contextMenu/btn_download"));
    }

    if(it = item(row, 3))
    {
        it->setIcon(QIcon(":/tiny/btn_delete_normal"));
    }

    if(it = item(row, 4))
    {
        it->setText({});
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
        case 2: showDownloadWidget(); break;
        case 3: removeItemAt(); break;
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

void MusicSongsListPlayedTableWidget::removeItems()
{
//    if(m_playRowIndex < 0)
//    {
//        return;
//    }

    //Remove play widget
    removeCellWidget(m_playRowIndex, 0);

    delete m_songsPlayWidget;
    m_songsPlayWidget = nullptr;

    m_playRowIndex = -1;
    //Remove all the original item
    MusicAbstractSongsListTableWidget::removeItems();
    setColumnCount(5);
}

void MusicSongsListPlayedTableWidget::removeItemAt()
{
    const TTKIntList deletedList(selectedIndexList());
    if(deletedList.isEmpty())
    {
        return;
    }

    if(deletedList.contains(m_playRowIndex) || deletedList[0] < m_playRowIndex)
    {
        adjustPlayWidgetRow();
    }

    for(int i = deletedList.count() - 1; i >= 0; --i)
    {
        const int index = deletedList[i];
        removeRow(index);
        m_songs->removeAt(index);
    }

    setFixedHeight(qMax(365, totalRowHeight()));
    Q_EMIT deleteItemAt(deletedList);
}

void MusicSongsListPlayedTableWidget::contextMenuEvent(QContextMenuEvent *event)
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

    menu.addAction(QIcon(":/contextMenu/btn_delete"), tr("Delete"), this, SLOT(removeItemAt()));
    menu.addSeparator();
    menu.exec(QCursor::pos());
}
