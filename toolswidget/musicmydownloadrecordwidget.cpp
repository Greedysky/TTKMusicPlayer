#include "musicmydownloadrecordwidget.h"
#include "musicmydownloadrecordobject.h"
#include "musicmessagebox.h"

#include <QDesktopServices>

MusicMyDownloadRecordWidget::MusicMyDownloadRecordWidget(QWidget *parent) :
    MusicTableWidgetAbstract(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(this,SIGNAL(cellEntered(int,int)),SLOT(listCellEntered(int,int)));
    connect(this,SIGNAL(cellDoubleClicked(int,int)),SLOT(listCellDoubleClicked(int,int)));
    musicSongsFileName();
}

MusicMyDownloadRecordWidget::~MusicMyDownloadRecordWidget()
{
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
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(i+1));
        item->setTextColor(QColor(50,50,50));
        item->setTextAlignment(Qt::AlignCenter);
        this->setItem(i,0,item);

        //To get the song name
        QTableWidgetItem *item1 = new QTableWidgetItem(QFontMetrics(font()).elidedText(
                                                m_musicFileNameList[i], Qt::ElideRight, 243));
        item1->setTextColor(QColor(50,50,50));
        item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        this->setItem(i,1,item1);

        //add a delete icon
        QTableWidgetItem *item2 = new QTableWidgetItem(QIcon(":/image/musicdelete"),"");
        item2->setTextAlignment(Qt::AlignCenter);
        this->setItem(i,2,item2);
    }
}

void MusicMyDownloadRecordWidget::clearAllItems()
{
    //Remove all the original item
    MusicTableWidgetAbstract::clearAllItems();
    setColumnCount(3);
}

void MusicMyDownloadRecordWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle01);
    rightClickMenu.addAction(QIcon(":/contextMenu/play"),tr("musicPlay"),this,SLOT(musicPlay()));
    rightClickMenu.addAction(tr("openFileDir"),this,SLOT(musicOpenFileDir()));
    rightClickMenu.addSeparator();
    rightClickMenu.addAction(QIcon(":/contextMenu/delete"),tr("delete"),this,SLOT(setDeleteItemAt()));
    rightClickMenu.addAction(tr("deleteAll"),this,SLOT(setDeleteItemAll()));
    rightClickMenu.exec(QCursor::pos());
//    //Menu location for the current mouse position
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

    for(int i=0; i<this->rowCount(); i++)
    {   //Re insertion sort
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(i+1));
        item->setTextColor(QColor(50,50,50));
        item->setTextAlignment(Qt::AlignCenter);
        delete this->takeItem(i,0);//Remove the original object delete
        this->setItem(i,0,item); //insert
    }
}

void MusicMyDownloadRecordWidget::listCellClicked(int , int column)
{
    switch(column)
    {
        case 2:
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
    emit musicPlay(QStringList(m_musicFilePathList[currentRow()]));
}
