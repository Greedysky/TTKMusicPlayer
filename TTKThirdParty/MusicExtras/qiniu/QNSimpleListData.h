#ifndef QNRSSTATWIDGET_H
#define QNRSSTATWIDGET_H

#include <QWidget>
#include <QNetworkReply>

class QLineEdit;
class QPushButton;
class QSettings;
class QTextEdit;
class QNetworkAccessManager;

class QNSimpleListData : public QObject
{
    Q_OBJECT
public:
    explicit QNSimpleListData(QNetworkAccessManager *networkManager,
                               QObject *parent = 0);
    void listDataToServer(const QString &bucket);

signals:
private slots:
    void receiveDataFromServer();
    void handleError(QNetworkReply::NetworkError error);


private:
    QNetworkAccessManager *m_networkManager;

};

#endif // QNRSSTATWIDGET_H
