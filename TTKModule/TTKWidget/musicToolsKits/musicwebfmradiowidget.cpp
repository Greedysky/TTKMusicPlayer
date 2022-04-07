#include "musicwebfmradiowidget.h"
#include "musicfmradiochannelrequest.h"
#include "musicwebfmradioplaywidget.h"
#include "musicdownloadsourcerequest.h"
#include "musicplatformmanager.h"
#include "musicwidgetheaders.h"
#include "musicitemdelegate.h"
#include "musicimageutils.h"

#if TTK_QT_VERSION_CHECK(5,0,0)
#  include <QStandardPaths>
#else
#  include <QDesktopServices>
#endif

#define ICON_SIZE       50

MusicWebFMRadioWidget::MusicWebFMRadioWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent),
      m_musicRadio(nullptr),
      m_getChannelThread(nullptr)
{
    setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    setColumnCount(4);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, ICON_SIZE + 20);
    headerview->resizeSection(2, 157);
    headerview->resizeSection(3, 75);

    m_outerIndex = -1;

    MusicUtils::Widget::setTransparent(this, 0);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle03);

    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));

}

MusicWebFMRadioWidget::~MusicWebFMRadioWidget()
{
    delete m_musicRadio;
    delete m_getChannelThread;
}

void MusicWebFMRadioWidget::initialize(int index)
{
    m_outerIndex = index;
    if(rowCount() == 0)
    {
        delete m_getChannelThread;
        m_getChannelThread = new MusicFMRadioChannelRequest(this);
        connect(m_getChannelThread, SIGNAL(downLoadDataChanged(QString)), SLOT(addListWidgetItem()));
        m_getChannelThread->startToDownload(QString());
    }
}

void MusicWebFMRadioWidget::itemCellEntered(int row, int column)
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

void MusicWebFMRadioWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(row);

    if(column == 3)
    {
        itemDoubleClicked(row, DEFAULT_NORMAL_LEVEL);
    }
}

void MusicWebFMRadioWidget::itemDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    if(!m_getChannelThread)
    {
        return;
    }

    const MusicFMRadioChannelDataList &channels = m_getChannelThread->musicChannel();
    if(m_musicRadio == nullptr)
    {
        m_musicRadio = new MusicWebFMRadioPlayWidget(this);
    }

    if(!channels.isEmpty())
    {
        m_musicRadio->updateRadioSong(channels[row].m_id);
    }
    m_musicRadio->show();
}

void MusicWebFMRadioWidget::addListWidgetItem()
{
    const MusicFMRadioChannelDataList &channels = m_getChannelThread->musicChannel();
    for(const MusicFMRadioChannelData &channel : qAsConst(channels))
    {
        const int index = rowCount();
        setRowCount(index + 1);
        setRowHeight(index, ITEM_ROW_HEIGHT_XL);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(MUSIC_DATA_ROLE, channel.m_coverUrl);
        setItem(index, 0, item);

                          item = new QTableWidgetItem;
        item->setIcon(MusicUtils::Image::pixmapToRound(QPixmap(":/image/lb_default_art"), QPixmap(":/image/lb_mask_50"), iconSize()));
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

        MusicDownloadSourceRequest *download = new MusicDownloadSourceRequest(this);
        connect(download, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        if(!channel.m_coverUrl.isEmpty() && channel.m_coverUrl != TTK_NULL_STR)
        {
            download->setHeader("id", index);
            download->startToDownload(channel.m_coverUrl);
        }
    }

    //radio outer flag
    if(m_outerIndex != -1)
    {
        selectRow(m_outerIndex);
        itemDoubleClicked(m_outerIndex, DEFAULT_NORMAL_LEVEL);
    }
}

void MusicWebFMRadioWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_LOGGER_ERROR("Input byte data is empty");
        return;
    }

    MusicDownloadSourceRequest *download(TTKStatic_cast(MusicDownloadSourceRequest*, sender()));
    if(!download)
    {
        return;
    }

    QTableWidgetItem *it = item(download->header("id").toInt(), 1);
    if(it)
    {
        QPixmap pix;
        pix.loadFromData(bytes);
        it->setIcon(MusicUtils::Image::pixmapToRound(pix, QPixmap(":/image/lb_mask_50"), iconSize()));
    }
}

void MusicWebFMRadioWidget::musicPlayClicked()
{
    const int row = currentRow();
    if(row >= 0)
    {
        itemDoubleClicked(row, DEFAULT_NORMAL_LEVEL);
    }
}

void MusicWebFMRadioWidget::sendToDesktopLink()
{
    const int row = currentRow();
    if(row < 0)
    {
        return;
    }

    QString fileName("Radio");
    QTableWidgetItem *it = item(row, 2);
    if(it)
    {
        fileName = it->text();
    }

#if TTK_QT_VERSION_CHECK(5,0,0)
    const QString &desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
#else
    const QString &desktop = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
#endif

    MusicPlatformManager platform;
#ifdef Q_OS_WIN
    platform.setFileLink(MusicObject::applicationPath() + APP_EXE_NAME, desktop + TTK_SEPARATOR + fileName + ".lnk", QString(), QString("%1 \"%2\"").arg(MUSIC_OUTSIDE_RADIO).arg(row), tr("TTK Radio Link"));
#else
    platform.setFileLink(QString(" %1 \"%2\"").arg(MUSIC_OUTSIDE_RADIO).arg(row), desktop, MAIN_DIR_FULL + APP_NAME, MusicObject::applicationPath(), fileName);
#endif
}

void MusicWebFMRadioWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractTableWidget::contextMenuEvent(event);

    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);

    menu.addAction(tr("Play"), this, SLOT(musicPlayClicked()));
    menu.addSeparator();
    menu.addAction(QIcon(":/contextMenu/btn_mobile"), tr("Song To Mobile"));
    menu.addAction(tr("Send To Desktop Link"), this, SLOT(sendToDesktopLink()));

    menu.exec(QCursor::pos());
}
