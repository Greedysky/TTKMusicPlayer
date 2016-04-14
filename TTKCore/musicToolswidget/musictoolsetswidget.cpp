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
#include "musicmessagebox.h"
#include "musicutils.h"

#include <QProcess>
#include <QTimer>

MusicToolSetsWidget::MusicToolSetsWidget(QWidget *parent)
    : QListWidget(parent), m_process(nullptr)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFrameShape(QFrame::NoFrame);//Set No Border
    setStyleSheet(MusicUIObject::MScrollBarStyle01);
    setIconSize(QSize(60, 60));
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
#ifdef Q_OS_WIN
    setSpacing(20);
    addListWidgetItem();
#else
    setSpacing(19);
    QTimer::singleShot(1, this, SLOT(addListWidgetItem()));
#endif
    MusicUtils::setTransparent(this, 50);
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)),
                  SLOT(itemHasClicked(QListWidgetItem*)));

    M_CONNECTION->setValue("MusicToolSetsWidget", this);
    M_CONNECTION->poolConnect("MusicToolSetsWidget", "MusicApplication");
    M_CONNECTION->poolConnect("MusicToolSetsWidget", "MusicLeftAreaWidget");
}

MusicToolSetsWidget::~MusicToolSetsWidget()
{
    M_CONNECTION->poolDisConnect("MusicToolSetsWidget");
    if(m_process)
    {
        m_process->kill();
    }
    delete m_process;
    clearAllItems();
}

void MusicToolSetsWidget::clearAllItems()
{
    clear();
}

void MusicToolSetsWidget::addListWidgetItem()
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/tools/localmanager")
                                                ,tr("localmanager"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/recorder")
                                                ,tr("recorder"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/bell")
                                                ,tr("bell"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/timer")
                                                ,tr("timer"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/transform")
                                                ,tr("transform"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/spectrum")
                                                ,tr("spectrum"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/wallpaper")
                                                ,tr("wallpaper"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/phone")
                                                ,tr("phone"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/speed")
                                                ,tr("speed"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
                     item = new QListWidgetItem(QIcon(":/tools/gain")
                                                ,tr("gain"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);

}

void MusicToolSetsWidget::itemHasClicked(QListWidgetItem *item)
{
    switch(row(item))
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
#ifdef Q_OS_WIN
                if(!QFile(MAKE_RING_AL).exists())
                {
                    MusicMessageBox message;
                    message.setText(tr("Lack of plugin file!"));
                    message.exec();
                    return;
                }
                if(m_process)
                {
                    m_process->kill();
                    delete m_process;
                }
                m_process = new QProcess(this);
                m_process->start(MAKE_RING_AL);
#endif
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
        default:
            break;
    }
}
