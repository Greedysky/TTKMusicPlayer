#include "musicsongitemselecteddialog.h"
#include "ui_musicsongitemselecteddialog.h"
#include "musicitemdelegate.h"
#include "musicuiobject.h"
#include "musicwidgetheaders.h"

MusicSongItemSelectedTableWidget::MusicSongItemSelectedTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

    setColumnCount(2);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
#ifdef Q_OS_UNIX
    headerview->resizeSection(1, 219);
#else
    headerview->resizeSection(1, 222);
#endif
}

void MusicSongItemSelectedTableWidget::createAllItems(MusicSongItems *items)
{
    if(items->count() >= 4)
    {
        items->takeAt(1);   //MUSIC_LOVEST_LIST
        items->takeAt(1);   //MUSIC_NETWORK_LIST
        items->takeAt(1);   //MUSIC_RECENT_LIST
    }

    setRowCount(items->count());
    QHeaderView *headerview = horizontalHeader();
    for(int i=0; i<items->count(); ++i)
    {
        const MusicSongItem &song = (*items)[i];
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(MUSIC_CHECK_ROLE, false);
        item->setData(MUSIC_DATAS_ROLE, song.m_itemIndex);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(song.m_itemName);
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 30));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);
    }
}

MIntList MusicSongItemSelectedTableWidget::getSelectedItems() const
{
    MIntList list;
    for(int i=0; i<rowCount(); ++i)
    {
        const QTableWidgetItem *it = item(i, 0);
        if(it && it->data(MUSIC_CHECK_ROLE) == true)
        {
            list << it->data(MUSIC_DATAS_ROLE).toInt();
        }
    }
    return list;
}

void MusicSongItemSelectedTableWidget::selectedAllItems(bool check)
{
    for(int i=0; i<rowCount(); ++i)
    {
        item(i, 0)->setData(MUSIC_CHECK_ROLE, check);
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

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->selectAllCheckButton->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
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

void MusicSongItemSelectedDialog::createAllItems(MusicSongItems *items)
{
    m_ui->itemTableWidget->createAllItems(items);
}

void MusicSongItemSelectedDialog::confirmButtonClicked()
{
    emit itemListsChanged( m_ui->itemTableWidget->getSelectedItems() );
    accept();
}

int MusicSongItemSelectedDialog::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
