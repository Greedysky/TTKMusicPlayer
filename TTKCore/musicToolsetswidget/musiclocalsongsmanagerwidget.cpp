#include "musiclocalsongsmanagerwidget.h"
#include "ui_musiclocalsongsmanagerwidget.h"
#include "musiclocalsongsmanagerthread.h"
#include "musiclocalsongstablewidget.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicwidgetutils.h"
#include "musicnumberutils.h"
#include "musicsongssummariziedwidget.h"

#include <QDateTime>
#include <QFileDialog>
#include <QButtonGroup>
#include <QStyledItemDelegate>

MusicLocalSongsManagerWidget::MusicLocalSongsManagerWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLocalSongsManagerWidget)
{
    Q_UNUSED(qRegisterMetaType<QFileInfoList>("QFileInfoList"));
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->toolWidget->setStyleSheet(QString("#toolWidget{%1}").arg(MusicUIObject::MBackgroundStyle07));

    m_ui->allSelectedcheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle03);
    m_ui->auditionButton->setStyleSheet(MusicUIObject::MToolButtonStyle01 + MusicUIObject::MToolButtonStyle02 +
                                      "QToolButton{ image:url(:/contextMenu/btn_audition);}");
    m_ui->addButton->setStyleSheet(MusicUIObject::MToolButtonStyle01 + MusicUIObject::MToolButtonStyle02 +
                                 "QToolButton{ image:url(:/contextMenu/btn_add);}");

    m_ui->auditionButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->addButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->scanButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->scanCustButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->searchLineLabel->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->loadingLabel->setType(MusicGifLabelWidget::Gif_Cicle_Blue);

    m_ui->allSelectedcheckBox->setText(tr("allselected"));
    connect(m_ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    m_ui->scanButton->setIcon(QIcon(":/toolSets/btn_search"));
    m_ui->scanButton->setStyleSheet(MusicUIObject::MPushButtonStyle10);
    m_ui->scanCustButton->setIcon(QIcon(":/toolSets/btn_search"));
    m_ui->scanCustButton->setStyleSheet(MusicUIObject::MPushButtonStyle10);

    m_ui->showlistButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->showlistButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->filterComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->filterComboBox));
    m_ui->filterComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->filterComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    m_ui->showPathButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->showPathButton->setCursor(QCursor(Qt::PointingHandCursor));

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_ui->scanButton, 0);
    buttonGroup->addButton(m_ui->scanCustButton, 1);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(filterScanChanged(int)));

    connect(m_ui->auditionButton, SIGNAL(clicked()), SLOT(auditionButtonClick()));
    connect(m_ui->addButton, SIGNAL(clicked()), SLOT(addButtonClick()));
    connect(m_ui->songlistsTable, SIGNAL(cellClicked(int,int)), SLOT(itemCellOnClick(int,int)));
    connect(m_ui->songlistsTable, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
    connect(m_ui->searchLineEdit, SIGNAL(cursorPositionChanged(int,int)), SLOT(musicSearchIndexChanged(int,int)));
    connect(m_ui->showlistButton, SIGNAL(clicked()), SLOT(setShowlistButton()));
    connect(m_ui->showPathButton, SIGNAL(clicked()), SLOT(setShowPathButton()));

    addDrivesList();
    m_ui->filterComboBox->setCurrentIndex(-1);

    m_currentIndex = -1;
    setShowlistButton();

    m_thread = new MusicLocalSongsManagerThread(this);
    connect(m_thread, SIGNAL(setSongNamePath(QFileInfoList)),
                      SLOT(setSongNamePath(QFileInfoList)));

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummariziedWidget::getClassName());
}

MusicLocalSongsManagerWidget::~MusicLocalSongsManagerWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName() );
    clearAllItems();
    m_thread->stopAndQuitThread();
    delete m_thread;
    delete m_ui;
}

QString MusicLocalSongsManagerWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLocalSongsManagerWidget::clearAllItems()
{
    ///Remove all the original item
    if(m_ui->allSelectedcheckBox->isChecked())
    {
        m_ui->allSelectedcheckBox->click();
    }
    m_currentIndex == 0 ? m_ui->songlistsTable->clearShowlist() : m_ui->songlistsTable->clearShowPath();
}

void MusicLocalSongsManagerWidget::addAllItems(const QFileInfoList &fileName)
{
    m_ui->songlistsTable->setRowCount(fileName.count());//reset row count
    m_ui->songCountLabel->setText(tr("showSongCount%1").arg(fileName.count()));

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
            m_ui->songlistsTable->setItem(i, 0, item);

                             item = new QTableWidgetItem;
            item->setText(MusicUtils::Widget::elidedText(font(), MusicUtils::Number::size2Number(fileName[i].size()),
                                                         Qt::ElideRight, 50));
            item->setToolTip(MusicUtils::Number::size2Label(fileName[i].size()));
            item->setTextAlignment(Qt::AlignCenter);
            m_ui->songlistsTable->setItem(i, 1, item);

                             item = new QTableWidgetItem(fileName[i].lastModified().date().toString(Qt::ISODate));
            item->setTextAlignment(Qt::AlignCenter);
            m_ui->songlistsTable->setItem(i, 2, item);

                             item = new QTableWidgetItem;
            item->setIcon(QIcon(":/contextMenu/btn_audition"));
            m_ui->songlistsTable->setItem(i, 3, item);

                             item = new QTableWidgetItem;
            item->setIcon(QIcon(":/contextMenu/btn_add"));
            m_ui->songlistsTable->setItem(i, 4, item);
        }
        else
        {
            var = fileName[i].absoluteFilePath();
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(MusicUtils::Widget::elidedText(font(), var, Qt::ElideRight, 525));
            item->setToolTip(var);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            m_ui->songlistsTable->setItem(i, 0, item);
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
    m_ui->filterComboBox->addItems(names);
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

    m_ui->loadingLabel->show();
    m_ui->loadingLabel->start();
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
    if(m_ui->filterComboBox->currentIndex() < 0)
    {
        MusicMessageBox message;
        message.setText(tr("please select one dirver"));
        message.exec();
        return false;
    }

    if(m_ui->filterComboBox->currentIndex() == 0)
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
        m_thread->setFindFilePath(m_ui->filterComboBox->currentText());
    }

    return true;
}

void MusicLocalSongsManagerWidget::selectedAllItems(bool check)
{
    if(!check)
    {
        m_ui->allSelectedcheckBox->setText(tr("allselected"));
        m_ui->songlistsTable->clearSelection();
        m_ui->songlistsTable->setCurrentIndex(QModelIndex());
    }
    else
    {
        m_ui->allSelectedcheckBox->setText(tr("allcanceled"));
        m_ui->songlistsTable->selectAll();
    }
}

void MusicLocalSongsManagerWidget::setSongNamePath(const QFileInfoList &name)
{
    M_LOGGER_INFO("stop fetch!");
    m_ui->loadingLabel->hide();
    m_ui->loadingLabel->stop();

    clearAllItems();
    addAllItems( m_filenames = name );
}

void MusicLocalSongsManagerWidget::itemsSelected()
{
    MusicObject::MIntSet auditionRow; //if selected multi rows
    foreach(QTableWidgetItem *item, m_ui->songlistsTable->selectedItems())
    {
        if(!m_searchfileListCache.isEmpty())
        {
            int count = m_ui->searchLineEdit->text().trimmed().count();
            auditionRow.insert(m_searchfileListCache.value(count)[item->row()]);
        }
        else
        {
            auditionRow.insert(item->row());
        }
    }
    m_ui->searchLineEdit->clear();
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
    if(m_ui->songlistsTable->selectedItems().count() > 0)
    {
        itemsSelected();
        return;
    }
    if(m_ui->songlistsTable->rowCount() <= 0 || m_ui->songlistsTable->currentRow() < 0 )
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
    if(m_ui->songlistsTable->selectedItems().count() > 0)
    {
        itemsSelected();
        return;
    }
    if(m_ui->songlistsTable->rowCount() <= 0 || m_ui->songlistsTable->currentRow() < 0 )
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
                  int count = m_ui->searchLineEdit->text().trimmed().count();
                  row = m_searchfileListCache.value(count)[row];
                  m_ui->searchLineEdit->clear();
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
        int count = m_ui->searchLineEdit->text().trimmed().count();
        row = m_searchfileListCache.value(count)[row];
        m_ui->searchLineEdit->clear();
        m_searchfileListCache.clear();
    }
    emit addSongToPlay(QStringList(m_filenames[row].absoluteFilePath()));
}

void MusicLocalSongsManagerWidget::musicSearchIndexChanged(int, int index)
{
    MusicObject::MIntList searchResult;
    for(int j=0; j<m_filenames.count(); ++j)
    {
        if(m_filenames[j].fileName().contains(m_ui->searchLineEdit->text().trimmed(), Qt::CaseInsensitive))
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
    m_ui->searchLineEdit->clear();
    m_searchfileListCache.clear();
    m_ui->songlistsTable->createShowlist();
    m_currentIndex = 0;
    addAllItems( m_filenames );
}

void MusicLocalSongsManagerWidget::setShowPathButton()
{
    clearAllItems();
    m_ui->searchLineEdit->clear();
    m_searchfileListCache.clear();
    m_ui->songlistsTable->createShowPath();
    m_currentIndex = 1;
    addAllItems( m_filenames );
}

int MusicLocalSongsManagerWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
