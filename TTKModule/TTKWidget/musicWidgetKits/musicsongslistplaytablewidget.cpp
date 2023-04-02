#include "musicsongslistplaytablewidget.h"
#include "musicsongslistiteminfowidget.h"
#include "musicsongslistplaywidget.h"
#include "musictransformwidget.h"
#include "musicsongringtonemakerwidget.h"
#include "musicitemdelegate.h"
#include "musicmessagebox.h"
#include "musicprogresswidget.h"
#include "musicstringutils.h"
#include "musicsongsharingwidget.h"
#include "musicrightareawidget.h"
#include "musicopenfilewidget.h"
#include "musicplayedlistpopwidget.h"
#include "musicapplication.h"
#include "musicleftareawidget.h"

#include <qmath.h>
#include <QAction>

#define SEARCH_ITEM_DEFINED(index, names)                                                   \
    case index:                                                                             \
    {                                                                                       \
        if(names.count() >= index)                                                          \
        {                                                                                   \
            MusicRightAreaWidget::instance()->musicSongSearchedFound(names[index - 1]);     \
        }                                                                                   \
        break;                                                                              \
    }


MusicSongsListPlayTableWidget::MusicSongsListPlayTableWidget(int index, QWidget *parent)
    : MusicAbstractSongsListTableWidget(index, parent),
      m_parent(parent),
      m_dragStartIndex(-1),
      m_mouseMoved(false),
      m_openFileWidget(nullptr),
      m_songsInfoWidget(nullptr),
      m_songsPlayWidget(nullptr),
      m_leftButtonPressed(false),
      m_renameActived(false),
      m_deleteItemWithFile(false),
      m_renameItem(nullptr),
      m_renameEditDelegate(nullptr),
      m_songSort(nullptr)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setColumnCount(6);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 20);
    headerview->resizeSection(1, 187);
    headerview->resizeSection(2, 20);
    headerview->resizeSection(3, 20);
    headerview->resizeSection(4, 20);
    headerview->resizeSection(5, 45);

    TTK::Widget::setTransparent(this, 0);

    connect(&m_timerShow, SIGNAL(timeout()), SLOT(showTimeOut()));
    connect(&m_timerStay, SIGNAL(timeout()), SLOT(stayTimeOut()));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), MusicApplication::instance(), SLOT(musicPlayIndexClicked(int,int)));
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
    QHeaderView *headerview = horizontalHeader();

    for(int i = count; i < songs.count(); ++i)
    {
        const MusicSong &v = songs[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(TTK::Widget::elidedText(font(), v.name(), Qt::ElideRight, headerview->sectionSize(1) - 10));
        item->setForeground(QColor(TTK::UI::Color01));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                          item = new QTableWidgetItem;
        setItem(i, 2, item);

                          item = new QTableWidgetItem;
        setItem(i, 3, item);

                          item = new QTableWidgetItem;
        setItem(i, 4, item);

                          item = new QTableWidgetItem(v.playTime());
        item->setForeground(QColor(TTK::UI::Color01));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 5, item);
    }

    setFixedHeight(totalHeight());
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

    const QString &name = !m_songs->isEmpty() ? m_songs->at(index).name() : QString();
    const QString &path = !m_songs->isEmpty() ? m_songs->at(index).path() : QString();
    QString timeLabel;

    m_songsPlayWidget = new MusicSongsListPlayWidget(index, this);
    m_songsPlayWidget->setParameter(name, path, timeLabel);

    if(!m_songs->isEmpty())
    {
        MusicSong *song = &(*m_songs)[index];
        if(song->playTime().isEmpty() || song->playTime() == MUSIC_TIME_INIT)
        {
            song->setPlayTime(timeLabel);
        }
    }

    setSpan(index, 0, 1, 6);
    setCellWidget(index, 0, m_songsPlayWidget);
    setRowHeight(index, TTK_ITEM_SIZE_XL);
    m_playRowIndex = index;

    setFixedHeight(totalHeight());
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
            m_songs->append((*songs)[index]);
        }
    }

    removeItems();
    if(!m_songs->isEmpty())
    {
        updateSongsList(*m_songs);
    }
    else
    {
        setFixedHeight(totalHeight());
    }
}

void MusicSongsListPlayTableWidget::updateTimeLabel(const QString &current, const QString &total) const
{
    if(m_songsPlayWidget)
    {
        m_songsPlayWidget->updateTimeLabel(current, total);
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

    const QString &name = !m_songs->isEmpty() ? m_songs->at(m_playRowIndex).name() : QString();

    setRowHeight(m_playRowIndex, TTK_ITEM_SIZE_M);

    removeCellWidget(m_playRowIndex, 0);
    delete takeItem(m_playRowIndex, 0);
    clearSpans();

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(m_playRowIndex, 0, item);

    item = new QTableWidgetItem(TTK::Widget::elidedText(font(), name, Qt::ElideRight, headerview->sectionSize(1) - 10));
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);

    setItem(m_playRowIndex, 1, item);
    setItem(m_playRowIndex, 2, new QTableWidgetItem);
    setItem(m_playRowIndex, 3, new QTableWidgetItem);
    setItem(m_playRowIndex, 4, new QTableWidgetItem);

    item = new QTableWidgetItem((*m_songs)[m_playRowIndex].playTime());
    item->setForeground(QColor(TTK::UI::Color01));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(m_playRowIndex, 5, item);

    delete m_songsPlayWidget;
    m_songsPlayWidget = nullptr;

    m_playRowIndex = -1;
    setFixedHeight(totalHeight());
}

bool MusicSongsListPlayTableWidget::createUploadFileModule()
{
    if(m_songs->isEmpty() && TTK::playlistRowValid(m_toolIndex))
    {
        setFixedSize(LEFT_SIDE_WIDTH_MIN, 100);
        if(m_openFileWidget == nullptr && m_parent)
        { 
            m_openFileWidget = new MusicOpenFileWidget(this);
            connect(m_openFileWidget, SIGNAL(uploadFileClicked()), m_parent, SLOT(musicImportSongsByFiles()));
            connect(m_openFileWidget, SIGNAL(uploadDirClicked()), m_parent, SLOT(musicImportSongsByDir()));
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
        it->setText((*m_songs)[m_previousColorRow].playTime());
    }

    ///draw new table item state
    if((it = item(row, 0)))
    {
        it->setIcon(QIcon(":/tiny/btn_play_later_normal"));
    }

    if((it = item(row, 2)))
    {
        it->setIcon(QIcon(":/tiny/btn_mv_normal"));
    }

    if((it = item(row, 3)))
    {
        const bool contains = MusicApplication::instance()->musicLovestContains(row);
        it->setIcon(QIcon(contains ? ":/tiny/btn_loved_normal" : ":/tiny/btn_unloved_normal"));
    }

    if((it = item(row, 4)))
    {
        it->setIcon(QIcon(":/tiny/btn_delete_normal"));
    }

    if((it = item(row, 5)))
    {
        it->setText(QString());
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
    m_timerShow.start(0.5 * MT_S2MS);
    m_timerStay.stop();
    m_timerStay.start(3 * MT_S2MS);
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
            musicAddToPlayLater();
            break;
        }
        case 2:
        {
            musicSongMovieFound();
            break;
        }
        case 3:
        {
            bool empty;
            Q_EMIT isSearchResultEmpty(empty);
            if(!empty)
            {
                return;
            }

            const bool contains = !MusicApplication::instance()->musicLovestContains(row);
            QTableWidgetItem *it = item(row, 3);
            if(it)
            {
                it->setIcon(QIcon(contains ? ":/tiny/btn_loved_normal" : ":/tiny/btn_unloved_normal"));
            }
            Q_EMIT addSongToLovestListAt(contains, row);
            break;
        }
        case 4:
        {
            bool empty;
            Q_EMIT isSearchResultEmpty(empty);
            if(!empty)
            {
                return;
            }

            setDeleteItemAt();
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

void MusicSongsListPlayTableWidget::setDeleteItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(!message.exec() || !isValid())
    {
        clearSelection();
        return;
    }

    const TTKIntList deleteList(multiSelectedIndexList());
    if(deleteList.isEmpty())
    {
        return;
    }

    MusicProgressWidget progress;
    progress.show();
    progress.setTitle(tr("Delete file mode"));
    progress.setRange(0, deleteList.count() / 3 * 2);

    for(int i = 0; i < deleteList.count(); ++i)
    {
        if(i % 3 == 0)
        {
            progress.setValue(i / 3);
        }
    }

    if(deleteList.contains(m_playRowIndex) || deleteList[0] < m_playRowIndex)
    {
        adjustPlayWidgetRow();
    }

    for(int i = deleteList.count() - 1; i >= 0; --i)
    {
        const int index = deleteList[i];
        removeRow(index);
        progress.setValue(deleteList.count() * 2 - i);
    }

    setFixedHeight(totalHeight());
    Q_EMIT deleteItemAt(deleteList, m_deleteItemWithFile);
}

void MusicSongsListPlayTableWidget::setDeleteItemWithFile()
{
    m_deleteItemWithFile = true;
    setDeleteItemAt();
    m_deleteItemWithFile = false;
}

void MusicSongsListPlayTableWidget::showTimeOut()
{
    m_timerShow.stop();
    if(m_songsInfoWidget)
    {
        if(m_previousColorRow < 0 || m_previousColorRow >= m_songs->count())
        {
            return;
        }

        const MusicSong &song = (*m_songs)[m_previousColorRow];
        m_songsInfoWidget->setMusicSongInformation(song);
        m_songsInfoWidget->move(mapToGlobal(QPoint(width(), 0)).x() + 8, QCursor::pos().y());

        bool state;
        Q_EMIT isCurrentIndex(state);
        m_songsInfoWidget->setVisible(state ? (m_songsPlayWidget && !m_songsPlayWidget->itemRenameState()) : true);
    }
}

void MusicSongsListPlayTableWidget::stayTimeOut()
{
    m_timerStay.stop();
    delete m_songsInfoWidget;
    m_songsInfoWidget = nullptr;
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
        m_songsPlayWidget->setItemRename();
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

void MusicSongsListPlayTableWidget::musicMakeRingWidget()
{
    if(!isValid())
    {
        return;
    }

    MusicSongRingtoneMaker(this).exec();
}

void MusicSongsListPlayTableWidget::musicTransformWidget()
{
    if(!isValid())
    {
        return;
    }

    MusicTransformWidget(this).exec();
}

void MusicSongsListPlayTableWidget::musicSearchQuery(QAction *action)
{
    if(action->data().isNull())
    {
        return;
    }

    const QString &songName = currentSongName();
    const QStringList names(TTK::String::stringSplit(songName));
    switch(action->data().toInt() - TTK_LOW_LEVEL)
    {
        case 0 : MusicRightAreaWidget::instance()->musicSongSearchedFound(songName); break;
        SEARCH_ITEM_DEFINED(1, names);
        SEARCH_ITEM_DEFINED(2, names);
        SEARCH_ITEM_DEFINED(3, names);
        default: break;
    }
}

void MusicSongsListPlayTableWidget::musicAddToPlayLater()
{
    const int row = currentRow();
    if(rowCount() == 0 || row < 0)
    {
        return;
    }

    MusicPlayedListPopWidget::instance()->insert(m_toolIndex, (*m_songs)[row]);
}

void MusicSongsListPlayTableWidget::musicAddToPlayedList()
{
    const int row = currentRow();
    if(rowCount() == 0 || row < 0)
    {
        return;
    }

    MusicPlayedListPopWidget::instance()->append(m_toolIndex, (*m_songs)[row]);
}

void MusicSongsListPlayTableWidget::setItemRenameFinished(const QString &name)
{
    if(m_playRowIndex >= rowCount() || m_playRowIndex < 0)
    {
        return;
    }

    (*m_songs)[m_playRowIndex].setName(name);
}

void MusicSongsListPlayTableWidget::musicListSongSortBy(QAction *action)
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
        Q_EMIT musicListSongSortBy(m_toolIndex);
    }
}

void MusicSongsListPlayTableWidget::mousePressEvent(QMouseEvent *event)
{
    MusicAbstractSongsListTableWidget::mousePressEvent(event);
    closeRenameItem();

    if(event->button() == Qt::LeftButton)
    {
        m_leftButtonPressed = true;
        m_dragStartIndex = currentRow();
        m_dragStartPoint = event->pos();
    }
}

void MusicSongsListPlayTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    MusicAbstractSongsListTableWidget::mouseMoveEvent(event);
    if(m_leftButtonPressed && abs(m_dragStartPoint.y() - event->pos().y()) > 15)
    {
        m_mouseMoved = true;
        setCursor(QCursor(QPixmap(":/functions/lb_drag_cursor")));
        setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void MusicSongsListPlayTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    MusicAbstractSongsListTableWidget::mouseReleaseEvent(event);
    startToDrag();

    m_leftButtonPressed = false;
    m_mouseMoved = false;
    setCursor(QCursor(Qt::ArrowCursor));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void MusicSongsListPlayTableWidget::leaveEvent(QEvent *event)
{
    MusicAbstractSongsListTableWidget::leaveEvent(event);
    itemCellEntered(-1, -1);

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
    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"), this, SLOT(musicPlayClicked()));
    menu.addAction(tr("Play Later"), this, SLOT(musicAddToPlayLater()));
    menu.addAction(tr("Add To Playlist"), this, SLOT(musicAddToPlayedList()));
    menu.addAction(tr("Download More..."), this, SLOT(musicSongDownload()));
    menu.addSeparator();

    QMenu sortFiles(tr("Sort"), &menu);
    sortFiles.addAction(tr("Sort By FileName"))->setData(0);
    sortFiles.addAction(tr("Sort By Singer"))->setData(1);
    sortFiles.addAction(tr("Sort By FileSize"))->setData(2);
    sortFiles.addAction(tr("Sort By AddTime"))->setData(3);
    sortFiles.addAction(tr("Sort By PlayTime"))->setData(4);
    sortFiles.addAction(tr("Sort By PlayCount"))->setData(5);
    TTK::Widget::adjustMenuPosition(&sortFiles);
    connect(&sortFiles, SIGNAL(triggered(QAction*)), SLOT(musicListSongSortBy(QAction*)));

    if(m_songSort)
    {
        const QList<QAction*> actions(sortFiles.actions());
        if(-1 < m_songSort->m_type && m_songSort->m_type < actions.count())
        {
            const bool asc = m_songSort->m_order == Qt::AscendingOrder;
            actions[m_songSort->m_type]->setIcon(QIcon(asc ? ":/tiny/lb_sort_asc" : ":/tiny/lb_sort_desc"));
        }
    }
    menu.addMenu(&sortFiles);

    menu.addAction(tr("Found Movie"), this, SLOT(musicSongMovieFound()));
    menu.addSeparator();

    createMoreMenu(&menu);

    QMenu toolMenu(tr("Tools"), &menu);
    toolMenu.addAction(tr("Make Bell"), this, SLOT(musicMakeRingWidget()));
    toolMenu.addAction(tr("Make Transform"), this, SLOT(musicTransformWidget()));
    menu.addMenu(&toolMenu);
    TTK::Widget::adjustMenuPosition(&toolMenu);

    bool status = m_toolIndex != MUSIC_NETWORK_LIST;
    menu.addAction(tr("Song Info..."), this, SLOT(musicFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_local_file"), tr("Open File Dir"), this, SLOT(musicOpenFileDir()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("Ablum"), this, SLOT(musicAlbumQueryWidget()));
    menu.addSeparator();

    status = false;
    Q_EMIT isSearchResultEmpty(status);
    menu.addAction(tr("Rename"), this, SLOT(setChangSongName()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_delete"), tr("Delete"), this, SLOT(setDeleteItemAt()))->setEnabled(status);
    menu.addAction(tr("Delete With File"), this, SLOT(setDeleteItemWithFile()))->setEnabled(status);
    menu.addAction(tr("Delete All"), this, SLOT(setDeleteItemAll()))->setEnabled(status);
    menu.addSeparator();

    const QString &songName = currentSongName();
    const QStringList names(TTK::String::stringSplit(songName));
    for(int i = 1; i <= names.count(); ++i)
    {
        menu.addAction(tr("Search '%1'").arg(names[i - 1].trimmed()))->setData(i + TTK_LOW_LEVEL);
    }
    menu.addAction(tr("Search '%1'").arg(songName))->setData(TTK_LOW_LEVEL);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(musicSearchQuery(QAction*)));

    menu.exec(QCursor::pos());
}

void MusicSongsListPlayTableWidget::closeRenameItem()
{
    if(!m_renameItem)
    {
        return;
    }

    //just close the rename edittext;
    if(m_renameActived)
    {
        closePersistentEditor(m_renameItem);
    }
    //it may be a bug in closePersistentEditor,so we select
    //the two if function to deal with
    if(m_renameActived)
    {
        QHeaderView *headerview = horizontalHeader();
        (*m_songs)[m_renameItem->row()].setName(m_renameItem->text());
        m_renameItem->setText(TTK::Widget::elidedText(font(), m_renameItem->text(), Qt::ElideRight, headerview->sectionSize(1) - 10));

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
    Q_EMIT isSearchResultEmpty(empty);
    if(empty && m_dragStartIndex > -1 && m_leftButtonPressed && m_mouseMoved)
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

        Q_EMIT queryMusicIndexSwaped(start, end, index, songs);
        for(int i = qMin(start, end); i <= qMax(start, end); ++i)
        {
            if(i == index)
            {
                continue; //skip the current play item index, because the play widget just has one item
            }

            QHeaderView *headerview = horizontalHeader();
            item(i, 1)->setText(TTK::Widget::elidedText(font(), songs[i].name(), Qt::ElideRight, headerview->sectionSize(1) - 10));
            item(i, 5)->setText(songs[i].playTime());
        }

        bool state;
        Q_EMIT isCurrentIndex(state);
        if(state)
        {
            selectRow(index);
        }
    }
}
