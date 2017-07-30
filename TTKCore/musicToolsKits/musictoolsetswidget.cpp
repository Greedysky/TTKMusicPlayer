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
#include "musicrightareawidget.h"
#include "musicsoundkmicrowidget.h"
#include "musicmessagebox.h"
#include "musicspectrumwidget.h"

#include <QTimer>

#define NEW_OPERATOR(flag, type)  \
    if((m_toolsFlags & flag) != flag) \
    { \
        m_toolsFlags |= flag; \
        type *w= new type(MusicApplication::instance()); \
        connect(w, SIGNAL(resetFlag(MusicObject::ToolsType)), SLOT(resetFlag(MusicObject::ToolsType))); \
        w->raise(); \
        w->show(); \
    }


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
    clearAllItems();
}

QString MusicToolSetsWidget::getClassName()
{
    return staticMetaObject.className();
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
                     item = new QListWidgetItem(QIcon(":/tools/lb_ktv")
                                                ,tr("kmicro"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
}

void MusicToolSetsWidget::itemHasClicked(QListWidgetItem *item)
{
    switch( row(item) )
    {
        case 0:
            {
                NEW_OPERATOR(MusicObject::TT_LocalManager, MusicLocalSongsManagerWidget);
                break;
            }
        case 1:
            {
                NEW_OPERATOR(MusicObject::TT_AudioRecord, MusicAudioRecorderWidget);
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
                NEW_OPERATOR(MusicObject::TT_Spectrum, MusicSpectrumWidget);
                break;
            }
        case 6:
            {
#ifdef Q_OS_WIN
                NEW_OPERATOR(MusicObject::TT_Wallpaper, MusicDesktopWallpaperWidget);
#else
                MusicMessageBox message;
                message.setText(tr("Not Supported On Current Plantform!"));
                message.exec();
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
                NEW_OPERATOR(MusicObject::TT_SpeedTest, MusicNetworkSpeedTestWidget);
                break;
            }
        case 9:
            {
                NEW_OPERATOR(MusicObject::TT_ConnectionTest, MusicNetworkConnectionTestWidget);
                break;
            }
        case 10:
            {
                NEW_OPERATOR(MusicObject::TT_SoundGain, MusicVolumeGainWidget);
                break;
            }
        case 11:
            {
                MusicRightAreaWidget::instance()->musicFunctionClicked(MusicRightAreaWidget::IndentifyWidget);
                break;
            }
        case 12:
            {
                NEW_OPERATOR(MusicObject::TT_SoundTouch, MusicSoundTouchWidget);
                break;
            }
        case 13:
            {
                NEW_OPERATOR(MusicObject::TT_GrabWindow, MusicGrabWidget);
                break;
            }
        case 14:
            {
                NEW_OPERATOR(MusicObject::TT_SoundKMicro, MusicSoundKMicroWidget);
                break;
            }
        default:
            break;
    }
}

void MusicToolSetsWidget::resetFlag(MusicObject::ToolsType flag)
{
    m_toolsFlags &= ~flag;
}

void MusicToolSetsWidget::clearAllItems()
{
    clear();
}

void MusicToolSetsWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
