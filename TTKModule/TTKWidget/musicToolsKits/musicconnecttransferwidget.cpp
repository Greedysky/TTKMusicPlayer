#include "musicconnecttransferwidget.h"
#include "ui_musicconnecttransferwidget.h"
#include "musicsongscontainerwidget.h"
#include "musictoastlabel.h"
#include "musicdeviceinfomodule.h"
#include "musicconnecttransferthread.h"

#include <QButtonGroup>

MusicConnectTransferWidget::MusicConnectTransferWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      MusicItemSearchInterfaceClass(),
      m_ui(new Ui::MusicConnectTransferWidget),
      m_currentIndex(-1),
      m_currentDeviceItem(nullptr)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);
    setStyleSheet(TTK::UI::ScrollBarStyle01);

    m_songCountLabel = m_ui->songCountLabel->text();
    m_selectCountLabel = m_ui->selectCountLabel->text();

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->allSelectedcheckBox->setStyleSheet(TTK::UI::CheckBoxStyle03);
    m_ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->allSelectedcheckBox->setText(tr("All"));
    connect(m_ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    m_ui->transferUSBButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->transferUSBButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->transferUSBButton, SIGNAL(clicked()), SLOT(startToTransferFiles()));

    m_ui->searchLineEdit->setStyleSheet(TTK::UI::LineEditStyle03);
    connect(m_ui->searchLineEdit, SIGNAL(cursorPositionChanged(int,int)), SLOT(searchResultChanged(int,int)));

    m_thread = new MusicConnectTransferThread(this);
    connect(m_thread, SIGNAL(transferFileFinished(QString)), m_ui->completeTableWidget, SLOT(addCellItem(QString)));

#ifdef Q_OS_UNIX
    m_ui->allSelectedcheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->transferUSBButton->setFocusPolicy(Qt::NoFocus);
    m_ui->searchLineLabel->setFocusPolicy(Qt::NoFocus);
#endif

    TTK_SIGNLE_SHOT(initialize, TTK_SLOT);
}

MusicConnectTransferWidget::~MusicConnectTransferWidget()
{
    delete m_ui;
    delete m_thread;
}

void MusicConnectTransferWidget::setDeviceInfoItem(MusicDeviceInfoItem *item)
{
    m_currentDeviceItem = item;
    m_ui->deviceInfoLabel->setToolTip(item->m_name + "(" + item->m_path + ")");
    m_ui->deviceInfoLabel->setText(TTK::Widget::elidedText(m_ui->deviceInfoLabel->font(), m_ui->deviceInfoLabel->toolTip(), Qt::ElideRight, 220));

    m_ui->songCountLabel->setText(m_songCountLabel.arg(0));
    m_ui->selectCountLabel->setText(m_selectCountLabel.arg(0).arg(0));
}

void MusicConnectTransferWidget::initialize()
{
    MusicSongItemList items;
    MusicSongsContainerWidget::instance()->querySongItemList(items);

    m_ui->playListLayoutWidget->setStyleSheet(TTK::UI::BackgroundStyle01);
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    QtButtonGroupConnect(buttonGroup, this, currentPlaylistSelected, TTK_SLOT);

    for(int i = 0; i < items.count(); ++i)
    {
        QPushButton *button = new QPushButton(QString("%1(%2)").arg(items[i].m_itemName).arg(items[i].m_songs.count()), this);
        button->setStyleSheet(TTK::UI::PushButtonStyle04);
        button->setCursor(QCursor(Qt::PointingHandCursor));
        button->setFixedSize(120, 25);
#ifdef Q_OS_UNIX
        button->setFocusPolicy(Qt::NoFocus);
#endif
        m_ui->playListLayout->addWidget(button);
        buttonGroup->addButton(button, i);
    }

    int count = 0;
    for(const MusicSongItem &item : qAsConst(items))
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
    const TTKIntList &rows = m_ui->listTableWidget->checkedItemRows();
    if(rows.isEmpty())
    {
        MusicToastLabel::popup(tr("Please select one item first"));
        return paths;
    }

    if(m_currentIndex == -1 || m_currentIndex > m_containerItems.count())
    {
        return paths;
    }

    for(int row : qAsConst(rows))
    {
        mappedSearchRow(m_ui->searchLineEdit->text().length(), row);
        paths << m_containerItems[row].path();
    }

    return paths;
}

void MusicConnectTransferWidget::itemSelectedChanged()
{
    const TTKIntList &rows = m_ui->listTableWidget->checkedItemRows();
    qint64 size = 0;

    for(int i = 0; i < rows.count(); ++i)
    {
        size += m_containerItems[rows[i]].size();
    }

    const double dSize = (size * TTK_RN_MAX / TTK_SN_MB2B) * 1.0 / TTK_RN_MAX;
    m_ui->selectCountLabel->setText(m_selectCountLabel.arg(rows.count()).arg(dSize));
}

void MusicConnectTransferWidget::currentPlaylistSelected(int index)
{
    MusicSongItemList items;
    MusicSongsContainerWidget::instance()->querySongItemList(items);

    if(index >= items.count() || index < 0)
    {
        return;
    }

    clearSearchResult();
    m_ui->searchLineEdit->clear();
    m_containerItems = items[m_currentIndex = index].m_songs;
    addCellItems(m_containerItems);
}

void MusicConnectTransferWidget::selectedAllItems(bool checked)
{
    m_ui->listTableWidget->checkedItemsStatus(checked);
    itemSelectedChanged();
}

void MusicConnectTransferWidget::startToTransferFiles()
{
    const QStringList &names = selectedFiles();
    if(names.isEmpty())
    {
        return;
    }

    m_thread->setFilePath(m_currentDeviceItem->m_path + TTK_SEPARATOR, names);
    m_thread->start();
}

void MusicConnectTransferWidget::searchResultChanged(int, int column)
{
    TTKIntList result;
    for(int i = 0; i < m_containerItems.count(); ++i)
    {
        if(m_containerItems[i].path().contains(m_ui->searchLineEdit->text(), Qt::CaseInsensitive))
        {
            result << i;
        }
    }

    MusicSongList data;
    for(const int index : qAsConst(result))
    {
        data.append(m_containerItems[index]);
    }

    m_searchResultLevel = column;
    m_searchResultItems.insert(column, result);

    addCellItems(data);
}
