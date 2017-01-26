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
}

QString MusicConnectTransferTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicConnectTransferTableWidget::createAllItems(const MusicSongs &songs)
{
    setRowCount(songs.count());
    for(int i=0; i<songs.count(); ++i)
    {
        const MusicSong song = songs[i];
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(MUSIC_CHECK_ROLE, false);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::Widget::elidedText(font(), song.getMusicName(), Qt::ElideRight, 245));
        item->setToolTip(song.getMusicPath());
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                item = new QTableWidgetItem;
        item->setText(MusicUtils::Widget::elidedText(font(), song.getMusicTime(), Qt::ElideRight, 40));
        item->setToolTip(song.getMusicTime());
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
