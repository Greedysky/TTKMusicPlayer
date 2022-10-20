#include "musicbackgroundmanager.h"
#include "musicstringutils.h"

#define MAX_INDEX 5

MusicBackgroundManager::MusicBackgroundManager()
    : m_currentIndex(0)
{

}

void MusicBackgroundManager::setArtistName(const QString &name)
{
    const QString &sName = MusicUtils::String::artistName(name);
    if(!m_currentArtistName.isEmpty() && m_currentArtistName != sName)
    {
        return;
    }

    m_photos.clear();
    const QString &filter = BACKGROUND_DIR_FULL + (m_currentArtistName = sName) + "%1" + SKN_FILE;
    for(int i = 0; i < MAX_INDEX; ++i)
    {
        if(QFile::exists(filter.arg(i)))
        {
            m_photos << filter.arg(i);
        }
    }
    Q_EMIT artistNameChanged();
}

void MusicBackgroundManager::clearArtistName()
{
    m_currentArtistName.clear();
}

void MusicBackgroundManager::indexIncrease()
{
    m_currentIndex++;
}

void MusicBackgroundManager::indexDecrease()
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

QString MusicBackgroundManager::artistPhotoPathNoIndex()
{
    if(m_photos.isEmpty())
    {
        return QString();
    }

    const int index = m_currentIndex - 1;
    if(-1 < index && index < m_photos.count())
    {
        return m_photos[index];
    }

    return QString();
}

QString MusicBackgroundManager::artistPhotoPathByIndex(int index) const
{
    if((0 < index && index != -1) || index > m_photos.count() || m_photos.isEmpty())
    {
        TTK_ERROR_STREAM("Index out of range");
        return QString();
    }
    return (index == -1) ? m_photos[m_currentIndex < m_photos.count() ? m_currentIndex : 0] : m_photos[index];
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
