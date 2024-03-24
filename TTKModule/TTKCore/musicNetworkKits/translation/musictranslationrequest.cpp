#include "musictranslationrequest.h"
#include "musicbdtranslationrequest.h"
#include "musicbingtranslationrequest.h"
#include "musicwytranslationrequest.h"
#include "musicydtranslationrequest.h"

MusicTranslationRequest::MusicTranslationRequest(QObject *parent)
    : MusicAbstractNetwork(parent),
      m_pluginIndex(-1)
{

}

void MusicTranslationRequest::startToRequest()
{
    m_pluginIndex = -1;
    findAllPlugins();
}

void MusicTranslationRequest::downLoadDataFinished(const QString &bytes)
{
    if(bytes.isEmpty())
    {
        findAllPlugins();
        return;
    }

    Q_EMIT downLoadDataChanged(bytes);
    deleteLater();
}

void MusicTranslationRequest::findAllPlugins()
{
    MusicAbstractTranslationRequest *d = nullptr;
    switch(++m_pluginIndex)
    {
        case 0: d = new MusicWYTranslationRequest(this); break;
        case 1: d = new MusicBingTranslationRequest(this); break;
        case 2: d = new MusicYDTranslationRequest(this); break;
        case 3: d = new MusicBDTranslationRequest(this); break;
        default: deleteLater(); break;
    }

    if(d)
    {
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataFinished(QString)));
        //
        d->setHeader("name", header("name"));
        d->startToRequest(header("data").toString());
    }
}
