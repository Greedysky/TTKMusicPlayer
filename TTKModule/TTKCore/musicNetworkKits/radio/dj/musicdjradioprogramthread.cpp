#include "musicdjradioprogramthread.h"
#///QJson import
#include "qjson/parser.h"

#include <QDateTime>

MusicDJRadioProgramThread::MusicDJRadioProgramThread(QObject *parent)
    : MusicDJRadioThreadAbstract(parent)
{

}

void MusicDJRadioProgramThread::startToDownload(MusicObject::Program type)
{
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    const QByteArray &parameter = makeTokenQueryUrl(&request, type == MusicObject::Recommed ?
                      MusicUtils::Algorithm::mdII(DJ_RECOMMEND_N_URL, false):
                      MusicUtils::Algorithm::mdII(DJ_HOT_N_URL, false),
                      MusicUtils::Algorithm::mdII(DJ_HOT_NDT_URL, false));
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDJRadioProgramThread::downLoadFinished()
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
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("djRadios"))
            {
                const QVariantList &datas = value["djRadios"].toList();
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
                    item.m_coverUrl = value["picUrl"].toString();
                    item.m_tags = value["category"].toString();
                    item.m_nickName = value["categoryId"].toString();
                    item.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["createTime"].toULongLong()).toString("yyyy-MM-dd");
                    item.m_playCount = value["subCount"].toString();

                    emit createProgramItem(item);
                    m_items << item;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
