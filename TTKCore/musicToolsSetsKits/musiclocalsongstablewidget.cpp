#include "musiclocalsongstablewidget.h"
#include "musicnumberutils.h"

#define ROW_HEIGHT 60
#define MUSIC_INFO_ROLE Qt::UserRole + 1000
Q_DECLARE_METATYPE(QFileInfoList)

MusicLocalSongsTableWidget::MusicLocalSongsTableWidget(QWidget *parent)
    : MusicSongsListAbstractTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    MusicUtils::Widget::setTransparent(this, 150);

    setColumnCount(5);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 315);
    headerview->resizeSection(1, 55);
    headerview->resizeSection(2, 105);
    headerview->resizeSection(3, 26);
    headerview->resizeSection(4, 26);

    m_musicSongs = new MusicSongs;
}

MusicLocalSongsTableWidget::~MusicLocalSongsTableWidget()
{
    clear();
    delete m_musicSongs;
}

QString MusicLocalSongsTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLocalSongsTableWidget::clear()
{
    MusicAbstractTableWidget::clear();
    m_musicSongs->clear();
}

void MusicLocalSongsTableWidget::addItems(const QFileInfoList &path)
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
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, 50));
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
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



MusicLocalSongsInfoTableWidget::MusicLocalSongsInfoTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    MusicUtils::Widget::setTransparent(this, 150);

    setColumnCount(3);
    setIconSize(QSize(50, 50));

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 60);
    headerview->resizeSection(1, 357);
    headerview->resizeSection(2, 110);
}

QString MusicLocalSongsInfoTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLocalSongsInfoTableWidget::clear()
{
    MusicAbstractTableWidget::clear();
}

void MusicLocalSongsInfoTableWidget::addItems(const MusicInfoData &data)
{
    MusicInfoDataIterator it(data);
    int i=0;
    while(it.hasNext())
    {
        it.next();
        setRowHeight(i, ROW_HEIGHT);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setIcon(QIcon(QPixmap(":/image/lb_defaultArt")));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 0, item);

                         item = new QTableWidgetItem;
        item->setToolTip(it.key());
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, 300));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                         item = new QTableWidgetItem;
        item->setText(QString("All Number %1").arg(it.value().count()));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        QVariant v;
        v.setValue(it.value());
        item->setData(MUSIC_INFO_ROLE, v);
        setItem(i, 2, item);

        ++i;
    }
}

void MusicLocalSongsInfoTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}
