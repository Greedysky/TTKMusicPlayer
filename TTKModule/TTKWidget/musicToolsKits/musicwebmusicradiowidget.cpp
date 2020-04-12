#include "musicwebmusicradiowidget.h"
#include "musicradiochannelthread.h"
#include "musicwebmusicradioplaywidget.h"
#include "musicwidgetutils.h"
#include "musicuiobject.h"
#include "musicdownloadsourcethread.h"
#include "musicwindowsmanager.h"
#include "musicwidgetheaders.h"
#include "musicitemdelegate.h"
#include "musiccoreutils.h"
#include "musicimageutils.h"

#ifdef TTK_GREATER_NEW
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif
#include <QNetworkCookieJar>

#define ICON_SIZE       50

MusicWebMusicRadioWidget::MusicWebMusicRadioWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent), m_musicRadio(nullptr), m_getChannelThread(nullptr)
{
    setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    setColumnCount(4);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, ICON_SIZE + 20);
    headerview->resizeSection(2, 157);
    headerview->resizeSection(3, 75);

    m_outerIndex = -1;
    m_cookJar = new QNetworkCookieJar(this);

    MusicUtils::Widget::setTransparent(this, 0);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle03);

    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemCellDoubleClicked(int,int)));

}

MusicWebMusicRadioWidget::~MusicWebMusicRadioWidget()
{
    delete m_cookJar;
    delete m_musicRadio;
    delete m_getChannelThread;
}

void MusicWebMusicRadioWidget::initListItems(int index)
{
    m_outerIndex = index;
    if(rowCount() == 0)
    {
        delete m_getChannelThread;
        m_getChannelThread = new MusicRadioChannelThread(this, m_cookJar);
        connect(m_getChannelThread, SIGNAL(downLoadDataChanged(QString)), SLOT(addListWidgetItem()));
        m_getChannelThread->startToDownload(QString());
    }
}

void MusicWebMusicRadioWidget::itemCellEntered(int row, int column)
{
    QTableWidgetItem *it = item(m_previousColorRow, 3);
    if(it)
    {
        it->setIcon(QIcon());
    }

    if((it = item(row, 3)))
    {
        it->setIcon(QIcon(":/contextMenu/btn_play"));
    }

    if(column == 3)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }

    MusicAbstractTableWidget::itemCellEntered(row, column);
}

void MusicWebMusicRadioWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(row);

    if(column == 3)
    {
        itemCellDoubleClicked(row, DEFAULT_LEVEL_LOWER);
    }
}

void MusicWebMusicRadioWidget::itemCellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    if(!m_getChannelThread)
    {
        return;
    }

    const MusicRadioChannelInfos &channels = m_getChannelThread->getMusicChannel();
    if(m_musicRadio == nullptr)
    {
        m_musicRadio = new MusicWebMusicRadioPlayWidget(this);
        m_musicRadio->setNetworkCookie(m_cookJar);
    }

    if(!channels.isEmpty())
    {
        m_musicRadio->updateRadioList(channels[row].m_id);
    }
    m_musicRadio->show();
}

void MusicWebMusicRadioWidget::addListWidgetItem()
{
    const MusicRadioChannelInfos &channels = m_getChannelThread->getMusicChannel();
    foreach(const MusicRadioChannelInfo &channel, channels)
    {
        const int index = rowCount();
        setRowCount(index + 1);
        setRowHeight(index, ITEM_ROW_HEIGHT_XL);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(MUSIC_DATAS_ROLE, channel.m_coverUrl);
        setItem(index, 0, item);

                          item = new QTableWidgetItem;
        item->setIcon(MusicUtils::Image::pixmapToRound(QPixmap(":/image/lb_defaultArt"), QPixmap(":/usermanager/lb_mask_50"), iconSize()));
        setItem(index, 1, item);

                          item = new QTableWidgetItem;
        QFont f = item->font();
        f.setBold(true);
        item->setFont(f);
        item->setText(channel.m_name);
#if TTK_QT_VERSION_CHECK(5,13,0)
        item->setForeground(QColor(100, 100, 100));
#else
        item->setTextColor(QColor(100, 100, 100));
#endif
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(index, 2, item);

                          item = new QTableWidgetItem;
        setItem(index, 3, item);

        MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
        connect(download, SIGNAL(downLoadExtDataChanged(QByteArray,QVariantMap)), SLOT(downLoadFinished(QByteArray,QVariantMap)));
        if(!channel.m_coverUrl.isEmpty() && channel.m_coverUrl != COVER_URL_NULL)
        {
            QVariantMap map;
            map["id"] = index;
            download->setRawData(map);
            download->startToDownload(channel.m_coverUrl);
        }
    }

    //radio outer flag
    if(m_outerIndex != -1)
    {
        selectRow(m_outerIndex);
        itemCellDoubleClicked(m_outerIndex, DEFAULT_LEVEL_LOWER);
    }
}

void MusicWebMusicRadioWidget::downLoadFinished(const QByteArray &data, const QVariantMap &ext)
{
    QTableWidgetItem *it = item(ext["id"].toInt(), 1);
    if(it)
    {
        QPixmap pix;
        pix.loadFromData(data);
        it->setIcon(MusicUtils::Image::pixmapToRound(pix, QPixmap(":/usermanager/lb_mask_50"), iconSize()));
    }
}

void MusicWebMusicRadioWidget::musicPlayClicked()
{
    const int row = currentRow();
    if(row >= 0)
    {
        itemCellDoubleClicked(row, DEFAULT_LEVEL_LOWER);
    }
}

void MusicWebMusicRadioWidget::sendToDesktopLink()
{
    const int row = currentRow();
    if(row < 0)
    {
        return;
    }

    QString fileName("Radio");
    QTableWidgetItem *it = nullptr;
    if((it = item(row, 2)))
    {
        fileName = it->text();
    }

#ifdef TTK_GREATER_NEW
    const QString &desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
#else
    const QString &desktop = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
#endif

    MusicWindowsManager win;
#ifdef Q_OS_WIN
    win.setFileLink(MAIN_DIR_FULL + APP_EXE_NAME, desktop + "/" + fileName + ".lnk", QString(), QString("%1 \"%2\"").arg(MUSIC_OUTER_RADIO).arg(row), tr("TTK Radio Link"));
#else
    win.setFileLink(QString(" %1 \"%2\"").arg(MUSIC_OUTER_RADIO).arg(row), desktop, MAIN_DIR_FULL + APP_NAME, MusicObject::getAppDir(), fileName);
#endif
}

void MusicWebMusicRadioWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);

    rightClickMenu.addAction(tr("musicPlay"), this, SLOT(musicPlayClicked()));
    rightClickMenu.addSeparator();
    rightClickMenu.addAction(QIcon(":/contextMenu/btn_mobile"), tr("songToMobile"));
    rightClickMenu.addAction(tr("sendToDesktop"), this, SLOT(sendToDesktopLink()));

    rightClickMenu.exec(QCursor::pos());
}
