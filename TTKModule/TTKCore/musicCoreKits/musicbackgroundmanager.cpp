#include "musicbackgroundmanager.h"
#include "musicstringutils.h"

MusicBackgroundManager::MusicBackgroundManager()
{
    m_currentIndex = 0;
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
    for(int i=0; i<MAX_INDEX; ++i)
    {
        if(QFile::exists(filter.arg(i)))
        {
            m_photos << filter.arg(i);
        }
    }
    emit artistNameChanged();
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

QString MusicBackgroundManager::getArtistPhotoPath()
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

QString MusicBackgroundManager::getArtistPhotoPathNoIndex()
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

QString MusicBackgroundManager::getArtistPhotoPathByIndex(int index) const
{
    if((0 < index && index != -1) || index > m_photos.count() || m_photos.isEmpty())
    {
        M_LOGGER_ERROR("index out of range");
        return QString();
    }
    return (index == -1) ? m_photos[m_currentIndex < m_photos.count() ? m_currentIndex : 0] : m_photos[index];
}

QStringList MusicBackgroundManager::getArtistPhotoPathList() const
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
    emit userSelectIndexChanged();
}

void MusicBackgroundManager::addObserver(QObject *object)
{
    m_observer << object;
    connect(this, SIGNAL(backgroundChanged()), object, SLOT(backgroundChanged()));
}

void MusicBackgroundManager::removeObserver(QObject *object)
{
    m_observer.removeAll(object);
    disconnect(this, SIGNAL(backgroundChanged()), object, SLOT(backgroundChanged()));
}

void MusicBackgroundManager::setBackgroundUrl(const QString &path)
{
    m_background = path;
}

QString MusicBackgroundManager::getBackgroundUrl() const
{
    return m_background;
}

void MusicBackgroundManager::backgroundHasChanged()
{
    emit backgroundChanged();
}
