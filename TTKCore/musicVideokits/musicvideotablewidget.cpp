#include "musicvideotablewidget.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicdownloadwidget.h"
#include "musicitemdelegate.h"
#include "musictime.h"

MusicVideoTableWidget::MusicVideoTableWidget(QWidget *parent)
    : MusicQueryItemTableWidget(parent)
{
    setColumnCount(8);
    resizeWindow(0);

    MusicTime::timeSRand();
    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicVideoTableWidget::~MusicVideoTableWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    clearAllItems();
}

QString MusicVideoTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicVideoTableWidget::startSearchQuery(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())
    {   //no network connection
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }
    MusicQueryItemTableWidget::startSearchQuery(text);
    m_downLoadManager->startSearchSong(MusicDownLoadQueryThreadAbstract::MovieQuery, text);
}

void MusicVideoTableWidget::musicDownloadLocal(int row)
{
    if(row < 0 || (row >= rowCount() - 1))
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }
    downloadLocalMovie(row);
}

void MusicVideoTableWidget::resizeWindow(int delta)
{
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 305 + delta*0.4);
    headerview->resizeSection(2, 160 + delta*0.4);
    headerview->resizeSection(3, 31 + delta*0.1);
    headerview->resizeSection(4, 55 + delta*0.1);
    headerview->resizeSection(5, 24);
    headerview->resizeSection(6, 24);
    headerview->resizeSection(7, 24);

    //resize row
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight,
                                                   headerview->sectionSize(1) - 5));
        it = item(i, 2);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight,
                                                   headerview->sectionSize(2) - 5));
    }
}

void MusicVideoTableWidget::listCellEntered(int row, int column)
{
    MusicQueryItemTableWidget::listCellEntered(row, column);
    if(column == 5 || column == 6 || column == 7)
    {
       setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
       unsetCursor();
    }
}

void MusicVideoTableWidget::listCellClicked(int row, int column)
{
    MusicQueryItemTableWidget::listCellClicked(row, column);
    switch(column)
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

void MusicVideoTableWidget::clearAllItems()
{
    MusicQueryItemTableWidget::clearAllItems();
    setColumnCount(8);
}

void MusicVideoTableWidget::createSearchedItems(const QString &songname,
                                                const QString &artistname,
                                                const QString &time)
{
    int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerview = horizontalHeader();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::Widget::elidedText(font(), songname, Qt::ElideRight,
                                                 headerview->sectionSize(1) - 5));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(songname);
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setText(MusicUtils::Widget::elidedText(font(), artistname, Qt::ElideRight,
                                                 headerview->sectionSize(2) - 5));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(artistname);
    setItem(count, 2, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(randToGetStrength()));
    setItem(count, 3, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 4, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_mv")));
    setItem(count, 5, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_audition")));
    setItem(count, 6, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_download")));
    setItem(count, 7, item);
}

void MusicVideoTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0 || row < 0 || (row >= rowCount() - 1))
    {
        return;
    }

    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    MusicObject::MusicSongAttributes attrs = musicSongInfos[row].m_songAttrs;
    if(!attrs.isEmpty())
    {
        emit mvURLNameChanged(item(row, 2)->toolTip() + " - " + item(row, 1)->toolTip(),
                              attrs.first().m_url);
    }
}

void MusicVideoTableWidget::getMusicMvInfo(MusicObject::MusicSongAttributes &data)
{
    if(!m_downLoadManager)
    {
        return;
    }
    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    data = (!musicSongInfos.isEmpty() && m_previousClickRow != -1) ?
            musicSongInfos[m_previousClickRow].m_songAttrs : MusicObject::MusicSongAttributes();
}

void MusicVideoTableWidget::downloadLocalFromControl()
{
    if( m_previousClickRow != -1 && currentRow() != -1)
    {
        downloadLocalMovie(currentRow());
    }
}

void MusicVideoTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryItemTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    createContextMenu(rightClickMenu);

    rightClickMenu.exec(QCursor::pos());
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

void MusicVideoTableWidget::downloadLocalMovie(int row)
{
    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row < 0 || row >= musicSongInfos.count())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], MusicDownLoadQueryThreadAbstract::MovieQuery);
    download->show();
}
