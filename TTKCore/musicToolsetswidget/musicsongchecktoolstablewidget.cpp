#include "musicsongchecktoolstablewidget.h"
#include "musicitemdelegate.h"

MusicSongCheckToolsRenameTableWidget::MusicSongCheckToolsRenameTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setColumnCount(3);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 290);
    headerview->resizeSection(2, 290);
}

MusicSongCheckToolsRenameTableWidget::~MusicSongCheckToolsRenameTableWidget()
{

}

QString MusicSongCheckToolsRenameTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsRenameTableWidget::createAllItems(const MusicSongs &songs)
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

void MusicSongCheckToolsRenameTableWidget::selectedAllItems(bool check)
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



MusicSongCheckToolsDuplicateTableWidget::MusicSongCheckToolsDuplicateTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setColumnCount(3);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 290);
    headerview->resizeSection(2, 290);
}

MusicSongCheckToolsDuplicateTableWidget::~MusicSongCheckToolsDuplicateTableWidget()
{

}

QString MusicSongCheckToolsDuplicateTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsDuplicateTableWidget::createAllItems(const MusicSongs &songs)
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

void MusicSongCheckToolsDuplicateTableWidget::selectedAllItems(bool check)
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



MusicSongCheckToolsQualityTableWidget::MusicSongCheckToolsQualityTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setColumnCount(3);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 290);
    headerview->resizeSection(2, 290);
}

MusicSongCheckToolsQualityTableWidget::~MusicSongCheckToolsQualityTableWidget()
{

}

QString MusicSongCheckToolsQualityTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsQualityTableWidget::createAllItems(const MusicSongs &songs)
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

void MusicSongCheckToolsQualityTableWidget::selectedAllItems(bool check)
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
