#include "musicsongslisttablewidget.h"
#include "musicsongslistiteminfowidget.h"
#include "musicsongslistplaywidget.h"
#include "musictransformwidget.h"
#include "musicsongringtonemakerwidget.h"
#include "musicmessagebox.h"
#include "musicprogresswidget.h"
#include "musiccoreutils.h"
#include "musicstringutils.h"
#include "musicnumberdefine.h"
#include "musicsongsharingwidget.h"
#include "musicrightareawidget.h"
#include "musicopenfilewidget.h"
#include "musicplayedlistwidget.h"
#include "musicapplication.h"

#include <QUrl>
#include <QAction>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

#define ROW_HIGHT   30

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
    m_transparent = 0;
    m_parentToolIndex = index;
    m_musicSort = nullptr;

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setColumnCount(6);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 20);
    headerview->resizeSection(1, 187);
    headerview->resizeSection(2, 20);
    headerview->resizeSection(3, 20);
    headerview->resizeSection(4, 20);
    headerview->resizeSection(5, 45);

    MusicUtils::Widget::setTransparent(this, 0);
    setStyleSheet(MusicUIObject::MTableWidgetStyle01 + \
                  MusicUIObject::MLineEditStyle01 + \
                  MusicUIObject::MTableWidgetStyle04);

    connect(&m_timerShow, SIGNAL(timeout()), SLOT(showTimeOut()));
    connect(&m_timerStay, SIGNAL(timeout()), SLOT(stayTimeOut()));
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
}

QString MusicSongsListTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsListTableWidget::updateSongsFileName(const MusicSongs &songs)
{
    if(createUploadFileWidget())
    {
        return;
    }
    ////////////////////////////////////////////////////////////////

    int count = rowCount();
    setRowCount(songs.count());    //reset row count
    for(int i=count; i<songs.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);
                          item = new QTableWidgetItem;
        item->setText(MusicUtils::Widget::elidedText(font(), songs[i].getMusicName(), Qt::ElideRight, 180));
        item->setTextColor(QColor(50, 50, 50));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);
                          item = new QTableWidgetItem;
        setItem(i, 2, item);
                          item = new QTableWidgetItem;
        setItem(i, 3, item);
                          item = new QTableWidgetItem;
        setItem(i, 4, item);
                          item = new QTableWidgetItem(songs[i].getMusicTime());
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 5, item);
    }
    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
}

void MusicSongsListTableWidget::clearAllItems()
{
    //Remove play widget
    setRowHeight(m_playRowIndex, ROW_HIGHT);
    removeCellWidget(m_playRowIndex, 0);

    delete m_musicSongsPlayWidget;
    m_musicSongsPlayWidget = nullptr;

    m_playRowIndex = 0;
    //Remove all the original item
    MusicSongsListAbstractTableWidget::clear();
    setColumnCount(6);
}

void MusicSongsListTableWidget::setMusicSongsSearchedFileName(MusicSongs *songs, const MusicObject::MIntList &fileIndexs)
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

int MusicSongsListTableWidget::allRowsHeight() const
{
    int height = 0;
    for(int i=0; i<rowCount(); ++i)
    {
        height += rowHeight(i);
    }
    return height;
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

    QString name = !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicName() : QString();
    QString path = !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicPath() : QString();

    m_musicSongsPlayWidget = new MusicSongsListPlayWidget(index, this);
    m_musicSongsPlayWidget->setParameter(name, path);

    setSpan(index, 0, 1, 6);
    setCellWidget(index, 0, m_musicSongsPlayWidget);
    setRowHeight(index, 2*ROW_HIGHT);
    m_playRowIndex = index;

    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
}

void MusicSongsListTableWidget::setTimerLabel(const QString &t) const
{
    if(m_musicSongsPlayWidget)
    {
        m_musicSongsPlayWidget->insertTimerLabel(t);
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
    QString name = !m_musicSongs->isEmpty() ? m_musicSongs->at(m_playRowIndex).getMusicName() : QString();

    setRowHeight(m_playRowIndex, ROW_HIGHT);

    removeCellWidget(m_playRowIndex, 0);
    delete takeItem(m_playRowIndex, 0);
    clearSpans();

    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(m_playRowIndex, 0, item);
    item = new QTableWidgetItem(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 180));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(m_playRowIndex, 1, item);
    setItem(m_playRowIndex, 2, new QTableWidgetItem);
    setItem(m_playRowIndex, 3, new QTableWidgetItem);
    setItem(m_playRowIndex, 4, new QTableWidgetItem);
    item = new QTableWidgetItem( (*m_musicSongs)[m_playRowIndex].getMusicTime() );
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(m_playRowIndex, 5, item);

    delete m_musicSongsPlayWidget;
    m_musicSongsPlayWidget = nullptr;

    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
}

bool MusicSongsListTableWidget::createUploadFileWidget()
{
    if(m_musicSongs->isEmpty() && m_parentToolIndex != MUSIC_LOVEST_LIST && m_parentToolIndex != MUSIC_NETWORK_LIST
                               && m_parentToolIndex != MUSIC_RECENT_LIST)
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
    if(it != nullptr)
    {
        it->setIcon(QIcon());
    }
    it = item(m_previousColorRow, 2);
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
    }
    it = item(m_previousColorRow, 5);
    if(it != nullptr)
    {
        it->setIcon(QIcon());
        it->setText((*m_musicSongs)[m_previousColorRow].getMusicTime());
    }

    ///draw new table item state
    if((it = item(row, 0)) != nullptr)
    {
        it->setIcon(QIcon(":/tiny/btn_play_later_normal"));
    }
    if((it = item(row, 2)) != nullptr)
    {
        it->setIcon(QIcon(":/tiny/btn_mv_normal"));
    }
    if((it = item(row, 3)) != nullptr)
    {
        bool contains = MusicApplication::instance()->musicListLovestContains(row);
        it->setIcon(QIcon(contains ? ":/tiny/btn_loved_normal" : ":/tiny/btn_unloved_normal"));
    }
    if((it = item(row, 4)) != nullptr)
    {
        it->setIcon(QIcon(":/tiny/btn_delete_normal"));
    }
    if((it = item(row, 5)) != nullptr)
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
    ///current play table item should not clear something
    bool isCurrentIndex;
    emit isCurrentIndexs(isCurrentIndex);
    if(isCurrentIndex)
    {
        if((it = item(m_playRowIndex, 0)) != nullptr)
        {
            it->setIcon(QIcon());
        }
        if((it = item(m_playRowIndex, 2)) != nullptr)
        {
            it->setIcon(QIcon());
        }
        if((it = item(m_playRowIndex, 3)) != nullptr)
        {
            it->setIcon(QIcon());
        }
        if((it = item(m_playRowIndex, 4)) != nullptr)
        {
            it->setIcon(QIcon());
        }
        if((it = item(m_playRowIndex, 5)) != nullptr)
        {
            it->setText(QString());
            it->setIcon(QIcon());
        }
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

                bool contains = !MusicApplication::instance()->musicListLovestContains(row);
                QTableWidgetItem *it = item(row, 3);
                if(it != nullptr)
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
    if(message.exec() || rowCount() == 0 || currentRow() < 0)
    {
        clearSelection();
        return;
    }

    MusicProgressWidget progress;
    progress.show();
    progress.setTitle(tr("Delete File Mode"));
    progress.setRange(0, selectedItems().count()/3*2);

    MusicObject::MIntSet deletedRow; //if selected multi rows
    for(int i=0; i<selectedIndexes().count(); ++i)
    {
        deletedRow.insert(selectedIndexes()[i].row());
        if(i%3 == 0)
        {
            progress.setValue(i/3);
        }
    }

    MusicObject::MIntList deleteList = deletedRow.toList();
    if(deleteList.count() == 0)
    {
        return;
    }

    qSort(deleteList);
    if(deleteList.contains(m_playRowIndex) || deleteList[0] < m_playRowIndex)
    {
        replacePlayWidgetRow();
    }

    for(int i=deleteList.count() - 1; i>=0; --i)
    {
        int index = deleteList[i];
        removeRow(index);           //Delete the current row
        MusicPlayedListWidget::instance()->remove(m_parentToolIndex, (*m_musicSongs)[index]);
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
        MusicSong song = (*m_musicSongs)[m_previousColorRow];
        m_musicSongsInfoWidget->setMusicSongInformation( song );
        m_musicSongsInfoWidget->setGeometry(mapToGlobal(QPoint(width(), 0)).x() + 8,
                                            QCursor::pos().y(), 264, 108);
        bool isCurrentIndex;
        emit isCurrentIndexs(isCurrentIndex);
        m_musicSongsInfoWidget->setVisible( isCurrentIndex ? (m_musicSongsPlayWidget &&
                                            !m_musicSongsPlayWidget->getItemRenameState()) : true);
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
    if((currentRow() == m_playRowIndex) && m_musicSongsPlayWidget)
    //the playing widget allow renaming
    {
        m_musicSongsPlayWidget->setItemRename();
        return;
    }
    //others
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

void MusicSongsListTableWidget::musicSongMovieFoundPy()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicVideoButtonSearched( getSongName(m_playRowIndex) );
}

void MusicSongsListTableWidget::musicSimilarFoundWidgetPy()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicSimilarFound( getSongName(m_playRowIndex) );
}

void MusicSongsListTableWidget::musicSongSharedWidgetPy()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicSongSharingWidget shareWidget(this);
    shareWidget.setSongName( getSongName(m_playRowIndex) );
    shareWidget.exec();
}

void MusicSongsListTableWidget::musicSearchQuery(QAction *action)
{
    if(action->data().isNull())
    {
        return;
    }

    QString songName = getCurrentSongName();
    QStringList names(MusicUtils::String::splitString(songName));
    switch(action->data().toInt() - DEFAULT_INDEX_LEVEL1)
    {
        case 0:
            if(names.count() >= 1)
            {
                MusicRightAreaWidget::instance()->songResearchButtonSearched(names[0]);
            }
            break;
        case 1:
            if(names.count() >= 2)
            {
                MusicRightAreaWidget::instance()->songResearchButtonSearched(names[1]);
            }
            break;
        case 2:
            if(names.count() >= 3)
            {
                MusicRightAreaWidget::instance()->songResearchButtonSearched(names[2]);
            }
            break;
        default: break;
    }
}

void MusicSongsListTableWidget::musicAddToPlayLater()
{
    int row = currentRow();
    if(rowCount() == 0 || row < 0 )
    {
        return;
    }

    MusicPlayedListWidget::instance()->insert(m_parentToolIndex, (*m_musicSongs)[row]);
}

void MusicSongsListTableWidget::musicAddToPlayedList()
{
    int row = currentRow();
    if(rowCount() == 0 || row < 0 )
    {
        return;
    }

    MusicPlayedListWidget::instance()->append(m_parentToolIndex, (*m_musicSongs)[row]);
}

void MusicSongsListTableWidget::setItemRenameFinished(const QString &name)
{
    (*m_musicSongs)[m_playRowIndex].setMusicName(name);
}

void MusicSongsListTableWidget::musicListSongSortBy(QAction *action)
{
    if(m_musicSort)
    {
        int bIndex = m_musicSort->m_index;
        int newIndex = action->data().toInt();
        m_musicSort->m_index = newIndex;
        if(bIndex == newIndex)
        {
            bool asc = m_musicSort->m_sortType == Qt::AscendingOrder;
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

    if( event->button() == Qt::LeftButton )//Press the left key
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

void MusicSongsListTableWidget::paintEvent(QPaintEvent *event)
{
    QWidget *w = viewport();
    QPainter painter(w);
    painter.fillRect(w->rect(), QColor(255, 255, 255, m_transparent));
    painter.end();

    MusicSongsListAbstractTableWidget::paintEvent(event);
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
    rightClickMenu.addAction(tr("addToPlayList"), this, SLOT(musicAddToPlayedList()));
    rightClickMenu.addAction(tr("downloadMore..."), this, SLOT(musicSongDownload()));
    rightClickMenu.addSeparator();

    rightClickMenu.addMenu(&musicPlaybackMode);
    QAction *order = musicPlaybackMode.addAction(tr("OrderPlay"), MusicApplication::instance(), SLOT(musicPlayOrder()));
    QAction *random = musicPlaybackMode.addAction(tr("RandomPlay"), MusicApplication::instance(), SLOT(musicPlayRandom()));
    QAction *lCycle = musicPlaybackMode.addAction(tr("ListCycle"), MusicApplication::instance(), SLOT(musicPlayListLoop()));
    QAction *sCycle = musicPlaybackMode.addAction(tr("SingleCycle"), MusicApplication::instance(), SLOT(musicPlayOneLoop()));
    QAction *once = musicPlaybackMode.addAction(tr("PlayOnce"), MusicApplication::instance(), SLOT(musicPlayItemOnce()));
    MusicObject::SongPlayMode mode = MStatic_cast(MusicObject::SongPlayMode, MusicApplication::instance()->getPlayMode());
    (mode == MusicObject::MC_PlayOrder) ? order->setIcon(QIcon(":/contextMenu/btn_selected")) : order->setIcon(QIcon());
    (mode == MusicObject::MC_PlayRandom) ? random->setIcon(QIcon(":/contextMenu/btn_selected")) : random->setIcon(QIcon());
    (mode == MusicObject::MC_PlayListLoop) ? lCycle->setIcon(QIcon(":/contextMenu/btn_selected")) : lCycle->setIcon(QIcon());
    (mode == MusicObject::MC_PlayOneLoop) ? sCycle->setIcon(QIcon(":/contextMenu/btn_selected")) : sCycle->setIcon(QIcon());
    (mode == MusicObject::MC_PlayOnce) ? once->setIcon(QIcon(":/contextMenu/btn_selected")) : once->setIcon(QIcon());

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
        QList<QAction*> actions(musicSortFiles.actions());
        if(-1 < m_musicSort->m_index && m_musicSort->m_index < actions.count())
        {
            bool asc = m_musicSort->m_sortType == Qt::AscendingOrder;
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
        (*m_musicSongs)[m_renameItem->row()].setMusicName(m_renameItem->text());
        m_renameItem->setText(MusicUtils::Widget::elidedText(font(), m_renameItem->text(), Qt::ElideRight, 180));
        m_renameActived = false;
        m_renameItem = nullptr;
    }
}

void MusicSongsListTableWidget::startToDrag()
{
    bool empty;
    emit isSearchFileListEmpty(empty);
    if(empty && m_dragStartIndex > -1 && m_leftButtonPressed && m_mouseMoved)
    {
        MusicSongs songs;
        int start = m_dragStartIndex;
        int end = currentRow();
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

            item(i, 1)->setText(MusicUtils::Widget::elidedText(font(), songs[i].getMusicName(), Qt::ElideRight, 180));
            item(i, 5)->setText(songs[i].getMusicTime());
        }

        bool isCurrent;
        emit isCurrentIndexs(isCurrent);
        if(isCurrent)
        {
            selectRow(index);
        }
    }
}

void MusicSongsListTableWidget::createContextMenu(QMenu &menu)
{
    QString songName = getCurrentSongName();
    QStringList names(MusicUtils::String::splitString(songName));
    for(int i=0; i<names.count(); ++i)
    {
        menu.addAction(tr("search '%1'").arg(names[i].trimmed()))->setData(i + DEFAULT_INDEX_LEVEL1);
    }
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(musicSearchQuery(QAction*)));
}
