#include "musiclocalsongsmanagerwidget.h"
#include "ui_musiclocalsongsmanagerwidget.h"
#include "musiclocalsongsmanagerthread.h"
#include "musiclocalsongstablewidget.h"
#include "musicsongssummariziedwidget.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicsongtag.h"
#include "musicotherdefine.h"
#include "musicsinglemanager.h"

#ifdef TTK_GREATER_NEW
#  include <QtConcurrent/QtConcurrent>
#else
#  include <QtConcurrentRun>
#endif
#include <QButtonGroup>
#include <QStyledItemDelegate>

MusicLocalSongsManagerWidget::MusicLocalSongsManagerWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicLocalSongsManagerWidget)
{
    Q_UNUSED(qRegisterMetaType<QFileInfoList>("QFileInfoList"));
    m_ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, true);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->toolWidget->setStyleSheet(QString("#toolWidget{%1}").arg(MusicUIObject::MBackgroundStyle07));

    m_ui->allSelectedcheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle03);
    m_ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->allSelectedcheckBox->setText(tr("allselected"));
    connect(m_ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    m_ui->auditionButton->setStyleSheet(MusicUIObject::MToolButtonStyle01 + MusicUIObject::MToolButtonStyle02 + "QToolButton{ image:url(:/contextMenu/btn_audition); }");
    m_ui->addButton->setStyleSheet(MusicUIObject::MToolButtonStyle01 + MusicUIObject::MToolButtonStyle02 + "QToolButton{ image:url(:/contextMenu/btn_add); }");

    m_ui->auditionButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->addButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->scanButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->scanCustButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->searchLineLabel->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->loadingLabel->setType(MusicGifLabelWidget::Gif_Cicle_Blue);

    m_ui->scanButton->setIcon(QIcon(":/toolSets/btn_search"));
    m_ui->scanButton->setStyleSheet(MusicUIObject::MPushButtonStyle10);
    m_ui->scanCustButton->setIcon(QIcon(":/toolSets/btn_search"));
    m_ui->scanCustButton->setStyleSheet(MusicUIObject::MPushButtonStyle10);

    m_ui->filterComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->filterComboBox));
    m_ui->filterComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->filterComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    m_ui->showlistButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->showlistButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->showArtButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->showArtButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->showAlbumButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->showAlbumButton->setCursor(QCursor(Qt::PointingHandCursor));

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_ui->scanButton, 0);
    buttonGroup->addButton(m_ui->scanCustButton, 1);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(filterScanChanged(int)));

    connect(m_ui->auditionButton, SIGNAL(clicked()), SLOT(auditionButtonClick()));
    connect(m_ui->addButton, SIGNAL(clicked()), SLOT(addButtonClick()));
    connect(m_ui->searchLineEdit, SIGNAL(cursorPositionChanged(int,int)), SLOT(musicSearchIndexChanged(int,int)));

    connect(m_ui->songlistsTable, SIGNAL(cellClicked(int,int)), SLOT(itemCellOnClick(int,int)));
    connect(m_ui->songlistsTable, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
    connect(m_ui->songInfoTable, SIGNAL(updateFileLists(QFileInfoList)), SLOT(updateFileLists(QFileInfoList)));

    connect(m_ui->showlistButton, SIGNAL(clicked()), SLOT(setShowlistButton()));
    connect(m_ui->showArtButton, SIGNAL(clicked()), SLOT(setShowArtButton()));
    connect(m_ui->showAlbumButton, SIGNAL(clicked()), SLOT(setShowAlbumButton()));

#ifdef Q_OS_UNIX
    m_ui->scanButton->setFocusPolicy(Qt::NoFocus);
    m_ui->scanCustButton->setFocusPolicy(Qt::NoFocus);
    m_ui->showlistButton->setFocusPolicy(Qt::NoFocus);
    m_ui->showArtButton->setFocusPolicy(Qt::NoFocus);
    m_ui->showAlbumButton->setFocusPolicy(Qt::NoFocus);
    m_ui->allSelectedcheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->searchLineLabel->setFocusPolicy(Qt::NoFocus);
#endif

    m_runTypeChanged = false;
    addDrivesList();
    m_ui->filterComboBox->setCurrentIndex(-1);

    m_thread = new MusicLocalSongsManagerThread(this);
    connect(m_thread, SIGNAL(setSongNamePath(QFileInfoList)), SLOT(setSongNamePath(QFileInfoList)));

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummariziedWidget::getClassName());
}

MusicLocalSongsManagerWidget::~MusicLocalSongsManagerWidget()
{
    m_runTypeChanged = false;
    M_CONNECTION_PTR->removeValue(getClassName());
    M_SINGLE_MANAGER_PTR->removeObject(getClassName());
    clearAllItems();
    m_thread->stopAndQuitThread();
    delete m_thread;
    delete m_ui;
}

void MusicLocalSongsManagerWidget::findExtraDevicePath(const QString &dir)
{
    MusicUtils::Widget::setComboBoxText(m_ui->filterComboBox, dir);
    m_thread->setFindFilePath(dir);
    filterScanChanged(DEFAULT_LEVEL_LOWER);
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

void MusicLocalSongsManagerWidget::itemCellOnClick(int row, int col)
{
    switch(col)
    {
        case 3:
        case 4:
           {
              if(!m_searchfileListCache.isEmpty())
              {
                  const int count = m_ui->searchLineEdit->text().trimmed().count();
                  row = m_searchfileListCache.value(count)[row];
                  m_ui->searchLineEdit->clear();
                  m_searchfileListCache.clear();
              }
              emit addSongToPlay(QStringList(m_fileNames[row].absoluteFilePath()));
              break;
           }
        default: break;
    }
}

void MusicLocalSongsManagerWidget::itemDoubleClicked(int row, int)
{
    if(!m_searchfileListCache.isEmpty())
    {
        const int count = m_ui->searchLineEdit->text().trimmed().count();
        row = m_searchfileListCache.value(count)[row];
        m_ui->searchLineEdit->clear();
        m_searchfileListCache.clear();
    }
    emit addSongToPlay(QStringList(m_fileNames[row].absoluteFilePath()));
}

void MusicLocalSongsManagerWidget::setSongNamePath(const QFileInfoList &name)
{
    M_LOGGER_INFO("stop fetch!");
    loadingLabelState(false);

    m_ui->songlistsTable->setFiles(name);
    setShowlistButton();
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

    loadingLabelState(true);
    m_thread->start();
}

void MusicLocalSongsManagerWidget::musicSearchIndexChanged(int, int index)
{
    MIntList searchResult;
    for(int j=0; j<m_fileNames.count(); ++j)
    {
        if(m_fileNames[j].fileName().contains(m_ui->searchLineEdit->text().trimmed(), Qt::CaseInsensitive))
        {
            searchResult << j;
        }
    }
    m_searchfileListCache.insert(index, searchResult);
    clearAllItems();

    QFileInfoList names;
    foreach(const int index, searchResult)
    {
        names.append(m_fileNames[index]);
    }
    addAllItems(names);
}

void MusicLocalSongsManagerWidget::updateFileLists(const QFileInfoList &list)
{
    m_fileNames = list;
    m_ui->stackedWidget->setCurrentIndex(LOCAL_MANAGER_INDEX_0);
    controlEnable(true);
    addAllItems(m_fileNames);
}

void MusicLocalSongsManagerWidget::setShowlistButton()
{
    m_runTypeChanged = false;
    loadingLabelState(true);
    m_ui->stackedWidget->setCurrentIndex(LOCAL_MANAGER_INDEX_0);
    controlEnable(true);
    addAllItems( m_fileNames = m_ui->songlistsTable->getFiles() );
    loadingLabelState(false);
}

void MusicLocalSongsManagerWidget::setShowArtButton()
{
    m_runTypeChanged = false;
    loadingLabelState(true);
    m_ui->stackedWidget->setCurrentIndex(LOCAL_MANAGER_INDEX_1);
    controlEnable(false);
    m_runTypeChanged = true;

    QtConcurrent::run([&]
    {
        MusicInfoData arts;
        MusicSongTag tag;
        foreach(const QFileInfo &info, m_ui->songlistsTable->getFiles())
        {
            if(!m_runTypeChanged)
            {
                break;
            }

            if(tag.read(info.absoluteFilePath()))
            {
                QString artString = tag.getArtist().trimmed();
                if(artString.isEmpty())
                {
                    artString = "Various Artists";
                }

                if(!arts.contains(artString))
                {
                    arts.insert(artString, QFileInfoList() << info);
                }
                else
                {
                    arts.insert(artString, arts[artString] << info);
                }
            }
        }

        m_ui->songInfoTable->setRowCount(arts.count());
        m_ui->songInfoTable->addItems(arts);

        loadingLabelState(false);
    });
}

void MusicLocalSongsManagerWidget::setShowAlbumButton()
{
    m_runTypeChanged = false;
    loadingLabelState(true);
    m_ui->stackedWidget->setCurrentIndex(LOCAL_MANAGER_INDEX_1);
    controlEnable(false);
    m_runTypeChanged = true;

    QtConcurrent::run([&]
    {
        MusicInfoData albums;
        MusicSongTag tag;
        foreach(const QFileInfo &info, m_ui->songlistsTable->getFiles())
        {
            if(!m_runTypeChanged)
            {
                break;
            }

            if(tag.read(info.absoluteFilePath()))
            {
                QString albumString = tag.getAlbum().trimmed();
                if(albumString.isEmpty())
                {
                    albumString = "Various Album";
                }

                if(!albums.contains(albumString))
                {
                    albums.insert(albumString, QFileInfoList() << info);
                }
                else
                {
                    albums.insert(albumString, albums[albumString] << info);
                }
            }
        }

        m_ui->songInfoTable->setRowCount(albums.count());
        m_ui->songInfoTable->addItems(albums);

        loadingLabelState(false);
    });
}

void MusicLocalSongsManagerWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicLocalSongsManagerWidget::clearAllItems()
{
    ///Remove all the original item
    if(m_ui->allSelectedcheckBox->isChecked())
    {
        m_ui->allSelectedcheckBox->click();
    }

    switch(m_ui->stackedWidget->currentIndex())
    {
        case 0: m_ui->songlistsTable->clear(); break;
        case 1:
        case 2: m_ui->songInfoTable->clear(); break;
        default: break;
    }
}

void MusicLocalSongsManagerWidget::addAllItems(const QFileInfoList &fileName)
{
    m_ui->songlistsTable->setRowCount(fileName.count()); //reset row count
    m_ui->songCountLabel->setText(tr("showSongCount%1").arg(fileName.count()));
    m_ui->songlistsTable->addItems(fileName);
}

void MusicLocalSongsManagerWidget::addDrivesList()
{
    QStringList names;
    names << tr("Overall");
    const QFileInfoList &drives = QDir::drives();
    foreach(const QFileInfo &driver, drives)
    {
       names << driver.absoluteDir().absolutePath();
    }
    m_ui->filterComboBox->addItems(names);
}

void MusicLocalSongsManagerWidget::itemsSelected()
{
    MIntSet auditionRow; //if selected multi rows
    foreach(QTableWidgetItem *item, m_ui->songlistsTable->selectedItems())
    {
        if(!m_searchfileListCache.isEmpty())
        {
            const int count = m_ui->searchLineEdit->text().trimmed().count();
            auditionRow.insert(m_searchfileListCache.value(count)[item->row()]);
        }
        else
        {
            auditionRow.insert(item->row());
        }
    }
    m_ui->searchLineEdit->clear();
    m_searchfileListCache.clear();

    MIntList auditionList = auditionRow.toList();
    qSort(auditionList);

    QStringList names;
    foreach(const int index, auditionList)
    {
        names << m_fileNames[index].absoluteFilePath();
    }

    emit addSongToPlay(names);
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
        const QFileInfoList &drives = QDir::drives();
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

void MusicLocalSongsManagerWidget::controlEnable(bool state)
{
    clearAllItems();
    m_ui->searchLineEdit->clear();
    m_searchfileListCache.clear();

    m_ui->allSelectedcheckBox->setEnabled(state);
    m_ui->auditionButton->setEnabled(state);
    m_ui->addButton->setEnabled(state);
    m_ui->searchLineEdit->setEnabled(state);
    m_ui->searchLineLabel->setEnabled(state);
}

void MusicLocalSongsManagerWidget::loadingLabelState(bool state)
{
    m_ui->loadingLabel->run(state);
}
