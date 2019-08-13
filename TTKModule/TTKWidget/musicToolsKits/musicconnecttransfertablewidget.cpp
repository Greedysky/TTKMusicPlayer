#include "musicconnecttransfertablewidget.h"
#include "musicitemdelegate.h"

MusicConnectTransferTableWidget::MusicConnectTransferTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setColumnCount(3);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 280);
    headerview->resizeSection(2, 43);

    connect(m_checkBoxDelegate, SIGNAL(buttonChecked()), parent->parent(), SLOT(itemSelectedChanged()));
}

void MusicConnectTransferTableWidget::createAllItems(const MusicSongs &songs)
{
    setRowCount(songs.count());
    QHeaderView *headerview = horizontalHeader();

    for(int i=0; i<songs.count(); ++i)
    {
        const MusicSong song = songs[i];
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(MUSIC_CHECK_ROLE, false);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(song.getMusicPath());
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                item = new QTableWidgetItem;
        item->setToolTip(song.getMusicPlayTime());
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(2) - 5));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);
    }
}

void MusicConnectTransferTableWidget::selectedAllItems(bool check)
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
