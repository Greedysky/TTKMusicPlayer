#include "musicwebmusicradiolistview.h"
#include "musicradiochannelthread.h"
#include "musicwebmusicradiowidget.h"
#include "musicutils.h"
#include "musicuiobject.h"

#include <QNetworkCookieJar>

MusicWebMusicRadioListView::MusicWebMusicRadioListView(QWidget *parent)
    : QListWidget(parent), m_getChannelThread(nullptr), m_musicRadio(nullptr)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFrameShape(QFrame::NoFrame);//Set No Border
    setStyleSheet(MusicUIObject::MScrollBarStyle01);
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    MusicUtils::setTransparent(this, 50);

    m_cookJar = new QNetworkCookieJar;
#ifdef Q_OS_WIN
    setSpacing(20);
#else
    setSpacing(19);
#endif
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(itemHasClicked(QListWidgetItem*)));
}

MusicWebMusicRadioListView::~MusicWebMusicRadioListView()
{
    delete m_getChannelThread;
    delete m_musicRadio;
    delete m_cookJar;
}

void MusicWebMusicRadioListView::initListItems()
{
    if(count() == 0)
    {
        delete m_getChannelThread;
        m_getChannelThread = new MusicRadioChannelThread(this, m_cookJar);
        connect(m_getChannelThread, SIGNAL(networkReplyFinished(QString)), SLOT(addListWidgetItem()));
        m_getChannelThread->startToDownload(QString());
    }
}

void MusicWebMusicRadioListView::addListWidgetItem()
{
    ChannelInfos channels = m_getChannelThread->getMusicChannel();

    foreach(ChannelInfo channel, channels)
    {
        QListWidgetItem *item = new QListWidgetItem(channel.m_name, this);
        item->setSizeHint(QSize(80, 30));
        addItem(item);
    }
}

void MusicWebMusicRadioListView::itemHasClicked(QListWidgetItem *item)
{
    ChannelInfos channels = m_getChannelThread->getMusicChannel();
    if(m_musicRadio == nullptr)
    {
        m_musicRadio = new MusicWebMusicRadioWidget(this);
        m_musicRadio->setNetworkCookie(m_cookJar);
    }

    if(!channels.isEmpty())
    {
        m_musicRadio->updateRadioList(channels[row(item)].m_id);
    }
    m_musicRadio->show();
}
