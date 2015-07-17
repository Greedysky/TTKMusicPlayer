#include "musiclocalsongsmanagerwidget.h"
#include "ui_musiclocalsongsmanagerwidget.h"
#include "../core/musiclocalsongsmanagerthread.h"
#include <QDateTime>
#include <QMessageBox>
#include <QMovie>
#include <QFileDialog>

MusicLocalSongsManagerWidget::MusicLocalSongsManagerWidget(QWidget *parent)
        : MusicMoveDialogAbstract(parent),m_movie(NULL),
        ui(new Ui::MusicLocalSongsManagerWidget)
{
    ui->setupUi(this);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),4,4);
    setMask(bmp);
    //set window radius

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    ui->toolWidget->setStyleSheet("#toolWidget{background:rgba(0,0,0,35)}");

    ui->allSelectedcheckBox->setStyleSheet("\
                            QCheckBox{ background-color:transparent; } \
                            QCheckBox::indicator::unchecked{ image: url(:/control/checkbox_unchecked); } \
                            QCheckBox::checked { color: rgb(0,169,236); } \
                            QCheckBox::indicator:checked { image: url(:/control/checkbox_checked); }");
    ui->auditionButton->setStyleSheet("QToolButton{background:transparent;image: url(:/share/autionplay);}"
                                      "QToolButton::hover{ border:1px solid #000000;}");
    ui->addButton->setStyleSheet("QToolButton{background:transparent;image: url(:/image/addtoplaylist);}"
                                 "QToolButton::hover{ border:1px solid #000000;}");

    ui->auditionButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->addButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    ui->scanButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->searchLineLabel->setCursor(QCursor(Qt::PointingHandCursor));

    ui->allSelectedcheckBox->setText(tr("allselected"));
    connect(ui->allSelectedcheckBox,SIGNAL(clicked(bool)),SLOT(selectedAllItems(bool)));

    ui->scanButton->setIcon(QIcon(":/image/searchlocalfile"));
    ui->scanButton->setStyleSheet("QPushButton{ background:transparent;}"
                                  "QPushButton::hover{ border:1px solid #000000;}");


    ui->showlistButton->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->showlistButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->filterComboBox->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);

    ui->showPathButton->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->showPathButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui->scanButton,SIGNAL(clicked()),SLOT(filterIndexChanged()));
    connect(ui->auditionButton,SIGNAL(clicked()),SLOT(auditionButtonClick()));
    connect(ui->addButton,SIGNAL(clicked()),SLOT(addButtonClick()));
    connect(ui->songlistsTable,SIGNAL(cellClicked(int,int)),SLOT(itemCellOnClick(int,int)));
    connect(ui->songlistsTable,SIGNAL(cellDoubleClicked(int,int)),SLOT(itemDoubleClicked(int,int)));
    connect(ui->songlistsTable,SIGNAL(cellEntered(int,int)),ui->songlistsTable,SLOT(listCellEntered(int,int)));
    connect(ui->searchLineEdit,SIGNAL(cursorPositionChanged(int,int)),SLOT(musicSearchIndexChanged(int,int)));
    connect(ui->showlistButton,SIGNAL(clicked()),SLOT(setShowlistButton()));
    connect(ui->showPathButton,SIGNAL(clicked()),SLOT(setShowPathButton()));

    addDrivesList();
    ui->filterComboBox->setCurrentIndex(-1);

    m_currentIndex = -1;
    setShowlistButton();

    m_thread = new MusicLocalSongsManagerThread(this);
    connect(m_thread,SIGNAL(setSongNamePath(QStringList,QStringList)),
            SLOT(setSongNamePath(QStringList,QStringList)));

}

MusicLocalSongsManagerWidget::~MusicLocalSongsManagerWidget()
{
    delete m_movie;
    clearAllItems();
    m_thread->stopAndQuitThread();
    delete m_thread;
    delete ui;
}

void MusicLocalSongsManagerWidget::clearAllItems()
{
    //Remove all the original item
    if(m_currentIndex == 0)
    {
        ui->songlistsTable->clearContents();
        ui->songlistsTable->setColumnCount(5);
        ui->songlistsTable->setRowCount(0);
    }
    else
    {
        ui->songlistsTable->clearContents();
        ui->songlistsTable->setColumnCount(1);
        ui->songlistsTable->setRowCount(0);
    }
}

void MusicLocalSongsManagerWidget::addAllItems(const QStringList& filename,
                                               const QStringList& fileDir)
{
    ui->songlistsTable->setRowCount(filename.count());//reset row count
    ui->songCountLabel->setText(tr("showSongCount%1").arg(filename.count()));

    for(int i=0; i<filename.count(); i++)
    {
        if(m_currentIndex == 0)
        {
            QTableWidgetItem *item = new QTableWidgetItem(filename[i]);
            item->setTextColor(QColor(50,50,50));
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            ui->songlistsTable->setItem(i,0,item);

            QFileInfo info(fileDir[i]);
            QString time;
            int k = info.size();
            if( k< 1024) time = QString("%1%2").arg(k).arg('B');
            else if( 1024 <= k && k < 1024*1024 ) time = QString("%1%2").arg(k /= 1024).arg('K');
            else time = QString("%1%2").arg(k /= (1024*1024)).arg('M');

            QTableWidgetItem *item2 = new QTableWidgetItem(time);
            item2->setTextColor(QColor(50,50,50));
            item2->setTextAlignment(Qt::AlignCenter);
            ui->songlistsTable->setItem(i,1,item2);

            QTableWidgetItem *item3 = new QTableWidgetItem(info.lastModified().date().toString(Qt::ISODate));
            item3->setTextColor(QColor(50,50,50));
            item3->setTextAlignment(Qt::AlignCenter);
            ui->songlistsTable->setItem(i,2,item3);

            QTableWidgetItem *item4 = new QTableWidgetItem(QIcon(QString::fromUtf8(":/share/autionplay")),"");
            item4->setTextAlignment(Qt::AlignCenter);
            ui->songlistsTable->setItem(i,3,item4);

            QTableWidgetItem *item5 = new QTableWidgetItem(QIcon(QString::fromUtf8(":/image/addtoplaylist")),"");
            item5->setTextAlignment(Qt::AlignCenter);
            ui->songlistsTable->setItem(i,4,item5);
        }
        else
        {
            QTableWidgetItem *item = new QTableWidgetItem(fileDir[i]);
            item->setTextColor(QColor(50,50,50));
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            ui->songlistsTable->setItem(i,0,item);
        }
    }
}

void MusicLocalSongsManagerWidget::addDrivesList()
{
    QStringList names;
    names<<tr("Overall")<<tr("CustmorDir");
    QFileInfoList drives = QDir::drives();
    for(int i=0; i<drives.count(); ++i)
    {
       names<<drives[i].absoluteDir().absolutePath();
    }
    ui->filterComboBox->addItems(names);
}

void MusicLocalSongsManagerWidget::filterIndexChanged()
{
    if(ui->filterComboBox->currentIndex() < 0)
        return;

    qDebug()<<"start fetch!";
    m_thread->stopAndQuitThread();

    if(ui->filterComboBox->currentIndex() == 0)
    {
        QStringList names;
        QFileInfoList drives = QDir::drives();
        for(int i=0; i<drives.count(); ++i)
        {
           names<<drives[i].absoluteDir().absolutePath();
        }
        m_thread->setFindFilePath(names);
    }
    else if(ui->filterComboBox->currentIndex() == 1)
    {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::Directory );
        dialog.setViewMode(QFileDialog::Detail);
        if(dialog.exec())
        {
            m_thread->setFindFilePath(dialog.directory().absolutePath());
        }
        else
            return;
    }
    else
        m_thread->setFindFilePath(ui->filterComboBox->currentText());

    ui->loadingLabel->show();
    ui->loadingLabel->setMovie(m_movie = new QMovie(":/image/loading",QByteArray(),this));
    m_movie->start();
    m_thread->start();
}

void MusicLocalSongsManagerWidget::selectedAllItems(bool check)
{
    if(!check)
    {
        ui->allSelectedcheckBox->setText(tr("allselected"));
        ui->songlistsTable->clearSelection();
        ui->songlistsTable->setCurrentIndex(QModelIndex());
    }
    else
    {
        ui->allSelectedcheckBox->setText(tr("allcanceled"));
        ui->songlistsTable->selectAll();
    }
}

void MusicLocalSongsManagerWidget::setSongNamePath(const QStringList& name, const QStringList& dir)
{
    qDebug()<<"stop fetch!";
    ui->loadingLabel->hide();
    delete m_movie;
    m_movie = NULL;
    m_filename = name;
    m_fileDir = dir;
    clearAllItems();
    addLoaclSongList();
}

void MusicLocalSongsManagerWidget::addLoaclSongList()
{
    QStringList filename;
    QStringList fileDir;
    for(int i=0; i<m_filename.count(); ++i)
    {
        QFileInfo fileInfo(m_fileDir[i] + m_filename[i]);
        if(fileInfo.suffix() == "mp3")
        {
            fileDir<<m_fileDir[i] + '/' + m_filename[i];
            filename<<m_filename[i].remove(".mp3");
        }
    }

    addAllItems(filename.isEmpty() ? m_filename : m_filename = filename,
                fileDir.isEmpty() ? m_fileDir : m_fileDir = fileDir );
}

void MusicLocalSongsManagerWidget::itemsSelected()
{
    MIntSet auditionRow; //if selected multi rows
    for(int i=0; i<ui->songlistsTable->selectedItems().count(); ++i)
    {
        if(!m_searchfileListCache.isEmpty())
        {
            int count = ui->searchLineEdit->text().trimmed().count();
            auditionRow.insert(m_searchfileListCache.value(count)
                              [ui->songlistsTable->selectedItems()[i]->row()]);
        }
        else
            auditionRow.insert(ui->songlistsTable->selectedItems()[i]->row());
    }
    ui->searchLineEdit->clear();
    m_searchfileListCache.clear();

    MIntList auditionList = auditionRow.toList();
    qSort(auditionList);
    QStringList names;
    for(int i=0; i<auditionList.count(); ++i)
        names<<m_fileDir[auditionList[i]];

    emit addSongToPlay(names);
}

void MusicLocalSongsManagerWidget::auditionButtonClick()
{
    if(ui->songlistsTable->selectedItems().count() > 0)
    {
        itemsSelected();
        return;
    }
    if(ui->songlistsTable->rowCount() <= 0 ||
       ui->songlistsTable->currentRow() < 0 )
    {
        QMessageBox::about(this,tr("QMusicPlayer"),tr("please select one item"));
        return;
    }

    itemsSelected();
}

void MusicLocalSongsManagerWidget::addButtonClick()
{
    if(ui->songlistsTable->selectedItems().count() > 0)
    {
        itemsSelected();
        return;
    }
    if(ui->songlistsTable->rowCount() <= 0 ||
       ui->songlistsTable->currentRow() < 0 )
    {
        QMessageBox::about(this,tr("QMusicPlayer"),tr("please select one item"));
        return;
    }

    itemsSelected();
}

void MusicLocalSongsManagerWidget::itemCellOnClick(int row,int col)
{
    switch(col)
    {
      case 3:
      case 4:
        if(!m_searchfileListCache.isEmpty())
        {
            int count = ui->searchLineEdit->text().trimmed().count();
            row = m_searchfileListCache.value(count)[row];
            ui->searchLineEdit->clear();
            m_searchfileListCache.clear();
        }
        emit addSongToPlay(QStringList(m_fileDir[row]));break;
      default:break;
    }
}

void MusicLocalSongsManagerWidget::itemDoubleClicked(int row, int)
{
    if(!m_searchfileListCache.isEmpty())
    {
        int count = ui->searchLineEdit->text().trimmed().count();
        row = m_searchfileListCache.value(count)[row];
        ui->searchLineEdit->clear();
        m_searchfileListCache.clear();
    }
    emit addSongToPlay(QStringList(m_fileDir[row]));
}

void MusicLocalSongsManagerWidget::musicSearchIndexChanged(int,int index)
{
    MIntList searchResult;
    for(int j=0; j<m_filename.count(); ++j)
      if(m_filename[j].contains(ui->searchLineEdit->text().trimmed(),Qt::CaseInsensitive))
      {
        searchResult<<j;
      }
    m_searchfileListCache.insert(index,searchResult);
    clearAllItems();

    QStringList name;
    QStringList dir;
    for(int i=0; i<searchResult.count(); ++i)
    {
        name.append(m_filename[searchResult[i]]);
        dir.append(m_fileDir[searchResult[i]]);
    }
    addAllItems(name, dir);
}

void MusicLocalSongsManagerWidget::setShowlistButton()
{
    clearAllItems();
    ui->searchLineEdit->clear();
    m_searchfileListCache.clear();
    ui->songlistsTable->setColumnCount(5);
    ui->songlistsTable->setRowCount(0);
    QHeaderView *headerview = ui->songlistsTable->horizontalHeader();
    headerview->resizeSection(0,285);
    headerview->resizeSection(1,55);
    headerview->resizeSection(2,105);
    headerview->resizeSection(3,26);
    headerview->resizeSection(4,26);
    m_currentIndex = 0;
    addLoaclSongList();
}

void MusicLocalSongsManagerWidget::setShowPathButton()
{
    clearAllItems();
    ui->searchLineEdit->clear();
    m_searchfileListCache.clear();
    ui->songlistsTable->setColumnCount(1);
    ui->songlistsTable->setRowCount(0);
    QHeaderView *headerview = ui->songlistsTable->horizontalHeader();
    headerview->resizeSection(0,492);
    m_currentIndex = 1;
    addLoaclSongList();
}
