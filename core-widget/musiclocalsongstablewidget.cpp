#include "musiclocalsongstablewidget.h"

MusicLocalSongsTableWidget::MusicLocalSongsTableWidget(QWidget *parent)
    : MusicTableWidgetAbstract(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(this, SIGNAL(cellEntered(int,int)), SLOT(listCellEntered(int,int)));
}

void MusicLocalSongsTableWidget::clearShowlist()
{
    MusicTableWidgetAbstract::clearAllItems();
    setColumnCount(5);
}

void MusicLocalSongsTableWidget::clearShowPath()
{
    MusicTableWidgetAbstract::clearAllItems();
    setColumnCount(1);
}

void MusicLocalSongsTableWidget::createShowlist()
{
    clearShowlist();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,285);
    headerview->resizeSection(1,55);
    headerview->resizeSection(2,105);
    headerview->resizeSection(3,26);
    headerview->resizeSection(4,26);
}

void MusicLocalSongsTableWidget::createShowPath()
{
    clearShowPath();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,498);
}
