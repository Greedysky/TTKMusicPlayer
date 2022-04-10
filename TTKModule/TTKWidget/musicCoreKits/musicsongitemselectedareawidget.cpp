#include "musicsongitemselectedareawidget.h"
#include "ui_musicsongitemselecteddialog.h"
#include "musicitemdelegate.h"
#include "musicwidgetheaders.h"
#include "musicclickedlabel.h"
#include "musicconnectionpool.h"
#include "musicsongssummariziedwidget.h"

MusicSongItemSelectedTableWidget::MusicSongItemSelectedTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    verticalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);

    setColumnCount(2);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
#ifdef Q_OS_UNIX
    headerview->resizeSection(1, 219);
#else
    headerview->resizeSection(1, 222);
#endif
}

void MusicSongItemSelectedTableWidget::createAllItems(MusicSongItemList *items)
{
    if(items->count() >= 4)
    {
        items->takeAt(1);   //MUSIC_LOVEST_LIST
        items->takeAt(1);   //MUSIC_NETWORK_LIST
        items->takeAt(1);   //MUSIC_RECENT_LIST
    }

    setRowCount(items->count());
    QHeaderView *headerview = horizontalHeader();
    for(int i = 0; i < items->count(); ++i)
    {
        const MusicSongItem &song = items->at(i);
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(MUSIC_CHECK_ROLE, Qt::Unchecked);
        item->setData(MUSIC_DATA_ROLE, song.m_itemIndex);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(song.m_itemName);
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 30));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);
    }
}

TTKIntList MusicSongItemSelectedTableWidget::selectedItems() const
{
    TTKIntList list;
    for(int i = 0; i < rowCount(); ++i)
    {
        const QTableWidgetItem *it = item(i, 0);
        if(it && it->data(MUSIC_CHECK_ROLE) == Qt::Checked)
        {
            list << it->data(MUSIC_DATA_ROLE).toInt();
        }
    }
    return list;
}

void MusicSongItemSelectedTableWidget::selectedAllItems(bool check)
{
    for(int i = 0; i < rowCount(); ++i)
    {
        item(i, 0)->setData(MUSIC_CHECK_ROLE, check ? Qt::Checked : Qt::Unchecked);
    }

    if(!check)
    {
        clearSelection();
        setCurrentIndex(QModelIndex());
    }
    else
    {
        selectAll();
    }
}



MusicSongItemSelectedDialog::MusicSongItemSelectedDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSongItemSelectedDialog)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->confirmButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->selectAllCheckButton->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->selectAllCheckButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
    connect(m_ui->selectAllCheckButton, SIGNAL(clicked(bool)), m_ui->itemTableWidget, SLOT(selectedAllItems(bool)));
}

MusicSongItemSelectedDialog::~MusicSongItemSelectedDialog()
{
    delete m_ui;
}

void MusicSongItemSelectedDialog::createAllItems(MusicSongItemList *items)
{
    m_ui->itemTableWidget->createAllItems(items);
}

void MusicSongItemSelectedDialog::confirmButtonClicked()
{
    Q_EMIT itemListChanged(m_ui->itemTableWidget->selectedItems());
    accept();
}

int MusicSongItemSelectedDialog::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}



MusicSongItemSelectedAreaWidget::MusicSongItemSelectedAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    m_selected = false;
    m_label = new QLabel(tr("Range:"));
    m_itemLabel = new QLabel(tr("All List"));
    m_modifiedItemButton = new MusicClickedLabel(tr("Mod"));

    m_label->setFixedWidth(60);
    m_itemLabel->setFixedWidth(75);
    m_modifiedItemButton->setFixedWidth(30);

    m_label->setStyleSheet(MusicUIObject::MQSSFontStyle01 + MusicUIObject::MQSSColorStyle09);
    m_itemLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04);
    m_modifiedItemButton->setStyleSheet(MusicUIObject::MQSSColorStyle08);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_label);
    layout->addWidget(m_itemLabel);
    layout->addWidget(m_modifiedItemButton);
    setLayout(layout);

    connect(m_modifiedItemButton, SIGNAL(clicked()), SLOT(modifiedItemButtonClicked()));

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->poolConnect(className(), MusicSongsSummariziedWidget::className());
}

MusicSongItemSelectedAreaWidget::~MusicSongItemSelectedAreaWidget()
{
    G_CONNECTION_PTR->removeValue(className());
    delete m_label;
    delete m_itemLabel;
    delete m_modifiedItemButton;
}

MusicSongList MusicSongItemSelectedAreaWidget::selectedSongItems()
{
    MusicSongItemList songs;
    Q_EMIT queryMusicItemList(songs);

    MusicSongList selectedSongs;
    for(const MusicSongItem &item : qAsConst(songs))
    {
        if(m_selected)
        {
            if(m_selectedItems.contains(item.m_itemIndex))
            {
                selectedSongs << item.m_songs;
            }
        }
        else
        {
            if(MusicObject::playlistRowValid(item.m_itemIndex))
            {
                selectedSongs << item.m_songs;
            }
        }
    }
    return selectedSongs;
}

void MusicSongItemSelectedAreaWidget::modifiedItemButtonClicked()
{
    MusicSongItemList songs;
    Q_EMIT queryMusicItemList(songs);

    m_selected = true;

    MusicSongItemSelectedDialog dialog;
    connect(&dialog, SIGNAL(itemListChanged(TTKIntList)), SLOT(itemListChanged(TTKIntList)));
    dialog.createAllItems(&songs);
    dialog.exec();

    Q_EMIT confirmChanged();
}

void MusicSongItemSelectedAreaWidget::itemListChanged(const TTKIntList &items)
{
    m_selectedItems = items;
    m_itemLabel->setText(tr("Custom List"));
}
