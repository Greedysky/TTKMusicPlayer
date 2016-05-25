#ifndef MUSICTRANSLATIONTHREAD_H
#define MUSICTRANSLATIONTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QNetworkReply>
#include "musicglobaldefine.h"

const QString TRANSLATION_URL = "http://fanyi.baidu.com/v2transapi?from=%1&query=%2&to=%3";

/*! @brief The class of translation words thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicTranslationThread : public QObject
{
    Q_OBJECT
public:
    enum TranslationType
    {
        Type_Auto,  ///automatic detection
        Type_Ara,   ///Arabic
        Type_De,    ///German
        Type_Ru,    ///Russian
        Type_Fra,   ///French
        Type_Kor,   ///Korean
        Type_Nl,    ///Dutch
        Type_Pt,    ///Portuguese
        Type_Jp,    ///Japanese
        Type_Th,    ///Thai
        Type_Wyw,   ///Classical
        Type_Spa,   ///Spanish
        Type_El,    ///Greek
        Type_It,    ///Italian
        Type_En,    ///English
        Type_Yue,   ///Cantonese
        Type_Zh     ///Chinese
    };

    explicit MusicTranslationThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicTranslationThread();

    void deleteAll();
    /*!
     * Release the network object.
     */
    void startToTranslation(TranslationType from, TranslationType to, const QString &data);
    /*!
     * Start to translation data.
     */

Q_SIGNALS:
    void recievedData(const QString &data);
    /*!
     * Send translated data from net.
     */

public Q_SLOTS:
    void downLoadFinished();
    /*!
     * Download data from net finished.
     */
    void replyError(QNetworkReply::NetworkError error);
    /*!
     * Download reply error.
     */
#ifndef QT_NO_SSL
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    /*!
     * Download ssl reply error.
     */
#endif

protected:
    QString mapTypeFromEnumToString(TranslationType type);

    QNetworkReply *m_reply;
    QNetworkAccessManager *m_manager;

};

#endif // MUSICTRANSLATIONTHREAD_H
