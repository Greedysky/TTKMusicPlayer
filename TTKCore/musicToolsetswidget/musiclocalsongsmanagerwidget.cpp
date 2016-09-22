#include "musiclocalsongsmanagerwidget.h"
#include "ui_musiclocalsongsmanagerwidget.h"
#include "musiclocalsongsmanagerthread.h"
#include "musiclocalsongstablewidget.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicwidgetutils.h"
#include "musicnumberutils.h"
#include "musicsongssummarizied.h"

#include <QDateTime>
#include <QFileDialog>
#include <QButtonGroup>
#include <QStyledItemDelegate>

MusicLocalSongsManagerWidget::MusicLocalSongsManagerWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicLocalSongsManagerWidget)
{
    Q_UNUSED(qRegisterMetaType<QFileInfoList>("QFileInfoList"));
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->toolWidget->setStyleSheet(QString("#toolWidget{%1}").arg(MusicUIObject::MBackgroundStyle07));

    ui->allSelectedcheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle03);
    ui->auditionButton->setStyleSheet(MusicUIObject::MToolButtonStyle01 + MusicUIObject::MToolButtonStyle02 +
                                      "QToolButton{ image:url(:/contextMenu/btn_audition);}");
    ui->addButton->setStyleSheet(MusicUIObject::MToolButtonStyle01 + MusicUIObject::MToolButtonStyle02 +
                                 "QToolButton{ image:url(:/contextMenu/btn_add);}");

    ui->auditionButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->addButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    ui->scanButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->scanCustButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->searchLineLabel->setCursor(QCursor(Qt::PointingHandCursor));

    ui->loadingLabel->setType(MusicGifLabelWidget::Gif_Cicle_Blue);

    ui->allSelectedcheckBox->setText(tr("allselected"));
    connect(ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    ui->scanButton->setIcon(QIcon(":/toolSets/btn_search"));
    ui->scanButton->setStyleSheet(MusicUIObject::MPushButtonStyle10);
    ui->scanCustButton->setIcon(QIcon(":/toolSets/btn_search"));
    ui->scanCustButton->setStyleSheet(MusicUIObject::MPushButtonStyle10);

    ui->showlistButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->showlistButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->filterComboBox->setItemDelegate(new QStyledItemDelegate(ui->filterComboBox));
    ui->filterComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->filterComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    ui->showPathButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->showPathButton->setCursor(QCursor(Qt::PointingHandCursor));

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->scanButton, 0);
    buttonGroup->addButton(ui->scanCustButton, 1);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(filterScanChanged(int)));

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

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummarizied::getClassName());
}

MusicLocalSongsManagerWidget::~MusicLocalSongsManagerWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName() );
    clearAllItems();
    m_thread->stopAndQuitThread();
    delete m_thread;
    delete ui;
}

QString MusicLocalSongsManagerWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLocalSongsManagerWidget::clearAllItems()
{
    ///Remove all the original item
    if(ui->allSelectedcheckBox->isChecked())
    {
        ui->allSelectedcheckBox->click();
    }
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
            item->setText(MusicUtils::Widget::elidedText(font(), var, Qt::ElideRight, 310));
            item->setToolTip(var);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            ui->songlistsTable->setItem(i, 0, item);

                             item = new QTableWidgetItem;
            item->setText(MusicUtils::Widget::elidedText(font(), MusicUtils::Number::size2Number(fileName[i].size()),
                                                         Qt::ElideRight, 50));
            item->setToolTip(MusicUtils::Number::size2Label(fileName[i].size()));
            item->setTextAlignment(Qt::AlignCenter);
            ui->songlistsTable->setItem(i, 1, item);

                             item = new QTableWidgetItem(fileName[i].lastModified().date().toString(Qt::ISODate));
            item->setTextAlignment(Qt::AlignCenter);
            ui->songlistsTable->setItem(i, 2, item);

                             item = new QTableWidgetItem;
            item->setIcon(QIcon(":/contextMenu/btn_audition"));
            ui->songlistsTable->setItem(i, 3, item);

                             item = new QTableWidgetItem;
            item->setIcon(QIcon(":/contextMenu/btn_add"));
            ui->songlistsTable->setItem(i, 4, item);
        }
        else
        {
            var = fileName[i].absoluteFilePath();
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(MusicUtils::Widget::elidedText(font(), var, Qt::ElideRight, 525));
            item->setToolTip(var);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            ui->songlistsTable->setItem(i, 0, item);
        }
    }
}

void MusicLocalSongsManagerWidget::addDrivesList()
{
    QStringList names;
    names << tr("Overall");
    QFileInfoList drives = QDir::drives();
    foreach(const QFileInfo &driver, drives)
    {
       names << driver.absoluteDir().absolutePath();
    }
    ui->filterComboBox->addItems(names);
}

void MusicLocalSongsManagerWidget::filterScanChanged(int index)
{
    M_LOGGER_INFO("start fetch!");
    m_thread->stopAndQuitThread();

    if(index == 0)
    {
        if(!filterIndexChanged())
        {
            return;
        }
    }
    else if(index == 1)
    {
        if(!filterIndexCustChanged())
        {
            return;
        }
    }

    ui->loadingLabel->show();
    ui->loadingLabel->start();
    m_thread->start();
}

bool MusicLocalSongsManagerWidget::filterIndexCustChanged()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec())
    {
        m_thread->setFindFilePath(dialog.directory().absolutePath());
    }
    else
    {
        return false;
    }
    return true;
}

bool MusicLocalSongsManagerWidget::filterIndexChanged()
{
    if(ui->filterComboBox->currentIndex() < 0)
    {
        MusicMessageBox message;
        message.setText(tr("please select one dirver"));
        message.exec();
        return false;
    }

    if(ui->filterComboBox->currentIndex() == 0)
    {
        QStringList names;
        QFileInfoList drives = QDir::drives();
        foreach(const QFileInfo &driver, drives)
        {
           names << driver.absoluteDir().absolutePath();
        }
        m_thread->setFindFilePath(names);
    }
    else
    {
        m_thread->setFindFilePath(ui->filterComboBox->currentText());
    }

    return true;
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
    M_LOGGER_INFO("stop fetch!");
    ui->loadingLabel->hide();
    ui->loadingLabel->stop();

    clearAllItems();
    addAllItems( m_filenames = name );
}

void MusicLocalSongsManagerWidget::itemsSelected()
{
    MusicObject::MIntSet auditionRow; //if selected multi rows
    foreach(QTableWidgetItem *item, ui->songlistsTable->selectedItems())
    {
        if(!m_searchfileListCache.isEmpty())
        {
            int count = ui->searchLineEdit->text().trimmed().count();
            auditionRow.insert(m_searchfileListCache.value(count)[item->row()]);
        }
        else
        {
            auditionRow.insert(item->row());
        }
    }
    ui->searchLineEdit->clear();
    m_searchfileListCache.clear();

    MusicObject::MIntList auditionList = auditionRow.toList();
    qSort(auditionList);

    QStringList names;
    foreach(int index, auditionList)
    {
        names << m_filenames[index].absoluteFilePath();
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
    if(ui->songlistsTable->rowCount() <= 0 || ui->songlistsTable->currentRow() < 0 )
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
    if(ui->songlistsTable->rowCount() <= 0 || ui->songlistsTable->currentRow() < 0 )
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
    MusicObject::MIntList searchResult;
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
    foreach(int index, searchResult)
    {
        names.append(m_filenames[index]);
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
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
