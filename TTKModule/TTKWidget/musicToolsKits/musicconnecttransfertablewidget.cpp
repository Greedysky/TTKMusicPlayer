#include "musicconnecttransfertablewidget.h"

MusicConnectTransferTableWidget::MusicConnectTransferTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(3);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 30);
    headerView->resizeSection(1, 280);
    headerView->resizeSection(2, 43);

    connect(m_checkBoxDelegate, SIGNAL(buttonChecked()), parent->parent(), SLOT(itemSelectedChanged()));
}

void MusicConnectTransferTableWidget::addCellItems(const MusicSongList &songs)
{
    setRowCount(songs.count());
    QHeaderView *headerView = horizontalHeader();

    for(int i = 0; i < songs.count(); ++i)
    {
        const MusicSong &v = songs[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(v.path());
        item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 20));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                item = new QTableWidgetItem;
        item->setToolTip(v.duration());
        item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(2) - 5));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);
    }
}



MusicConnectTransferCompleteTableWidget::MusicConnectTransferCompleteTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setColumnCount(2);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 20);
    headerView->resizeSection(1, 305);
}

void MusicConnectTransferCompleteTableWidget::addCellItem(const QString &name)
{
    const int index = rowCount();
    setRowCount(index + 1);

    QHeaderView *headerView = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setIcon(QPixmap(":/tiny/lb_right"));
    setItem(index, 0, item);

    item = new QTableWidgetItem;
    item->setToolTip(name);
    item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 20));
    QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
    setItem(index, 1, item);
}
