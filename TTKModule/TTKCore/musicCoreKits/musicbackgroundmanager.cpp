#include "musicbackgroundmanager.h"
#include "musicsong.h"

MusicBackgroundManager::MusicBackgroundManager()
    : m_currentIndex(0)
{

}

void MusicBackgroundManager::setArtistName(const QString &name)
{
    m_currentArtistName = TTK::generateSongArtist(name);
    updateArtistPhotoList();
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
        return {};
    }
}

QString MusicBackgroundManager::artistPhotoDefaultPath() const
{
    if(m_photos.isEmpty())
    {
        return {};
    }

    const int index = m_currentIndex - 1;
    return (-1 < index && index < m_photos.count()) ? m_photos[index] : QString();
}

void MusicBackgroundManager::updateArtistPhotoList()
{
    m_photos.clear();
    m_currentIndex = 0;

    const QString &filter = BACKGROUND_DIR_FULL + m_currentArtistName + "%1" + SKN_FILE;
    for(int i = 0; i < MAX_IMAGE_COUNT; ++i)
    {
        QFile file(filter.arg(i));
        if(file.exists() && file.open(QIODevice::ReadOnly))
        {
            if(file.size() > 0)
            {
                m_photos << file.fileName();
            }
            file.close();
        }
    }

    Q_EMIT artistChanged();
}

QStringList MusicBackgroundManager::artistPhotoList() const
{
    return m_photos;
}

void MusicBackgroundManager::setArtistPhotoList(const QStringList &list)
{
    m_photos = list;
}

void MusicBackgroundManager::setSelectArtistIndex(int index)
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
