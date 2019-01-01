#include "musicmvradiocategorythread.h"
#///QJson import
#include "qjson/parser.h"

MusicMVRadioCategoryThread::MusicMVRadioCategoryThread(QObject *parent)
    : MusicMVRadioThreadAbstract(parent)
{

}

void MusicMVRadioCategoryThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        bytes = QString(bytes).split("var mvfmdata = ").back().split("$img = ").front().toUtf8();
        bytes.chop(3);

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            const QVariantList &datas = data.toList();
            foreach(const QVariant &var, datas)
            {
                if(m_interrupt) return;

                if(var.isNull())
                {
                    continue;
                }

                QVariantMap value = var.toMap();
                if(value["classId"].toString() == m_searchText)
                {
                    const QVariantList &fmList = value["fm_list"].toList();
                    foreach(const QVariant &var, fmList)
                    {
                        if(m_interrupt) return;

                        if(var.isNull())
                        {
                            continue;
                        }

                        value = var.toMap();
                        MusicResultsItem item;
                        item.m_name = value["fmName"].toString();
                        item.m_id = value["fmId"].toString();
                        item.m_coverUrl = value["imgUrlMv"].toString();

                        emit createCategoryItem(item);
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
