#ifndef OSSAPI_H
#define OSSAPI_H

#include <QObject>
#include <QMap>

/*! @brief The class of the alioss cloud data item.
 * @author Greedysky <greedysky@163.com>
 */
class OSSApi : public QObject
{
    Q_OBJECT
public:
    OSSApi(QObject *parent = 0);

    void listobjects(QString bucket);
    void deleteobject(QString bucket, QString file_name);
    void uploadobject(QString bucket, QString file_name, QString file_path);

private:
    void insertAuthorization(QString method, QMap<QString,QString>& headers, QString resource);

private slots:
    void receiveDataFromServer();
    /*!
     * Show upload progress.
     */
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

private:
    QString m_host;

};

#endif // OSSAPI_H
