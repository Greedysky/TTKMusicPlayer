#include "musicwyauthenticationthread.h"
#include "musicalgorithmutils.h"
#///QJson import
#include "qjson/parser.h"

MusicWYAuthenticationThread::MusicWYAuthenticationThread(QObject *parent)
    : MusicAuthenticationThread(parent)
{

}

QString MusicWYAuthenticationThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWYAuthenticationThread::startToDownload(const QString &usr, const QString &pwd)
{
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    QByteArray parameter = makeTokenQueryUrl(&request,
               MusicUtils::Algorithm::mdII(WY_AUT_N_URL, false),
               MusicUtils::Algorithm::mdII(WY_AUT_NDT_URL, false).arg(usr).arg(pwd));
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    setSslConfiguration(&request);

    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYAuthenticationThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;
    m_info.clear();

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("code") && value["code"].toInt() == 200)
            {
                value = value["profile"].toMap();
                m_info.m_nickName = value["nickname"].toString();
                m_info.m_coverUrl = value["avatarUrl"].toString();
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
