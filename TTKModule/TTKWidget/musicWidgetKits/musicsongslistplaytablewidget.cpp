#include "musicsongslistplaytablewidget.h"
#include "musicsongslistiteminfowidget.h"
#include "musicsongslistplaywidget.h"
#include "musictransformwidget.h"
#include "musicsongringtonemakerwidget.h"
#include "musicitemdelegate.h"
#include "musicmessagebox.h"
#include "musicprogresswidget.h"
#include "musicrightareawidget.h"
#include "musicopenfilewidget.h"
#include "musicplayedlistpopwidget.h"
#include "musicapplication.h"

#include <qmath.h>
#include <QAction>

MusicSongsListPlayTableWidget::MusicSongsListPlayTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent),
      m_parent(parent),
      m_dragStartIndex(-1),
      m_mouseMoved(false),
      m_openFileWidget(nullptr),
      m_songsInfoWidget(nullptr),
      m_songsPlayWidget(nullptr),
      m_mouseLeftPressed(false),
      m_renameActived(false),
      m_deleteItemWithFile(false),
      m_renameItem(nullptr),
      m_renameEditDelegate(nullptr),
      m_songSort(nullptr)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnCount(6);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 20);
    headerView->resizeSection(1, 187);
    headerView->resizeSection(2, 20);
    headerView->resizeSection(3, 20);
    headerView->resizeSection(4, 20);
    headerView->resizeSection(5, 45);

    TTK::Widget::setTransparent(this, 0);

    connect(&m_timerShow, SIGNAL(timeout()), SLOT(showTimeOut()));
    connect(&m_timerStay, SIGNAL(timeout()), SLOT(stayTimeOut()));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), MusicApplication::instance(), SLOT(playIndexClicked(int,int)));
}

MusicSongsListPlayTableWidget::~MusicSongsListPlayTableWidget()
{
    removeItems();
    delete m_openFileWidget;
    delete m_songsInfoWidget;
    delete m_songsPlayWidget;
    delete m_renameEditDelegate;
}

void MusicSongsListPlayTableWidget::updateSongsList(const MusicSongList &songs)
{
    if(createUploadFileModule())
    {
        return;
    }

    const int count = rowCount();
    setRowCount(songs.count());
    QHeaderView *headerView = horizontalHeader();

    for(int i = count; i < songs.count(); ++i)
    {
        const MusicSong &v = songs[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(TTK::Widget::elidedText(font(), v.name(), Qt::ElideRight, headerView->sectionSize(1) - 10));
        item->setForeground(QColor(TTK::UI::Color01));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                          item = new QTableWidgetItem;
        setItem(i, 2, item);

                          item = new QTableWidgetItem;
        setItem(i, 3, item);

                          item = new QTableWidgetItem;
        setItem(i, 4, item);

                          item = new QTableWidgetItem(v.duration());
        item->setForeground(QColor(TTK::UI::Color01));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 5, item);
    }

    setFixedHeight(totalRowHeight());
}

void MusicSongsListPlayTableWidget::selectRow(int index)
{
    if(index < 0)
    {
        return;
    }

    MusicAbstractSongsListTableWidget::selectRow(index);

    adjustPlayWidgetRow();
    for(int i = 0; i < columnCount(); ++i)
    {
        delete takeItem(index, i);
    }

    QString durationLabel;
    const QString &name = songName(index);
    const QString &path = songPath(index);

    m_songsPlayWidget = new MusicSongsListPlayWidget(index, this);
    m_songsPlayWidget->initialize(name, path, durationLabel);

    if(!m_songs->isEmpty())
    {
        MusicSong *song = &(*m_songs)[index];
        if(song->duration().isEmpty() || song->duration() == TTK_TIME_INIT)
        {
            song->setDuration(durationLabel);
        }
    }

    setSpan(index, 0, 1, 6);
    setCellWidget(index, 0, m_songsPlayWidget);
    setRowHeight(index, TTK_ITEM_SIZE_2XL);
    m_playRowIndex = index;

    setFixedHeight(totalRowHeight());
}

void MusicSongsListPlayTableWidget::updateSearchFileName(MusicSongList *songs, const TTKIntList &result)
{
    m_searchedSongs.clear();
    if(songs->count() == result.count())
    {
        m_songs = songs;
    }
    else
    {
        m_songs = &m_searchedSongs;
        for(int index : qAsConst(result))
        {
            m_songs->append(songs->at(index));
        }
    }

    removeItems();
    if(!m_songs->isEmpty())
    {
        updateSongsList(*m_songs);
    }
    else
    {
        setFixedHeight(totalRowHeight());
    }
}

void MusicSongsListPlayTableWidget::updateDurationLabel(const QString &current, const QString &total) const
{
    if(m_songsPlayWidget)
    {
        m_songsPlayWidget->updateDurationLabel(current, total);
    }
}

void MusicSongsListPlayTableWidget::updateCurrentArtist()
{
    if(m_songsPlayWidget)
    {
        m_songsPlayWidget->updateCurrentArtist();
    }
}

void MusicSongsListPlayTableWidget::adjustPlayWidgetRow()
{
    if(m_playRowIndex >= rowCount() || m_playRowIndex < 0)
    {
        return;
    }

    const QString &name = songName(m_playRowIndex);

    setRowHeight(m_playRowIndex, TTK_ITEM_SIZE_M);

    removeCellWidget(m_playRowIndex, 0);
    delete takeItem(m_playRowIndex, 0);
    clearSpans();

    QHeaderView *headerView = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(m_playRowIndex, 0, item);

    item = new QTableWidgetItem(TTK::Widget::elidedText(font(), name, Qt::ElideRight, headerView->sectionSize(1) - 10));
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);

    setItem(m_playRowIndex, 1, item);
    setItem(m_playRowIndex, 2, new QTableWidgetItem);
    setItem(m_playRowIndex, 3, new QTableWidgetItem);
    setItem(m_playRowIndex, 4, new QTableWidgetItem);

    item = new QTableWidgetItem((*m_songs)[m_playRowIndex].duration());
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(m_playRowIndex, 5, item);

    delete m_songsPlayWidget;
    m_songsPlayWidget = nullptr;

    m_playRowIndex = -1;
    setFixedHeight(totalRowHeight());
}

bool MusicSongsListPlayTableWidget::createUploadFileModule()
{
    if(m_songs->isEmpty() && TTK::playlistRowValid(m_playlistRow))
    {
        setFixedSize(LEFT_SIDE_WIDTH_MIN, 100);
        if(m_openFileWidget == nullptr && m_parent)
        {
            m_openFileWidget = new MusicOpenFileWidget(this);
            connect(m_openFileWidget, SIGNAL(uploadFileClicked()), m_parent, SLOT(importSongsByFiles()));
            connect(m_openFileWidget, SIGNAL(uploadDirClicked()), m_parent, SLOT(importSongsByDir()));
            m_openFileWidget->adjustWidgetRect(width(), height());
        }

        if(m_openFileWidget)
        {
            m_openFileWidget->raise();
            m_openFileWidget->show();
        }
        return true;
    }
    else
    {
        delete m_openFileWidget;
        m_openFileWidget = nullptr;
    }
    return false;
}

void MusicSongsListPlayTableWidget::itemCellEntered(int row, int column)
{
    QTableWidgetItem *it = item(m_previousColorRow, 0);
    if(it)
    {
        it->setIcon(QIcon());
    }

    it = item(m_previousColorRow, 2);
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
    }

    it = item(m_previousColorRow, 5);
    if(it)
    {
        it->setIcon(QIcon());
        it->setText((*m_songs)[m_previousColorRow].duration());
    }

    ///draw new table item state
    if(it = item(row, 0))
    {
        it->setIcon(QIcon(":/tiny/btn_play_later_normal"));
    }

    if(it = item(row, 2))
    {
        it->setIcon(QIcon(":/tiny/btn_mv_normal"));
    }

    if(it = item(row, 3))
    {
        const bool contains = MusicApplication::instance()->containsLovestItem(row);
        it->setIcon(QIcon(contains ? ":/tiny/btn_loved_normal" : ":/tiny/btn_unloved_normal"));
    }

    if(it = item(row, 4))
    {
        it->setIcon(QIcon(":/tiny/btn_delete_normal"));
    }

    if(it = item(row, 5))
    {
        it->setText({});
        it->setIcon(QIcon(":/tiny/btn_more_normal"));
    }

    if(column != 1)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }

    MusicAbstractSongsListTableWidget::itemCellEntered(row, column);

    //To show music Songs Item information
    if(m_songsInfoWidget == nullptr)
    {
        m_songsInfoWidget = new MusicSongsListItemInfoWidget;
        m_songsInfoWidget->hide();
    }

    m_timerShow.stop();
    m_timerShow.start(0.5 * TTK_DN_S2MS);

    m_timerStay.stop();
    m_timerStay.start(3 * TTK_DN_S2MS);
}

void MusicSongsListPlayTableWidget::itemCellClicked(int row, int column)
{
    if(row == m_playRowIndex)
    {
        return;
    }

    switch(column)
    {
        case 0:
        {
            addToPlayLater();
            break;
        }
        case 2:
        {
            showMovieQueryWidget();
            break;
        }
        case 3:
        {
            bool empty;
            Q_EMIT isSearchedResultEmpty(empty);

            if(empty)
            {
                const bool contains = !MusicApplication::instance()->containsLovestItem(row);
                QTableWidgetItem *it = item(row, 3);
                if(it)
                {
                    it->setIcon(QIcon(contains ? ":/tiny/btn_loved_normal" : ":/tiny/btn_unloved_normal"));
                }
                Q_EMIT addSongToLovestList(contains, row);
            }
            break;
        }
        case 4:
        {
            bool empty;
            Q_EMIT isSearchedResultEmpty(empty);

            if(empty)
            {
                removeItemAt();
            }
            break;
        }
        case 5:
        {
            QMenu menu(this);
            createMoreMenu(&menu);
            menu.exec(QCursor::pos());
            break;
        }
        default: break;
    }
}

void MusicSongsListPlayTableWidget::removeItems()
{
//    if(m_playRowIndex < 0)
//    {
//        return;
//    }

    //Remove play widget
    setRowHeight(m_playRowIndex, TTK_ITEM_SIZE_M);
    removeCellWidget(m_playRowIndex, 0);

    delete m_songsPlayWidget;
    m_songsPlayWidget = nullptr;

    m_playRowIndex = -1;
    //Remove all the original item
    MusicAbstractSongsListTableWidget::removeItems();
    setColumnCount(6);
}

void MusicSongsListPlayTableWidget::removeItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(!message.exec() || !isValid())
    {
        clearSelection();
        return;
    }

    const TTKIntList deletedList(selectedIndexList());
    if(deletedList.isEmpty())
    {
        return;
    }

    MusicProgressWidget progress;
    progress.show();
    progress.setTitle(tr("Delete file mode"));
    progress.setRange(0, deletedList.count() / 3 * 2);

    for(int i = 0; i < deletedList.count(); ++i)
    {
        if(i % 3 == 0)
        {
            progress.setValue(i / 3);
        }
    }

    if(deletedList.contains(m_playRowIndex) || deletedList[0] < m_playRowIndex)
    {
        adjustPlayWidgetRow();
    }

    for(int i = deletedList.count() - 1; i >= 0; --i)
    {
        const int index = deletedList[i];
        removeRow(index);
        progress.setValue(deletedList.count() * 2 - i);
    }

    setFixedHeight(totalRowHeight());
    Q_EMIT deleteItemAt(deletedList, m_deleteItemWithFile);
}

void MusicSongsListPlayTableWidget::removeItemWithFile()
{
    m_deleteItemWithFile = true;
    removeItemAt();
    m_deleteItemWithFile = false;
}

void MusicSongsListPlayTableWidget::setChangSongName()
{
    if(!isValid())
    {
        return;
    }

    //the playing widget allow renaming
    if((currentRow() == m_playRowIndex) && m_songsPlayWidget)
    {
        m_songsPlayWidget->enableRenameMode();
        return;
    }

    //others
    delete m_renameEditDelegate;
    m_renameEditDelegate = new MusicLineEditItemDelegate(this);
    setItemDelegateForRow(currentRow(), m_renameEditDelegate);

    m_renameActived = true;
    m_renameItem = item(currentRow(), 1);
    m_renameItem->setText((*m_songs)[m_renameItem->row()].name());
    openPersistentEditor(m_renameItem);
    editItem(m_renameItem);
}

void MusicSongsListPlayTableWidget::showMakeRingWidget()
{
    if(!isValid())
    {
        return;
    }

    MusicSongRingtoneMaker(this).exec();
}

void MusicSongsListPlayTableWidget::showTransformWidget()
{
    if(!isValid())
    {
        return;
    }

    MusicTransformWidget(this).exec();
}

void MusicSongsListPlayTableWidget::searchQueryByName(QAction *action)
{
    if(action->data().isNull())
    {
        return;
    }

    MusicRightAreaWidget::instance()->showSongSearchedFound(action->data().toString());
}

void MusicSongsListPlayTableWidget::addToPlayLater()
{
    const int row = TTKObjectCast(QPushButton*, sender()) ? m_playRowIndex : currentRow();
    if(rowCount() == 0 || row < 0)
    {
        return;
    }

    MusicPlayedListPopWidget::instance()->insert(m_playlistRow, (*m_songs)[row]);
}

void MusicSongsListPlayTableWidget::addToPlayedList()
{
    const int row = currentRow();
    if(rowCount() == 0 || row < 0)
    {
        return;
    }

    MusicPlayedListPopWidget::instance()->append(m_playlistRow, (*m_songs)[row]);
}

void MusicSongsListPlayTableWidget::itemRenameFinished(const QString &name)
{
    if(m_playRowIndex >= rowCount() || m_playRowIndex < 0)
    {
        return;
    }

    (*m_songs)[m_playRowIndex].setName(name);
}

void MusicSongsListPlayTableWidget::songListSortBy(QAction *action)
{
    const int newType = action->data().toInt();
    if(newType < 0 || newType > 5)
    {
        return;
    }

    if(m_songSort)
    {
        const int type = m_songSort->m_type;
        m_songSort->m_type = newType;

        if(type == newType)
        {
            const bool asc = m_songSort->m_order == Qt::AscendingOrder;
            m_songSort->m_order = asc ? Qt::DescendingOrder : Qt::AscendingOrder;
        }
        else
        {
            m_songSort->m_order = Qt::AscendingOrder;
        }
        Q_EMIT songListSortBy(m_playlistRow);
    }
}

void MusicSongsListPlayTableWidget::showTimeOut()
{
    m_timerShow.stop();
    if(!m_songsInfoWidget || m_previousColorRow < 0 || m_previousColorRow >= m_songs->count())
    {
        return;
    }

    const MusicSong &song = (*m_songs)[m_previousColorRow];
    m_songsInfoWidget->initialize(m_playlistRow, song);
    m_songsInfoWidget->move(mapToGlobal(QPoint(width(), 0)).x() + 8, QCursor::pos().y());

    bool state;
    Q_EMIT isCurrentPlaylistRow(state);
    m_songsInfoWidget->setVisible(state ? (m_songsPlayWidget && !m_songsPlayWidget->isRenameMode()) : true);
}

void MusicSongsListPlayTableWidget::stayTimeOut()
{
    m_timerStay.stop();
    delete m_songsInfoWidget;
    m_songsInfoWidget = nullptr;
}

void MusicSongsListPlayTableWidget::mousePressEvent(QMouseEvent *event)
{
    MusicAbstractSongsListTableWidget::mousePressEvent(event);
    closeRenameItem();

    if(event->button() == Qt::LeftButton)
    {
        m_mouseLeftPressed = true;
        m_dragStartIndex = currentRow();
        m_dragStartPoint = event->pos();
    }
}

void MusicSongsListPlayTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    MusicAbstractSongsListTableWidget::mouseMoveEvent(event);
    if(m_mouseLeftPressed && abs(m_dragStartPoint.y() - event->pos().y()) > 15)
    {
        m_mouseMoved = true;
        setCursor(QCursor(QPixmap(":/functions/lb_drag_cursor")));
    }
}

void MusicSongsListPlayTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    MusicAbstractSongsListTableWidget::mouseReleaseEvent(event);
    startToDrag();

    m_mouseMoved = false;
    m_mouseLeftPressed = false;
    setCursor(QCursor(Qt::ArrowCursor));
}

void MusicSongsListPlayTableWidget::leaveEvent(QEvent *event)
{
    MusicAbstractSongsListTableWidget::leaveEvent(event);

    delete m_songsInfoWidget;
    m_songsInfoWidget = nullptr;
    closeRenameItem();
}

void MusicSongsListPlayTableWidget::wheelEvent(QWheelEvent *event)
{
    MusicAbstractSongsListTableWidget::wheelEvent(event);
    closeRenameItem();
    Q_EMIT showFloatWidget();
}

void MusicSongsListPlayTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu menu(this);
    menu.setStyleSheet(TTK::UI::MenuStyle02);

    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"), this, SLOT(playClicked()));
    menu.addAction(tr("Play Later"), this, SLOT(addToPlayLater()));
    menu.addAction(tr("Add To Playlist"), this, SLOT(addToPlayedList()));
    menu.addAction(tr("Download More..."), this, SLOT(showDownloadWidget()));
    menu.addSeparator();

    QMenu *sortFilesMenu = menu.addMenu(tr("Sort"));
    sortFilesMenu->addAction(tr("Sort By FileName"))->setData(0);
    sortFilesMenu->addAction(tr("Sort By Singer"))->setData(1);
    sortFilesMenu->addAction(tr("Sort By FileSize"))->setData(2);
    sortFilesMenu->addAction(tr("Sort By AddTime"))->setData(3);
    sortFilesMenu->addAction(tr("Sort By Duration"))->setData(4);
    sortFilesMenu->addAction(tr("Sort By PlayCount"))->setData(5);
    TTK::Widget::adjustMenuPosition(sortFilesMenu);
    connect(sortFilesMenu, SIGNAL(triggered(QAction*)), SLOT(songListSortBy(QAction*)));

    if(m_songSort)
    {
        const QList<QAction*> actions(sortFilesMenu->actions());
        if(-1 < m_songSort->m_type && m_songSort->m_type < actions.count())
        {
            const bool asc = m_songSort->m_order == Qt::AscendingOrder;
            actions[m_songSort->m_type]->setIcon(QIcon(asc ? ":/tiny/lb_sort_asc" : ":/tiny/lb_sort_desc"));
        }
    }

    menu.addAction(tr("Found Movie"), this, SLOT(showMovieQueryWidget()));
    menu.addSeparator();

    createMoreMenu(&menu);

    QMenu *toolMenu = menu.addMenu(tr("Tools"));
    toolMenu->addAction(tr("Make Bell"), this, SLOT(showMakeRingWidget()));
    toolMenu->addAction(tr("Make Transform"), this, SLOT(showTransformWidget()));
    TTK::Widget::adjustMenuPosition(toolMenu);

    bool status = !(m_songs->isEmpty() || TTK::String::isNetworkUrl(currentSongPath()));
    menu.addAction(tr("Song Info..."), this, SLOT(showFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_local_file"), tr("Open File Dir"), this, SLOT(openFileDir()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("Ablum"), this, SLOT(showAlbumQueryWidget()));
    menu.addSeparator();

    status = false;
    Q_EMIT isSearchedResultEmpty(status);
    menu.addAction(tr("Rename"), this, SLOT(setChangSongName()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_delete"), tr("Delete"), this, SLOT(removeItemAt()))->setEnabled(status);
    menu.addAction(tr("Delete With File"), this, SLOT(removeItemWithFile()))->setEnabled(status);
    menu.addAction(tr("Delete All"), this, SLOT(clearItems()))->setEnabled(status);
    menu.addSeparator();

    const QString &songName = currentSongName();
    QString name = TTK::generateSongTitle(songName);
    menu.addAction(tr("Search '%1'").arg(name))->setData(name);

    const QStringList artists(TTK::String::split(TTK::generateSongArtist(songName), ";"));
    for(int i = 0; i < artists.count(); ++i)
    {
        name = artists[i].trimmed();
        menu.addAction(tr("Search '%1'").arg(name))->setData(name);
    }
    menu.addAction(tr("Search '%1'").arg(songName))->setData(songName);

    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(searchQueryByName(QAction*)));
    menu.exec(QCursor::pos());
}

void MusicSongsListPlayTableWidget::closeRenameItem()
{
    if(!m_renameItem)
    {
        return;
    }

    //just close the rename editor;
    if(m_renameActived)
    {
        closePersistentEditor(m_renameItem);
    }
    //it may be a bug in closePersistentEditor,so we select
    //the two if function to deal with
    if(m_renameActived)
    {
        QHeaderView *headerView = horizontalHeader();
        (*m_songs)[m_renameItem->row()].setName(m_renameItem->text());
        m_renameItem->setText(TTK::Widget::elidedText(font(), m_renameItem->text(), Qt::ElideRight, headerView->sectionSize(1) - 10));

        m_renameActived = false;
        setItemDelegateForRow(m_renameItem->row(), nullptr);
        m_renameItem = nullptr;

        delete m_renameEditDelegate;
        m_renameEditDelegate = nullptr;
    }
}

void MusicSongsListPlayTableWidget::startToDrag()
{
    bool empty;
    Q_EMIT isSearchedResultEmpty(empty);

    if(empty && m_dragStartIndex > -1 && m_mouseLeftPressed && m_mouseMoved)
    {
        MusicSongList songs;
        const int start = m_dragStartIndex;
        const int end = currentRow();
        int index = m_playRowIndex;

        if(m_playRowIndex == start)
        {
            index = end;
        }
        else if(m_playRowIndex == end)
        {
            index = (start > end) ? (end + 1) : (end - 1);
        }
        else
        {
            if(start > m_playRowIndex && end < m_playRowIndex)
            {
                ++index;
            }
            else if(start < m_playRowIndex && end > m_playRowIndex)
            {
                --index;
            }
        }

        Q_EMIT itemIndexSwaped(start, end, index, songs);
        for(int i = qMin(start, end); i <= qMax(start, end); ++i)
        {
            if(i == index)
            {
                continue; //skip the current play item index, because the play widget just has one item
            }

            QHeaderView *headerView = horizontalHeader();
            item(i, 1)->setText(TTK::Widget::elidedText(font(), songs[i].name(), Qt::ElideRight, headerView->sectionSize(1) - 10));
            item(i, 5)->setText(songs[i].duration());
        }

        bool state;
        Q_EMIT isCurrentPlaylistRow(state);

        if(state)
        {
            selectRow(index);
        }
    }
}
