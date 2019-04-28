#include "kugouwindow.h"
#include "kugouuiobject.h"

#ifdef Q_OS_WIN
 #include <ActiveQt/QAxWidget>
#else
# ifdef MUSIC_WEBKIT
#   ifdef TTK_GREATER_NEW
#    include <QtWebKitWidgets/QWebView>
#    include <QtWebKitWidgets/QWebFrame>
#   else
#    include <QWebView>
#    include <QWebFrame>
#   endif
# elif defined MUSIC_WEBENGINE
#   include <QtWebEngineWidgets/QWebEngineView>
#   include <QtWebEngineWidgets/QWebEngineSettings>
# endif
#endif

#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

class KugouWindowPrivate : public TTKPrivate<KugouWindow>
{
public:
    KugouWindowPrivate();
    ~KugouWindowPrivate();

    QWidget *m_webView, *m_topWidget;
    QButtonGroup *m_buttonGroup;
};

KugouWindowPrivate::KugouWindowPrivate()
{
    m_buttonGroup = nullptr;
    m_topWidget = nullptr;
    m_webView = nullptr;
}

KugouWindowPrivate::~KugouWindowPrivate()
{
    delete m_buttonGroup;
    delete m_topWidget;
    delete m_webView;
}



KugouWindow::KugouWindow(KuGouType type, QWidget *parent)
    : QWidget(parent)
{
    TTK_INIT_PRIVATE;

#ifdef Q_OS_UNIX
 #ifdef MUSIC_WEBKIT
    QWebSettings *settings = QWebSettings::globalSettings();
    settings->setAttribute(QWebSettings::PluginsEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
 #elif defined MUSIC_WEBENGINE
    QWebEngineSettings *settings = QWebEngineSettings::defaultSettings();
    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
 #endif
#endif

    switch(type)
    {
        case KuGouSong:
                createKugouSongWidget();
                break;
        case KuGouRadio:
                createKugouRadioWidget();
                break;
        case KuGouList:
                createKugouListWidget();
                break;
        case KugouMv:
                createKugouMVWidget();
                break;
        case KuGouSingle:
                createKugouSingleWidget();
                break;
        case KuGouLive: break;
        case KuGouLrc: break;
    }
}

void KugouWindow::setUrl(const QString &url)
{
    TTK_D(KugouWindow);
#ifdef Q_OS_WIN
    QAxWidget *w = MObject_cast(QAxWidget*, d->m_webView);
    if(w)
    {
        w->dynamicCall("Navigate(const QString&)", url);
    }
#else
 #ifdef MUSIC_WEBKIT
    QWebView *w = MObject_cast(QWebView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
 #elif defined MUSIC_WEBENGINE
    QWebEngineView *w = MObject_cast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
 #else
    Q_UNUSED(url);
    Q_UNUSED(d);
 #endif
#endif
}

void KugouWindow::goBack()
{
#ifdef Q_OS_UNIX
    TTK_D(KugouWindow);
 #ifdef MUSIC_WEBKIT
    QWebView *w = MObject_cast(QWebView*, d->m_webView);
    if(w)
    {
        w->back();
    }
 #elif defined MUSIC_WEBENGINE
    QWebEngineView *w = MObject_cast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->back();
    }
 #endif
#endif
}

void KugouWindow::kugouSongIndexChanged(int index)
{
    TTK_D(KugouWindow);
    changeClickedButtonStyle(index);
    QString url = KugouUrl::getSongRecommendUrl();
    switch(index)
    {
        case 0: url = KugouUrl::getSongRecommendUrl(); break;
        case 1: url = KugouUrl::getSongRankUrl(); break;
        case 2: url = KugouUrl::getSongSingerUrl(); break;
        case 3: url = KugouUrl::getSongCategoryUrl(); break;
        case 4: url = KugouUrl::getSongShowUrl(); break;
        case 5: url = KugouUrl::getSongHeroesUrl(); break;
    }

#ifdef Q_OS_WIN
    QAxWidget *w = MObject_cast(QAxWidget*, d->m_webView);
    if(w)
    {
        w->dynamicCall("Navigate(const QString&)", url);
    }
#else
 #ifdef MUSIC_WEBKIT
    QWebView *w = MObject_cast(QWebView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
 #elif defined MUSIC_WEBENGINE
    QWebEngineView *w = MObject_cast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
 #else
    Q_UNUSED(url);
    Q_UNUSED(d);
 #endif
#endif
}

void KugouWindow::kugouRadioIndexChanged(int index)
{
    TTK_D(KugouWindow);
    changeClickedButtonStyle(index);
    QString url = KugouUrl::getRadioPublicUrl();
    switch(index)
    {
        case 0: url = KugouUrl::getRadioPublicUrl(); break;
        case 1: url = KugouUrl::getRadioHighFmUrl(); break;
        case 2: url = KugouUrl::getRadioFxUrl(); break;
        case 3: url = KugouUrl::getRadioHomeUrl(); break;
    }

#ifdef Q_OS_WIN
    QAxWidget *w = MObject_cast(QAxWidget*, d->m_webView);
    if(w)
    {
        w->dynamicCall("Navigate(const QString&)", url);
    }
#else
 #ifdef MUSIC_WEBKIT
    QWebView *w = MObject_cast(QWebView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
 #elif defined MUSIC_WEBENGINE
    QWebEngineView *w = MObject_cast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
 #else
   Q_UNUSED(url);
   Q_UNUSED(d);
 #endif
#endif
}

void KugouWindow::kugouMVIndexChanged(int index)
{
    TTK_D(KugouWindow);
    changeClickedButtonStyle(index);
    QString url = KugouUrl::getMVRadioUrl();
    switch(index)
    {
        case 0: url = KugouUrl::getMVRadioUrl(); break;
        case 1: url = KugouUrl::getMVRecommendUrl(); break;
        case 2: url = KugouUrl::getMVFanxingUrl(); break;
    }

#ifdef Q_OS_WIN
    QAxWidget *w = MObject_cast(QAxWidget*, d->m_webView);
    if(w)
    {
        w->dynamicCall("Navigate(const QString&)", url);
    }
#else
 #ifdef MUSIC_WEBKIT
    QWebView *w = MObject_cast(QWebView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
 #elif defined MUSIC_WEBENGINE
    QWebEngineView *w = MObject_cast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
 #else
    Q_UNUSED(url);
    Q_UNUSED(d);
 #endif
#endif
}

void KugouWindow::createWebViewer()
{
    TTK_D(KugouWindow);
    if(d->m_webView)
    {
        delete d->m_webView;
    }
#ifdef Q_OS_WIN
    //ie web view
    QAxWidget *view = new QAxWidget(this);
    view->setWindowFlags(Qt::FramelessWindowHint);
    view->setMouseTracking(false);
    view->setControl("{8856F961-340A-11D0-A96B-00C04FD705A2}");
    view->setObjectName("WebWidget");
    view->setFocusPolicy(Qt::StrongFocus);
    view->setProperty("DisplayAlerts", false);
    view->setProperty("DisplayScrollBars", false);
    d->m_webView = view;
#else
 #ifdef MUSIC_WEBKIT
    QWebView *view = new QWebView(this);
    view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    d->m_webView = view;
 #elif defined MUSIC_WEBENGINE
    QWebEngineView *view = new QWebEngineView(this);
    d->m_webView = view;
 #endif
#endif
}

void KugouWindow::createKugouSongWidget()
{
    TTK_D(KugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined MUSIC_WEBKIT || defined MUSIC_WEBENGINE
    d->m_topWidget = new QWidget(this);
    d->m_topWidget->setFixedHeight(25);
    d->m_topWidget->setStyleSheet(MusicUIObject::MPushButtonStyle01 + MusicUIObject::MWidgetStyle01);
    QHBoxLayout *topLayout = new QHBoxLayout(d->m_topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(25);

    d->m_buttonGroup = new QButtonGroup(this);
    QPushButton *bt = new QPushButton(tr(" SongRecommend "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 0);
    bt = new QPushButton(tr(" SongRank "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 1);
    bt = new QPushButton(tr(" SongSinger "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 2);
    bt = new QPushButton(tr(" SongCategory "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 3);
    bt = new QPushButton(tr(" SongShow "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 4);
    bt = new QPushButton(tr(" SongHeroes "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 5);
    connect(d->m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(kugouSongIndexChanged(int)));

    topLayout->addStretch(1);
    topLayout->addWidget(d->m_buttonGroup->button(0));
    topLayout->addWidget(d->m_buttonGroup->button(1));
    topLayout->addWidget(d->m_buttonGroup->button(2));
    topLayout->addWidget(d->m_buttonGroup->button(3));
    topLayout->addWidget(d->m_buttonGroup->button(4));
    topLayout->addWidget(d->m_buttonGroup->button(5));
    topLayout->addStretch(1);
#ifdef Q_OS_UNIX
    d->m_buttonGroup->button(0)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(1)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(2)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(3)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(4)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(5)->setFocusPolicy(Qt::NoFocus);
#endif
    createWebViewer();
    layout->addWidget(d->m_topWidget);
    layout->addWidget(d->m_webView);
    kugouSongIndexChanged(0);
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setScaledContents(true);
    pix->setPixmap(QPixmap(":/image/lb_no_webkit_song"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void KugouWindow::createKugouRadioWidget()
{
    TTK_D(KugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined MUSIC_WEBKIT || defined MUSIC_WEBENGINE
    d->m_topWidget = new QWidget(this);
    d->m_topWidget->setFixedHeight(25);
    d->m_topWidget->setStyleSheet(MusicUIObject::MPushButtonStyle01 + MusicUIObject::MWidgetStyle01);
    QHBoxLayout *topLayout = new QHBoxLayout(d->m_topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(50);

    d->m_buttonGroup = new QButtonGroup(this);
    QPushButton *bt = new QPushButton(tr(" RadioPublic "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 0);
    bt = new QPushButton(tr(" RadioHigh "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 1);
    bt = new QPushButton(tr(" RadioFx "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 2);
    bt = new QPushButton(tr(" RadioHome "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 3);
    connect(d->m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(kugouRadioIndexChanged(int)));

    topLayout->addStretch(1);
    topLayout->addWidget(d->m_buttonGroup->button(0));
    topLayout->addWidget(d->m_buttonGroup->button(1));
    topLayout->addWidget(d->m_buttonGroup->button(2));
    topLayout->addWidget(d->m_buttonGroup->button(3));
    topLayout->addStretch(1);

    createWebViewer();
    layout->addWidget(d->m_topWidget);
    layout->addWidget(d->m_webView);
    kugouRadioIndexChanged(0);
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setScaledContents(true);
    pix->setPixmap(QPixmap(":/image/lb_no_webkit_radio"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void KugouWindow::createKugouListWidget()
{
    TTK_D(KugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined MUSIC_WEBKIT || defined MUSIC_WEBENGINE
    createWebViewer();
    layout->addWidget(d->m_webView);
 #ifdef Q_OS_WIN
    MObject_cast(QAxWidget*, d->m_webView)->dynamicCall("Navigate(const QString&)", KugouUrl::getListUrl());
 #else
  #ifdef MUSIC_WEBENGINE
    MObject_cast(QWebEngineView*, d->m_webView)->setUrl(KugouUrl::getListUrl());
  #else
    MObject_cast(QWebView*, d->m_webView)->setUrl(KugouUrl::getListUrl());
  #endif
 #endif
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setScaledContents(true);
    pix->setPixmap(QPixmap(":/image/lb_no_webkit_list"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void KugouWindow::createKugouMVWidget()
{
    TTK_D(KugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined MUSIC_WEBKIT || defined MUSIC_WEBENGINE
    d->m_topWidget = new QWidget(this);
    d->m_topWidget->setFixedHeight(25);
    d->m_topWidget->setStyleSheet(MusicUIObject::MPushButtonStyle01 + MusicUIObject::MWidgetStyle01);
    QHBoxLayout *topLayout = new QHBoxLayout(d->m_topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(50);

    d->m_buttonGroup = new QButtonGroup(this);
    QPushButton *bt = new QPushButton(tr(" MVRadio "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 0);
    bt = new QPushButton(tr(" MVRecommend "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 1);
    bt = new QPushButton(tr(" MVFanxing "), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 2);
    connect(d->m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(kugouMVIndexChanged(int)));

    topLayout->addStretch(1);
    topLayout->addWidget(d->m_buttonGroup->button(0));
    topLayout->addWidget(d->m_buttonGroup->button(1));
    topLayout->addWidget(d->m_buttonGroup->button(2));
    topLayout->addStretch(1);

    createWebViewer();
    layout->addWidget(d->m_topWidget);
    layout->addWidget(d->m_webView);
    kugouMVIndexChanged(0);
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setScaledContents(true);
    pix->setPixmap(QPixmap(":/image/lb_no_webkit_mv"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void KugouWindow::createKugouSingleWidget()
{
    TTK_D(KugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined MUSIC_WEBKIT || defined MUSIC_WEBENGINE
    createWebViewer();
    layout->addWidget(d->m_webView);
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setScaledContents(true);
    pix->setStyleSheet("background:white;");
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void KugouWindow::changeClickedButtonStyle(int index)
{
    TTK_D(KugouWindow);
    for(int i=0; i<d->m_buttonGroup->buttons().count(); ++i)
    {
        d->m_buttonGroup->button(i)->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    }
    d->m_buttonGroup->button(index)->setStyleSheet(MusicUIObject::MPushButtonStyle02);
}
