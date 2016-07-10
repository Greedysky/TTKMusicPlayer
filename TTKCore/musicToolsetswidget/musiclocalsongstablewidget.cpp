#include "musiclocalsongstablewidget.h"

MusicLocalSongsTableWidget::MusicLocalSongsTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    MusicUtils::UWidget::setTransparent(this, 150);
}

QString MusicLocalSongsTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLocalSongsTableWidget::clearShowlist()
{
    MusicAbstractTableWidget::clear();
    setColumnCount(5);
}

void MusicLocalSongsTableWidget::clearShowPath()
{
    MusicAbstractTableWidget::clear();
    setColumnCount(1);
}

void MusicLocalSongsTableWidget::createShowlist()
{
    clearShowlist();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 315);
    headerview->resizeSection(1, 55);
    headerview->resizeSection(2, 105);
    headerview->resizeSection(3, 26);
    headerview->resizeSection(4, 26);
}

void MusicLocalSongsTableWidget::createShowPath()
{
    clearShowPath();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 528);
}

void MusicLocalSongsTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}
