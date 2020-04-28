#include "musicconnecttransferwidget.h"
#include "ui_musicconnecttransferwidget.h"
#include "musicuiobject.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"
#include "musicmessagebox.h"
#include "musicdeviceinfoobject.h"
#include "musicsongssummariziedwidget.h"
#include "musicconnecttransferthread.h"

#include <QFile>
#include <QTimer>
#include <QButtonGroup>

MusicConnectTransferWidget::MusicConnectTransferWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicConnectTransferWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);

    m_currentIndex = -1;
    m_buttonGroup = nullptr;
    m_currentDeviceItem = nullptr;
    m_songCountLabel = m_ui->songCountLabel->text();
    m_selectCountLabel = m_ui->selectCountLabel->text();

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->allSelectedcheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle03);
    m_ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->allSelectedcheckBox->setText(tr("allselected"));
    connect(m_ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    m_ui->transferUSBButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->transferUSBButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->transferUSBButton, SIGNAL(clicked()), SLOT(startToTransferFiles()));

    m_ui->searchLineEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle05);
    connect(m_ui->searchLineEdit, SIGNAL(cursorPositionChanged(int,int)), SLOT(musicSearchIndexChanged(int,int)));

    m_transferThread = new MusicConnectTransferThread(this);
    connect(m_transferThread, SIGNAL(transferFileFinished(QString)), m_ui->completeTableWidget, SLOT(createItem(QString)));

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
    delete m_transferThread;
}

void MusicConnectTransferWidget::setDeviceInfoItem(MusicDeviceInfoItem *item)
{
    m_currentDeviceItem = item;
    m_ui->deviceInfoLabel->setToolTip(item->m_name + "(" + item->m_path + ")");
    m_ui->deviceInfoLabel->setText(MusicUtils::Widget::elidedText(m_ui->deviceInfoLabel->font(), m_ui->deviceInfoLabel->toolTip(), Qt::ElideRight, 220));

    m_ui->songCountLabel->setText(m_songCountLabel.arg(0));
    m_ui->selectCountLabel->setText(m_selectCountLabel.arg(0).arg(0));
}

void MusicConnectTransferWidget::initColumns()
{
    MusicSongItems songs;
    Q_EMIT getMusicLists(songs);

    m_ui->playListLayoutWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_buttonGroup = new QButtonGroup(this);
    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(currentPlaylistSelected(int)));
    for(int i=0; i<songs.count(); ++i)
    {
        QPushButton *button = new QPushButton(QString("%1(%2)").arg(songs[i].m_itemName).arg(songs[i].m_songs.count()), this);
        button->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
        button->setCursor(QCursor(Qt::PointingHandCursor));
        button->setFixedSize(90, 25);
#ifdef Q_OS_UNIX
        button->setFocusPolicy(Qt::NoFocus);
#endif
        m_ui->playListLayout->addWidget(button);
        m_buttonGroup->addButton(button, i);
    }

    int count = 0;
    foreach(const MusicSongItem &item, songs)
    {
        count += item.m_songs.count();
    }
    m_ui->songCountLabel->setText(m_songCountLabel.arg(count));
}

void MusicConnectTransferWidget::createAllItems(const MusicSongs &songs)
{
    m_ui->listTableWidget->clear();
    if(m_ui->allSelectedcheckBox->isChecked())
    {
        m_ui->allSelectedcheckBox->click();
    }

    m_ui->listTableWidget->createAllItems(songs);
}

QStringList MusicConnectTransferWidget::getSelectedFiles()
{
    QStringList paths;
    const TTKIntList list(m_ui->listTableWidget->getSelectedItems());
    if(list.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
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
    const TTKIntList list(m_ui->listTableWidget->getSelectedItems());
    qint64 size = 0;

    for(int i=0; i<list.count(); ++i)
    {
        size += m_currentSongs[list[i]].getMusicSize();
    }

    double dSize = (size * 100 / MH_MB2B) * 1.0 / 100;
    m_ui->selectCountLabel->setText(m_selectCountLabel.arg(list.count()).arg(dSize));
}

void MusicConnectTransferWidget::currentPlaylistSelected(int index)
{
    MusicSongItems songs;
    Q_EMIT getMusicLists(songs);

    if(index >= songs.count() || index < 0)
    {
        return;
    }

    m_searchfileListCache.clear();
    m_ui->searchLineEdit->clear();
    m_currentSongs = songs[m_currentIndex = index].m_songs;
    createAllItems(m_currentSongs);
}

void MusicConnectTransferWidget::selectedAllItems(bool check)
{
    m_ui->allSelectedcheckBox->setText(check ? tr("allcanceled") : tr("allselected"));
    m_ui->listTableWidget->selectedAllItems(check);
    itemSelectedChanged();
}

void MusicConnectTransferWidget::startToTransferFiles()
{
    const QStringList &names = getSelectedFiles();
    if(names.isEmpty())
    {
        return;
    }

    m_transferThread->setCopyFilePath(m_currentDeviceItem->m_path, names);
    m_transferThread->start();
}

void MusicConnectTransferWidget::musicSearchIndexChanged(int, int index)
{
    TTKIntList searchResult;
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
