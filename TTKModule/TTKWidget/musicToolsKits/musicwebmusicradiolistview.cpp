#include "musicwebmusicradiolistview.h"
#include "musicradiochannelthread.h"
#include "musicwebmusicradiowidget.h"
#include "musicwidgetutils.h"
#include "musicuiobject.h"
#include "musicrightareawidget.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QNetworkCookieJar>

MusicWebMusicRadioListView::MusicWebMusicRadioListView(QWidget *parent)
    : QListWidget(parent), m_getChannelThread(nullptr), m_musicRadio(nullptr)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setFrameShape(QFrame::NoFrame);
    setStyleSheet(MusicUIObject::MScrollBarStyle03);

    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);

    MusicUtils::Widget::setTransparent(this, 0);

    m_cookJar = new QNetworkCookieJar(this);
#ifdef Q_OS_WIN
    setSpacing(17);
#else
    setSpacing(16);
#endif
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(itemHasClicked(QListWidgetItem*)));
}

MusicWebMusicRadioListView::~MusicWebMusicRadioListView()
{
    delete m_getChannelThread;
    delete m_musicRadio;
    delete m_cookJar;
}

QString MusicWebMusicRadioListView::getClassName()
{
    return staticMetaObject.className();
}

void MusicWebMusicRadioListView::initListItems()
{
    if(count() == 0)
    {
        delete m_getChannelThread;
        m_getChannelThread = new MusicRadioChannelThread(this, m_cookJar);
        connect(m_getChannelThread, SIGNAL(downLoadDataChanged(QString)), SLOT(addListWidgetItem()));
        m_getChannelThread->startToDownload(QString());
    }
}

void MusicWebMusicRadioListView::addListWidgetItem()
{
    MusicRadioChannelInfos channels = m_getChannelThread->getMusicChannel();

    foreach(const MusicRadioChannelInfo &channel, channels)
    {
        QListWidgetItem *item = new QListWidgetItem(channel.m_name, this);
        item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
        item->setSizeHint(QSize(80, 30));
        addItem(item);
    }
}

void MusicWebMusicRadioListView::itemHasClicked(QListWidgetItem *item)
{
    MusicRadioChannelInfos channels = m_getChannelThread->getMusicChannel();
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



MusicWebRadioView::MusicWebRadioView(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_musicRadio = new MusicWebMusicRadioListView(this);

    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(40);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setSpacing(0);
    bottomLayout->setContentsMargins(5, 0, 0, 0);
    QPushButton *button = new QPushButton(this);
    button->setIcon(QIcon(":/desktopTool/btn_setting_hover"));
    button->setCursor(QCursor(Qt::PointingHandCursor));
    button->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    button->setFixedWidth(40);
#ifdef Q_OS_UNIX
    button->setFocusPolicy(Qt::NoFocus);
#endif

    bottomLayout->addStretch(1);
    bottomLayout->addWidget(button);
    bottomWidget->setLayout(bottomLayout);

    layout->addWidget(m_musicRadio);
    layout->addWidget(bottomWidget);
    setLayout(layout);

    connect(button, SIGNAL(clicked()), SLOT(openDJRadioWindow()));
}

MusicWebRadioView::~MusicWebRadioView()
{
    delete m_musicRadio;
}

QString MusicWebRadioView::getClassName()
{
    return staticMetaObject.className();
}

void MusicWebRadioView::initListItems()
{
    m_musicRadio->initListItems();
}

void MusicWebRadioView::openDJRadioWindow()
{
    MusicRightAreaWidget::instance()->musicFunctionClicked(MusicRightAreaWidget::WebDJRadio);
}

void MusicWebRadioView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
