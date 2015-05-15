#include "musicmydownloadrecordwidget.h"
#include "musicmydownloadrecordobject.h"
#include <QDesktopServices>
#include <QMessageBox>

MusicMyDownloadRecordWidget::MusicMyDownloadRecordWidget(QWidget *parent) :
    QTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    this->setFont(QFont("Helvetica"));
    this->setColumnCount(3);
    this->setRowCount(0);
    this->setShowGrid(false);//Does not display the grid
    QHeaderView *headerview = this->horizontalHeader();
    headerview->setVisible(false);
    headerview->resizeSection(0,40);
    headerview->resizeSection(1,252);
    headerview->resizeSection(2,26);
    this->verticalHeader()->setVisible(false);
    this->setMouseTracking(true);  //Open the capture mouse function
    this->setStyleSheet("QTableWidget{selection-background-color:pink;}" + \
                         MusicObject::MusicScrollBarStyle + \
                         MusicObject::MusicListEditStyle );
    //Set the color of selected row
    this->setFrameShape(QFrame::NoFrame);//Set No Border
    this->setEditTriggers(QTableWidget::NoEditTriggers);//No edit
    this->setSelectionBehavior(QTableWidget::SelectRows);
    //Multi-line election
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //Only select one row
    this->setTransparent(80);
    this->setFocusPolicy(Qt::NoFocus);

    m_previousColorRow = -1;
    m_defaultBkColor = QColor(255,255,255,0);
    connect(this,SIGNAL(cellEntered(int,int)),SLOT(listCellEntered(int,int)));
    connect(this,SIGNAL(cellClicked(int,int)),SLOT(listCellClicked(int,int)));
    connect(this,SIGNAL(cellDoubleClicked(int,int)),SLOT(listCellDoubleClicked(int,int)));
    musicSongsFileName();
}

MusicMyDownloadRecordWidget::~MusicMyDownloadRecordWidget()
{
    clearAllItems();
    MusicMyDownloadRecordObject xml;
    xml.writeDownloadConfig(m_musicFileNameList,m_musicFilePathList);
}

void MusicMyDownloadRecordWidget::setTransparent(int angle)
{
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Base,QBrush(QColor(255,255,255,angle)));
    this->setPalette(pal);
    this->repaint();
}

void MusicMyDownloadRecordWidget::musicSongsFileName()
{
    MusicMyDownloadRecordObject xml;
    if(!xml.readDownloadXMLConfig())
        return;
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
    clearContents();
//    for(int j=0; j<3; j++)
//      for(int i=0; i<rowCount(); i++)
//      {
//          QTableWidgetItem *item = this->item(i,j);
//          delete item;
//          item = NULL;
//      }
    setColumnCount(3);
    setRowCount(0);
}

void MusicMyDownloadRecordWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicObject::MusicSystemTrayMenu);
    rightClickMenu.addAction(QIcon(QPixmap(":/contextMenu/play")),tr("musicPlay"),this,SLOT(musicPlay()));
    rightClickMenu.addAction(tr("openFileDir"),this,SLOT(musicOpenFileDir()));
    rightClickMenu.addSeparator();
    rightClickMenu.addAction(QIcon(QPixmap(":/contextMenu/delete")),tr("delete"),this,SLOT(setDeleteItemAt()));
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
    if(this->rowCount() == 0 || QMessageBox::information(0,tr("QMusicPlayer"),
                                 tr("Are you sure to delete?"),
                                 tr("Yes"),
                                 tr("Cancel")) != 0 )
       return;
    QSet<int> deletedRow; //if selected multi rows
    for(int i=0; i<selectedItems().count(); ++i)
        deletedRow.insert(selectedItems()[i]->row());
    QList<int> deleteList = deletedRow.toList();
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

void MusicMyDownloadRecordWidget::listCellEntered(int row, int column)
{
    QTableWidgetItem *item = this->item(m_previousColorRow, 0);
    if(item != 0)
       this->setRowColor(m_previousColorRow, m_defaultBkColor);

    item = this->item(row, column);
    if(item != 0 && !item->isSelected() && !item->text().isEmpty())
       this->setRowColor(row, QColor(193,210,240));

    m_previousColorRow = row;
}

void MusicMyDownloadRecordWidget::setRowColor(int row,const QColor& color)
{
    for(int col=0; col<this->columnCount(); col++)
    {
        QTableWidgetItem *item = this->item(row, col);
        item->setBackgroundColor(color);
    }
}

void MusicMyDownloadRecordWidget::musicOpenFileDir()
{
    if(rowCount() == 0 || currentRow() <0)
        return;

    if(!QDesktopServices::openUrl(QUrl(QFileInfo(m_musicFilePathList[currentRow()]).absolutePath()
                              , QUrl::TolerantMode)))
    QMessageBox::about(0,tr("QMusicPlayer"),
                         tr("The origin one does not exsit?"));
}

void MusicMyDownloadRecordWidget::musicPlay()
{
    if(rowCount() == 0 || currentRow() <0)
        return;
    emit musicPlay(QStringList(m_musicFilePathList[currentRow()]));
}
