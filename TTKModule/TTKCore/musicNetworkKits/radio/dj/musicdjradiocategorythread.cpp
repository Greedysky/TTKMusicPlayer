#include "musicdjradiocategorythread.h"
#///QJson import
#include "qjson/parser.h"

MusicDJRadioCategoryThread::MusicDJRadioCategoryThread(QObject *parent)
    : MusicDJRadioThreadAbstract(parent)
{

}

QString MusicDJRadioCategoryThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDJRadioCategoryThread::startToDownload()
{
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    QByteArray parameter = makeTokenQueryUrl(&request,
               "http://music.163.com/weapi/djradio/category/get",
               QString("{}"));
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    setSslConfiguration(&request);

    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDJRadioCategoryThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    m_items.clear();
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("categories"))
            {
                QVariantList datas = value["categories"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(m_interrupt) return;

                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicResultsItem item;
                    item.m_name = value["name"].toString();
                    item.m_id = value["id"].toString();
                    item.m_coverUrl = value["picMacUrl"].toString();
                    m_items << item;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
