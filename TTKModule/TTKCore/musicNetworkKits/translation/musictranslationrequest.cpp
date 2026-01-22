#include "musictranslationrequest.h"
#include "musicbdtranslationrequest.h"
#include "musicbingtranslationrequest.h"
#include "musicwytranslationrequest.h"
#include "musicydtranslationrequest.h"

MusicTranslationRequest::MusicTranslationRequest(QObject *parent)
    : MusicAbstractNetwork(parent),
      m_pluginSelector(-1)
{

}

void MusicTranslationRequest::startToRequest()
{
    m_pluginSelector = -1;
    findAllPlugins();
}

void MusicTranslationRequest::downloadDataFinished(const QString &bytes)
{
    if(bytes.isEmpty())
    {
        findAllPlugins();
        return;
    }

    Q_EMIT downloadDataChanged(bytes);
    deleteLater();
}

void MusicTranslationRequest::findAllPlugins()
{
    MusicAbstractTranslationRequest *req = nullptr;
    switch(++m_pluginSelector)
    {
        case 0: req = new MusicWYTranslationRequest(this); break;
        case 1: req = new MusicYDTranslationRequest(this); break;
        case 2: req = new MusicBingTranslationRequest(this); break;
        case 3: req = new MusicBDTranslationRequest(this); break;
        default: deleteLater(); break;
    }

    if(req)
    {
        connect(req, SIGNAL(downloadDataChanged(QString)), SLOT(downloadDataFinished(QString)));
        //
        req->setHeader("name", header("name"));
        req->startToRequest(header("data").toString());
    }
}
