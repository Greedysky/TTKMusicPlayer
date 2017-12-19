#include "musictranslationthreadabstract.h"

MusicTranslationThreadAbstract::MusicTranslationThreadAbstract(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_reply = nullptr;
    m_manager = new QNetworkAccessManager(this);
}

QString MusicTranslationThreadAbstract::getClassName()
{
    return staticMetaObject.className();
}

void MusicTranslationThreadAbstract::setRawData(const QVariantMap &data)
{
    m_rawData = data;
}

void MusicTranslationThreadAbstract::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}
