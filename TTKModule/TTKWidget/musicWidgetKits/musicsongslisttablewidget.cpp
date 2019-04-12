#include "musicsongslisttablewidget.h"
#include "musicsongslistiteminfowidget.h"
#include "musicsongslistplaywidget.h"
#include "musictransformwidget.h"
#include "musicsongringtonemakerwidget.h"
#include "musicitemdelegate.h"
#include "musicmessagebox.h"
#include "musicprogresswidget.h"
#include "musiccoreutils.h"
#include "musicstringutils.h"
#include "musicnumberdefine.h"
#include "musicsongsharingwidget.h"
#include "musicrightareawidget.h"
#include "musicopenfilewidget.h"
#include "musicplayedlistpopwidget.h"
#include "musicapplication.h"
#include "musicleftareawidget.h"
#include "musicotherdefine.h"

#include <qmath.h>
#include <QAction>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

#define SEARCH_ITEM_DEFINED(index, names)                                                   \
    case index:                                                                             \
    {                                                                                       \
        if(names.count() >= index)                                                          \
        {                                                                                   \
            MusicRightAreaWidget::instance()->musicSongSearchedFound(names[index - 1]);     \
        }                                                                                   \
        break;                                                                              \
    }


MusicSongsListTableWidget::MusicSongsListTableWidget(int index, QWidget *parent)
    : MusicSongsListAbstractTableWidget(parent), m_openFileWidget(nullptr),
      m_musicSongsInfoWidget(nullptr), m_musicSongsPlayWidget(nullptr)
{
    m_deleteItemWithFile = false;
    m_renameActived = false;
    m_renameItem = nullptr;
    m_dragStartIndex = -1;
    m_leftButtonPressed = false;
    m_listHasSearched = false;
    m_mouseMoved = false;
    m_parentToolIndex = index;
    m_renameLineEditDelegate = nullptr;
    m_musicSort = nullptr;

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

    MusicUtils::Widget::setTransparent(this, 0);

    connect(&m_timerShow, SIGNAL(timeout()), SLOT(showTimeOut()));
    connect(&m_timerStay, SIGNAL(timeout()), SLOT(stayTimeOut()));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), MusicApplication::instance(), SLOT(musicPlayIndexClicked(int,int)));
}

MusicSongsListTableWidget::~MusicSongsListTableWidget()
{
    if(m_listHasSearched)
    {
        delete m_musicSongs;
    }
    clearAllItems();

    delete m_openFileWidget;
    delete m_musicSongsInfoWidget;
    delete m_musicSongsPlayWidget;
    delete m_renameLineEditDelegate;
}

void MusicSongsListTableWidget::updateSongsFileName(const MusicSongs &songs)
{
    if(createUploadFileWidget())
    {
        return;
    }

    const int count = rowCount();
    setRowCount(songs.count());
    QHeaderView *headerview = horizontalHeader();
    for(int i=count; i<songs.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);
                          item = new QTableWidgetItem;
        item->setText(MusicUtils::Widget::elidedText(font(), songs[i].getMusicName(), Qt::ElideRight, headerview->sectionSize(1) - 10));
        item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                          item = new QTableWidgetItem;
        setItem(i, 2, item);

                          item = new QTableWidgetItem;
        setItem(i, 3, item);

                          item = new QTableWidgetItem;
        setItem(i, 4, item);

                          item = new QTableWidgetItem(songs[i].getMusicPlayTime());
        item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 5, item);
    }
    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
}

void MusicSongsListTableWidget::clearAllItems()
{
    //Remove play widget
    setRowHeight(m_playRowIndex, ITEM_ROW_HEIGHT_M);
    removeCellWidget(m_playRowIndex, 0);

    delete m_musicSongsPlayWidget;
    m_musicSongsPlayWidget = nullptr;

    m_playRowIndex = 0;
    //Remove all the original item
    MusicSongsListAbstractTableWidget::clear();
    setColumnCount(6);
}

void MusicSongsListTableWidget::setMusicSongsSearchedFileName(MusicSongs *songs, const MIntList &fileIndexs)
{
    if(songs->count() == fileIndexs.count())
    {
        if(m_listHasSearched)
        {
            delete m_musicSongs;
        }
        m_listHasSearched = false;
        m_musicSongs = songs;
    }
    else
    {
        if(m_listHasSearched)
        {
            delete m_musicSongs;
        }
        m_listHasSearched = true;
        m_musicSongs = new MusicSongs;
        foreach(int index, fileIndexs)
        {
            m_musicSongs->append((*songs)[index]);
        }
    }

    clearAllItems();
    if(!m_musicSongs->isEmpty())
    {
        updateSongsFileName(*m_musicSongs);
    }
    else
    {
        setFixedHeight( allRowsHeight() );
    }
}

void MusicSongsListTableWidget::selectRow(int index)
{
    if(index < 0)
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
    const QString &path = !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicPath() : QString();
    QString timeLabel;

    m_musicSongsPlayWidget = new MusicSongsListPlayWidget(index, this);
    m_musicSongsPlayWidget->setParameter(name, path, timeLabel);

    if(!m_musicSongs->isEmpty())
    {
        MusicSong *song = &(*m_musicSongs)[index];
        if(song->getMusicPlayTime().isEmpty() || song->getMusicPlayTime() == MUSIC_TIME_INIT)
        {
            song->setMusicPlayTime(timeLabel);
        }
    }

    setSpan(index, 0, 1, 6);
    setCellWidget(index, 0, m_musicSongsPlayWidget);
    setRowHeight(index, ITEM_ROW_HEIGHT_XL);
    m_playRowIndex = index;

    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
}

void MusicSongsListTableWidget::updateTimeLabel(const QString &current, const QString &total) const
{
    if(m_musicSongsPlayWidget)
    {
        m_musicSongsPlayWidget->updateTimeLabel(current, total);
    }
}

void MusicSongsListTableWidget::updateCurrentArtist()
{
    if(m_musicSongsPlayWidget)
    {
        m_musicSongsPlayWidget->updateCurrentArtist();
    }
}

void MusicSongsListTableWidget::replacePlayWidgetRow()
{
    if(m_playRowIndex >= rowCount() || m_playRowIndex < 0)
    {
        m_playRowIndex = 0;
    }

    const QString &name = !m_musicSongs->isEmpty() ? m_musicSongs->at(m_playRowIndex).getMusicName() : QString();

    setRowHeight(m_playRowIndex, ITEM_ROW_HEIGHT_M);

    removeCellWidget(m_playRowIndex, 0);
    delete takeItem(m_playRowIndex, 0);
    clearSpans();

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(m_playRowIndex, 0, item);

    item = new QTableWidgetItem(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, headerview->sectionSize(1) - 10));
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    setItem(m_playRowIndex, 1, item);
    setItem(m_playRowIndex, 2, new QTableWidgetItem);
    setItem(m_playRowIndex, 3, new QTableWidgetItem);
    setItem(m_playRowIndex, 4, new QTableWidgetItem);

    item = new QTableWidgetItem( (*m_musicSongs)[m_playRowIndex].getMusicPlayTime() );
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(m_playRowIndex, 5, item);

    delete m_musicSongsPlayWidget;
    m_musicSongsPlayWidget = nullptr;

    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
}

bool MusicSongsListTableWidget::createUploadFileWidget()
{
    if(m_musicSongs->isEmpty() && m_parentToolIndex != MUSIC_LOVEST_LIST && m_parentToolIndex != MUSIC_NETWORK_LIST && m_parentToolIndex != MUSIC_RECENT_LIST)
    {
        setFixedSize(320, 100);
        if(m_openFileWidget == nullptr)
        {
            m_openFileWidget = new MusicOpenFileWidget(this);
            connect(m_openFileWidget, SIGNAL(uploadFileClicked()), SIGNAL(musicAddNewFiles()));
            connect(m_openFileWidget, SIGNAL(uploadFilesClicked()), SIGNAL(musicAddNewDir()));
            m_openFileWidget->adjustRect(width(), height());
        }
        m_openFileWidget->raise();
        m_openFileWidget->show();
        return true;
    }
    else
    {
        delete m_openFileWidget;
        m_openFileWidget = nullptr;
    }
    return false;
}

void MusicSongsListTableWidget::listCellEntered(int row, int column)
{
    ///clear previous table item state
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
        it->setText((*m_musicSongs)[m_previousColorRow].getMusicPlayTime());
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
        const bool contains = MusicApplication::instance()->musicListLovestContains(row);
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

    MusicSongsListAbstractTableWidget::listCellEntered(row, column);

    //To show music Songs Item information
    if(m_musicSongsInfoWidget == nullptr)
    {
        m_musicSongsInfoWidget = new MusicSongsListItemInfoWidget;
        m_musicSongsInfoWidget->hide();
    }
    m_timerShow.stop();
    m_timerShow.start(0.5*MT_S2MS);
    m_timerStay.stop();
    m_timerStay.start(3*MT_S2MS);
}

void MusicSongsListTableWidget::listCellClicked(int row, int column)
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
                emit isSearchFileListEmpty(empty);
                if(!empty)
                {
                    return;
                }

                const bool contains = !MusicApplication::instance()->musicListLovestContains(row);
                QTableWidgetItem *it = item(row, 3);
                if(it)
                {
                    it->setIcon(QIcon(contains ? ":/tiny/btn_loved_normal" : ":/tiny/btn_unloved_normal"));
                }
                emit musicListSongToLovestListAt(contains, row);
                break;
            }
        case 4:
            {
                bool empty;
                emit isSearchFileListEmpty(empty);
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
        default:
            break;
    }
}

void MusicSongsListTableWidget::setDeleteItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(!message.exec() || rowCount() == 0 || currentRow() < 0)
    {
        clearSelection();
        return;
    }

    MusicProgressWidget progress;
    progress.show();
    progress.setTitle(tr("Delete File Mode"));
    progress.setRange(0, getMultiIndexSet().count()/3*2);

    MIntList deleteList(getMultiIndexSet());
    if(deleteList.isEmpty())
    {
        return;
    }

    for(int i=0; i<deleteList.count(); ++i)
    {
        if(i%3 == 0)
        {
            progress.setValue(i/3);
        }
    }

    if(deleteList.contains(m_playRowIndex) || deleteList[0] < m_playRowIndex)
    {
        replacePlayWidgetRow();
    }

    for(int i=deleteList.count() - 1; i>=0; --i)
    {
        const int index = deleteList[i];
        removeRow(index);           //Delete the current row
        progress.setValue(deleteList.count()*2 - i);
    }

    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );

    emit deleteItemAt(deleteList, m_deleteItemWithFile);
}

void MusicSongsListTableWidget::setDeleteItemWithFile()
{
    m_deleteItemWithFile = true;
    setDeleteItemAt();
    m_deleteItemWithFile = false;
}

void MusicSongsListTableWidget::showTimeOut()
{
    m_timerShow.stop();
    if(m_musicSongsInfoWidget)
    {
        if(m_previousColorRow < 0 || m_previousColorRow >= m_musicSongs->count())
        {
            return;
        }

        const MusicSong &song = (*m_musicSongs)[m_previousColorRow];
        m_musicSongsInfoWidget->setMusicSongInformation( song );
        m_musicSongsInfoWidget->move(mapToGlobal(QPoint(width(), 0)).x() + 8, QCursor::pos().y());

        bool state;
        emit isCurrentIndex(state);
        m_musicSongsInfoWidget->setVisible( state ? (m_musicSongsPlayWidget && !m_musicSongsPlayWidget->getItemRenameState()) : true);
    }
}

void MusicSongsListTableWidget::stayTimeOut()
{
    m_timerStay.stop();
    delete m_musicSongsInfoWidget;
    m_musicSongsInfoWidget = nullptr;
}

void MusicSongsListTableWidget::setChangSongName()
{
    if(rowCount() == 0 || currentRow() < 0 /*|| currentItem()->column() != 1*/)
    {
        return;
    }

    //the playing widget allow renaming
    if((currentRow() == m_playRowIndex) && m_musicSongsPlayWidget)
    {
        m_musicSongsPlayWidget->setItemRename();
        return;
    }

    //others
    delete m_renameLineEditDelegate;
    m_renameLineEditDelegate = new MusicRenameLineEditDelegate(this);
    setItemDelegateForRow(currentRow(), m_renameLineEditDelegate);
    m_renameActived = true;
    m_renameItem = item(currentRow(), 1);
    m_renameItem->setText((*m_musicSongs)[m_renameItem->row()].getMusicName());
    openPersistentEditor(m_renameItem);
    editItem(m_renameItem);
}

void MusicSongsListTableWidget::musicMakeRingWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicSongRingtoneMaker(this).exec();
}

void MusicSongsListTableWidget::musicTransformWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicTransformWidget(this).exec();
}

void MusicSongsListTableWidget::musicSearchQuery(QAction *action)
{
    if(action->data().isNull())
    {
        return;
    }

    const QString &songName = getCurrentSongName();
    const QStringList names(MusicUtils::String::splitString(songName));
    switch(action->data().toInt() - DEFAULT_LEVEL_NORMAL)
    {
        case 0 : MusicRightAreaWidget::instance()->musicSongSearchedFound(songName); break;
        SEARCH_ITEM_DEFINED(1, names);
        SEARCH_ITEM_DEFINED(2, names);
        SEARCH_ITEM_DEFINED(3, names);
        default: break;
    }
}

void MusicSongsListTableWidget::musicAddToPlayLater()
{
    const int row = currentRow();
    if(rowCount() == 0 || row < 0 )
    {
        return;
    }

    MusicPlayedListPopWidget::instance()->insert(m_parentToolIndex, (*m_musicSongs)[row]);
}

void MusicSongsListTableWidget::musicAddToPlayedList()
{
    const int row = currentRow();
    if(rowCount() == 0 || row < 0 )
    {
        return;
    }

    MusicPlayedListPopWidget::instance()->append(m_parentToolIndex, (*m_musicSongs)[row]);
}

void MusicSongsListTableWidget::setItemRenameFinished(const QString &name)
{
    (*m_musicSongs)[m_playRowIndex].setMusicName(name);
}

void MusicSongsListTableWidget::musicListSongSortBy(QAction *action)
{
    const int newIndex = action->data().toInt();
    if(newIndex < 0 || newIndex > 5)
    {
        return;
    }

    if(m_musicSort)
    {
        const int bIndex = m_musicSort->m_index;
        m_musicSort->m_index = newIndex;

        if(bIndex == newIndex)
        {
            const bool asc = m_musicSort->m_sortType == Qt::AscendingOrder;
            m_musicSort->m_sortType = asc ? Qt::DescendingOrder : Qt::AscendingOrder;
        }
        else
        {
            m_musicSort->m_sortType = Qt::AscendingOrder;
        }
        emit musicListSongSortBy(m_parentToolIndex);
    }
}

void MusicSongsListTableWidget::mousePressEvent(QMouseEvent *event)
{
    MusicSongsListAbstractTableWidget::mousePressEvent(event);
    closeRenameItem();

    if(event->button() == Qt::LeftButton)
    {
        m_leftButtonPressed = true;
        m_dragStartIndex = currentRow();
        m_dragStartPoint = event->pos();
    }
}

void MusicSongsListTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    MusicSongsListAbstractTableWidget::mouseMoveEvent(event);
    if(m_leftButtonPressed && abs(m_dragStartPoint.y() - event->pos().y()) > 15)
    {
        m_mouseMoved = true;
        setCursor(QCursor(QPixmap(":/functions/lb_drag_cursor")));
        setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void MusicSongsListTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    MusicSongsListAbstractTableWidget::mouseReleaseEvent(event);
    startToDrag();

    m_leftButtonPressed = false;
    m_mouseMoved = false;
    setCursor(QCursor(Qt::ArrowCursor));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void MusicSongsListTableWidget::leaveEvent(QEvent *event)
{
    MusicSongsListAbstractTableWidget::leaveEvent(event);
    listCellEntered(-1, -1);

    delete m_musicSongsInfoWidget;
    m_musicSongsInfoWidget = nullptr;
    closeRenameItem();
}

void MusicSongsListTableWidget::wheelEvent(QWheelEvent *event)
{
    MusicSongsListAbstractTableWidget::wheelEvent(event);
    closeRenameItem();
    emit showFloatWidget();
}

void MusicSongsListTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicSongsListAbstractTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    QMenu musicPlaybackMode(tr("playbackMode"), &rightClickMenu);

    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay"), this, SLOT(musicPlayClicked()));
    rightClickMenu.addAction(tr("playLater"), this, SLOT(musicAddToPlayLater()));
    rightClickMenu.addAction(tr("addToPlaylist"), this, SLOT(musicAddToPlayedList()));
    rightClickMenu.addAction(tr("downloadMore..."), this, SLOT(musicSongDownload()));
    rightClickMenu.addSeparator();

    rightClickMenu.addMenu(&musicPlaybackMode);
    QList<QAction*> actions;
    actions << musicPlaybackMode.addAction(tr("OrderPlay"), MusicApplication::instance(), SLOT(musicPlayOrder()));
    actions << musicPlaybackMode.addAction(tr("RandomPlay"), MusicApplication::instance(), SLOT(musicPlayRandom()));
    actions << musicPlaybackMode.addAction(tr("ListCycle"), MusicApplication::instance(), SLOT(musicPlaylistLoop()));
    actions << musicPlaybackMode.addAction(tr("SingleCycle"), MusicApplication::instance(), SLOT(musicPlayOneLoop()));
    actions << musicPlaybackMode.addAction(tr("PlayOnce"), MusicApplication::instance(), SLOT(musicPlayItemOnce()));

    const MusicObject::PlayMode mode = MusicApplication::instance()->getPlayMode();
    int index = -1;
    switch(mode)
    {
        case MusicObject::PM_PlayOrder: index = 0; break;
        case MusicObject::PM_PlayRandom: index = 1; break;
        case MusicObject::PM_PlaylistLoop: index = 2; break;
        case MusicObject::PM_PlayOneLoop: index = 3; break;
        case MusicObject::PM_PlayOnce: index = 4; break;
        default: break;
    }
    if(index > -1 && index < actions.count())
    {
        actions[index]->setIcon(QIcon(":/contextMenu/btn_selected"));
    }

    QMenu musicAddNewFiles(tr("addNewFiles"), &rightClickMenu);
    rightClickMenu.addMenu(&musicAddNewFiles);
    musicAddNewFiles.setEnabled(m_parentToolIndex != MUSIC_LOVEST_LIST && m_parentToolIndex != MUSIC_NETWORK_LIST);
    musicAddNewFiles.addAction(tr("openOnlyFiles"), this, SIGNAL(musicAddNewFiles()));
    musicAddNewFiles.addAction(tr("openOnlyDir"), this, SIGNAL(musicAddNewDir()));

    QMenu musicSortFiles(tr("sort"), &rightClickMenu);
    musicSortFiles.addAction(tr("sortByFileName"))->setData(0);
    musicSortFiles.addAction(tr("sortBySinger"))->setData(1);
    musicSortFiles.addAction(tr("sortByFileSize"))->setData(2);
    musicSortFiles.addAction(tr("sortByAddTime"))->setData(3);
    musicSortFiles.addAction(tr("sortByPlayTime"))->setData(4);
    musicSortFiles.addAction(tr("sortByPlayCount"))->setData(5);
    connect(&musicSortFiles, SIGNAL(triggered(QAction*)), SLOT(musicListSongSortBy(QAction*)));
    if(m_musicSort)
    {
        const QList<QAction*> actions(musicSortFiles.actions());
        if(-1 < m_musicSort->m_index && m_musicSort->m_index < actions.count())
        {
            const bool asc = m_musicSort->m_sortType == Qt::AscendingOrder;
            actions[m_musicSort->m_index]->setIcon(QIcon(asc ? ":/tiny/lb_sort_asc" : ":/tiny/lb_sort_desc"));
        }
    }
    rightClickMenu.addMenu(&musicSortFiles);

    rightClickMenu.addAction(tr("foundMV"), this, SLOT(musicSongMovieFound()));
    rightClickMenu.addSeparator();

    createMoreMenu(&rightClickMenu);

    QMenu musicToolMenu(tr("musicTool"), &rightClickMenu);
    musicToolMenu.addAction(tr("bell"), this, SLOT(musicMakeRingWidget()));
    musicToolMenu.addAction(tr("transform"), this, SLOT(musicTransformWidget()));
    rightClickMenu.addMenu(&musicToolMenu);

    rightClickMenu.addAction(tr("musicInfo..."), this, SLOT(musicFileInformation()));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_localFile"), tr("openFileDir"), this, SLOT(musicOpenFileDir()));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("ablum"), this, SLOT(musicAlbumFoundWidget()));
    rightClickMenu.addSeparator();

    bool empty;
    emit isSearchFileListEmpty(empty);
    rightClickMenu.addAction(tr("changSongName"), this, SLOT(setChangSongName()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_delete"), tr("delete"), this, SLOT(setDeleteItemAt()))->setEnabled(empty);
    rightClickMenu.addAction(tr("deleteWithFile"), this, SLOT(setDeleteItemWithFile()))->setEnabled(empty);
    rightClickMenu.addAction(tr("deleteAll"), this, SLOT(setDeleteItemAll()))->setEnabled(empty);
    rightClickMenu.addSeparator();

    createContextMenu(rightClickMenu);

    rightClickMenu.exec(QCursor::pos());
}

void MusicSongsListTableWidget::closeRenameItem()
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
        (*m_musicSongs)[m_renameItem->row()].setMusicName(m_renameItem->text());
        m_renameItem->setText(MusicUtils::Widget::elidedText(font(), m_renameItem->text(), Qt::ElideRight, headerview->sectionSize(1) - 10));

        m_renameActived = false;
        setItemDelegateForRow(m_renameItem->row(), nullptr);
        m_renameItem = nullptr;

        delete m_renameLineEditDelegate;
        m_renameLineEditDelegate = nullptr;
    }
}

void MusicSongsListTableWidget::startToDrag()
{
    bool empty;
    emit isSearchFileListEmpty(empty);
    if(empty && m_dragStartIndex > -1 && m_leftButtonPressed && m_mouseMoved)
    {
        MusicSongs songs;
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

        emit getMusicIndexSwaped(start, end, index, songs);
        for(int i=qMin(start, end); i<=qMax(start, end); ++i)
        {
            if(i == index)
            {
                continue; //skip the current play item index, because the play widget just has one item
            }

            QHeaderView *headerview = horizontalHeader();
            item(i, 1)->setText(MusicUtils::Widget::elidedText(font(), songs[i].getMusicName(), Qt::ElideRight, headerview->sectionSize(1) - 10));
            item(i, 5)->setText(songs[i].getMusicPlayTime());
        }

        bool state;
        emit isCurrentIndex(state);
        if(state)
        {
            selectRow(index);
        }
    }
}

void MusicSongsListTableWidget::createContextMenu(QMenu &menu)
{
    const QString &songName = getCurrentSongName();
    const QStringList names(MusicUtils::String::splitString(songName));
    for(int i=1; i<=names.count(); ++i)
    {
        menu.addAction(tr("search '%1'").arg(names[i - 1].trimmed()))->setData(i + DEFAULT_LEVEL_NORMAL);
    }
    menu.addAction(tr("search '%1'").arg(songName))->setData(DEFAULT_LEVEL_NORMAL);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(musicSearchQuery(QAction*)));
}
