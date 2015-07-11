#include "musictoolsetswidget.h"
#include "../musicapplication.h"
#include "../toolsetswidget/musicaudiorecorderwidget.h"
#include "../toolsetswidget/musictimerwidget.h"
#include "../toolsetswidget/musiclocalsongsmanagerwidget.h"
#include "../toolsetswidget/musictransformwidget.h"
#include "../toolsetswidget/musicspectrumwidget.h"
#include "../toolsetswidget/musicdesktopwallpaperwidget.h"
#include <QProcess>

MusicToolSetsWidget::MusicToolSetsWidget(MusicApplication *parent) :
        QListWidget(parent),m_parentWidget(parent),m_process(NULL),
        m_musicSpectrumWidget(NULL),m_wallpaper(NULL)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFrameShape(QFrame::NoFrame);//Set No Border
    setStyleSheet(MusicObject::MusicScrollBarStyle);
    setIconSize(QSize(60,60));
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    setSpacing(15);
    setTransparent(50);
    connect(this,SIGNAL(itemClicked(QListWidgetItem*)),
            SLOT(itemHasClicked(QListWidgetItem*)));

    addListWidgetItem();
}

MusicToolSetsWidget::~MusicToolSetsWidget()
{
    delete m_wallpaper;
    delete m_musicSpectrumWidget;
    delete m_process;
    clearAllItems();
}

void MusicToolSetsWidget::setSongStringList(const QStringList& l)
{
    m_songlist = l;
}

void MusicToolSetsWidget::setTransparent(int angle)
{
    QPalette pal = palette();
    pal.setBrush(QPalette::Base,QBrush(QColor(255,255,255,angle)));
    setPalette(pal);
    repaint();
}

void MusicToolSetsWidget::clearAllItems()
{
    clear();
}

void MusicToolSetsWidget::addListWidgetItem()
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/tools/localmanager")
                                                ,tr("localmanager"),this);
    item->setSizeHint(QSize(80,90));
    addItem(item);
    QListWidgetItem *item1 = new QListWidgetItem(QIcon(":/tools/recorder")
                                                ,tr("recorder"),this);
    item1->setSizeHint(QSize(80,90));
    addItem(item1);
    QListWidgetItem *item2 = new QListWidgetItem(QIcon(":/tools/bell")
                                                ,tr("bell"),this);
    item2->setSizeHint(QSize(80,90));
    addItem(item2);
    QListWidgetItem *item3 = new QListWidgetItem(QIcon(":/tools/timer")
                                                ,tr("timer"),this);
    item3->setSizeHint(QSize(80,90));
    addItem(item3);
    QListWidgetItem *item4 = new QListWidgetItem(QIcon(":/tools/transform")
                                                ,tr("transform"),this);
    item4->setSizeHint(QSize(80,90));
    addItem(item4);
    QListWidgetItem *item5 = new QListWidgetItem(QIcon(":/tools/spectrum")
                                                ,tr("spectrum"),this);
    item5->setSizeHint(QSize(80,90));
    addItem(item5);
    QListWidgetItem *item6 = new QListWidgetItem(QIcon(":/tools/wallpaper")
                                                ,tr("wallpaper"),this);
    item6->setSizeHint(QSize(80,90));
    addItem(item6);
}

void MusicToolSetsWidget::itemHasClicked(QListWidgetItem* it)
{
    switch(row(it))
    {
        case 0:
           {
                MusicLocalSongsManagerWidget local(this);
                connect(&local,SIGNAL(addSongToPlay(QStringList)),
                        m_parentWidget,SLOT(addSongToPlayList(QStringList)));
                local.exec();
                break;
           }
        case 1:
            MusicAudioRecorderWidget(this).exec();
            break;
        case 2:
           {
                if(!QFile(MAKE_RING).exists())
                    return;
                if(m_process)
                    delete m_process;
                m_process = new QProcess(this);
                m_process->start(MAKE_RING);
                break;
           }
        case 3:
           {
                MusicTimerWidget timer(this);
                timer.setSongStringList(m_songlist);
                connect(&timer,SIGNAL(timerParameterChanged()),
                        m_parentWidget,SLOT(musicToolSetsParameter()));
                timer.exec();
                break;
            }
        case 4:
            MusicTransformWidget(this).exec();
            break;
        case 5:
           {
                delete m_musicSpectrumWidget;
                m_musicSpectrumWidget = new MusicSpectrumWidget;
                connect(m_musicSpectrumWidget,SIGNAL(setSpectrum(HWND,int,int)),
                        SIGNAL(setSpectrum(HWND,int,int)));
                m_musicSpectrumWidget->show();
                break;
           }
        case 6:
           {
                delete m_wallpaper;
                m_wallpaper = new MusicDesktopWallpaperWidget(this);
                m_wallpaper->show();
           }
    }
}

void MusicToolSetsWidget::setCurrentFileName(QString& name)
{
    name = m_parentWidget->getCurrentFileName();
}
