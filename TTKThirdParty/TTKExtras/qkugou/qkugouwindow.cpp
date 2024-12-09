#include "qkugouwindow.h"
#include "qkugouuiobject.h"

#ifdef TTK_MINIBLINK
#  include "miniblink/miniblink.h"
#elif defined TTK_WEBKIT
#  if TTK_QT_VERSION_CHECK(5,0,0)
#    include <QtWebKitWidgets/QWebView>
#    include <QtWebKitWidgets/QWebFrame>
#  else
#    include <QtWebKit/QWebView>
#    include <QtWebKit/QWebFrame>
#  endif
#elif defined TTK_WEBENGINE
#  include <QtWebEngineWidgets/QWebEngineView>
#  include <QtWebEngineWidgets/QWebEngineSettings>
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

#ifdef TTK_WEBKIT
    QWebSettings *settings = QWebSettings::globalSettings();
    settings->setAttribute(QWebSettings::PluginsEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
#elif defined TTK_WEBENGINE
    QWebEngineSettings *settings = QWebEngineSettings::defaultSettings();
    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
#endif

    if(type != KuGouSong && type != None)
    {
        createWebViewer(type);
    }
    else
    {
        createKugouSongWidget(type == KuGouSong);
    }
}

void QKugouWindow::setUrl(const QString &url)
{
    TTK_D(QKugouWindow);
#ifdef TTK_MINIBLINK
    Miniblink *w = TTKObjectCast(Miniblink*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
#elif defined TTK_WEBKIT
    QWebView *w = TTKObjectCast(QWebView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
#elif defined TTK_WEBENGINE
    QWebEngineView *w = TTKObjectCast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->setUrl(url);
    }
#else
    Q_UNUSED(url);
    Q_UNUSED(d);
#endif
}

void QKugouWindow::refresh()
{
    TTK_D(QKugouWindow);
#ifdef TTK_MINIBLINK
    Miniblink *w = TTKObjectCast(Miniblink*, d->m_webView);
    if(w)
    {
        w->reload();
    }
#elif defined TTK_WEBKIT
    QWebView *w = TTKObjectCast(QWebView*, d->m_webView);
    if(w)
    {
        w->reload();
    }
#elif defined TTK_WEBENGINE
    QWebEngineView *w = TTKObjectCast(QWebEngineView*, d->m_webView);
    if(w)
    {
        w->reload();
    }
#else
    Q_UNUSED(d);
#endif
}

void QKugouWindow::kugouSongIndexChanged(int index)
{
    if(index == 0)
    {
        TTK_D(QKugouWindow);
        const QList<QAbstractButton*> &buttons = d->m_buttonGroup->buttons();
        buttons[index]->setStyleSheet(TTK::UI::PushButtonStyle02);
        setUrl(QKugouUrl::makeSongRecommendUrl());
    }
    else
    {
        Q_EMIT buttonClicked(index + 5); // from kugou song submoudle
    }
}

void QKugouWindow::createWebViewer()
{
    TTK_D(QKugouWindow);
    delete d->m_webView;
#ifdef TTK_MINIBLINK
    Miniblink *view = new Miniblink(this);
    d->m_webView = view;
#elif defined TTK_WEBKIT
    QWebView *view = new QWebView(this);
    view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    d->m_webView = view;
#elif defined TTK_WEBENGINE
    QWebEngineView *view = new QWebEngineView(this);
    d->m_webView = view;
#endif
}

void QKugouWindow::createWebViewer(Module type)
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_MINIBLINK || defined TTK_WEBKIT || defined TTK_WEBENGINE
    createWebViewer();
    if(d->m_webView)
    {
        layout->addWidget(d->m_webView);
    }

    switch(type)
    {
    case KuGouRadio: setUrl(QKugouUrl::makeRadioPublicUrl()); break;
    case KugouMovie: setUrl(QKugouUrl::makeMovieRecommendUrl()); break;
    case KuGouSingle: setUrl(QKugouUrl::makeKuiSheUrl()); break;
    default: break;
    }
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setAlignment(Qt::AlignCenter);
    pix->setStyleSheet(TTK::UI::WidgetStyle01);
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void QKugouWindow::createKugouSongWidget(bool power)
{
    TTK_D(QKugouWindow);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#if defined TTK_MINIBLINK || defined TTK_WEBKIT || defined TTK_WEBENGINE
    d->m_topWidget = new QWidget(this);
    d->m_topWidget->setFixedHeight(25);
    d->m_topWidget->setStyleSheet(TTK::UI::PushButtonStyle01 + TTK::UI::WidgetStyle01);
    QHBoxLayout *topLayout = new QHBoxLayout(d->m_topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(25);

    d->m_buttonGroup = new QButtonGroup(this);
    QPushButton *bt = new QPushButton(tr("Discovery"), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 0);
    bt = new QPushButton(tr("Recommend"), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 1);
    bt = new QPushButton(tr("Toplist"), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 2);
    bt = new QPushButton(tr("Artists"), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 3);
    bt = new QPushButton(tr("Category"), d->m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    d->m_buttonGroup->addButton(bt, 4);
    QtButtonGroupConnect(d->m_buttonGroup, this, kugouSongIndexChanged, TTK_SLOT);

    topLayout->addStretch(1);
    topLayout->addWidget(d->m_buttonGroup->button(0));
    topLayout->addWidget(d->m_buttonGroup->button(1));
    topLayout->addWidget(d->m_buttonGroup->button(2));
    topLayout->addWidget(d->m_buttonGroup->button(3));
    topLayout->addWidget(d->m_buttonGroup->button(4));
    topLayout->addStretch(1);

#ifdef Q_OS_UNIX
    d->m_buttonGroup->button(0)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(1)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(2)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(3)->setFocusPolicy(Qt::NoFocus);
    d->m_buttonGroup->button(4)->setFocusPolicy(Qt::NoFocus);
#endif

    layout->addWidget(d->m_topWidget);

    if(power)
    {
        createWebViewer();
        if(d->m_webView)
        {
            layout->addWidget(d->m_webView);
        }
    }
    else
    {
        QLabel *pix = new QLabel(this);
        pix->setAlignment(Qt::AlignCenter);
        pix->setStyleSheet(TTK::UI::WidgetStyle01);
        pix->setPixmap(QPixmap(":/image/lb_no_power_mode"));
        layout->addWidget(pix);
    }

    kugouSongIndexChanged(0);
#else
    Q_UNUSED(d);
    QLabel *pix = new QLabel(this);
    pix->setAlignment(Qt::AlignCenter);
    pix->setStyleSheet(TTK::UI::WidgetStyle01);
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}
