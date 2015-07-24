#include "musicbgthememanager.h"

MusicBgThemeManager::MusicBgThemeManager()
{
    m_currentIndex = 0;
}

void MusicBgThemeManager::setArtName(const QString& name)
{
    m_photos.clear();

    QString filter = ART_BG + name.split('-').front().trimmed()+ "%1" + SKN_FILE;
    for(int i=0; i<MAX_INDEX; ++i)
    {
        if(QFile::exists(filter.arg(i)))
        {
            m_photos << filter.arg(i);
        }
    }
}

QString MusicBgThemeManager::getArtPhotoPath()
{
    if(!m_photos.isEmpty())
        return m_photos[m_currentIndex < m_photos.count() ? m_currentIndex++ : m_currentIndex = 0];
    else
        return QString();
}

QString MusicBgThemeManager::getArtPhotoPathByIndex(int index)
{
    if((0 < index && index != -1) || index > m_photos.count() )
    {
        qDebug()<<"index out of range";
        return QString();
    }
    return (index == -1) ? m_photos[m_currentIndex < m_photos.count() ? m_currentIndex : 0]
                         : m_photos[index];
}

QStringList MusicBgThemeManager::getArtPhotoPaths()
{
    return m_photos;
}
