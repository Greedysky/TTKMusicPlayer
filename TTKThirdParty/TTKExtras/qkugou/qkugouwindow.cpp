#include "qkugouwindow.h"
#include "qkugouuiobject.h"

#ifdef Q_OS_WIN
#  include <ActiveQt/QAxWidget>
#else
#  ifdef TTK_WEBKIT
#    if TTK_QT_VERSION_CHECK(5,0,0)
#      include <QtWebKitWidgets/QWebView>
#      include <QtWebKitWidgets/QWebFrame>
#    else
#      include <QtWebKit/QWebView>
#      include <QtWebKit/QWebFrame>
#    endif
#  elif defined TTK_WEBENGINE
#    include <QtWebEngineWidgets/QWebEngineView>
#    include <QtWebEngineWidgets/QWebEngineSettings>
#  endif
#endif

#include <QLabel>
#include <QTimer>
#include <QBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

/*! @brief The class of the kugou window widget private.
 * @author Greedysky <greedysky@163.com>
 */
class QKugouWindowPrivate : public TTKPrivate<QKugouWindow>
{
public:
    QKugouWindowPrivate();
    ~QKugouWindowPrivate();

    QWidget *m_webView, *m_topWidget;
    QButtonGroup *m_buttonGroup;
};

QKugouWindowPrivate::QKugouWindowPrivate()
    : m_webView(nullptr),
      m_topWidget(nullptr),
      m_buttonGroup(nullptr)
{

}

QKugouWindowPrivate::~QKugouWindowPrivate()
{
    delete m_buttonGroup;
    delete m_topWidget;
    delete m_webView;
}



QKugouWindow::QKugouWindow(Module type, QWidget *parent)
    : QWidget(parent)
{
    TTK_INIT_PRIVATE(QKugouWindow);

#ifdef Q_OS_UNIX
#  ifdef TTK_WEBKIT
    QWebSettings *settings = QWebSettings::globalSettings();
    settings->setAttribute(QWebSettings::PluginsEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
#  elif defined TTK_WEBENGINE
    QWebEngineSettings *settings = QWebEngineSettings::defaultSettings();
    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
#  endif
#endif

    switch(type)
    {
        case KuGouSong: createKugouSongWidget(); break;
        case KuGouRadio: createKugouRadioWidget(); break;
        case KuGouList: createKugouListWidget(); break;
        case KugouMovie: createKugouMovieWidget(); break;
        case KuGouSingle: createKugouSingleWidget(); break;
        default: break;
    }
}

void QKugouWindow::setUrl(const QString &url)
{
    TTK_D(QKugouWindow);
#ifdef Q_OS_WIN
    QAxWidget *w = TTKObjectCast(QAxWidget*, d->m_webView);
    if(w)
    {
        w->dynamicCall("Navigate(const QString&)", url);
    }
#else
#  ifdef TTK_WEBKIT
    QWebView *w = TTKObjectCast(QWebView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
#  elif defined TTK_WEBENGINE
    QWebEngineView *w = TTKObjectCast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
#  else
    Q_UNUSED(url);
    Q_UNUSED(d);
#  endif
#endif
}

void QKugouWindow::refresh()
{
    TTK_D(QKugouWindow);
#ifdef Q_OS_WIN
    TTK_SIGNLE_SHOT(TTKObjectCast(QAxWidget*, d->m_webView), Refresh, TTK_SLOT);
#else
#  ifdef TTK_WEBKIT
    QWebView *w = TTKObjectCast(QWebView*, d->m_webView);
    if(w)
    {
        w->reload();
    }
#  elif defined TTK_WEBENGINE
    QWebEngineView *w = TTKObjectCast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->reload();
    }
#  else
    Q_UNUSED(d);
#  endif
#endif
}

void QKugouWindow::kugouSongIndexChanged(int index)
{
    changeClickedButtonStyle(index);
    QString url = QKugouUrl::makeSongRecommendUrl();
    switch(index)
    {
        case 0: url = QKugouUrl::makeSongRecommendUrl(); break;
        case 1: url = QKugouUrl::makeSongRankUrl(); break;
        case 2: url = QKugouUrl::makeSongSingerUrl(); break;
        case 3: url = QKugouUrl::makeSongCategoryUrl(); break;
        default: break;
    }

    setUrl(url);
}

void QKugouWindow::kugouRadioIndexChanged(int index)
{
    changeClickedButtonStyle(index);
    QString url = QKugouUrl::makeRadioPublicUrl();
    switch(index)
    {
        case 0: url = QKugouUrl::makeRadioPublicUrl(); break;
        case 1: url = QKugouUrl::makeRadioHighFmUrl(); break;
        default: break;
    }

    setUrl(url);
}

void QKugouWindow::kugouMVIndexChanged(int index)
{
    changeClickedButtonStyle(index);
    QString url = QKugouUrl::makeMVRadioUrl();
    switch(index)
    {
        case 0: url = QKugouUrl::makeMVRadioUrl(); break;
        case 1: url = QKugouUrl::makeMVRecommendUrl(); break;
        default: break;
    }

    setUrl(url);
}

void QKugouWindow::createWebViewer()
{
    TTK_D(QKugouWindow);
    delete d->m_webView;
#ifdef Q_OS_WIN
    QAxWidget *view = new QAxWidget(this);
    view->setWindowFlags(Qt::FramelessWindowHint);
    view->setMouseTracking(false);
    view->setControl("{8856F961-340A-11D0-A96B-00C04FD705A2}"); //ie web view
    view->setObjectName("WebWidget");
    view->setFocusPolicy(Qt::StrongFocus);
    view->setProperty("DisplayAlerts", false);
    view->setProperty("DisplayScrollBars", false);
    view->setProperty("Silent", true);
    d->m_webView = view;
#else
#  ifdef TTK_WEBKIT
    QWebView *view = new QWebView(this);
    view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    d->m_webView = view;
#  elif defined TTK_WEBENGINE
    QWebEngineView *view = new QWebEngineView(this);
    d->m_webView = view;
#  endif
#endif
}

void QKugouWindow::createKugouSongWidget()
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_WEBKIT || defined TTK_WEBENGINE
    d->m_topWidget = new QWidget(this);
    d->m_topWidget->setFixedHeight(25);
    d->m_topWidget->setStyleSheet(UI::PushButtonStyle01 + UI::WidgetStyle01);
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
    QtButtonGroupConnect(d->m_buttonGroup, this, kugouSongIndexChanged, TTK_SLOT);

    topLayout->addStretch(1);
    topLayout->addWidget(d->m_buttonGroup->button(0));
    topLayout->addWidget(d->m_buttonGroup->button(1));
    topLayout->addWidget(d->m_buttonGroup->button(2));
    topLayout->addWidget(d->m_buttonGroup->button(3));
    topLayout->addStretch(1);

#ifdef Q_OS_UNIX
    d->m_buttonGroup->button(0)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(1)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(2)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(3)->setFocusPolicy(Qt::NoFocus);
#endif

    createWebViewer();
    layout->addWidget(d->m_topWidget);
    layout->addWidget(d->m_webView);
    kugouSongIndexChanged(0);
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setAlignment(Qt::AlignCenter);
    pix->setStyleSheet("background:white;");
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void QKugouWindow::createKugouRadioWidget()
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_WEBKIT || defined TTK_WEBENGINE
    d->m_topWidget = new QWidget(this);
    d->m_topWidget->setFixedHeight(25);
    d->m_topWidget->setStyleSheet(UI::PushButtonStyle01 + UI::WidgetStyle01);
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
    QtButtonGroupConnect(d->m_buttonGroup, this, kugouRadioIndexChanged, TTK_SLOT);

    topLayout->addStretch(1);
    topLayout->addWidget(d->m_buttonGroup->button(0));
    topLayout->addWidget(d->m_buttonGroup->button(1));
    topLayout->addStretch(1);

#ifdef Q_OS_UNIX
    d->m_buttonGroup->button(0)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(1)->setFocusPolicy(Qt::NoFocus);
#endif

    createWebViewer();
    layout->addWidget(d->m_topWidget);
    layout->addWidget(d->m_webView);
    kugouRadioIndexChanged(0);
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setAlignment(Qt::AlignCenter);
    pix->setStyleSheet("background:white;");
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void QKugouWindow::createKugouListWidget()
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_WEBKIT || defined TTK_WEBENGINE
    createWebViewer();
    layout->addWidget(d->m_webView);
#  ifdef Q_OS_WIN
    TTKObjectCast(QAxWidget*, d->m_webView)->dynamicCall("Navigate(const QString&)", QKugouUrl::makeListUrl());
#  else
#    ifdef TTK_WEBENGINE
    TTKObjectCast(QWebEngineView*, d->m_webView)->setUrl(QKugouUrl::makeListUrl());
#    else
    TTKObjectCast(QWebView*, d->m_webView)->setUrl(QKugouUrl::makeListUrl());
#    endif
#  endif
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setAlignment(Qt::AlignCenter);
    pix->setStyleSheet("background:white;");
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void QKugouWindow::createKugouMovieWidget()
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_WEBKIT || defined TTK_WEBENGINE
    d->m_topWidget = new QWidget(this);
    d->m_topWidget->setFixedHeight(25);
    d->m_topWidget->setStyleSheet(UI::PushButtonStyle01 + UI::WidgetStyle01);
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
    QtButtonGroupConnect(d->m_buttonGroup, this, kugouMVIndexChanged, TTK_SLOT);

    topLayout->addStretch(1);
    topLayout->addWidget(d->m_buttonGroup->button(0));
    topLayout->addWidget(d->m_buttonGroup->button(1));
    topLayout->addStretch(1);

#ifdef Q_OS_UNIX
    d->m_buttonGroup->button(0)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(1)->setFocusPolicy(Qt::NoFocus);
#endif

    createWebViewer();
    layout->addWidget(d->m_topWidget);
    layout->addWidget(d->m_webView);
    kugouMVIndexChanged(0);
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setAlignment(Qt::AlignCenter);
    pix->setStyleSheet("background:white;");
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void QKugouWindow::createKugouSingleWidget()
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_WEBKIT || defined TTK_WEBENGINE
    createWebViewer();
    layout->addWidget(d->m_webView);
    setUrl(QKugouUrl::makeKuiSheUrl());
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setAlignment(Qt::AlignCenter);
    pix->setStyleSheet("background:white;");
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void QKugouWindow::changeClickedButtonStyle(int index)
{
    TTK_D(QKugouWindow);
    const QList<QAbstractButton*> &buttons = d->m_buttonGroup->buttons();
    for(int i = 0; i < buttons.count(); ++i)
    {
        buttons[i]->setStyleSheet(UI::PushButtonStyle01);
    }
    buttons[index]->setStyleSheet(UI::PushButtonStyle02);
}
