#include "musicbackgroundmanager.h"
#include "musicstringutils.h"

MusicBackgroundManager::MusicBackgroundManager()
{
    m_currentIndex = 0;
}

QString MusicBackgroundManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicBackgroundManager::setArtName(const QString &name)
{
    QString sName = MusicUtils::String::artistName(name);
    if(!m_currentArtName.isEmpty() && m_currentArtName != sName)
    {
        return;
    }

    m_photos.clear();
    QString filter = BACKGROUND_DIR_FULL + (m_currentArtName = sName) + "%1" + SKN_FILE;
    for(int i=0; i<MAX_INDEX; ++i)
    {
        if(QFile::exists(filter.arg(i)))
        {
            m_photos << filter.arg(i);
        }
    }
    emit artHasChanged();
}

void MusicBackgroundManager::clearArtName()
{
    m_currentArtName.clear();
}

void MusicBackgroundManager::indexIncrease()
{
    m_currentIndex++;
}

void MusicBackgroundManager::indexDecrease()
{
    m_currentIndex--;
}

QString MusicBackgroundManager::getArtPhotoPath()
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

QString MusicBackgroundManager::getArtPhotoPathNoIndex()
{
    if(m_photos.isEmpty())
    {
        return QString();
    }

    int index = m_currentIndex - 1;
    if(-1 < index && index < m_photos.count())
    {
        return m_photos[index];
    }

    return QString();
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

QStringList MusicBackgroundManager::getArtPhotoPathList() const
{
    return m_photos;
}

void MusicBackgroundManager::setArtPhotoPathList(const QStringList &list)
{
    m_photos = list;
}

void MusicBackgroundManager::setUserSelectArtIndex(int index)
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

void MusicBackgroundManager::setMBackground(const QString &path)
{
    m_background = path;
}

QString MusicBackgroundManager::getMBackground() const
{
    return m_background;
}

void MusicBackgroundManager::backgroundHasChanged()
{
    emit backgroundChanged();
}
