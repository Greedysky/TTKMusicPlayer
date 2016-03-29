#include "musicvideotablewidget.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musictime.h"
#include "musicdownloadmgmtwidget.h"
#include "musicitemdelegate.h"

MusicVideoTableWidget::MusicVideoTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    setColumnCount(8);
    resizeWindow(1.0f);
    MusicUtils::setTransparent(this, 255);

    MusicTime::timeSRand();
    M_CONNECTION->setValue("MusicVideoTableWidget", this);
}

MusicVideoTableWidget::~MusicVideoTableWidget()
{
    M_CONNECTION->poolDisConnect("MusicVideoTableWidget");
    clearAllItems();
}

void MusicVideoTableWidget::startSearchQuery(const QString &text)
{
    if(!M_NETWORK->isOnline())
    {   //no network connection
        emit showDownLoadInfoFor(MusicObject::DisConnection);
        return;
    }
    m_downLoadManager->startSearchSong(MusicDownLoadQueryThreadAbstract::MovieQuery, text);
}

void MusicVideoTableWidget::clearAllItems()
{
    MusicAbstractTableWidget::clear();
    setColumnCount(8);
}

QString MusicVideoTableWidget::randToGetStrength() const
{
    switch(qrand()%5)
    {
        case 0: return QString::fromUtf8(":/video/video_1");
        case 1: return QString::fromUtf8(":/video/video_2");
        case 2: return QString::fromUtf8(":/video/video_3");
        case 3: return QString::fromUtf8(":/video/video_4");
        case 4: return QString::fromUtf8(":/video/video_5");
        default:return QString::fromUtf8(":/video/video_5");
    }
}

void MusicVideoTableWidget::createSearchedItems(const QString &songname,
                                                const QString &artistname,
                                                const QString &time)
{
    int count;
    setRowCount(count = m_downLoadManager->getSongIdIndex());
    QHeaderView *headerview = horizontalHeader();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    setItem(count - 1, 0, item);

                      item = new QTableWidgetItem;
    item->setText(QFontMetrics(font()).elidedText(songname, Qt::ElideRight,
                                                  headerview->sectionSize(1) - 5));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(songname);
    setItem(count - 1, 1, item);

                      item = new QTableWidgetItem;
    item->setText(QFontMetrics(font()).elidedText(artistname, Qt::ElideRight,
                                                  headerview->sectionSize(2) - 5));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(artistname);
    setItem(count - 1, 2, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(randToGetStrength()));
    setItem(count - 1, 3, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 4, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/share/showMV")));
    setItem(count - 1, 5, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/share/autionplay")));
    setItem(count - 1, 6, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/share/musicdownload")));
    setItem(count - 1, 7, item);
}

void MusicVideoTableWidget::listCellClicked(int row, int col)
{
    MusicQueryTableWidget::listCellClicked(row, col);
    switch(col)
    {
        case 5:
        case 6:
            itemDoubleClicked(row, 999);
            break;
        case 7:
            musicDownloadLocal(row);
            break;
        default:break;
    }
}

void MusicVideoTableWidget::musicDownloadLocal(int row)
{
    if(row < 0)
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    downloadLocalMovie(row);
}

void MusicVideoTableWidget::downloadLocalFromControl()
{
    if( m_previousClickRow != -1 && currentRow() != -1)
    {
        downloadLocalMovie(currentRow());
    }
}

void MusicVideoTableWidget::downloadLocalMovie(int row)
{
    MusicDownloadMgmtWidget mgmt(this);
    mgmt.setSongName(item(row, 2)->text() + " - " + item(row, 1)->text(),
                     MusicDownLoadQueryThreadAbstract::MovieQuery);
}

void MusicVideoTableWidget::resizeWindow(float delta)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30*delta);
    headerview->resizeSection(1, 210*delta);
    headerview->resizeSection(2, 136*delta);
    headerview->resizeSection(3, 30*delta);
    headerview->resizeSection(4, 55*delta);
    headerview->resizeSection(5, 24*delta);
    headerview->resizeSection(6, 24*delta);
    headerview->resizeSection(7, 24*delta);

    //resize row
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(QFontMetrics(font()).elidedText(it->toolTip(), Qt::ElideRight,
                                                    headerview->sectionSize(1) - 5));
        it = item(i, 2);
        it->setText(QFontMetrics(font()).elidedText(it->toolTip(), Qt::ElideRight,
                                                    headerview->sectionSize(2) - 5));
    }
}

void MusicVideoTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0)
    {
        return;
    }
    MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    emit mvURLNameChanged(item(row, 2)->toolTip() + " - " + item(row, 1)->toolTip(),
                          musicSongInfos[row].m_songAttrs.first().m_url);
}

void MusicVideoTableWidget::getMusicMvInfo(MusicSongAttributes &data)
{
    MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    data = (!musicSongInfos.isEmpty() && m_previousClickRow != -1) ?
            musicSongInfos[m_previousClickRow].m_songAttrs : MusicSongAttributes();
}

void MusicVideoTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    createContextMenu(rightClickMenu);

    rightClickMenu.exec(QCursor::pos());
}
