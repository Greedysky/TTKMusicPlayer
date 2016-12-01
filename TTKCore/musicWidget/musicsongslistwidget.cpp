#include "musicsongslistwidget.h"
#include "musicsongslistiteminfowidget.h"
#include "musicsongslistplaywidget.h"
#include "musictransformwidget.h"
#include "musicfileinformationwidget.h"
#include "musicsongringtonemakerwidget.h"
#include "musicsongslistfunctionwidget.h"
#include "musicmessagebox.h"
#include "musicprogresswidget.h"
#include "musiccoreutils.h"
#include "musicstringutils.h"
#include "musicnumberdefine.h"
#include "musicsongsharingwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicrightareawidget.h"
#include "musicdownloadwidget.h"
#include "musicopenfilewidget.h"
#include "musicplayedlistwidget.h"
#include "musicapplication.h"

#include <QUrl>
#include <QAction>
#include <QTimer>
#include <QPainter>

#define ROW_HIGHT   30

MusicSongsListWidget::MusicSongsListWidget(int index, QWidget *parent)
    : MusicSlowMovingTableWidget(parent), m_parentToolIndex(index), m_openFileWidget(nullptr),
      m_musicSongsInfoWidget(nullptr), m_musicSongsPlayWidget(nullptr), m_floatWidget(nullptr)
{
    m_deleteItemWithFile = false;
    m_renameActived = false;
    m_renameItem = nullptr;
    m_playRowIndex = 0;
    m_dragStartIndex = -1;
    m_leftButtonPressed = false;
    m_mouseMoved = false;
    m_transparent = 0;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

MusicSongsListWidget::~MusicSongsListWidget()
{
    clearAllItems();
    delete m_openFileWidget;
    delete m_musicSongsInfoWidget;
    delete m_musicSongsPlayWidget;
    delete m_floatWidget;
}

QString MusicSongsListWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsListWidget::setSongsFileName(MusicSongs *songs)
{
    m_musicSongs = songs;
    updateSongsFileName(*songs);
}

void MusicSongsListWidget::updateSongsFileName(const MusicSongs &songs)
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

void MusicSongsListWidget::clearAllItems()
{
    //Remove play widget
    setRowHeight(m_playRowIndex, ROW_HIGHT);
    removeCellWidget(m_playRowIndex, 0);

    delete m_musicSongsPlayWidget;
    m_musicSongsPlayWidget = nullptr;

    m_playRowIndex = 0;
    //Remove all the original item
    MusicSlowMovingTableWidget::clear();
    setColumnCount(6);
}

void MusicSongsListWidget::setParentToolIndex(int index)
{
    m_parentToolIndex = index;
}

int MusicSongsListWidget::allRowsHeight() const
{
    int height = 0;
    for(int i=0; i<rowCount(); ++i)
    {
        height += rowHeight(i);
    }
    return height;
}

void MusicSongsListWidget::selectRow(int index)
{
    if(index < 0)
    {
        return;
    }
    QTableWidget::selectRow(index);

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

void MusicSongsListWidget::setTimerLabel(const QString &t) const
{
    if(m_musicSongsPlayWidget)
    {
        m_musicSongsPlayWidget->insertTimerLabel(t);
    }
}

void MusicSongsListWidget::updateCurrentArtist()
{
    if(m_musicSongsPlayWidget)
    {
        m_musicSongsPlayWidget->updateCurrentArtist();
    }
}

void MusicSongsListWidget::replacePlayWidgetRow()
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

bool MusicSongsListWidget::createUploadFileWidget()
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

void MusicSongsListWidget::listCellEntered(int row, int column)
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
    MusicSlowMovingTableWidget::listCellEntered(row, column);

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

void MusicSongsListWidget::listCellClicked(int row, int column)
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

void MusicSongsListWidget::musicPlayClicked()
{
    if(rowCount() == 0 || currentRow() < 0 )
    {
        return;
    }
    emit cellDoubleClicked(currentRow(), 0);
}

void MusicSongsListWidget::setDeleteItemAt()
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
    for(int i=0; i<selectedItems().count(); ++i)
    {
        deletedRow.insert(selectedItems()[i]->row());
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

void MusicSongsListWidget::setDeleteItemAll()
{
    selectAll();
    setDeleteItemAt();
}

void MusicSongsListWidget::setDeleteItemWithFile()
{
    m_deleteItemWithFile = true;
    setDeleteItemAt();
    m_deleteItemWithFile = false;
}

void MusicSongsListWidget::showTimeOut()
{
    m_timerShow.stop();
    if(m_musicSongsInfoWidget)
    {
        MusicSong song = (*m_musicSongs)[m_previousColorRow];
        song.setMusicSize( QFileInfo(song.getMusicPath()).size() );
        m_musicSongsInfoWidget->setMusicSongInformation( song );
        m_musicSongsInfoWidget->setGeometry(mapToGlobal(QPoint(width(), 0)).x() + 8,
                                            QCursor::pos().y(), 264, 108);
        bool isCurrentIndex;
        emit isCurrentIndexs(isCurrentIndex);
        m_musicSongsInfoWidget->setVisible( isCurrentIndex ? (m_musicSongsPlayWidget &&
                                            !m_musicSongsPlayWidget->getItemRenameState()) : true);
    }
}

void MusicSongsListWidget::stayTimeOut()
{
    m_timerStay.stop();
    delete m_musicSongsInfoWidget;
    m_musicSongsInfoWidget = nullptr;
}

void MusicSongsListWidget::setChangSongName()
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
    m_renameItem = currentItem();
    openPersistentEditor(m_renameItem);
    editItem(m_renameItem);
}

void MusicSongsListWidget::musicOpenFileDir()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    QString path = !m_musicSongs->isEmpty() ? m_musicSongs->at(currentRow()).getMusicPath() : QString();
    if(!MusicUtils::Core::openUrl(QFileInfo(path).absoluteFilePath(), true))
    {
        MusicMessageBox message;
        message.setText(tr("The origin one does not exist!"));
        message.exec();
    }
}

void MusicSongsListWidget::musicMakeRingWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicSongRingtoneMaker(this).exec();
}

void MusicSongsListWidget::musicTransformWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicTransformWidget(this).exec();
}

void MusicSongsListWidget::musicFileInformation()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicFileInformationWidget file(this);
    file.setFileInformation( getCurrentSongPath() );
    file.exec();
}

void MusicSongsListWidget::musicSongMovieFound()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicVideoButtonSearched( getCurrentSongName() );
}

void MusicSongsListWidget::musicSongMovieFoundPy()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicVideoButtonSearched( getSongName(m_playRowIndex) );
}

void MusicSongsListWidget::musicAlbumFoundWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicAlbumFound( getCurrentSongName() );
}

void MusicSongsListWidget::musicSimilarFoundWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicSimilarFound( getCurrentSongName() );
}

void MusicSongsListWidget::musicSimilarFoundWidgetPy()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicRightAreaWidget::instance()->musicSimilarFound( getSongName(m_playRowIndex) );
}

void MusicSongsListWidget::musicSongSharedWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicSongSharingWidget shareWidget(this);
    shareWidget.setSongName( getCurrentSongName() );
    shareWidget.exec();
}

void MusicSongsListWidget::musicSongSharedWidgetPy()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicSongSharingWidget shareWidget(this);
    shareWidget.setSongName( getSongName(m_playRowIndex) );
    shareWidget.exec();
}

void MusicSongsListWidget::musicSongTransferWidget()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicConnectTransferWidget transferWidget(this);
    transferWidget.redirectToCurrentSong(m_parentToolIndex, currentRow());
    transferWidget.exec();
}

void MusicSongsListWidget::musicSongDownload()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(getCurrentSongName(), MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();
}

void MusicSongsListWidget::musicSearchQuery(QAction *action)
{
    if(action->data().isNull())
    {
        return;
    }

    QString songName = getCurrentSongName();
    QStringList names(MusicUtils::Core::splitString(songName));
    switch(action->data().toInt())
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

void MusicSongsListWidget::musicAddToPlayLater()
{
    int row = currentRow();
    if(rowCount() == 0 || row < 0 )
    {
        return;
    }

    MusicPlayedListWidget::instance()->insert(m_parentToolIndex, (*m_musicSongs)[row]);
}

void MusicSongsListWidget::musicAddToPlayedList()
{
    int row = currentRow();
    if(rowCount() == 0 || row < 0 )
    {
        return;
    }

    MusicPlayedListWidget::instance()->append(m_parentToolIndex, (*m_musicSongs)[row]);
}

void MusicSongsListWidget::setItemRenameFinished(const QString &name)
{
    (*m_musicSongs)[m_playRowIndex].setMusicName(name);
}

void MusicSongsListWidget::deleteFloatWidget()
{
    delete m_floatWidget;
    m_floatWidget = nullptr;
}

void MusicSongsListWidget::mousePressEvent(QMouseEvent *event)
{
    MusicSlowMovingTableWidget::mousePressEvent(event);
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

    if( event->button() == Qt::LeftButton )//Press the left key
    {
        m_leftButtonPressed = true;
        m_dragStartIndex = currentRow();
        m_dragStartPoint = event->pos();
    }
}

void MusicSongsListWidget::mouseMoveEvent(QMouseEvent *event)
{
    MusicSlowMovingTableWidget::mouseMoveEvent(event);
    if(m_leftButtonPressed && abs(m_dragStartPoint.y() - event->pos().y()) > 15)
    {
        m_mouseMoved = true;
        setCursor(QCursor(QPixmap(":/functions/lb_drag_cursor")));
        setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void MusicSongsListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    MusicSlowMovingTableWidget::mouseReleaseEvent(event);
    startToDrag();

    m_leftButtonPressed = false;
    m_mouseMoved = false;
    setCursor(QCursor(Qt::ArrowCursor));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void MusicSongsListWidget::leaveEvent(QEvent *event)
{
    MusicSlowMovingTableWidget::leaveEvent(event);
    listCellEntered(-1, -1);
    delete m_musicSongsInfoWidget;
    m_musicSongsInfoWidget = nullptr;

    if(m_renameActived && m_renameItem)
    {
        closePersistentEditor(m_renameItem);
        m_renameActived = false;
        m_renameItem = nullptr;
    }
}

void MusicSongsListWidget::paintEvent(QPaintEvent *event)
{
    QWidget *w = viewport();
    QPainter painter(w);
    painter.fillRect(w->rect(), QColor(255, 255, 255, m_transparent));
    painter.end();

    MusicSlowMovingTableWidget::paintEvent(event);
}

void MusicSongsListWidget::wheelEvent(QWheelEvent *event)
{
    MusicSlowMovingTableWidget::wheelEvent(event);
    if(m_renameActived && m_renameItem)
    {
        closePersistentEditor(m_renameItem);
        m_renameActived = false;
        m_renameItem = nullptr;
    }

    if(m_floatWidget == nullptr)
    {
        m_floatWidget = new MusicSongsListFunctionWidget(this);
        connect(m_floatWidget, SIGNAL(deleteObject()), SLOT(deleteFloatWidget()));
        m_floatWidget->setGeometry();
        m_floatWidget->show();
    }
    else
    {
        m_floatWidget->active();
    }
}

void MusicSongsListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicSlowMovingTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    QMenu musicPlaybackMode(tr("playbackMode"), &rightClickMenu);

    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay"), this, SLOT(musicPlayClicked()));
    rightClickMenu.addAction(tr("playLater"), this, SLOT(musicAddToPlayLater()));
    rightClickMenu.addAction(tr("addToPlayList"), this, SLOT(musicAddToPlayedList()));
    rightClickMenu.addAction(tr("downloadMore..."), this, SLOT(musicSongDownload()));
    rightClickMenu.addSeparator();

    rightClickMenu.addMenu(&musicPlaybackMode);
    QAction *order = musicPlaybackMode.addAction(tr("OrderPlay"), this, SIGNAL(musicPlayOrder()));
    QAction *random = musicPlaybackMode.addAction(tr("RandomPlay"), this, SIGNAL(musicPlayRandom()));
    QAction *lCycle = musicPlaybackMode.addAction(tr("ListCycle"), this, SIGNAL(musicPlayListLoop()));
    QAction *sCycle = musicPlaybackMode.addAction(tr("SingleCycle"), this, SIGNAL(musicPlayOneLoop()));
    QAction *once = musicPlaybackMode.addAction(tr("PlayOnce"), this, SIGNAL(musicPlayItemOnce()));
    (m_songplaymode == MusicObject::MC_PlayOrder) ? order->setIcon(QIcon(":/contextMenu/btn_selected")) : order->setIcon(QIcon());
    (m_songplaymode == MusicObject::MC_PlayRandom) ? random->setIcon(QIcon(":/contextMenu/btn_selected")) : random->setIcon(QIcon());
    (m_songplaymode == MusicObject::MC_PlayListLoop) ? lCycle->setIcon(QIcon(":/contextMenu/btn_selected")) : lCycle->setIcon(QIcon());
    (m_songplaymode == MusicObject::MC_PlayOneLoop) ? sCycle->setIcon(QIcon(":/contextMenu/btn_selected")) : sCycle->setIcon(QIcon());
    (m_songplaymode == MusicObject::MC_PlayOnce) ? once->setIcon(QIcon(":/contextMenu/btn_selected")) : once->setIcon(QIcon());

    QMenu musicAddNewFiles(tr("addNewFiles"), &rightClickMenu);
    rightClickMenu.addMenu(&musicAddNewFiles);
    musicAddNewFiles.setEnabled(m_parentToolIndex != MUSIC_LOVEST_LIST && m_parentToolIndex != MUSIC_NETWORK_LIST);
    musicAddNewFiles.addAction(tr("openOnlyFiles"), this, SIGNAL(musicAddNewFiles()));
    musicAddNewFiles.addAction(tr("openOnlyDir"), this, SIGNAL(musicAddNewDir()));
    rightClickMenu.addAction(tr("foundMV"), this, SLOT(musicSongMovieFound()));
    rightClickMenu.addSeparator();

    QMenu *addMenu = rightClickMenu.addMenu(QIcon(":/contextMenu/btn_add"), tr("addToList"));
    addMenu->addAction(tr("musicCloud"));

    rightClickMenu.addAction(QIcon(":/contextMenu/btn_mobile"), tr("songToMobile"), this, SLOT(musicSongTransferWidget()));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_ring"), tr("ringToMobile"), this, SLOT(musicSongTransferWidget()));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_similar"), tr("similar"), this, SLOT(musicSimilarFoundWidget()));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_share"), tr("songShare"), this, SLOT(musicSongSharedWidget()));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_kmicro"), tr("KMicro"));

    QMenu musicToolMenu(tr("musicTool"), &rightClickMenu);
    musicToolMenu.addAction(tr("bell"), this, SLOT(musicMakeRingWidget()));
    musicToolMenu.addAction(tr("transform"), this, SLOT(musicTransformWidget()));
    rightClickMenu.addMenu(&musicToolMenu);

    bool empty;
    emit isSearchFileListEmpty(empty);
    rightClickMenu.addAction(tr("musicInfo..."), this, SLOT(musicFileInformation()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_localFile"), tr("openFileDir"), this, SLOT(musicOpenFileDir()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("ablum"), this, SLOT(musicAlbumFoundWidget()));
    rightClickMenu.addSeparator();

    rightClickMenu.addAction(tr("changSongName"), this, SLOT(setChangSongName()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_delete"), tr("delete"), this, SLOT(setDeleteItemAt()))->setEnabled(empty);
    rightClickMenu.addAction(tr("deleteWithFile"), this, SLOT(setDeleteItemWithFile()))->setEnabled(empty);
    rightClickMenu.addAction(tr("deleteAll"), this, SLOT(setDeleteItemAll()))->setEnabled(empty);
    rightClickMenu.addSeparator();

    createContextMenu(rightClickMenu);

    rightClickMenu.exec(QCursor::pos());
    //Menu location for the current mouse position
    event->accept();
}

void MusicSongsListWidget::startToDrag()
{
    bool empty;
    emit isSearchFileListEmpty(empty);
    if(empty && m_dragStartIndex > -1 && m_leftButtonPressed && m_mouseMoved)
    {
        QStringList list;
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

        emit getMusicIndexSwaped(start, end, index, list);

        for(int i=qMin(start, end); i<=qMax(start, end); ++i)
        {
            item(i, 1)->setText(list[i]);
        }

        bool isCurrent;
        emit isCurrentIndexs(isCurrent);
        if(isCurrent)
        {
            selectRow(index);
        }
    }
}

void MusicSongsListWidget::createContextMenu(QMenu &menu)
{
    QString songName = getCurrentSongName();
    QStringList names(MusicUtils::Core::splitString(songName));
    for(int i=0; i<names.count(); ++i)
    {
        menu.addAction(tr("search '%1'").arg(names[i].trimmed()))->setData(i);
    }
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(musicSearchQuery(QAction*)));
}

void MusicSongsListWidget::createMoreMenu(QMenu *menu)
{
    menu->setStyleSheet(MusicUIObject::MMenuStyle02);

    QMenu *addMenu = menu->addMenu(QIcon(":/contextMenu/btn_add"), tr("addToList"));
    addMenu->addAction(tr("musicCloud"));

    menu->addAction(QIcon(":/contextMenu/btn_mobile"), tr("songToMobile"), this, SLOT(musicSongTransferWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_ring"), tr("ringToMobile"), this, SLOT(musicSongTransferWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_similar"), tr("similar"), this, SLOT(musicSimilarFoundWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_share"), tr("songShare"), this, SLOT(musicSongSharedWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_kmicro"), tr("KMicro"));
}

QString MusicSongsListWidget::getCurrentSongPath() const
{
    if(rowCount() == 0 || currentRow() < 0 )
    {
        return QString();
    }

    return getSongPath( currentRow() );
}

QString MusicSongsListWidget::getSongPath(int index) const
{
    return !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicPath().trimmed() : QString();
}

QString MusicSongsListWidget::getCurrentSongName() const
{
    if(rowCount() == 0 || currentRow() < 0 )
    {
        return QString();
    }

    return getSongName( currentRow() );
}

QString MusicSongsListWidget::getSongName(int index) const
{
    return !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicName().trimmed() : QString();
}
