#include "musicsongslistwidget.h"
#include "musicsongslistiteminfowidget.h"
#include "musicsongslistplaywidget.h"
#include "musictransformwidget.h"
#include "musicfileinformationwidget.h"
#include "musicsongringtonemakerwidget.h"
#include "musicmessagebox.h"
#include "musicprogresswidget.h"
#include "musicutils.h"
#include "musicnumberdefine.h"

#include <QUrl>
#include <QAction>
#include <QTimer>
#include <QPainter>
#include <QDebug>

#define ROW_HIGHT   30

MusicSongsListWidget::MusicSongsListWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent), m_musicSongsInfoWidget(nullptr),
      m_musicSongsPlayWidget(nullptr)
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

    MusicUtils::UWidget::setTransparent(this, 0);
#ifndef MUSIC_GREATER_NEW
    setStyleSheet(MusicUIObject::MTableWidgetStyle01 + \
                  MusicUIObject::MLineEditStyle01 + \
                  MusicUIObject::MTableWidgetStyle02);
#endif

    connect(&m_timerShow, SIGNAL(timeout()), SLOT(showTimeOut()));
    connect(&m_timerStay, SIGNAL(timeout()), SLOT(stayTimeOut()));
}

MusicSongsListWidget::~MusicSongsListWidget()
{
    clearAllItems();
    delete m_musicSongsInfoWidget;
    delete m_musicSongsPlayWidget;
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
    int count = rowCount();
    setRowCount(songs.count());    //reset row count
    for(int i=count; i<songs.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);
        //To get the song name
                          item = new QTableWidgetItem;
        item->setText(MusicUtils::UWidget::elidedText(font(), songs[i].getMusicName(), Qt::ElideRight, 232));
        item->setTextColor(QColor(50, 50, 50));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);
        //add a delete icon
                          item = new QTableWidgetItem(songs[i].getMusicTime());
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);
    }
    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
}

void MusicSongsListWidget::clearAllItems()
{
    //Remove play widget
    setRowHeight(m_playRowIndex, ROW_HIGHT);
    removeCellWidget(m_playRowIndex, 0);
    removeCellWidget(m_playRowIndex, 1);
    removeCellWidget(m_playRowIndex, 2);

    delete m_musicSongsPlayWidget;
    m_musicSongsPlayWidget = nullptr;

    m_playRowIndex = 0;
    //Remove all the original item
    MusicAbstractTableWidget::clear();
    setColumnCount(3);
}

void MusicSongsListWidget::mousePressEvent(QMouseEvent *event)
{
    MusicAbstractTableWidget::mousePressEvent(event);
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
        m_renameItem->setText(MusicUtils::UWidget::elidedText(font(), m_renameItem->text(), Qt::ElideRight, 243));
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
    MusicAbstractTableWidget::mouseMoveEvent(event);
    if(m_leftButtonPressed && abs(m_dragStartPoint.y() - event->pos().y()) > 15)
    {
        m_mouseMoved = true;
        setCursor(QCursor(Qt::CrossCursor));
        setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void MusicSongsListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    MusicAbstractTableWidget::mouseReleaseEvent(event);
    startToDrag();

    m_leftButtonPressed = false;
    m_mouseMoved = false;
    setCursor(QCursor(Qt::ArrowCursor));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void MusicSongsListWidget::leaveEvent(QEvent *event)
{
    MusicAbstractTableWidget::leaveEvent(event);
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
    painter.fillRect(0, 0, w->width(), w->height(), QColor(255, 255, 255, m_transparent));
    painter.end();

    MusicAbstractTableWidget::paintEvent(event);
}

void MusicSongsListWidget::wheelEvent(QWheelEvent *event)
{
    MusicAbstractTableWidget::wheelEvent(event);
    if(m_renameActived && m_renameItem)
    {
        closePersistentEditor(m_renameItem);
        m_renameActived = false;
        m_renameItem = nullptr;
    }
}

void MusicSongsListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    QMenu musicPlaybackMode(tr("playbackMode"), &rightClickMenu);

    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay"), this, SLOT(musicPlayClicked()));
    rightClickMenu.addAction(tr("playLater"));
    rightClickMenu.addAction(tr("addToPlayList"));
    rightClickMenu.addAction(tr("downloadMore..."));
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
    musicAddNewFiles.addAction(tr("openOnlyFiles"), this, SIGNAL(musicAddNewFiles()));
    musicAddNewFiles.addAction(tr("openOnlyDir"), this, SIGNAL(musicAddNewDir()));
    rightClickMenu.addAction(tr("foundMV"));
    rightClickMenu.addSeparator();

    QMenu *addMenu = rightClickMenu.addMenu(QIcon(":/contextMenu/btn_add"), tr("addToList"));
    addMenu->addAction(tr("musicCloud"));

    rightClickMenu.addAction(QIcon(":/contextMenu/btn_mobile"), tr("songToMobile"));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_ring"), tr("ringToMobile"));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_similar"), tr("similar"));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_share"), tr("songShare"));
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_kmicro"), tr("KMicro"));

    QMenu musicToolMenu(tr("musicTool"), &rightClickMenu);
    musicToolMenu.addAction(tr("bell"), this, SLOT(musicMakeRingWidget()));
    musicToolMenu.addAction(tr("transform"), this, SLOT(musicTransformWidget()));
    rightClickMenu.addMenu(&musicToolMenu);

    bool empty;
    emit isSearchFileListEmpty(empty);
    rightClickMenu.addAction(tr("musicInfo..."), this, SLOT(musicFileInformation()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_localFile"), tr("openFileDir"), this, SLOT(musicOpenFileDir()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("ablum"));
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
    QString songName = currentRow() != -1 && rowCount() > 0 ?
                m_musicSongs->at(currentRow()).getMusicName() : QString();

    QStringList names = songName.split("-");
    foreach(QString name, names)
    {
        menu.addAction(tr("search '%1'").arg(name.trimmed()));
    }
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

void MusicSongsListWidget::setDeleteItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if(message.exec() || rowCount() == 0 || currentRow() < 0)
    {
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
        removeRow(deleteList[i]); //Delete the current row
        progress.setValue(deleteList.count()*2 - i);
    }

    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );

    emit deleteItemAt(deleteList, m_deleteItemWithFile);
}

void MusicSongsListWidget::listCellClicked(int row, int column)
{
    //the playing widget allow deleting
    if(row == m_playRowIndex)
    {
        return;
    }
    switch(column)
    {
        case 2:
            setDeleteItemAt();
            break;
        default:
            break;
    }
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
        it->setText((*m_musicSongs)[m_previousColorRow].getMusicTime());
    }

    ///draw new table item state
    if((it = item(row, 0)) != nullptr)
    {
        it->setIcon(QIcon(":/tiny/btn_play_later_normal"));
    }
    if((it = item(row, 2)) != nullptr)
    {
        it->setText(QString());
        it->setIcon(QIcon(":/tiny/btn_delete_hover"));
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
            it->setText(QString());
            it->setIcon(QIcon());
        }
    }
    MusicAbstractTableWidget::listCellEntered(row, column);

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
                                            !m_musicSongsPlayWidget->getItemRenameState()) :
                                            true);
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
    if(rowCount() == 0 || currentRow() < 0 || currentItem()->column() != 1)
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

void MusicSongsListWidget::setItemRenameFinished(const QString &name)
{
    (*m_musicSongs)[m_playRowIndex].setMusicName(name);
}

void MusicSongsListWidget::musicOpenFileDir()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    QString path = !m_musicSongs->isEmpty() ? m_musicSongs->at(currentRow()).getMusicPath() : QString();
    if(!MusicUtils::UCore::openUrl(QFileInfo(path).absoluteFilePath(), true))
    {
        MusicMessageBox message;
        message.setText(tr("The origin one does not exist!"));
        message.exec();
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

void MusicSongsListWidget::musicMakeRingWidget()
{
    MusicSongRingtoneMaker(this).exec();
}

void MusicSongsListWidget::musicTransformWidget()
{
    MusicTransformWidget(this).exec();
}

void MusicSongsListWidget::musicFileInformation()
{
    if(rowCount() == 0 || currentRow() < 0 )
    {
        return;
    }

    MusicFileInformationWidget file(this);
    QString path = !m_musicSongs->isEmpty() ? m_musicSongs->at(currentRow()).getMusicPath() : QString();
    file.setFileInformation(path);
    file.exec();
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
    removeCellWidget(m_playRowIndex, 1);
    removeCellWidget(m_playRowIndex, 2);

    delete takeItem(m_playRowIndex, 0);
    delete takeItem(m_playRowIndex, 1);
    delete takeItem(m_playRowIndex, 2);

    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(m_playRowIndex, 0, item);
    item = new QTableWidgetItem(MusicUtils::UWidget::elidedText(font(), name, Qt::ElideRight, 242));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(m_playRowIndex, 1, item);
    item = new QTableWidgetItem( (*m_musicSongs)[m_playRowIndex].getMusicTime() );
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(m_playRowIndex, 2, item);

    delete m_musicSongsPlayWidget;
    m_musicSongsPlayWidget = nullptr;

    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
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
    delete takeItem(index, 0);
    delete takeItem(index, 1);
    delete takeItem(index, 2);
    setItem(index, 0, new QTableWidgetItem);
    setItem(index, 1, new QTableWidgetItem);
    setItem(index, 2, new QTableWidgetItem);

    QString name = !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicName() : QString();
    QString path = !m_musicSongs->isEmpty() ? m_musicSongs->at(index).getMusicPath() : QString();

    m_musicSongsPlayWidget = new MusicSongsListPlayWidget(index, this);
    m_musicSongsPlayWidget->setParameter(name, path);
    QWidget *widget, *widget1;
    m_musicSongsPlayWidget->getWidget(widget, widget1);

    setCellWidget(index, 0, widget);
    setCellWidget(index, 1, m_musicSongsPlayWidget);
    setCellWidget(index, 2, widget1);
    setRowHeight(index, 2*ROW_HIGHT);
    m_playRowIndex = index;

    //just fix table widget size hint
    setFixedHeight( allRowsHeight() );
}
