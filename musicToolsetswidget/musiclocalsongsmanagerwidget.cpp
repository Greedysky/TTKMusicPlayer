#include "musiclocalsongsmanagerwidget.h"
#include "ui_musiclocalsongsmanagerwidget.h"
#include "musiclocalsongsmanagerthread.h"
#include "musiclocalsongstablewidget.h"
#include "musicbgthememanager.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musictime.h"

#include <QDateTime>
#include <QMovie>
#include <QFileDialog>

MusicLocalSongsManagerWidget::MusicLocalSongsManagerWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicLocalSongsManagerWidget), m_movie(nullptr)
{
    Q_UNUSED(qRegisterMetaType<QFileInfoList>("QFileInfoList"));
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->toolWidget->setStyleSheet("#toolWidget{" + MusicUIObject::MCustomStyle27 + "}");

    ui->allSelectedcheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle02);
    ui->auditionButton->setStyleSheet(MusicUIObject::MToolButtonStyle07);
    ui->addButton->setStyleSheet(MusicUIObject::MToolButtonStyle08);

    ui->auditionButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->addButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    ui->scanButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->searchLineLabel->setCursor(QCursor(Qt::PointingHandCursor));

    ui->allSelectedcheckBox->setText(tr("allselected"));
    connect(ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    ui->scanButton->setIcon(QIcon(":/image/searchlocalfile"));
    ui->scanButton->setStyleSheet("QPushButton{ background:transparent;}"
                                  "QPushButton::hover{ border:1px solid #000000;}");

    ui->showlistButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->showlistButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->filterComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->filterComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    ui->showPathButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->showPathButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui->scanButton, SIGNAL(clicked()), SLOT(filterIndexChanged()));
    connect(ui->auditionButton, SIGNAL(clicked()), SLOT(auditionButtonClick()));
    connect(ui->addButton, SIGNAL(clicked()), SLOT(addButtonClick()));
    connect(ui->songlistsTable, SIGNAL(cellClicked(int,int)), SLOT(itemCellOnClick(int,int)));
    connect(ui->songlistsTable, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
    connect(ui->searchLineEdit, SIGNAL(cursorPositionChanged(int,int)), SLOT(musicSearchIndexChanged(int,int)));
    connect(ui->showlistButton, SIGNAL(clicked()), SLOT(setShowlistButton()));
    connect(ui->showPathButton, SIGNAL(clicked()), SLOT(setShowPathButton()));

    addDrivesList();
    ui->filterComboBox->setCurrentIndex(-1);

    m_currentIndex = -1;
    setShowlistButton();

    m_thread = new MusicLocalSongsManagerThread(this);
    connect(m_thread, SIGNAL(setSongNamePath(QFileInfoList)),
                      SLOT(setSongNamePath(QFileInfoList)));

    M_CONNECTION->setValue("MusicLocalSongsManagerWidget", this);
    M_CONNECTION->connect("MusicLocalSongsManagerWidget", "MusicApplication");
}

MusicLocalSongsManagerWidget::~MusicLocalSongsManagerWidget()
{
    M_CONNECTION->disConnect("MusicLocalSongsManagerWidget");
    delete m_movie;
    clearAllItems();
    m_thread->stopAndQuitThread();
    delete m_thread;
    delete ui;
}

void MusicLocalSongsManagerWidget::clearAllItems()
{
    //Remove all the original item
    m_currentIndex == 0 ? ui->songlistsTable->clearShowlist() : ui->songlistsTable->clearShowPath();
}

void MusicLocalSongsManagerWidget::addAllItems(const QFileInfoList &fileName)
{
    ui->songlistsTable->setRowCount(fileName.count());//reset row count
    ui->songCountLabel->setText(tr("showSongCount%1").arg(fileName.count()));

    QString var;
    for(int i=0; i<fileName.count(); i++)
    {
        if(m_currentIndex == 0)
        {
            var = fileName[i].fileName();
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(QFontMetrics(font()).elidedText(var, Qt::ElideRight, 285));
            item->setToolTip(var);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            ui->songlistsTable->setItem(i, 0, item);

                              item = new QTableWidgetItem;
            item->setText(QFontMetrics(font()).elidedText(MusicTime::fileSize2Label(fileName[i].size()),
                                                          Qt::ElideRight, 50));
            item->setToolTip(MusicTime::fileSize2Normal(fileName[i].size()));
            item->setTextAlignment(Qt::AlignCenter);
            ui->songlistsTable->setItem(i, 1, item);

                              item = new QTableWidgetItem(fileName[i].lastModified().date().toString(Qt::ISODate));
            item->setTextAlignment(Qt::AlignCenter);
            ui->songlistsTable->setItem(i, 2, item);

                             item = new QTableWidgetItem;
            item->setIcon(QIcon(QString::fromUtf8(":/share/autionplay")));
            ui->songlistsTable->setItem(i, 3, item);

                             item = new QTableWidgetItem;
            item->setIcon(QIcon(QString::fromUtf8(":/image/addtoplaylist")));
            ui->songlistsTable->setItem(i, 4, item);
        }
        else
        {
            var = fileName[i].absoluteFilePath();
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(QFontMetrics(font()).elidedText(var, Qt::ElideRight, 500));
            item->setToolTip(var);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            ui->songlistsTable->setItem(i, 0, item);
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
    {
        return;
    }

    M_LOGGER << "start fetch!" << LOG_END;
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
        {
            return;
        }
    }
    else
    {
        m_thread->setFindFilePath(ui->filterComboBox->currentText());
    }

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

void MusicLocalSongsManagerWidget::setSongNamePath(const QFileInfoList &name)
{
    M_LOGGER << "stop fetch!" << LOG_END;
    ui->loadingLabel->hide();
    delete m_movie;
    m_movie = nullptr;
    clearAllItems();
    addAllItems( m_filenames = name );
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
    {
        names<<m_filenames[auditionList[i]].absoluteFilePath();
    }

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
        MusicMessageBox message;
        message.setText(tr("please select one item"));
        message.exec();
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
        MusicMessageBox message;
        message.setText(tr("please select one item"));
        message.exec();
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
           {
              if(!m_searchfileListCache.isEmpty())
              {
                  int count = ui->searchLineEdit->text().trimmed().count();
                  row = m_searchfileListCache.value(count)[row];
                  ui->searchLineEdit->clear();
                  m_searchfileListCache.clear();
              }
              emit addSongToPlay(QStringList(m_filenames[row].absoluteFilePath()));
              break;
           }
        default: break;
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
    emit addSongToPlay(QStringList(m_filenames[row].absoluteFilePath()));
}

void MusicLocalSongsManagerWidget::musicSearchIndexChanged(int, int index)
{
    MIntList searchResult;
    for(int j=0; j<m_filenames.count(); ++j)
    {
        if(m_filenames[j].fileName().contains(ui->searchLineEdit->text().trimmed(), Qt::CaseInsensitive))
        {
            searchResult << j;
        }
    }
    m_searchfileListCache.insert(index, searchResult);
    clearAllItems();

    QFileInfoList names;
    for(int i=0; i<searchResult.count(); ++i)
    {
        names.append(m_filenames[searchResult[i]]);
    }
    addAllItems(names);
}

void MusicLocalSongsManagerWidget::setShowlistButton()
{
    clearAllItems();
    ui->searchLineEdit->clear();
    m_searchfileListCache.clear();
    ui->songlistsTable->createShowlist();
    m_currentIndex = 0;
    addAllItems( m_filenames );
}

void MusicLocalSongsManagerWidget::setShowPathButton()
{
    clearAllItems();
    ui->searchLineEdit->clear();
    m_searchfileListCache.clear();
    ui->songlistsTable->createShowPath();
    m_currentIndex = 1;
    addAllItems( m_filenames );
}

int MusicLocalSongsManagerWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
