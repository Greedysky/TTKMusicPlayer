#include "musicmydownloadrecordwidget.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicitemdelegate.h"
#include "musicsongssummarizied.h"
#include "musiccoreutils.h"

#include <QMenu>
#include <QContextMenuEvent>

MusicMyDownloadRecordWidget::MusicMyDownloadRecordWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 170);
    headerview->resizeSection(2, 83);
    headerview->resizeSection(3, 50);

    m_delegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(2, m_delegate);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(listCellDoubleClicked(int,int)));
    musicSongsFileName();

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummarizied::getClassName());
}

MusicMyDownloadRecordWidget::~MusicMyDownloadRecordWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName() );
    delete m_delegate;
    clearAllItems();

    MusicMyDownloadRecordConfigManager xml;
    xml.writeDownloadConfig(m_musicRecords);
}

QString MusicMyDownloadRecordWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicMyDownloadRecordWidget::musicSongsFileName()
{
    MusicMyDownloadRecordConfigManager xml;
    if(!xml.readDownloadXMLConfig())
    {
        return;
    }
    xml.readDownloadConfig(m_musicRecords);

    setRowCount(m_loadRecordCount = m_musicRecords.count()); //reset row count
    for(int i=0; i<m_musicRecords.count(); i++)
    {
        createItem(i, m_musicRecords[i].m_name, m_musicRecords[i].m_size, 999);
    }
}

void MusicMyDownloadRecordWidget::createItem(int index, const QString &name,
                                             const QString &size, qint64 time)
{
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 160));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip( name );
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, 100);
    setItem(index, 2, item);

                      item = new QTableWidgetItem( size );
    item->setTextAlignment(Qt::AlignCenter);
    item->setData(MUSIC_TIMES_ROLE, time);
    setItem(index, 3, item);
}

void MusicMyDownloadRecordWidget::clearAllItems()
{
    //Remove all the original item
    MusicAbstractTableWidget::clear();
    setColumnCount(4);
}

void MusicMyDownloadRecordWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay"), this, SLOT(musicPlay()));
    rightClickMenu.addAction(tr("openFileDir"), this, SLOT(musicOpenFileDir()));
    rightClickMenu.addSeparator();
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_delete"), tr("delete"), this, SLOT(setDeleteItemAt()));
    rightClickMenu.addAction(tr("deleteAll"), this, SLOT(setDeleteItemAll()));
    rightClickMenu.exec(QCursor::pos());
    event->accept();
}

void MusicMyDownloadRecordWidget::setDeleteItemAll()
{
    selectAll();
    setDeleteItemAt();
}

void MusicMyDownloadRecordWidget::setDeleteItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if( message.exec() || rowCount() == 0 )
    {
       return;
    }

    MusicObject::MIntSet deletedRow; //if selected multi rows
    foreach(QTableWidgetItem *item, selectedItems())
    {
        deletedRow.insert(item->row());
    }

    MusicObject::MIntList deleteList = deletedRow.toList();
    qSort(deleteList);
    for(int i=deleteList.count() - 1; i>=0; --i)
    {
        int index = deleteList[i];
        removeRow(index); //Delete the current row
        m_musicRecords.removeAt(index);
        --m_loadRecordCount;
    }
}

void MusicMyDownloadRecordWidget::listCellClicked(int, int)
{

}

void MusicMyDownloadRecordWidget::listCellDoubleClicked(int, int)
{
    musicPlay();
}

void MusicMyDownloadRecordWidget::musicOpenFileDir()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    if(!MusicUtils::Core::openUrl(QFileInfo(m_musicRecords[currentRow()].m_path).absoluteFilePath(), true))
    {
        MusicMessageBox message;
        message.setText(tr("The origin one does not exist!"));
        message.exec();
    }
}

void MusicMyDownloadRecordWidget::musicPlay()
{
    if(rowCount() == 0 || currentRow() <0)
    {
        return;
    }
    emit addSongToPlay(QStringList(m_musicRecords[currentRow()].m_path));
}

void MusicMyDownloadRecordWidget::downloadProgressChanged(float percent, const QString &total, qint64 time)
{
    for(int i=m_loadRecordCount; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 3);
        if(it && it->data(MUSIC_TIMES_ROLE).toLongLong() == time)
        {
            item(i, 2)->setData(MUSIC_PROCS_ROLE, percent);
            item(i, 3)->setText( total );
            if(percent == 100)
            {
                m_musicRecords[i].m_size = total;
            }
            break;
        }
    }
}

void MusicMyDownloadRecordWidget::createDownloadItem(const QString &name, qint64 time)
{
    setRowCount( rowCount() + 1);
    QString musicName = name;
    musicName.remove(MusicUtils::Core::musicPrefix()).chop(4);

    MusicDownloadRecord record;
    record.m_name = musicName;
    record.m_path = QFileInfo(name).absoluteFilePath();
    record.m_size = "0.0M";
    m_musicRecords << record;

    createItem(rowCount() - 1, musicName, "0.0M", time);
}
