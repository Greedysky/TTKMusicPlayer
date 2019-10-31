#include "ossapi.h"
#include "ossutils.h"
#include "ossconf.h"

#include <stdlib.h>
#include <time.h>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

namespace QAliOSS{
OSSApi::OSSApi(QObject *parent) :
    QObject(parent)
{
}


void QAliOSS::OSSApi::insertAuthorization(QString method, QMap<QString,QString>& headers, QString resource)
{
    if(!QNConf::ACCESS_KEY.isEmpty() && !QNConf::SECRET_KEY.isEmpty())
    {
        headers.insert("Authorization", Utl::createSignForNormalAuth(method,QNConf::ACCESS_KEY, QNConf::SECRET_KEY, headers,resource));
    }
    else if(!QNConf::ACCESS_KEY.isEmpty())
    {
        headers.insert("Authorization",QNConf::ACCESS_KEY);
    }
}

void QAliOSS::OSSApi::listobjects(QString bucket)
{
    QString method = "GET";
    QString url = "/";
    QString resource = "/" + bucket + "/";
    QString date = Utl::getGMT();

    QMap<QString,QString> headers;
    headers.insert("Date",date);
    headers.insert("Host", bucket + "." +QNConf::OSS_HOST);

    insertAuthorization(method, headers, resource);

    QNetworkAccessManager *m_networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl("http://" + bucket + "." +QNConf::OSS_HOST));
    QMapIterator<QString,QString> it(headers);
    while(it.hasNext()){
        it.next();
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));

}

void QAliOSS::OSSApi::receiveDataFromServer()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(reply)
    {
        qDebug() << "fffffffffffffff \n" << reply->readAll();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//        QNDataItems items;
//        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
//        {
//            QJson::Parser parser;
//            bool ok;
//            const QVariant &data = parser.parse(reply->readAll(), &ok);
//            if(ok)
//            {
//                QVariantMap value = data.toMap();
//                const QVariantList &array = value["items"].toList();
//                foreach(const QVariant &var, array)
//                {
//                    value = var.toMap();
//                    QNDataItem item;
//                    item.m_name = value["key"].toString();
//                    item.m_hash = value["hash"].toString();
//                    item.m_mimeType = value["mimeType"].toString();
//                    item.m_size = value["fsize"].toInt();
//                    item.m_putTime = value["putTime"].toLongLong()/10000;
//                    items << item;
//                }
//            }
//        }
//        reply->deleteLater();
//        emit receiveFinshed( items );
    }
//    else
//    {
//        emit receiveFinshed( QNDataItems() );
//    }
}

void  QAliOSS::OSSApi::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug() <<bytesSent << bytesTotal;
}


void QAliOSS::OSSApi::deleteobject(QString bucket, QString file_name)
{
    QString method = "DELETE";
    QString url = "/" + file_name;
    QString resource = "/" + bucket + "/" + file_name;
    QString date = Utl::getGMT();

    QMap<QString,QString> headers;
    headers.insert("Date",date);
    headers.insert("Host", bucket + "." +QNConf::OSS_HOST);

    insertAuthorization(method, headers, resource);

    QNetworkAccessManager *m_networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl("http://" + bucket + "." +QNConf::OSS_HOST + url));
    QMapIterator<QString,QString> it(headers);
    while(it.hasNext()){
        it.next();
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = m_networkManager->deleteResource(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
}


void QAliOSS::OSSApi::uploadobject(QString bucket, QString file_name, QString file_path)
{
    QString method = "PUT";
    QString url = "/" + file_name;
    QString resource = "/" + bucket + "/" + file_name;
    QString date = Utl::getGMT();

    QMap<QString,QString> headers;
    headers.insert("Date",date);
    headers.insert("Host", bucket + "." +QNConf::OSS_HOST);

    QFile file(file_path);
    file.open(QFile::ReadOnly);
    QByteArray datas = file.readAll();


    insertAuthorization(method, headers, resource);

    QNetworkAccessManager *m_networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl("http://" + bucket + "." +QNConf::OSS_HOST + url));
    QMapIterator<QString,QString> it(headers);
    while(it.hasNext()){
        it.next();
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = m_networkManager->put(request, datas);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));

    connect(reply, SIGNAL(uploadProgress(qint64,qint64)), SLOT(uploadProgress(qint64,qint64)));
}

}



int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    QNConf::ACCESS_KEY = "LTAI4FrJfKgXX2Afw1BLS2JH";
    QNConf::SECRET_KEY =  "RWGtsCm8LIeN0Zi2k82R9tJziCAVzY";
    QAliOSS::OSSApi aa;

//    aa.listobjects("gmytest");
//    aa.deleteobject("gmytest", "1.mp3");
    aa.uploadobject("gmytest", "1.mp3", "/home/greedysky/qmmp_all/1.mp3");

    return app.exec();
}
