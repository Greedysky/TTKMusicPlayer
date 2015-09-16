#include "musicmydownloadrecordwidget.h"
#include "musicmydownloadrecordobject.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicitemdelegate.h"

#include <QDesktopServices>
#include <QContextMenuEvent>
#include <QMenu>


MusicMyDownloadRecordWidget::MusicMyDownloadRecordWidget(QWidget *parent) :
    MusicAbstractTableWidget(parent)
{
    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,15);
    headerview->resizeSection(1,175);
    headerview->resizeSection(2,103);
    headerview->resizeSection(3,30);

    m_delegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(2, m_delegate);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(listCellDoubleClicked(int,int)));
    musicSongsFileName();

    M_Connection->setValue("MusicMyDownloadRecordWidget", this);
    M_Connection->connect("MusicMyDownloadRecordWidget", "MusicApplication");
}

MusicMyDownloadRecordWidget::~MusicMyDownloadRecordWidget()
{
    M_Connection->disConnect("MusicMyDownloadRecordWidget");
    delete m_delegate;
    clearAllItems();
    MusicMyDownloadRecordObject xml;
    xml.writeDownloadConfig(m_musicFileNameList,m_musicFilePathList);
}

void MusicMyDownloadRecordWidget::musicSongsFileName()
{
    MusicMyDownloadRecordObject xml;
    if(!xml.readDownloadXMLConfig())
    {
        return;
    }
    xml.readDownloadConfig(m_musicFileNameList,m_musicFilePathList);

    setRowCount(m_musicFileNameList.count());//reset row count
    for(int i=0; i<m_musicFileNameList.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);

                          item = new QTableWidgetItem(QFontMetrics(font()).elidedText(
                                                  m_musicFileNameList[i], Qt::ElideRight, 170));
        item->setTextColor(QColor(50,50,50));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        item->setToolTip(m_musicFileNameList[i]);
        setItem(i, 1, item);

                          item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole, 100);
        setItem(i, 2, item);

                          item = new QTableWidgetItem(QIcon(":/image/musicdelete"), QString());
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 3, item);
    }
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
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle01);
    rightClickMenu.addAction(QIcon(":/contextMenu/play"), tr("musicPlay"), this, SLOT(musicPlay()));
    rightClickMenu.addAction(tr("openFileDir"),this,SLOT(musicOpenFileDir()));
    rightClickMenu.addSeparator();
    rightClickMenu.addAction(QIcon(":/contextMenu/delete"), tr("delete"), this, SLOT(setDeleteItemAt()));
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
    MIntSet deletedRow; //if selected multi rows
    for(int i=0; i<selectedItems().count(); ++i)
    {
        deletedRow.insert(selectedItems()[i]->row());
    }
    MIntList deleteList = deletedRow.toList();
    qSort(deleteList);
    for(int i=deleteList.count() - 1; i>=0; --i)
    {
        int ind = deleteList[i];
        removeRow(ind); //Delete the current row
        m_musicFileNameList.removeAt(ind);
        m_musicFilePathList.removeAt(ind);
    }
}

void MusicMyDownloadRecordWidget::listCellClicked(int , int column)
{
    switch(column)
    {
        case 3:
            setDeleteItemAt();
            break;
        default:
            break;
    }
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

    if(!QDesktopServices::openUrl(QUrl(QFileInfo(m_musicFilePathList[currentRow()]).absolutePath()
                              , QUrl::TolerantMode)))
    {
        MusicMessageBox message;
        message.setText(tr("The origin one does not exsit!"));
        message.exec();
    }
}

void MusicMyDownloadRecordWidget::musicPlay()
{
    if(rowCount() == 0 || currentRow() <0)
    {
        return;
    }
    emit addSongToPlay(QStringList(m_musicFilePathList[currentRow()]));
}
