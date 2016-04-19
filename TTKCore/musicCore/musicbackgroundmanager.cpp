#include "musicbackgroundmanager.h"

MusicBackgroundManager::MusicBackgroundManager()
{
    m_currentIndex = 0;
}

void MusicBackgroundManager::clearArtName()
{
    m_currentArtName.clear();
}

void MusicBackgroundManager::setArtName(const QString &name)
{
    QString sName = name.split('-').front().trimmed();
    if(!m_currentArtName.isEmpty() && m_currentArtName != sName)
    {
        return;
    }

    m_photos.clear();
    QString filter = ART_BG_AL + (m_currentArtName = sName) + "%1" + SKN_FILE;
    for(int i=0; i<MAX_INDEX; ++i)
    {
        if(QFile::exists(filter.arg(i)))
        {
            m_photos << filter.arg(i);
        }
    }
    emit artHasChanged();
}

QString MusicBackgroundManager::getArtPhotoPath()
{
    if(!m_photos.isEmpty())
    {
        return m_photos[m_currentIndex < m_photos.count() ?
                        m_currentIndex++ : m_currentIndex = 0];
    }
    else
    {
        return QString();
    }
}

QString MusicBackgroundManager::getArtPhotoPathByIndex(int index) const
{
    if((0 < index && index != -1) || index > m_photos.count() || m_photos.isEmpty())
    {
        M_LOGGER_ERROR("index out of range");
        return QString();
    }
    return (index == -1) ? m_photos[m_currentIndex < m_photos.count() ? m_currentIndex : 0]
                         : m_photos[index];
}

QStringList MusicBackgroundManager::getArtPhotoPaths() const
{
    return m_photos;
}

void MusicBackgroundManager::setArtPhotoPaths(const QStringList &list)
{
    m_photos = list;
}

void MusicBackgroundManager::setUserSelectArtIndex(int index)
{
    m_currentIndex = index;
    emit userSelectIndexChanged();
}
