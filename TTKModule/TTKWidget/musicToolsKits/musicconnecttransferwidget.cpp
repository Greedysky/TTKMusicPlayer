#include "musicconnecttransferwidget.h"
#include "ui_musicconnecttransferwidget.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"
#include "musictoastlabel.h"
#include "musicdeviceinfomodule.h"
#include "musicsongssummariziedwidget.h"
#include "musicconnecttransferthread.h"

MusicConnectTransferWidget::MusicConnectTransferWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicConnectTransferWidget),
      m_currentIndex(-1),
      m_currentDeviceItem(nullptr)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);

    m_songCountLabel = m_ui->songCountLabel->text();
    m_selectCountLabel = m_ui->selectCountLabel->text();

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->allSelectedcheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle03);
    m_ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->allSelectedcheckBox->setText(tr("All"));
    connect(m_ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    m_ui->transferUSBButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->transferUSBButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->transferUSBButton, SIGNAL(clicked()), SLOT(startToTransferFiles()));

    m_ui->searchLineEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle03);
    connect(m_ui->searchLineEdit, SIGNAL(cursorPositionChanged(int,int)), SLOT(searchResultChanged(int,int)));

    m_thread = new MusicConnectTransferThread(this);
    connect(m_thread, SIGNAL(transferFileFinished(QString)), m_ui->completeTableWidget, SLOT(addCellItem(QString)));

#ifdef Q_OS_UNIX
    m_ui->allSelectedcheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->transferUSBButton->setFocusPolicy(Qt::NoFocus);
    m_ui->searchLineLabel->setFocusPolicy(Qt::NoFocus);
#endif

    QTimer::singleShot(MT_MS, this, SLOT(initialize()));

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->poolConnect(className(), MusicSongsSummariziedWidget::className());
}

MusicConnectTransferWidget::~MusicConnectTransferWidget()
{
    G_CONNECTION_PTR->removeValue(className());
    delete m_ui;
    delete m_thread;
}

void MusicConnectTransferWidget::setDeviceInfoItem(MusicDeviceInfoItem *item)
{
    m_currentDeviceItem = item;
    m_ui->deviceInfoLabel->setToolTip(item->m_name + "(" + item->m_path + ")");
    m_ui->deviceInfoLabel->setText(MusicUtils::Widget::elidedText(m_ui->deviceInfoLabel->font(), m_ui->deviceInfoLabel->toolTip(), Qt::ElideRight, 220));

    m_ui->songCountLabel->setText(m_songCountLabel.arg(0));
    m_ui->selectCountLabel->setText(m_selectCountLabel.arg(0).arg(0));
}

void MusicConnectTransferWidget::initialize()
{
    MusicSongItemList songs;
    Q_EMIT queryMusicItemList(songs);

    m_ui->playListLayoutWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    QtButtonGroupConnect(buttonGroup, this, currentPlaylistSelected);

    for(int i = 0; i < songs.count(); ++i)
    {
        QPushButton *button = new QPushButton(QString("%1(%2)").arg(songs[i].m_itemName).arg(songs[i].m_songs.count()), this);
        button->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
        button->setCursor(QCursor(Qt::PointingHandCursor));
        button->setFixedSize(90, 25);
#ifdef Q_OS_UNIX
        button->setFocusPolicy(Qt::NoFocus);
#endif
        m_ui->playListLayout->addWidget(button);
        buttonGroup->addButton(button, i);
    }

    int count = 0;
    for(const MusicSongItem &item : qAsConst(songs))
    {
        count += item.m_songs.count();
    }
    m_ui->songCountLabel->setText(m_songCountLabel.arg(count));
}

void MusicConnectTransferWidget::addCellItems(const MusicSongList &songs)
{
    m_ui->listTableWidget->removeItems();
    if(m_ui->allSelectedcheckBox->isChecked())
    {
        m_ui->allSelectedcheckBox->click();
    }
    m_ui->listTableWidget->addCellItems(songs);
}

QStringList MusicConnectTransferWidget::selectedFiles() const
{
    QStringList paths;
    const TTKIntList &list = m_ui->listTableWidget->checkedIndexList();
    if(list.isEmpty())
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
        return paths;
    }

    if(m_currentIndex == -1 || m_currentIndex > m_songItems.count())
    {
        return paths;
    }

    for(int index : list)
    {
        if(!m_searchResultCache.value(0).isEmpty())
        {
            const int count = m_ui->searchLineEdit->text().trimmed().count();
            index = m_searchResultCache.value(count)[index];
        }
        paths << m_songItems[index].path();
    }

    return paths;
}

void MusicConnectTransferWidget::itemSelectedChanged()
{
    const TTKIntList &list = m_ui->listTableWidget->checkedIndexList();
    qint64 size = 0;

    for(int i = 0; i < list.count(); ++i)
    {
        size += m_songItems[list[i]].size();
    }

    double dSize = (size * 100 / MH_MB2B) * 1.0 / 100;
    m_ui->selectCountLabel->setText(m_selectCountLabel.arg(list.count()).arg(dSize));
}

void MusicConnectTransferWidget::currentPlaylistSelected(int index)
{
    MusicSongItemList songs;
    Q_EMIT queryMusicItemList(songs);

    if(index >= songs.count() || index < 0)
    {
        return;
    }

    m_searchResultCache.clear();
    m_ui->searchLineEdit->clear();
    m_songItems = songs[m_currentIndex = index].m_songs;
    addCellItems(m_songItems);
}

void MusicConnectTransferWidget::selectedAllItems(bool check)
{
    m_ui->listTableWidget->checkedItemsStatus(check);
    itemSelectedChanged();
}

void MusicConnectTransferWidget::startToTransferFiles()
{
    const QStringList &names = selectedFiles();
    if(names.isEmpty())
    {
        return;
    }

    m_thread->setCopyFilePath(m_currentDeviceItem->m_path, names);
    m_thread->start();
}

void MusicConnectTransferWidget::searchResultChanged(int, int column)
{
    TTKIntList result;
    for(int i = 0; i < m_songItems.count(); ++i)
    {
        if(m_songItems[i].name().contains(m_ui->searchLineEdit->text().trimmed(), Qt::CaseInsensitive))
        {
            result << i;
        }
    }

    MusicSongList data;
    for(const int index : qAsConst(result))
    {
        data.append(m_songItems[index]);
    }

    m_searchResultCache.insert(column, result);
    addCellItems(data);
}

int MusicConnectTransferWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
