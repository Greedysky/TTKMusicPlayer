#include "miniblink/miniblink.h"
#include "miniblink/wke.h"

#include <QFile>
#include <QApplication>

static void onLoadingFinish(wkeWebView, void *param, const wkeString, wkeLoadingResult result, const wkeString)
{
    //0 = WKE_LOADING_SUCCEEDED
    //1 = WKE_LOADING_FAILED
    //2 = WKE_LOADING_CANCELED
    ((Miniblink *)param)->finish(result == WKE_LOADING_SUCCEEDED);
}

Miniblink::Miniblink(QWidget *parent)
    : QWidget(parent)
{
    init();

    m_webView = wkeCreateWebWindow(WKE_WINDOW_TYPE_CONTROL, (HWND)winId(), 0, 0, width(), height());
    wkeOnLoadingFinish(m_webView, onLoadingFinish, this);
    wkeShowWindow(m_webView, TRUE);
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
    emit loadFinished(ok);
}

bool Miniblink::init()
{
    static bool loaded = false;
    if(!loaded)
    {
        const QString &dll = QCoreApplication::applicationDirPath() + "/GPlugins/node.dll";
        if(!QFile(dll).exists())
        {
            return false;
        }

        wkeSetWkeDllPath(reinterpret_cast<const wchar_t *>(dll.utf16()));
        wkeInitialize();
        loaded = true;
    }
    return loaded;
}

bool Miniblink::release()
{
    wkeFinalize();
    return true;
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
