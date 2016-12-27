#include "musictoolsetswidget.h"
#include "musicaudiorecorderwidget.h"
#include "musictimerwidget.h"
#include "musiclocalsongsmanagerwidget.h"
#include "musictransformwidget.h"
#include "musicdesktopwallpaperwidget.h"
#include "musicnetworkspeedtestwidget.h"
#include "musicnetworkconnectiontestwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicvolumegainwidget.h"
#include "musicsoundtouchwidget.h"
#include "musicsongringtonemakerwidget.h"
#include "musicgrabwidget.h"
#include "musicmessagebox.h"
#include "musicapplication.h"
#include "musicleftareawidget.h"
#include "musicrightareawidget.h"

#include <QTimer>

MusicToolSetsWidget::MusicToolSetsWidget(QWidget *parent)
    : QListWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setFrameShape(QFrame::NoFrame);
    setStyleSheet(MusicUIObject::MScrollBarStyle02 + \
                  MusicUIObject::MScrollBarStyle03.arg(50));
    setIconSize(QSize(60, 60));
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);

    m_containItem = nullptr;
#ifdef Q_OS_WIN
    setSpacing(17);
    addListWidgetItem();
#else
    setSpacing(16);
    QTimer::singleShot(MT_MS, this, SLOT(addListWidgetItem()));
#endif
    MusicUtils::Widget::setTransparent(this, 50);
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)),
                  SLOT(itemHasClicked(QListWidgetItem*)));
}

MusicToolSetsWidget::~MusicToolSetsWidget()
{
    delete m_containItem;
    clearAllItems();
}

QString MusicToolSetsWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicToolSetsWidget::clearAllItems()
{
    clear();
}

void MusicToolSetsWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicToolSetsWidget::addListWidgetItem()
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/tools/lb_localmanager")
                                                ,tr("localmanager"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_recorder")
                                                ,tr("recorder"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_bell")
                                                ,tr("bell"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_timer")
                                                ,tr("timer"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_transform")
                                                ,tr("transform"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_spectrum")
                                                ,tr("spectrum"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_wallpaper")
                                                ,tr("wallpaper"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_phone")
                                                ,tr("phone"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_speed")
                                                ,tr("speed"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_connections")
                                                ,tr("connections"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_gain")
                                                ,tr("gain"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_detect")
                                                ,tr("detect"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_soundtouch")
                                                ,tr("soundtouch"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_grabwindow")
                                                ,tr("grabwindow"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
}

void MusicToolSetsWidget::itemHasClicked(QListWidgetItem *item)
{
    switch( row(item) )
    {
        case 0:
            {
                MusicLocalSongsManagerWidget(this).exec();
                break;
            }
        case 1:
            {
                MusicAudioRecorderWidget(this).exec();
                break;
            }
        case 2:
            {
                MusicSongRingtoneMaker(this).exec();
                break;
            }
        case 3:
            {
                MusicTimerWidget timer(this);
                QStringList songlist;
                MusicApplication::instance()->getCurrentPlayList(songlist);
                timer.setSongStringList(songlist);
                timer.exec();
                break;
            }
        case 4:
            {
                MusicTransformWidget(this).exec();
                break;
            }
        case 5:
            {
                MusicLeftAreaWidget::instance()->musicAnalyzerSpectrumWidget();
                break;
            }
        case 6:
            {
#ifdef Q_OS_WIN
                delete m_containItem;
                MusicDesktopWallpaperWidget *w = new MusicDesktopWallpaperWidget(this);
                m_containItem = w;
                w->show();
#endif
                break;
            }
        case 7:
            {
                MusicConnectTransferWidget(this).exec();
                break;
            }
        case 8:
            {
                delete m_containItem;
                MusicNetworkSpeedTestWidget *w = new MusicNetworkSpeedTestWidget(this);
                m_containItem = w;
                w->show();
                break;
            }
        case 9:
            {
                delete m_containItem;
                MusicNetworkConnectionTestWidget *w = new MusicNetworkConnectionTestWidget(this);
                m_containItem = w;
                w->show();
                break;
            }
        case 10:
            {
                MusicVolumeGainWidget(this).exec();
                break;
            }
        case 11:
            {
                MusicRightAreaWidget::instance()->musicFunctionClicked(MusicRightAreaWidget::IndentifyWidget);
                break;
            }
        case 12:
            {
                MusicSoundTouchWidget(this).exec();
                break;
            }
        case 13:
            {
                (new MusicGrabWidget)->show();
                break;
            }
        default:
            break;
    }
}
