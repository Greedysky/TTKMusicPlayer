#include "musicconnecttransferwidget.h"
#include "ui_musicconnecttransferwidget.h"
#include "musicuiobject.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"
#include "musicmessagebox.h"
#include "musicprogresswidget.h"
#include "musicsongssummarizied.h"
#include "musicfilesenderserver.h"

#include <QFile>
#include <QTimer>
#include <QButtonGroup>
#ifdef Q_OS_WIN
#   include <windows.h>
#endif

MusicConnectTransferWidget::MusicConnectTransferWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicConnectTransferWidget)
{
    ui->setupUi(this);
    setStyleSheet(MusicUIObject::MScrollBarStyle01);

    m_currentIndex = -1;
    m_buttonGroup = nullptr;
    m_sendServer = nullptr;

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->allSelectedcheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle03);
    ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    ui->allSelectedcheckBox->setText(tr("allselected"));
    connect(ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    ui->reflashUSBButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->reflashUSBButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->reflashUSBButton, SIGNAL(clicked()), SLOT(reflashRemovableDir()));

    ui->transferUSBButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->transferUSBButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->transferUSBButton, SIGNAL(clicked()), SLOT(startToTransferUSBFiles()));

    ui->transferWIFIButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->transferWIFIButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->transferWIFIButton, SIGNAL(clicked()), SLOT(startToTransferWIFIFiles()));

    ui->switchButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->switchButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->switchButton, SIGNAL(clicked()), SLOT(switchDiffDevice()));

    ui->lineWIFIEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->searchLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle04);
    connect(ui->searchLineEdit, SIGNAL(cursorPositionChanged(int,int)), SLOT(musicSearchIndexChanged(int,int)));

    QTimer::singleShot(MT_MS, this, SLOT(initColumns()));

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummarizied::getClassName());
}

MusicConnectTransferWidget::~MusicConnectTransferWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_buttonGroup;
    delete m_sendServer;
    delete ui;
}

QString MusicConnectTransferWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicConnectTransferWidget::openTransferFiles(int mode)
{
    if(mode == 1)
    {
        switchDiffDevice();
    }
}

void MusicConnectTransferWidget::redirectToCurrentSong(int toolIndex, int songIndex)
{
    currentPlayListSelected(toolIndex);
    ui->playListTableWidget->listCellClicked(songIndex, 0);
}

void MusicConnectTransferWidget::initColumns()
{
    MusicSongItems songs;
    emit getMusicLists(songs);

    ui->playListLayoutWidget->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    m_buttonGroup = new QButtonGroup(this);
    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(currentPlayListSelected(int)));
    for(int i=0; i<songs.count(); ++i)
    {
        QPushButton *button = new QPushButton(QString("%1(%2)").arg(songs[i].m_itemName)
                                              .arg(songs[i].m_songs.count()), this);
        button->setStyleSheet(MusicUIObject::MPushButtonStyle04);
        button->setCursor(QCursor(Qt::PointingHandCursor));
        button->setFixedSize(90, 25);
        ui->playListLayout->addWidget(button);
        m_buttonGroup->addButton(button, i);
    }

    reflashRemovableDir();
}

void MusicConnectTransferWidget::createAllItems(const MusicSongs &songs)
{
    ui->playListTableWidget->clear();
    if(ui->allSelectedcheckBox->isChecked())
    {
        ui->allSelectedcheckBox->click();
    }
    ui->playListTableWidget->setRowCount(songs.count());
    for(int i=0; i<songs.count(); ++i)
    {
        MusicSong song = songs[i];
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(MUSIC_CHECK_ROLE, false);
        ui->playListTableWidget->setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::Widget::elidedText(font(), song.getMusicName(), Qt::ElideRight, 245));
        item->setToolTip(song.getMusicPath());
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->playListTableWidget->setItem(i, 1, item);

                item = new QTableWidgetItem;
        item->setText(MusicUtils::Widget::elidedText(font(), song.getMusicTime(), Qt::ElideRight, 40));
        item->setToolTip(song.getMusicTime());
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->playListTableWidget->setItem(i, 2, item);
    }
}

QStringList MusicConnectTransferWidget::getSelectedFiles()
{
    QStringList names;
    MusicObject::MIntList list(ui->playListTableWidget->getSelectedItems());
    if(list.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("please select one item"));
        message.exec();
        return names;
    }

    if(m_currentIndex == -1 || m_currentIndex > m_currentSongs.count())
    {
        return names;
    }

    foreach(int index, list)
    {
        if(!m_searchfileListCache.value(0).isEmpty())
        {
            int count = ui->searchLineEdit->text().trimmed().count();
            index = m_searchfileListCache.value(count)[index];
        }
        names << m_currentSongs[index].getMusicPath();
    }

    return names;
}

QString MusicConnectTransferWidget::getRemovableDrive()
{
#ifdef Q_OS_WIN
    QFileInfoList drives = QDir::drives();
    foreach(const QFileInfo &driver, drives)
    {
        QString path = driver.absoluteDir().absolutePath();
        if(GetDriveTypeA(path.toStdString().c_str()) == DRIVE_REMOVABLE)
        {
            return path;
        }
    }
#endif
    return QString();
}

void MusicConnectTransferWidget::currentPlayListSelected(int index)
{
    MusicSongItems songs;
    emit getMusicLists(songs);
    if(index >= songs.count())
    {
        return;
    }

    m_searchfileListCache.clear();
    ui->searchLineEdit->clear();
    m_currentSongs = songs[m_currentIndex = index].m_songs;
    createAllItems(m_currentSongs);
}

void MusicConnectTransferWidget::selectedAllItems(bool check)
{
    for(int i=0; i<ui->playListTableWidget->rowCount(); ++i)
    {
        ui->playListTableWidget->item(i, 0)->setData(MUSIC_CHECK_ROLE, check);
    }

    if(!check)
    {
        ui->allSelectedcheckBox->setText(tr("allselected"));
        ui->playListTableWidget->clearSelection();
        ui->playListTableWidget->setCurrentIndex(QModelIndex());
    }
    else
    {
        ui->allSelectedcheckBox->setText(tr("allcanceled"));
        ui->playListTableWidget->selectAll();
    }
}

void MusicConnectTransferWidget::startToTransferUSBFiles()
{
    QStringList names = getSelectedFiles();
    if(names.isEmpty())
    {
        return;
    }

    ui->switchButton->setEnabled(false);
    QString path = M_SETTING_PTR->value(MusicSettingManager::MobileDevicePathChoiced).toString();
    if(path.isEmpty())
    {
        path = getRemovableDrive();
    }
    MusicProgressWidget progress;
    progress.show();
    progress.setTitle(tr("Copy File Mode"));
    progress.setRange(0, names.count());
    for(int i=0; i<names.count(); ++i)
    {
        QFile::copy(names[i], QString("%1/%2").arg(path).arg(names[i].split("/").last()));
        progress.setValue(i);
    }

    ui->switchButton->setEnabled(true);
    if(ui->allSelectedcheckBox->isChecked())
    {
        ui->allSelectedcheckBox->click();
    }
    ui->playListTableWidget->setSelectedAllItems(false);
}

void MusicConnectTransferWidget::startToTransferWIFIFiles()
{
    QStringList names = getSelectedFiles();
    if(names.isEmpty())
    {
        return;
    }

    QRegExp reg("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QString address = ui->lineWIFIEdit->text();
    if(!address.contains(reg))
    {
        MusicMessageBox message;
        message.setText(tr("the ip address is incorrect!"));
        message.exec();
        return;
    }

    if(m_sendServer == nullptr)
    {
        m_sendServer = new MusicFileSenderServer(this);
    }
    ui->switchButton->setEnabled(false);
    m_sendServer->setSendFiles(names);
    m_sendServer->start();

    ui->switchButton->setEnabled(true);
    if(ui->allSelectedcheckBox->isChecked())
    {
        ui->allSelectedcheckBox->click();
    }
    ui->playListTableWidget->setSelectedAllItems(false);
}

void MusicConnectTransferWidget::reflashRemovableDir()
{
    QString path = M_SETTING_PTR->value(MusicSettingManager::MobileDevicePathChoiced).toString();
    if(path.isEmpty())
    {
        path = getRemovableDrive();
    }
    ui->textUSBLabel->setText(QString("( %1 )").arg(path));
    ui->transferUSBButton->setEnabled( !path.isEmpty() );
}

void MusicConnectTransferWidget::switchDiffDevice()
{
    bool state = ui->switchButton->text() == tr("wifi");
    ui->switchButton->setText(state ? tr("mobile") : tr("wifi"));
    ui->stackedWidget->setCurrentIndex(state);
}

void MusicConnectTransferWidget::musicSearchIndexChanged(int, int index)
{
    MusicObject::MIntList searchResult;
    for(int j=0; j<m_currentSongs.count(); ++j)
    {
        if(m_currentSongs[j].getMusicName().contains(ui->searchLineEdit->text().trimmed(), Qt::CaseInsensitive))
        {
            searchResult << j;
        }
    }
    m_searchfileListCache.insert(index, searchResult);

    MusicSongs songs;
    foreach(int index, searchResult)
    {
        songs.append(m_currentSongs[index]);
    }
    createAllItems(songs);
}

int MusicConnectTransferWidget::exec()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
