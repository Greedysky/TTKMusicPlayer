#include "musictoolsetswidget.h"
#include "musicaudiorecorderwidget.h"
#include "musictimerwidget.h"
#include "musiclocalsongsmanagerwidget.h"
#include "musictransformwidget.h"
#include "musicdesktopwallpaperwidget.h"
#include "musicconnectionpool.h"
#include "musicnetworktestwidget.h"
#include "musicconnecttransferwidget.h"
#include "musicvolumegainwidget.h"
#include "musicsoundtouchwidget.h"
#include "musicsongringtonemakerwidget.h"
#include "musicmessagebox.h"
#include "musicutils.h"
#include "musicapplication.h"
#include "musicleftareawidget.h"

#include <QTimer>

MusicToolSetsWidget::MusicToolSetsWidget(QWidget *parent)
    : QListWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFrameShape(QFrame::NoFrame);//Set No Border
    setStyleSheet(MusicUIObject::MScrollBarStyle01);
    setIconSize(QSize(60, 60));
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);

#ifdef Q_OS_WIN
    setSpacing(17);
    addListWidgetItem();
#else
    setSpacing(16);
    QTimer::singleShot(MT_MS, this, SLOT(addListWidgetItem()));
#endif
    MusicUtils::UWidget::setTransparent(this, 50);
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)),
                  SLOT(itemHasClicked(QListWidgetItem*)));

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicApplication::getClassName());
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicLeftAreaWidget::getClassName());
}

MusicToolSetsWidget::~MusicToolSetsWidget()
{
    M_CONNECTION_PTR->poolDisConnect(getClassName());
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
                     item = new QListWidgetItem(QIcon(":/tools/lb_gain")
                                                ,tr("gain"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/lb_soundtouch")
                                                ,tr("soundtouch"), this);
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
                emit getCurrentPlayList(songlist);
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
                emit showSpectrumWidget();
                break;
            }
        case 6:
            {
#ifdef Q_OS_WIN
                (new MusicDesktopWallpaperWidget(this))->show();
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
                (new MusicNetworkTestWidget(this))->show();
                break;
            }
        case 9:
            {
                MusicVolumeGainWidget(this).exec();
                break;
            }
        case 10:
            {
                MusicSoundTouchWidget(this).exec();
                break;
            }
        default:
            break;
    }
}
