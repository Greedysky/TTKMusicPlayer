#include "musiclocalsongsmanagerwidget.h"
#include "ui_musiclocalsongsmanagerwidget.h"
#include "musiclocalsongsmanagerthread.h"
#include "musiclocalsongstablewidget.h"
#include "musicsongssummariziedwidget.h"
#include "musictoastlabel.h"
#include "musicconnectionpool.h"
#include "musicsongmeta.h"
#include "musicfileutils.h"

#include <QButtonGroup>

MusicLocalSongsManagerWidget::MusicLocalSongsManagerWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicLocalSongsManagerWidget)
{
    Q_UNUSED(qRegisterMetaType<QFileInfoList>("QFileInfoList"));
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->toolWidget->setStyleSheet(QString("#toolWidget{%1}").arg(MusicUIObject::MQSSBackgroundStyle07));

    m_ui->allSelectedcheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle03);
    m_ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->allSelectedcheckBox->setText(tr("All"));
    connect(m_ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    m_ui->auditionButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle01 + MusicUIObject::MQSSToolButtonStyle02 + "QToolButton{ image:url(:/contextMenu/btn_audition); }");
    m_ui->addButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle01 + MusicUIObject::MQSSToolButtonStyle02 + "QToolButton{ image:url(:/contextMenu/btn_add); }");

    m_ui->auditionButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->addButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->scanButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->scanCustButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->searchLineLabel->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->loadingLabel->setType(MusicGifLabelWidget::CicleBlue);

    m_ui->scanButton->setIcon(QIcon(":/toolSets/btn_search"));
    m_ui->scanButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle08);
    m_ui->scanCustButton->setIcon(QIcon(":/toolSets/btn_search"));
    m_ui->scanCustButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle08);

    MusicUtils::Widget::generateComboBoxFormat(m_ui->filterComboBox);

    m_ui->showlistButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->showlistButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->showArtButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->showArtButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->showAlbumButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->showAlbumButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->toolWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle12);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_ui->scanButton, 0);
    buttonGroup->addButton(m_ui->scanCustButton, 1);
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(buttonGroup, SIGNAL(idClicked(int)), SLOT(filterScanChanged(int)));
#else
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(filterScanChanged(int)));
#endif

    connect(m_ui->auditionButton, SIGNAL(clicked()), SLOT(auditionButtonClick()));
    connect(m_ui->addButton, SIGNAL(clicked()), SLOT(addButtonClick()));
    connect(m_ui->searchLineEdit, SIGNAL(cursorPositionChanged(int,int)), SLOT(musicSearchResultChanged(int,int)));

    connect(m_ui->songlistTable, SIGNAL(cellClicked(int,int)), SLOT(itemCellOnClick(int,int)));
    connect(m_ui->songlistTable, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));
    connect(m_ui->songInfoTable, SIGNAL(updateFileList(QFileInfoList)), SLOT(updateFileList(QFileInfoList)));

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
    m_ui->filterComboBox->setCurrentIndex(DEFAULT_NORMAL_LEVEL);

    m_thread = new MusicLocalSongsManagerThread(this);
    connect(m_thread, SIGNAL(setSongNamePath(QFileInfoList)), SLOT(setSongNamePath(QFileInfoList)));

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->poolConnect(className(), MusicSongsSummariziedWidget::className());
}

MusicLocalSongsManagerWidget::~MusicLocalSongsManagerWidget()
{
    m_runTypeChanged = false;
    G_CONNECTION_PTR->removeValue(className());
    G_SINGLE_MANAGER_PTR->removeObject(className());
    clearAllItems();
    m_thread->stopAndQuitThread();
    delete m_thread;
    delete m_ui;
}

void MusicLocalSongsManagerWidget::findExtraDevicePath(const QString &dir)
{
    MusicUtils::Widget::setComboBoxText(m_ui->filterComboBox, dir);
    m_thread->setFindFilePath(dir);
    filterScanChanged(DEFAULT_NORMAL_LEVEL);
}

void MusicLocalSongsManagerWidget::selectedAllItems(bool check)
{
    if(!check)
    {
        m_ui->songlistTable->clearSelection();
        m_ui->songlistTable->setCurrentIndex(QModelIndex());
    }
    else
    {
        m_ui->songlistTable->selectAll();
    }
}

void MusicLocalSongsManagerWidget::auditionButtonClick()
{
    if(m_ui->songlistTable->selectedItems().count() > 0)
    {
        itemsSelected();
        return;
    }
    if(m_ui->songlistTable->rowCount() <= 0 || m_ui->songlistTable->currentRow() < 0)
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
        return;
    }

    itemsSelected();
}

void MusicLocalSongsManagerWidget::addButtonClick()
{
    if(m_ui->songlistTable->selectedItems().count() > 0)
    {
        itemsSelected();
        return;
    }

    if(m_ui->songlistTable->rowCount() <= 0 || m_ui->songlistTable->currentRow() < 0)
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
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
            if(!m_searchResultCache.isEmpty())
            {
                const int count = m_ui->searchLineEdit->text().trimmed().count();
                row = m_searchResultCache.value(count)[row];
                m_ui->searchLineEdit->clear();
                m_searchResultCache.clear();
            }
            Q_EMIT addSongToPlaylist(QStringList(m_fileNames[row].absoluteFilePath()));
            break;
        }
        default: break;
    }
}

void MusicLocalSongsManagerWidget::itemDoubleClicked(int row, int)
{
    if(!m_searchResultCache.isEmpty())
    {
        const int count = m_ui->searchLineEdit->text().trimmed().count();
        row = m_searchResultCache.value(count)[row];
        m_ui->searchLineEdit->clear();
        m_searchResultCache.clear();
    }
    Q_EMIT addSongToPlaylist(QStringList(m_fileNames[row].absoluteFilePath()));
}

void MusicLocalSongsManagerWidget::setSongNamePath(const QFileInfoList &name)
{
    TTK_LOGGER_INFO("Stop fetch result");
    loadingLabelState(false);

    m_ui->songlistTable->setFiles(name);
    setShowlistButton();
}

void MusicLocalSongsManagerWidget::filterScanChanged(int index)
{
    TTK_LOGGER_INFO("Start fetch result");
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

void MusicLocalSongsManagerWidget::musicSearchResultChanged(int, int index)
{
    TTKIntList result;
    for(int i = 0; i < m_fileNames.count(); ++i)
    {
        if(m_fileNames[i].fileName().contains(m_ui->searchLineEdit->text().trimmed(), Qt::CaseInsensitive))
        {
            result << i;
        }
    }

    m_searchResultCache.insert(index, result);
    clearAllItems();

    QFileInfoList names;
    for(const int index : qAsConst(result))
    {
        names.append(m_fileNames[index]);
    }
    addAllItems(names);
}

void MusicLocalSongsManagerWidget::updateFileList(const QFileInfoList &list)
{
    m_fileNames = list;
    m_ui->stackedWidget->setCurrentIndex(LOCAL_MANAGER_INDEX_0);
    controlEnabled(true);
    addAllItems(m_fileNames);
}

void MusicLocalSongsManagerWidget::setShowlistButton()
{
    m_runTypeChanged = false;
    loadingLabelState(true);
    m_ui->stackedWidget->setCurrentIndex(LOCAL_MANAGER_INDEX_0);
    controlEnabled(true);
    addAllItems(m_fileNames = m_ui->songlistTable->getFiles());
    loadingLabelState(false);
}

void MusicLocalSongsManagerWidget::setShowArtButton()
{
    m_runTypeChanged = false;
    loadingLabelState(true);
    m_ui->stackedWidget->setCurrentIndex(LOCAL_MANAGER_INDEX_1);
    controlEnabled(false);
    m_runTypeChanged = true;

    MusicInfoData arts;
    MusicSongMeta meta;
    for(const QFileInfo &fin : m_ui->songlistTable->getFiles())
    {
        if(!m_runTypeChanged)
        {
            return;
        }

        qApp->processEvents();
        if(meta.read(fin.absoluteFilePath()))
        {
            QString artString = meta.artist().trimmed();
            if(artString.isEmpty())
            {
                artString = "Various Artists";
            }

            if(!arts.contains(artString))
            {
                arts.insert(artString, {fin});
            }
            else
            {
                arts.insert(artString, arts[artString] << fin);
            }
        }
    }

    m_ui->songInfoTable->setRowCount(arts.count());
    m_ui->songInfoTable->addItems(arts);

    loadingLabelState(false);
}

void MusicLocalSongsManagerWidget::setShowAlbumButton()
{
    m_runTypeChanged = false;
    loadingLabelState(true);
    m_ui->stackedWidget->setCurrentIndex(LOCAL_MANAGER_INDEX_1);
    controlEnabled(false);
    m_runTypeChanged = true;

    MusicInfoData albums;
    MusicSongMeta meta;
    for(const QFileInfo &fin : m_ui->songlistTable->getFiles())
    {
        if(!m_runTypeChanged)
        {
            return;
        }

        qApp->processEvents();
        if(meta.read(fin.absoluteFilePath()))
        {
            QString albumString = meta.album().trimmed();
            if(albumString.isEmpty())
            {
                albumString = "Various Album";
            }

            if(!albums.contains(albumString))
            {
                albums.insert(albumString, {fin});
            }
            else
            {
                albums.insert(albumString, albums[albumString] << fin);
            }
        }
    }

    m_ui->songInfoTable->setRowCount(albums.count());
    m_ui->songInfoTable->addItems(albums);

    loadingLabelState(false);
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
        case 0: m_ui->songlistTable->clear(); break;
        case 1:
        case 2: m_ui->songInfoTable->clear(); break;
        default: break;
    }
}

void MusicLocalSongsManagerWidget::addAllItems(const QFileInfoList &fileName)
{
    m_ui->songlistTable->setRowCount(fileName.count()); //reset row count
    m_ui->songCountLabel->setText(tr("showSongCount\n(%1)").arg(fileName.count()));
    m_ui->songlistTable->addItems(fileName);
}

void MusicLocalSongsManagerWidget::addDrivesList()
{
    QStringList names;
    names << tr("Overall");
    const QFileInfoList &drives = QDir::drives();
    for(const QFileInfo &fin : qAsConst(drives))
    {
       names << fin.absoluteDir().absolutePath();
    }
    m_ui->filterComboBox->addItems(names);
}

void MusicLocalSongsManagerWidget::itemsSelected()
{
    TTKIntSet auditionRow; //if selected multi rows
    for(QTableWidgetItem *item : m_ui->songlistTable->selectedItems())
    {
        if(!m_searchResultCache.isEmpty())
        {
            const int count = m_ui->searchLineEdit->text().trimmed().count();
            auditionRow.insert(m_searchResultCache.value(count)[item->row()]);
        }
        else
        {
            auditionRow.insert(item->row());
        }
    }
    m_ui->searchLineEdit->clear();
    m_searchResultCache.clear();

    TTKIntList indexs = auditionRow.values();
    std::sort(indexs.begin(), indexs.end());

    QStringList names;
    for(const int index : qAsConst(indexs))
    {
        names << m_fileNames[index].absoluteFilePath();
    }

    Q_EMIT addSongToPlaylist(names);
}

bool MusicLocalSongsManagerWidget::filterIndexChanged()
{
    if(m_ui->filterComboBox->currentIndex() < 0)
    {
        MusicToastLabel::popup(tr("Please select one dirver"));
        return false;
    }

    if(m_ui->filterComboBox->currentIndex() == 0)
    {
        QStringList names;
        const QFileInfoList &drives = QDir::drives();
        for(const QFileInfo &fin : qAsConst(drives))
        {
           names << fin.absoluteDir().absolutePath();
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
    const QString &path = MusicUtils::File::openDirectoryDialog(this);
    if(!path.isEmpty())
    {
        m_thread->setFindFilePath(path);
    }
    else
    {
        return false;
    }

    return true;
}

void MusicLocalSongsManagerWidget::controlEnabled(bool state)
{
    clearAllItems();
    m_ui->searchLineEdit->clear();
    m_searchResultCache.clear();

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
