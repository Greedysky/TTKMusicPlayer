#include "musicsongitemselectedareawidget.h"
#include "ui_musicsongitemselecteddialog.h"
#include "musicsongscontainerwidget.h"
#include "musicwidgetheaders.h"
#include "musicconnectionpool.h"
#include "ttkclickedlabel.h"

MusicSongItemSelectedTableWidget::MusicSongItemSelectedTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(2);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 30);
#ifdef Q_OS_UNIX
    headerView->resizeSection(1, 219);
#else
    headerView->resizeSection(1, 222);
#endif
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle01);
}

void MusicSongItemSelectedTableWidget::addCellItems(MusicSongItemList *items)
{
    if(items->count() >= 4)
    {
        items->takeAt(1);   //MUSIC_LOVEST_LIST
        items->takeAt(1);   //MUSIC_NETWORK_LIST
        items->takeAt(1);   //MUSIC_RECENT_LIST
    }

    setRowCount(items->count());
    QHeaderView *headerView = horizontalHeader();

    for(int i = 0; i < items->count(); ++i)
    {
        const MusicSongItem &v = items->at(i);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
        item->setData(TTK_DATA_ROLE, v.m_itemIndex);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(v.m_itemName);
        item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 30));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);
    }
}

TTKIntList MusicSongItemSelectedTableWidget::checkedDataList() const
{
    TTKIntList list;
    for(int i = 0; i < rowCount(); ++i)
    {
        const QTableWidgetItem *it = item(i, 0);
        if(it && it->data(TTK_CHECKED_ROLE) == Qt::Checked)
        {
            list << it->data(TTK_DATA_ROLE).toInt();
        }
    }
    return list;
}



MusicSongItemSelectedDialog::MusicSongItemSelectedDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSongItemSelectedDialog)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->confirmButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->selectAllCheckButton->setStyleSheet(TTK::UI::CheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->selectAllCheckButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
    connect(m_ui->selectAllCheckButton, SIGNAL(clicked(bool)), m_ui->itemTableWidget, SLOT(checkedItemsStatus(bool)));
}

MusicSongItemSelectedDialog::~MusicSongItemSelectedDialog()
{
    delete m_ui;
}

void MusicSongItemSelectedDialog::addCellItems(MusicSongItemList *items)
{
    m_ui->itemTableWidget->addCellItems(items);
}

void MusicSongItemSelectedDialog::confirmButtonClicked()
{
    Q_EMIT itemListChanged(m_ui->itemTableWidget->checkedDataList());
    accept();
}



MusicSongItemSelectedAreaWidget::MusicSongItemSelectedAreaWidget(QWidget *parent)
    : QWidget(parent),
      m_selected(false)
{
    m_label = new QLabel(tr("Range:"));
    m_itemLabel = new QLabel(tr("All List"));
    m_modifiedItemButton = new TTKClickedLabel(tr("Mod"));

    m_label->setFixedWidth(60);
    m_itemLabel->setFixedWidth(75);
    m_modifiedItemButton->setFixedWidth(30);

    m_label->setStyleSheet(TTK::UI::FontStyle01 + TTK::UI::ColorStyle02);
    m_itemLabel->setStyleSheet(TTK::UI::ColorStyle04);
    m_modifiedItemButton->setStyleSheet(TTK::UI::ColorStyle07);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_label);
    layout->addWidget(m_itemLabel);
    layout->addWidget(m_modifiedItemButton);
    setLayout(layout);

    connect(m_modifiedItemButton, SIGNAL(clicked()), SLOT(modifiedItemButtonClicked()));

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->connect(className(), MusicSongsContainerWidget::className());
}

MusicSongItemSelectedAreaWidget::~MusicSongItemSelectedAreaWidget()
{
    G_CONNECTION_PTR->removeValue(this);
    delete m_label;
    delete m_itemLabel;
    delete m_modifiedItemButton;
}

MusicSongList MusicSongItemSelectedAreaWidget::selectedSongItems()
{
    MusicSongItemList songs;
    Q_EMIT querySongItemList(songs);

    MusicSongList selectedSongs;
    for(const MusicSongItem &item : qAsConst(songs))
    {
        if(m_selected)
        {
            if(m_items.contains(item.m_itemIndex))
            {
                selectedSongs << item.m_songs;
            }
        }
        else
        {
            if(TTK::playlistRowValid(item.m_itemIndex))
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
    Q_EMIT querySongItemList(songs);

    m_selected = true;

    MusicSongItemSelectedDialog widget;
    connect(&widget, SIGNAL(itemListChanged(TTKIntList)), SLOT(itemListChanged(TTKIntList)));
    widget.addCellItems(&songs);
    widget.exec();

    Q_EMIT confirmChanged();
}

void MusicSongItemSelectedAreaWidget::itemListChanged(const TTKIntList &items)
{
    m_items = items;
    m_itemLabel->setText(tr("Custom List"));
}
