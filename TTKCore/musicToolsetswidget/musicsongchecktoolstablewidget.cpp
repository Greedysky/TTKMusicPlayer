#include "musicsongchecktoolstablewidget.h"

MusicSongCheckToolsRenameTableWidget::MusicSongCheckToolsRenameTableWidget(QWidget *parent)
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

MusicSongCheckToolsRenameTableWidget::~MusicSongCheckToolsRenameTableWidget()
{

}

QString MusicSongCheckToolsRenameTableWidget::getClassName()
{
    return staticMetaObject.className();
}
