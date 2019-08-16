#include "musiclocalsongstablewidget.h"
#include "musicnumberutils.h"

#define MUSIC_INFO_ROLE     Qt::UserRole + 1000

Q_DECLARE_METATYPE(QFileInfoList)

MusicLocalSongsTableWidget::MusicLocalSongsTableWidget(QWidget *parent)
    : MusicSongsListAbstractTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    MusicUtils::Widget::setTransparent(this, 150);

    setColumnCount(5);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 405);
    headerview->resizeSection(1, 65);
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

void MusicLocalSongsTableWidget::clear()
{
    MusicAbstractTableWidget::clear();
    m_musicSongs->clear();
}

void MusicLocalSongsTableWidget::addItems(const QFileInfoList &path)
{
    QHeaderView *headerview = horizontalHeader();
    for(int i=0; i<path.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip(path[i].fileName());
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(0) - 20));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 0, item);

                         item = new QTableWidgetItem;
        item->setToolTip(MusicUtils::Number::size2Label(path[i].size()));
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 15));
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

    const bool empty = !m_musicSongs->isEmpty();
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
    headerview->resizeSection(1, 447);
    headerview->resizeSection(2, 120);

    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(listCellDoubleClicked(int,int)));
}

void MusicLocalSongsInfoTableWidget::clear()
{
    MusicAbstractTableWidget::clear();
}

void MusicLocalSongsInfoTableWidget::addItems(const MusicInfoData &data)
{
    QHeaderView *headerview = horizontalHeader();
    MusicInfoDataIterator it(data);
    int i=0;
    while(it.hasNext())
    {
        it.next();
        setRowHeight(i, ITEM_ROW_HEIGHT_XL);

        QTableWidgetItem *item = new QTableWidgetItem;
        QPixmap pix(ART_DIR_FULL + it.key() + SKN_FILE);
        if(pix.isNull())
        {
            pix.load(":/image/lb_defaultArt");
        }
        item->setIcon(QIcon(pix));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 0, item);

                         item = new QTableWidgetItem;
        item->setToolTip(it.key());
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                         item = new QTableWidgetItem;
        item->setText(tr("All Number %1").arg(it.value().count()));
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

void MusicLocalSongsInfoTableWidget::listCellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    QTableWidgetItem *it = item(row, 2);
    if(it)
    {
        QFileInfoList list = it->data(MUSIC_INFO_ROLE).value<QFileInfoList>();
        emit updateFileLists(list);
    }
}
