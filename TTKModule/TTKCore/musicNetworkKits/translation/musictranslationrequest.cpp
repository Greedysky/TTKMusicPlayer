#include "musictranslationrequest.h"
#include "musicsettingwidget.h"
#include "musicabstractqueryrequest.h"
#include "musicbdtranslationrequest.h"
#include "musicwytranslationrequest.h"
#include "musicydtranslationrequest.h"

MusicTranslationRequest::MusicTranslationRequest(const QString &data, QObject *parent)
    : MusicAbstractNetwork(parent),
      m_data(data),
      m_pluginIndex(-1)
{

}

void MusicTranslationRequest::startRequest()
{
    m_pluginIndex = -1;
    findAllPlugins();
}

void MusicTranslationRequest::downLoadFinished(const QString &bytes)
{
    if(bytes.isEmpty())
    {
        findAllPlugins();
    }
    else
    {
        Q_EMIT downLoadDataChanged(bytes);
        deleteLater();
    }
}

void MusicTranslationRequest::findAllPlugins()
{
    MusicAbstractTranslationRequest *d = nullptr;
    switch(++m_pluginIndex)
    {
        case 0:
        {
            const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
            if(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index) != MusicAbstractQueryRequest::QueryServer::WY)
            {
                downLoadFinished({});
                return;
            }

            d = new MusicWYTranslationRequest(this);
            break;
        }
        case 1: d = new MusicYDTranslationRequest(this); break;
        case 2: d = new MusicBDTranslationRequest(this); break;
        default: deleteLater(); break;
    }

    if(d)
    {
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
        d->setHeader("name", header("name"));
        d->startRequest(m_data);
    }
}
