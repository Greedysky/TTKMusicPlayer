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
#  include <QWebEngineView>
#  include <QWebEngineSettings>
#  if TTK_QT_VERSION_CHECK(6,0,0)
#    include <QWebEngineProfile>
#  endif
#endif

#include <QLabel>
#include <QBoxLayout>

/*! @brief The class of the kugou window widget private.
 * @author Greedysky <greedysky@163.com>
 */
class QKugouWindowPrivate : public TTKPrivate<QKugouWindow>
{
public:
    QKugouWindowPrivate() noexcept;
    ~QKugouWindowPrivate() noexcept;

    QWidget *m_webView;
};

QKugouWindowPrivate::QKugouWindowPrivate() noexcept
    : m_webView(nullptr)
{

}

QKugouWindowPrivate::~QKugouWindowPrivate() noexcept
{
    delete m_webView;
}



QKugouWindow::QKugouWindow(Module type, QWidget *parent)
    : QLabel(parent)
{
    TTK_INIT_PRIVATE(QKugouWindow);

#ifdef TTK_WEBKIT
    QWebSettings *settings = QWebSettings::globalSettings();
    settings->setAttribute(QWebSettings::PluginsEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
#elif defined TTK_WEBENGINE
#  if TTK_QT_VERSION_CHECK(6,0,0)
    QWebEngineSettings *settings = QWebEngineProfile::defaultProfile()->settings();
#  else
    QWebEngineSettings *settings = QWebEngineSettings::defaultSettings();
#  endif
    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
#endif

    setAlignment(Qt::AlignCenter);
    setStyleSheet(TTK::UI::WidgetStyle01);
    createWebViewer(type);
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

void QKugouWindow::createWebViewer()
{
    TTK_D(QKugouWindow);
    delete d->m_webView;
    d->m_webView = nullptr;
#ifdef TTK_MINIBLINK
    if(Miniblink::initialize())
    {
        Miniblink *view = new Miniblink(this);
        d->m_webView = view;
    }
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
    if(type == Module::None)
    {
        QLabel *pix = new QLabel(this);
        pix->setAlignment(Qt::AlignCenter);
        pix->setPixmap(QPixmap(":/image/lb_no_power_mode"));
        layout->addWidget(pix);
    }
    else
    {
        createWebViewer();
        if(d->m_webView)
        {
            layout->addWidget(d->m_webView);

            switch(type)
            {
            case Module::Radio: setUrl(QKugouUrl::makeRadioPublicUrl()); break;
            case Module::Movie: setUrl(QKugouUrl::makeMovieRecommendUrl()); break;
            case Module::Single: setUrl(QKugouUrl::makeKuiSheUrl()); break;
            default: break;
            }
        }
        else
        {
            QLabel *pix = new QLabel(this);
            pix->setAlignment(Qt::AlignCenter);
            pix->setPixmap(QPixmap(":/image/lb_no_module"));
            layout->addWidget(pix);
        }
    }
#else
    Q_UNUSED(d);
    Q_UNUSED(type);
    QLabel *pix = new QLabel(this);
    pix->setAlignment(Qt::AlignCenter);
    pix->setPixmap(QPixmap(":/image/lb_no_module"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}
