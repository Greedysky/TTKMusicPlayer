#include "musicbackgroundmanager.h"
#include "musicstringutils.h"

MusicBackgroundManager::MusicBackgroundManager()
    : m_currentIndex(0)
{

}

void MusicBackgroundManager::setArtistName(const QString &name)
{
    m_photos.clear();
    m_currentIndex = 0;
    m_currentArtistName = MusicUtils::String::artistName(name);

    const QString &filter = BACKGROUND_DIR_FULL + m_currentArtistName + "%1" + SKN_FILE;
    for(int i = 0; i < MAX_IMAGE_COUNTER; ++i)
    {
        QFile file(filter.arg(i));
        if(file.open(QIODevice::ReadOnly))
        {
            if(file.exists() && file.size() > 0)
            {
                m_photos << file.fileName();
            }
            file.close();
        }
    }

    Q_EMIT artistNameChanged();
}

QString MusicBackgroundManager::artistName() const
{
    return m_currentArtistName;
}

void MusicBackgroundManager::photoNext()
{
    m_currentIndex++;
}

void MusicBackgroundManager::photoPrevious()
{
    m_currentIndex--;
}

QString MusicBackgroundManager::artistPhotoPath()
{
    if(!m_photos.isEmpty())
    {
        return m_photos[m_currentIndex < m_photos.count() ? m_currentIndex : m_currentIndex = 0];
    }
    else
    {
        return QString();
    }
}

QString MusicBackgroundManager::artistPhotoDefaultPath() const
{
    if(m_photos.isEmpty())
    {
        return QString();
    }

    const int index = m_currentIndex - 1;
    return (-1 < index && index < m_photos.count()) ? m_photos[index] : QString();
}

QStringList MusicBackgroundManager::artistPhotoPathList() const
{
    return m_photos;
}

void MusicBackgroundManager::setArtistPhotoPathList(const QStringList &list)
{
    m_photos = list;
}

void MusicBackgroundManager::setUserSelectArtistIndex(int index)
{
    m_currentIndex = index;
    Q_EMIT selectIndexChanged();
}

void MusicBackgroundManager::addObserver(QObject *object)
{
    m_observer << object;
    connect(this, SIGNAL(backgroundChanged()), object, SLOT(backgroundChanged()));
}

void MusicBackgroundManager::removeObserver(QObject *object)
{
    m_observer.removeOne(object);
    disconnect(this, SIGNAL(backgroundChanged()), object, SLOT(backgroundChanged()));
}

void MusicBackgroundManager::setBackgroundUrl(const QString &path)
{
    m_background = path;
}

QString MusicBackgroundManager::backgroundUrl() const
{
    return m_background;
}

void MusicBackgroundManager::backgroundUrlChanged()
{
    Q_EMIT backgroundChanged();
}
