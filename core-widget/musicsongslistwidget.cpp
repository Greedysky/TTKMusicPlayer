#include "musicsongslistwidget.h"
#include "musicsongslistiteminfowidget.h"
#include "musicsongslistplaywidget.h"
#include "musictransformwidget.h"
#include "musicfileinformationwidget.h"
#include "musicmessagebox.h"

#include <QAction>
#include <QTimer>
#include <QProcess>

MusicSongsListWidget::MusicSongsListWidget(QWidget *parent) :
    MusicAbstractTableWidget(parent),m_musicSongsListItem(NULL),
    m_musicSongsPlayWidget(NULL)
{
    m_deleteItemWithFile = false;
    m_renameActived = false;
    m_renameItem = NULL;
    m_playRowIndex = 0;
    m_dragStartIndex = -1;
    m_leftButtonPressed = false;
    m_mouseMoved = false;

    m_timerShow = new QTimer(this);
    m_timerStay = new QTimer(this);

    connect(m_timerShow,SIGNAL(timeout()),SLOT(showTimeOut()));
    connect(m_timerStay,SIGNAL(timeout()),SLOT(stayTimeOut()));
}

MusicSongsListWidget::~MusicSongsListWidget()
{
    clearAllItems();
    delete m_timerShow;
    delete m_timerStay;
    delete m_musicSongsListItem;
    delete m_musicSongsPlayWidget;
}

void MusicSongsListWidget::musicSongsFileName(const QStringList &filenamelists)
{
    int count = rowCount();
    setRowCount(filenamelists.count());//reset row count
    for(int i=count; i<filenamelists.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(i+1));
        item->setTextColor(QColor(50,50,50));
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 0, item);
        //To get the song name
                          item = new QTableWidgetItem(QFontMetrics(font()).elidedText(
                                                filenamelists[i], Qt::ElideRight, 243));
        item->setTextColor(QColor(50,50,50));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

        //add a delete icon
                          item = new QTableWidgetItem(QIcon(":/image/musicdelete"), QString());
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 2, item);
    }
}

void MusicSongsListWidget::clearAllItems()
{
    //Remove play widget
    setRowHeight(m_playRowIndex, 30);
    removeCellWidget(m_playRowIndex, 0);
    removeCellWidget(m_playRowIndex, 1);
    removeCellWidget(m_playRowIndex, 2);
    delete m_musicSongsPlayWidget;
    m_musicSongsPlayWidget = NULL;
    m_playRowIndex = 0;
    //Remove all the original item
    MusicAbstractTableWidget::clear();
    setColumnCount(3);
}

void MusicSongsListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    QMenu musicPlaybackMode(tr("playbackMode"),&rightClickMenu);
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(tr("changSongName"),this,SLOT(setChangSongName()));
    rightClickMenu.addAction(QIcon(":/contextMenu/play"),tr("musicPlay"),this,SLOT(musicPlay()));

    rightClickMenu.addMenu(&musicPlaybackMode);
    QAction *order = musicPlaybackMode.addAction(tr("OrderPlay"),this,SIGNAL(musicPlayOrder()));
    QAction *random = musicPlaybackMode.addAction(tr("RandomPlay"),this,SIGNAL(musicPlayRandom()));
    QAction *lCycle = musicPlaybackMode.addAction(tr("ListCycle"),this,SIGNAL(musicPlayListLoop()));
    QAction *sCycle = musicPlaybackMode.addAction(tr("SingleCycle"),this,SIGNAL(musicPlayOneLoop()));
    QAction *once = musicPlaybackMode.addAction(tr("PlayOnce"),this,SIGNAL(musicPlayItemOnce()));
    (m_songplaymode == MusicObject::MC_PlayOrder) ? order->setIcon(QIcon(":/share/selected")) : order->setIcon(QIcon());
    (m_songplaymode == MusicObject::MC_PlayRandom) ? random->setIcon(QIcon(":/share/selected")) : random->setIcon(QIcon());
    (m_songplaymode == MusicObject::MC_PlayListLoop) ? lCycle->setIcon(QIcon(":/share/selected")) : lCycle->setIcon(QIcon());
    (m_songplaymode == MusicObject::MC_PlayOneLoop) ? sCycle->setIcon(QIcon(":/share/selected")) : sCycle->setIcon(QIcon());
    (m_songplaymode == MusicObject::MC_PlayOnce) ? once->setIcon(QIcon(":/share/selected")) : once->setIcon(QIcon());

    rightClickMenu.addSeparator();
    rightClickMenu.addAction(QIcon(":/contextMenu/love"),tr("addToLove"),this,SLOT(addMusicSongToLovestListAt()));

    QMenu musicAddNewFiles(tr("addNewFiles"),&rightClickMenu);
    rightClickMenu.addMenu(&musicAddNewFiles)->setIcon(QIcon(":/contextMenu/add"));
    musicAddNewFiles.addAction(tr("openOnlyFiles"),this,SIGNAL(musicAddNewFiles()));
    musicAddNewFiles.addAction(tr("openOnlyDir"),this,SIGNAL(musicAddNewDir()));

    QMenu musicToolMenu(tr("musicTool"),&rightClickMenu);
    musicToolMenu.addAction(tr("bell"),this,SLOT(musicMakeRingWidget()));
    musicToolMenu.addAction(tr("transform"),this,SLOT(musicTransformWidget()));
    rightClickMenu.addMenu(&musicToolMenu);
    rightClickMenu.addAction(tr("musicInfoD"),this,SLOT(musicFileInformation()));
    rightClickMenu.addAction(tr("openFileDir"),this,SLOT(musicOpenFileDir()));
    rightClickMenu.addSeparator();

    rightClickMenu.addAction(QIcon(":/contextMenu/delete"),tr("delete"),this,SLOT(setDeleteItemAt()));
    rightClickMenu.addAction(tr("deleteWithFile"),this,SLOT(setDeleteItemWithFile()));
    rightClickMenu.addAction(tr("deleteAll"),this,SLOT(setDeleteItemAll()));
    rightClickMenu.exec(QCursor::pos());
    //Menu location for the current mouse position
    event->accept();
}

void MusicSongsListWidget::mousePressEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);
    //just close the rename edittext;
    if(m_renameActived)
    {
        closePersistentEditor(m_renameItem);
    }
    //it may be a bug in closePersistentEditor,so we select
    //the two if function to deal with
    if(m_renameActived)
    {
        emit currentTextChanged(m_renameItem->row(),m_renameItem->text());
        m_renameItem->setText(QFontMetrics(font()).elidedText(
                       m_renameItem->text(), Qt::ElideRight, 243));
        m_renameActived = false;
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
    QTableWidget::mouseMoveEvent(event);
    if(m_leftButtonPressed && abs(m_dragStartPoint.y() - event->pos().y()) > 15)
    {
        m_mouseMoved = true;
        setCursor(QCursor(Qt::CrossCursor));
        setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void MusicSongsListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QTableWidget::mouseReleaseEvent(event);
    if(m_dragStartIndex > -1 && m_leftButtonPressed && m_mouseMoved)
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
            index = (start > end) ? index = end + 1 : index = end - 1;
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

        selectRow(index);
    }
    m_leftButtonPressed = false;
    m_mouseMoved = false;
    setCursor(QCursor(Qt::ArrowCursor));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void MusicSongsListWidget::leaveEvent(QEvent *event)
{
    QTableWidget::leaveEvent(event);
    delete m_musicSongsListItem;
    m_musicSongsListItem = NULL;
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

    MIntSet deletedRow; //if selected multi rows
    for(int i=0; i<selectedItems().count(); ++i)
    {
        deletedRow.insert(selectedItems()[i]->row());
    }
    MIntList deleteList = deletedRow.toList();
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
    }

    for(int i=0; i<this->rowCount(); i++)
    {   //Re insertion sort
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(i+1));
        item->setTextColor(QColor(50,50,50));
        item->setTextAlignment(Qt::AlignCenter);
        delete this->takeItem(i,0);//Remove the original object delete
        this->setItem(i,0,item); //insert
    }
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
    MusicAbstractTableWidget::listCellEntered(row, column);
    //To show music Songs Item information
    if(m_musicSongsListItem == NULL)
    {
        m_musicSongsListItem = new MusicSongsListItemInfoWidget;
    }
    m_musicSongsListItem->hide();
    m_timerShow->stop();
    m_timerShow->start(1000);
    m_timerStay->stop();
    m_timerStay->start(3000);
}

void MusicSongsListWidget::showTimeOut()
{
    if(m_musicSongsListItem)
    {
        MusicSong musicinfo;
        emit getMusicSongInformation(m_previousColorRow, musicinfo);
        m_musicSongsListItem->setMusicSongInformation(musicinfo);
        m_musicSongsListItem->setGeometry(QCursor::pos().x()+50,QCursor::pos().y(),264,108);
        m_musicSongsListItem->show();
    }
}

void MusicSongsListWidget::stayTimeOut()
{
    delete m_musicSongsListItem;
    m_musicSongsListItem = NULL;
}

void MusicSongsListWidget::addMusicSongToLovestListAt()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }
    emit musicSongToLovestListAt(currentRow());
}

void MusicSongsListWidget::addPlaySongToLovestListAt()
{
    if(rowCount() == 0 || m_playRowIndex < 0)
    {
        return;
    }
    emit musicSongToLovestListAt(m_playRowIndex);
}

void MusicSongsListWidget::setChangSongName()
{
    if(rowCount() == 0 || currentRow() < 0 ||
        currentItem()->column() != 1)
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
    emit currentTextChanged(m_playRowIndex, name);
}

void MusicSongsListWidget::musicOpenFileDir()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }
    emit musicOpenFileDir(currentRow());
}

void MusicSongsListWidget::musicPlay()
{
    if(rowCount() == 0 || currentRow() < 0 )
    {
        return;
    }
    emit musicPlay(currentRow());
}

void MusicSongsListWidget::musicMakeRingWidget()
{
    if(!QFile(MAKE_RING).exists())
    {
        return;
    }
    (new QProcess(this))->start(MAKE_RING);
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
    QString name, path;
    emit getMusicSongFileInformation(currentRow(), name, path, false);
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

void MusicSongsListWidget::updateArtPicture() const
{
    if(m_musicSongsPlayWidget)
    {
        m_musicSongsPlayWidget->updateArtPicture();
    }
}

void MusicSongsListWidget::replacePlayWidgetRow()
{
    QString name,path;
    if(m_playRowIndex >= rowCount() || m_playRowIndex < 0)
    {
        m_playRowIndex = 0;
    }
    emit getMusicSongFileInformation(m_playRowIndex, name, path);

    setRowHeight(m_playRowIndex, 30);
    removeCellWidget(m_playRowIndex, 0);
    removeCellWidget(m_playRowIndex, 1);
    removeCellWidget(m_playRowIndex, 2);

    delete takeItem(m_playRowIndex, 0);
    delete takeItem(m_playRowIndex, 1);
    delete takeItem(m_playRowIndex, 2);
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(m_playRowIndex + 1));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(m_playRowIndex, 0, item);
    item = new QTableWidgetItem(QFontMetrics(font()).elidedText(name, Qt::ElideRight, 243));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(m_playRowIndex, 1, item);
    item = new QTableWidgetItem(QIcon(":/image/musicdelete"), QString());
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(m_playRowIndex, 2, item);
    delete m_musicSongsPlayWidget;
    m_musicSongsPlayWidget = NULL;
}

void MusicSongsListWidget::selectRow(int index)
{
    if(index < 0)
    {
        return;
    }

    QTableWidget::selectRow(index);
    QString name,path;

    replacePlayWidgetRow();

    delete takeItem(index, 0);
    delete takeItem(index, 1);
    delete takeItem(index, 2);
    setItem(index, 0, new QTableWidgetItem);
    setItem(index, 1, new QTableWidgetItem);
    setItem(index, 2, new QTableWidgetItem);

    emit getMusicSongFileInformation(index, name, path);
    m_musicSongsPlayWidget = new MusicSongsListPlayWidget(this);
    connect(m_musicSongsPlayWidget, SIGNAL(renameFinished(QString)),
                                    SLOT(setItemRenameFinished(QString)));
    m_musicSongsPlayWidget->setParameter(name, path);
    QWidget *widget, *widget1;
    m_musicSongsPlayWidget->getWidget(widget, widget1);

    setCellWidget(index, 0, widget);
    setCellWidget(index, 1, m_musicSongsPlayWidget);
    setCellWidget(index, 2, widget1);
    setRowHeight(index, 60);
    m_playRowIndex = index;
}
