#include "musicvoicettsthread.h"
#include "musicsourcedownloadthread.h"

MusicVoiceTtsThread::MusicVoiceTtsThread(QObject *parent)
    : QObject(parent)
{
    m_speed = 5;
    m_pitch = 5;
    m_volume = 5;
    m_person = 0;

    m_parser = new MusicSourceDownloadThread(this);
    connect(m_parser, SIGNAL(downLoadByteDataChanged(QByteArray)),
                      SIGNAL(downLoadDataChanged(QByteArray)));
}

MusicVoiceTtsThread::~MusicVoiceTtsThread()
{
    delete m_parser;
}

QString MusicVoiceTtsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicVoiceTtsThread::setSpeed(int speed)
{
    m_speed = speed;
}

int MusicVoiceTtsThread::getSpeed() const
{
    return m_speed;
}

void MusicVoiceTtsThread::setPitch(int pitch)
{
    m_pitch = pitch;
}

int MusicVoiceTtsThread::getPitch() const
{
    return m_pitch;
}

void MusicVoiceTtsThread::setVolume(int volume)
{
    m_volume = volume;
}

int MusicVoiceTtsThread::getVolume() const
{
    return m_volume;
}

void MusicVoiceTtsThread::setPerson(int person)
{
    m_person = person;
}

int MusicVoiceTtsThread::getPerson() const
{
    return m_person;
}

void MusicVoiceTtsThread::startToTranslation(TranslationType type, const QString &data)
{
    m_parser->startToDownload( MusicCryptographicHash::decryptData(VOICETTS_URL, URL_KEY)
                               .arg(mapTypeFromEnumToString(type)).arg(data)
                               .arg(m_speed).arg(m_pitch).arg(m_volume).arg(m_person) );
}

QString MusicVoiceTtsThread::mapTypeFromEnumToString(TranslationType type)
{
    switch(type)
    {
        case Type_En: return "en";
        case Type_Uk: return "uk";
        case Type_Zh: return "zh";
        default: return QString();
    }
}
