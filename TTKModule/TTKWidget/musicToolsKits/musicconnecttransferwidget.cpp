#include "musicconnecttransferwidget.h"
#include "ui_musicconnecttransferwidget.h"
#include "musicuiobject.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"
#include "musicmessagebox.h"
#include "musicdeviceinfocore.h"
#include "musicprogresswidget.h"
#include "musicsongssummariziedwidget.h"

#include <QFile>
#include <QTimer>
#include <QButtonGroup>
#include <QtDebug>

MusicConnectTransferWidget::MusicConnectTransferWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicConnectTransferWidget)
{
    m_ui->setupUi(this);
    setStyleSheet(MusicUIObject::MScrollBarStyle01);

    m_currentIndex = -1;
    m_buttonGroup = nullptr;
    m_currentDeviceItem = nullptr;
    m_songCountLabel = m_ui->songCountLabel->text();
    m_selectCountLabel = m_ui->selectCountLabel->text();

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->allSelectedcheckBox->setStyleSheet(MusicUIObject::MCheckBoxStyle03);
    m_ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->allSelectedcheckBox->setText(tr("allselected"));
    connect(m_ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    m_ui->transferUSBButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->transferUSBButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->transferUSBButton, SIGNAL(clicked()), SLOT(startToTransferUSBFiles()));

    m_ui->searchLineEdit->setStyleSheet(MusicUIObject::MLineEditStyle05);
    connect(m_ui->searchLineEdit, SIGNAL(cursorPositionChanged(int,int)), SLOT(musicSearchIndexChanged(int,int)));

#ifdef Q_OS_UNIX
    m_ui->allSelectedcheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->transferUSBButton->setFocusPolicy(Qt::NoFocus);
    m_ui->searchLineLabel->setFocusPolicy(Qt::NoFocus);
#endif

    QTimer::singleShot(MT_MS, this, SLOT(initColumns()));

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummariziedWidget::getClassName());
}

MusicConnectTransferWidget::~MusicConnectTransferWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_buttonGroup;
    delete m_ui;
}

void MusicConnectTransferWidget::setDeviceInfoItem(MusicDeviceInfoItem *item)
{
    m_currentDeviceItem = item;
    m_ui->deviceInfoLabel->setText(item->m_name + "(" + item->m_path + ")");
    m_ui->songCountLabel->setText(m_songCountLabel.arg(0));
}

void MusicConnectTransferWidget::initColumns()
{
    MusicSongItems songs;
    emit getMusicLists(songs);

    m_ui->playListLayoutWidget->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    m_buttonGroup = new QButtonGroup(this);
    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(currentPlaylistSelected(int)));
    for(int i=0; i<songs.count(); ++i)
    {
        QPushButton *button = new QPushButton(QString("%1(%2)").arg(songs[i].m_itemName).arg(songs[i].m_songs.count()), this);
        button->setStyleSheet(MusicUIObject::MPushButtonStyle04);
        button->setCursor(QCursor(Qt::PointingHandCursor));
        button->setFixedSize(90, 25);
#ifdef Q_OS_UNIX
        button->setFocusPolicy(Qt::NoFocus);
#endif
        m_ui->playListLayout->addWidget(button);
        m_buttonGroup->addButton(button, i);
    }
}

void MusicConnectTransferWidget::createAllItems(const MusicSongs &songs)
{
    m_ui->playListTableWidget->clear();
    if(m_ui->allSelectedcheckBox->isChecked())
    {
        m_ui->allSelectedcheckBox->click();
    }

    m_ui->playListTableWidget->createAllItems(songs);
}

QStringList MusicConnectTransferWidget::getSelectedFiles()
{
    QStringList paths;
    const MIntList list(m_ui->playListTableWidget->getSelectedItems());
    if(list.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("please select one item"));
        message.exec();
        return paths;
    }

    if(m_currentIndex == -1 || m_currentIndex > m_currentSongs.count())
    {
        return paths;
    }

    foreach(int index, list)
    {
        if(!m_searchfileListCache.value(0).isEmpty())
        {
            const int count = m_ui->searchLineEdit->text().trimmed().count();
            index = m_searchfileListCache.value(count)[index];
        }
        paths << m_currentSongs[index].getMusicPath();
    }

    return paths;
}

void MusicConnectTransferWidget::itemSelectedChanged()
{
    const MIntList list(m_ui->playListTableWidget->getSelectedItems());
    qint64 size = 0;

    for(int i=0; i<list.count(); ++i)
    {
        size += m_currentSongs[ list[i] ].getMusicSize();
    }

    double dSize = (size * 100 / MH_MB2B) * 1.0 / 100;
    m_ui->selectCountLabel->setText(m_selectCountLabel.arg(list.count()).arg(dSize));
}

void MusicConnectTransferWidget::currentPlaylistSelected(int index)
{
    MusicSongItems songs;
    emit getMusicLists(songs);

    if(index >= songs.count() || index < 0)
    {
        return;
    }

    m_searchfileListCache.clear();
    m_ui->searchLineEdit->clear();
    m_currentSongs = songs[m_currentIndex = index].m_songs;
    createAllItems(m_currentSongs);

    m_ui->songCountLabel->setText(m_songCountLabel.arg(m_currentSongs.count()));
}

void MusicConnectTransferWidget::selectedAllItems(bool check)
{
    m_ui->allSelectedcheckBox->setText(check ? tr("allcanceled") : tr("allselected"));
    m_ui->playListTableWidget->selectedAllItems(check);
    itemSelectedChanged();
}

void MusicConnectTransferWidget::startToTransferUSBFiles()
{
//    const QStringList &names = getSelectedFiles();
//    if(names.isEmpty())
//    {
//        return;
//    }

//    QString path = M_SETTING_PTR->value(MusicSettingManager::ExtraDevicePathChoiced).toString();
//    if(path.isEmpty())
//    {
////        path = getRemovableDrive();
//    }

//    MusicProgressWidget progress;
//    progress.show();
//    progress.setTitle(tr("Copy File Mode"));
//    progress.setRange(0, names.count());
//    for(int i=0; i<names.count(); ++i)
//    {
//        QFile::copy(names[i], QString("%1/%2").arg(path).arg(names[i].split("/").last()));
//        progress.setValue(i);
//    }

//    if(m_ui->allSelectedcheckBox->isChecked())
//    {
//        m_ui->allSelectedcheckBox->click();
//    }
//    m_ui->playListTableWidget->setSelectedAllItems(false);
}

void MusicConnectTransferWidget::musicSearchIndexChanged(int, int index)
{
    MIntList searchResult;
    for(int j=0; j<m_currentSongs.count(); ++j)
    {
        if(m_currentSongs[j].getMusicName().contains(m_ui->searchLineEdit->text().trimmed(), Qt::CaseInsensitive))
        {
            searchResult << j;
        }
    }
    m_searchfileListCache.insert(index, searchResult);

    MusicSongs songs;
    foreach(const int index, searchResult)
    {
        songs.append(m_currentSongs[index]);
    }
    createAllItems(songs);
}

int MusicConnectTransferWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
