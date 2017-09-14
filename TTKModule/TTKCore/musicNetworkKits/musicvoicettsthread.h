#ifndef MUSICVOICETTSTHREAD_H
#define MUSICVOICETTSTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicglobaldefine.h"

const QString VOICETTS_URL = "a0h4b0NBMmlrcjJGeFNDc200UnZUUGEzTm5BZENRUUZBckFUcGpmNlQ5aE5FY1p1M3h0anpFdFM5SDV2UW5Ba1pENHRVeGFGa0RNbHRzK25SZ0RkQk95anNZcmhLYnRHTlJpSi9nRFF3Mi9hVXlvajZ4SGhVZz09";

class MusicDownloadSourceThread;

/*! @brief The class of translation voice by tts thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicVoiceTtsThread : public QObject
{
    Q_OBJECT
public:
    enum TranslationType
    {
        Type_Uk,    /*!< English*/
        Type_En,    /*!< American English*/
        Type_Zh     /*!< Chinese*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicVoiceTtsThread(QObject *parent = 0);

    ~MusicVoiceTtsThread();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set speed.
     */
    void setSpeed(int speed);
    /*!
     * Get speed.
     */
    int getSpeed() const;

    /*!
     * Set pitch.
     */
    void setPitch(int pitch);
    /*!
     * Get pitch.
     */
    int getPitch() const;

    /*!
     * Set volume.
     */
    void setVolume(int volume);
    /*!
     * Get volume.
     */
    int getVolume() const;

    /*!
     * Set person.
     */
    void setPerson(int person);
    /*!
     * Get person.
     */
    int getPerson() const;

    /*!
     * Start to translation data.
     */
    void startToDownload(TranslationType type, const QString &data);

Q_SIGNALS:
    /*!
     * Send translated data from net.
     */
    void downLoadDataChanged(const QByteArray &data);

protected:
    /*!
     * Map type from enum to string.
     */
    QString mapTypeFromEnumToString(TranslationType type);

    int m_speed, m_pitch, m_volume, m_person;
    MusicDownloadSourceThread *m_parser;

};

#endif // MUSICVOICETTSTHREAD_H
