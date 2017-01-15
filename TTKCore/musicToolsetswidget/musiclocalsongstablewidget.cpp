#include "musiclocalsongstablewidget.h"
#include "musicnumberutils.h"

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
    m_musicSongs->clear();
    setColumnCount(5);
}

void MusicLocalSongsTableWidget::clearShowPath()
{
    MusicAbstractTableWidget::clear();
    m_musicSongs->clear();
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

void MusicLocalSongsTableWidget::addShowlistItems(const QFileInfoList &path)
{
    for(int i=0; i<path.count(); i++)
    {

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip(path[i].fileName());
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, 260));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 0, item);

                         item = new QTableWidgetItem;
        item->setToolTip(MusicUtils::Number::size2Label(path[i].size()));
        item->setText(MusicUtils::Widget::elidedText(font(), MusicUtils::Number::size2Number(path[i].size()),
                                                     Qt::ElideRight, 50));
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 1, item);

                         item = new QTableWidgetItem(path[i].lastModified().date().toString(Qt::ISODate));
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 2, item);

                         item = new QTableWidgetItem;
        item->setIcon(QIcon(":/contextMenu/btn_audition"));
        setItem(i, 3, item);

                         item = new QTableWidgetItem;
        item->setIcon(QIcon(":/contextMenu/btn_add"));
        setItem(i, 4, item);

        m_musicSongs->append(MusicSong(path[i].absoluteFilePath()));
    }
}

void MusicLocalSongsTableWidget::addShowPathItems(const QFileInfoList &path)
{
    for(int i=0; i<path.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip(path[i].absoluteFilePath());
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, 445));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 0, item);

        m_musicSongs->append(MusicSong(path[i].absoluteFilePath()));
    }
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
