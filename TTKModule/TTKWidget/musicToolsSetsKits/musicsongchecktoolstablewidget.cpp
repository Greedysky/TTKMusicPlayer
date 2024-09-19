#include "musicsongchecktoolstablewidget.h"
#include "musicsongscontainerwidget.h"

static TTKPushButtonItemDelegate *makeButtonItemDelegate(QObject *parent)
{
    TTKPushButtonItemDelegate *delegate = new TTKPushButtonItemDelegate(parent);
    delegate->setStyleSheet(TTK::UI::BorderStyle02 + TTK::UI::BorderStyle05 + TTK::UI::BackgroundStyle10);
    return delegate;
}


MusicSongCheckToolsRenameTableWidget::MusicSongCheckToolsRenameTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(4);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 30);
    headerView->resizeSection(1, 290);
    headerView->resizeSection(2, 290);
#ifdef Q_OS_UNIX
    headerView->resizeSection(3, 51);
#else
    headerView->resizeSection(3, 54);
#endif

    setItemDelegateForColumn(3, makeButtonItemDelegate(this));
}

void MusicSongCheckToolsRenameTableWidget::addCellItems(const MusicSongCheckToolsRenameList &items)
{
    setRowCount(items.count());
    QHeaderView *headerView = horizontalHeader();

    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongCheckToolsRename &v = items[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(v.m_locaName);
        item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 10));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                item = new QTableWidgetItem;
        item->setToolTip(v.m_recommendName);
        item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(2) - 10));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);

                item = new QTableWidgetItem;
        item->setData(TTK_DISPLAY_ROLE, tr("Delete"));
        setItem(i, 3, item);
    }
}

void MusicSongCheckToolsRenameTableWidget::itemCellClicked(int row, int column)
{
    MusicFillItemTableWidget::itemCellClicked(row, column);
    if(column == 3)
    {
        removeCurrentRow();
    }
}

void MusicSongCheckToolsRenameTableWidget::removeCurrentRow()
{
    if(!isValid())
    {
        return;
    }

    removeRow(currentRow());
}



MusicSongCheckToolsDuplicateTableWidget::MusicSongCheckToolsDuplicateTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(7);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 30);
    headerView->resizeSection(1, 290);
    headerView->resizeSection(2, 65);
    headerView->resizeSection(3, 65);
    headerView->resizeSection(4, 80);
    headerView->resizeSection(5, 80);
#ifdef Q_OS_UNIX
    headerView->resizeSection(6, 51);
#else
    headerView->resizeSection(6, 54);
#endif

    setItemDelegateForColumn(5, makeButtonItemDelegate(this));
    setItemDelegateForColumn(6, makeButtonItemDelegate(this));
}

void MusicSongCheckToolsDuplicateTableWidget::addCellItems(const MusicSongCheckToolsDuplicateList &songs)
{
    setRowCount(songs.count());
    QHeaderView *headerView = horizontalHeader();

    for(int i = 0; i < songs.count(); ++i)
    {
        const MusicSongCheckToolsDuplicate &v = songs[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(v.m_song.name());
        item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 45));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                item = new QTableWidgetItem;
        item->setText(v.m_song.duration());
        QtItemSetTextAlignment(item, Qt::AlignCenter);
        setItem(i, 2, item);

                item = new QTableWidgetItem;
        item->setText(v.m_song.sizeStr());
        QtItemSetTextAlignment(item, Qt::AlignCenter);
        setItem(i, 3, item);

                item = new QTableWidgetItem;
        item->setText(v.m_bitrate);
        QtItemSetTextAlignment(item, Qt::AlignCenter);
        setItem(i, 4, item);

                item = new QTableWidgetItem;
        item->setData(TTK_DISPLAY_ROLE, tr("Play"));
        item->setData(TTK_DATA_ROLE, v.m_song.path());
        setItem(i, 5, item);

                item = new QTableWidgetItem;
        item->setData(TTK_DISPLAY_ROLE, tr("Delete"));
        setItem(i, 6, item);
    }
}

void MusicSongCheckToolsDuplicateTableWidget::itemCellClicked(int row, int column)
{
    MusicFillItemTableWidget::itemCellClicked(row, column);
    switch(column)
    {
        case 5: playSong(); break;
        case 6: removeCurrentRow(); break;
        default: break;
    }
}

void MusicSongCheckToolsDuplicateTableWidget::playSong()
{
    if(!isValid())
    {
        return;
    }

    const QTableWidgetItem *it = item(currentRow(), 5);
    if(it)
    {
        const QString &path = it->data(TTK_DATA_ROLE).toString();
        MusicSongsContainerWidget::instance()->addSongToPlaylist(QFile::exists(path) ? QStringList(path) : QStringList());
    }
}

void MusicSongCheckToolsDuplicateTableWidget::removeCurrentRow()
{
    if(!isValid())
    {
        return;
    }

    removeRow(currentRow());
}



MusicSongCheckToolsQualityTableWidget::MusicSongCheckToolsQualityTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(8);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 30);
    headerView->resizeSection(1, 220);
    headerView->resizeSection(2, 65);
    headerView->resizeSection(3, 65);
    headerView->resizeSection(4, 80);
    headerView->resizeSection(5, 70);
    headerView->resizeSection(6, 80);
#ifdef Q_OS_UNIX
    headerView->resizeSection(7, 51);
#else
    headerView->resizeSection(7, 54);
#endif

    setItemDelegateForColumn(6, makeButtonItemDelegate(this));
    setItemDelegateForColumn(7, makeButtonItemDelegate(this));
}

void MusicSongCheckToolsQualityTableWidget::addCellItems(const MusicSongCheckToolsQualityList &songs)
{
    setRowCount(songs.count());
    QHeaderView *headerView = horizontalHeader();

    for(int i = 0; i < songs.count(); ++i)
    {
        const MusicSongCheckToolsQuality &v = songs[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(v.m_song.name());
        item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerView->sectionSize(1) - 10));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                item = new QTableWidgetItem;
        item->setText(v.m_song.duration());
        QtItemSetTextAlignment(item, Qt::AlignCenter);
        setItem(i, 2, item);

                item = new QTableWidgetItem;
        item->setText(v.m_song.sizeStr());
        QtItemSetTextAlignment(item, Qt::AlignCenter);
        setItem(i, 3, item);

                item = new QTableWidgetItem;
        item->setText(v.m_bitrate);
        QtItemSetTextAlignment(item, Qt::AlignCenter);
        setItem(i, 4, item);

                item = new QTableWidgetItem;
        QColor color;
        QString bitrate;
        TTK::Number::bitrateToQuality(TTK::Number::bitrateToLevel(v.m_bitrate), bitrate, color);
        item->setText(bitrate);
        item->setForeground(color);
        QtItemSetTextAlignment(item, Qt::AlignCenter);
        setItem(i, 5, item);

                item = new QTableWidgetItem;
        item->setData(TTK_DISPLAY_ROLE, tr("Play"));
        item->setData(TTK_DATA_ROLE, v.m_song.path());
        setItem(i, 6, item);

                item = new QTableWidgetItem;
        item->setData(TTK_DISPLAY_ROLE, tr("Delete"));
        setItem(i, 7, item);
    }
}

void MusicSongCheckToolsQualityTableWidget::itemCellClicked(int row, int column)
{
    MusicFillItemTableWidget::itemCellClicked(row, column);
    switch(column)
    {
        case 6: playSong(); break;
        case 7: removeCurrentRow(); break;
        default: break;
    }
}

void MusicSongCheckToolsQualityTableWidget::playSong()
{
    if(!isValid())
    {
        return;
    }

    const QTableWidgetItem *it = item(currentRow(), 6);
    if(it)
    {
        const QString &path = it->data(TTK_DATA_ROLE).toString();
        MusicSongsContainerWidget::instance()->addSongToPlaylist(QFile::exists(path) ? QStringList(path) : QStringList());
    }
}

void MusicSongCheckToolsQualityTableWidget::removeCurrentRow()
{
    if(!isValid())
    {
        return;
    }

    removeRow(currentRow());
}
