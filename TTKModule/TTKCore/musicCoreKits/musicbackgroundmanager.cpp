#include "musicbackgroundmanager.h"
#include "musicsong.h"

MusicBackgroundManager::MusicBackgroundManager() noexcept
    : m_currentIndex(0)
{

}

void MusicBackgroundManager::setArtistName(const QString &name)
{
    m_currentArtistName = TTK::generateSongArtist(name);
    updateArtistImageList();
}

QString MusicBackgroundManager::artistName() const noexcept
{
    return m_currentArtistName;
}

void MusicBackgroundManager::imageNext() noexcept
{
    m_currentIndex++;
}

void MusicBackgroundManager::imagePrevious() noexcept
{
    m_currentIndex--;
}

QString MusicBackgroundManager::artistImagePath() noexcept
{
    if(m_images.isEmpty())
    {
        return {};
    }

    return m_images[m_currentIndex < m_images.count() ? m_currentIndex : m_currentIndex = 0];
}

QString MusicBackgroundManager::artistImageDefaultPath() const noexcept
{
    if(m_images.isEmpty())
    {
        return {};
    }

    const int index = m_currentIndex - 1;
    return (-1 < index && index < m_images.count()) ? m_images[index] : QString();
}

void MusicBackgroundManager::updateArtistImageList()
{
    m_images.clear();
    m_currentIndex = 0;

    const QString &filter = BACKGROUND_DIR_FULL + m_currentArtistName + "-%1" + SKN_FILE;
    for(int i = 0; i < MAX_IMAGE_COUNT; ++i)
    {
        QFile file(filter.arg(i));
        if(file.exists() && file.open(QIODevice::ReadOnly))
        {
            if(file.size() > 0)
            {
                m_images << file.fileName();
            }
            file.close();
        }
    }

    Q_EMIT artistChanged();
}

QStringList MusicBackgroundManager::artistImageList() const noexcept
{
    return m_images;
}

void MusicBackgroundManager::setArtistImageList(const QStringList &list) noexcept
{
    m_images = list;
}

void MusicBackgroundManager::setSelectArtistIndex(int index) noexcept
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

void MusicBackgroundManager::setBackgroundUrl(const QString &path) noexcept
{
    m_background = path;
    Q_EMIT backgroundChanged();
}

QString MusicBackgroundManager::backgroundUrl() const noexcept
{
    return m_background;
}
