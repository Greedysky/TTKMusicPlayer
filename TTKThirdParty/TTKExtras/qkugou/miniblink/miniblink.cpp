#include "miniblink.h"
#include "wke.h"

#include <QFile>
#include <QApplication>

static void onLoadingFinish(wkeWebView, void *param, const wkeString, wkeLoadingResult result, const wkeString)
{
    //0 = WKE_LOADING_SUCCEEDED
    //1 = WKE_LOADING_FAILED
    //2 = WKE_LOADING_CANCELED
    ((Miniblink *)param)->finish(result == WKE_LOADING_SUCCEEDED);
}

int Miniblink::m_ref = 0;

Miniblink::Miniblink(QWidget *parent)
    : QWidget(parent)
{
    m_webView = wkeCreateWebWindow(WKE_WINDOW_TYPE_CONTROL, (HWND)winId(), 0, 0, width(), height());
    wkeOnLoadingFinish(m_webView, onLoadingFinish, this);
    wkeShowWindow(m_webView, TRUE);
}

Miniblink::~Miniblink()
{
    wkeDestroyWebWindow(m_webView);
    //
    if(--m_ref == 0)
    {
        wkeFinalize();
    }
}

bool Miniblink::initialize()
{
    static bool loaded = false;
    if(!loaded)
    {
        const QString &dll = QCoreApplication::applicationDirPath() + "/GPlugins/node.dll";
        if(QFile(dll).exists())
        {
            wkeSetWkeDllPath(reinterpret_cast<const wchar_t *>(dll.utf16()));
            wkeInitialize();
            loaded = true;
        }
    }

    if(loaded)
    {
        ++m_ref;
    }
    return loaded;
}

void Miniblink::load(const QString& url)
{
    wkeLoadURL(m_webView, url.toUtf8().constData());
}

void Miniblink::setHtml(const QString& html)
{
    wkeLoadFile(m_webView, html.toUtf8().constData());
}

void Miniblink::setUrl(const QString &url)
{
    wkeLoadURL(m_webView, url.toUtf8().constData());
}

QString Miniblink::url() const
{
    return wkeGetURL(m_webView);
}

void Miniblink::finish(bool ok)
{
    Q_EMIT loadFinished(ok);
}

void Miniblink::stop()
{
    wkeStopLoading(m_webView);
}

void Miniblink::back()
{
    if(wkeCanGoBack(m_webView))
    {
        wkeGoBack(m_webView);
    }
}

void Miniblink::forward()
{
    if(wkeCanGoForward(m_webView))
    {
        wkeGoForward(m_webView);
    }
}

void Miniblink::reload()
{
    wkeReload(m_webView);
}

void Miniblink::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    wkeResize(m_webView, width(), height());
}
