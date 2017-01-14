#include "musiclocalsongstablewidget.h"

MusicLocalSongsTableWidget::MusicLocalSongsTableWidget(QWidget *parent)
    : MusicSongsListAbstractTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    MusicUtils::Widget::setTransparent(this, 150);

    m_musicSongs = new MusicSongs;
}

MusicLocalSongsTableWidget::~MusicLocalSongsTableWidget()
{
    delete m_musicSongs;
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

void MusicLocalSongsTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicSongsListAbstractTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);

    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_play"), tr("musicPlay"), this, SLOT(musicPlayClicked()));
    rightClickMenu.addAction(tr("downloadMore..."), this, SLOT(musicSongDownload()));
    rightClickMenu.addSeparator();

    createMoreMenu(&rightClickMenu);

    bool empty = !m_musicSongs->isEmpty();
    rightClickMenu.addAction(tr("musicInfo..."), this, SLOT(musicFileInformation()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_localFile"), tr("openFileDir"), this, SLOT(musicOpenFileDir()))->setEnabled(empty);
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("ablum"), this, SLOT(musicAlbumFoundWidget()));
    rightClickMenu.addSeparator();

    rightClickMenu.exec(QCursor::pos());
}
