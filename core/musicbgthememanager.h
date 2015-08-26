#ifndef MUSICBGTHEMEMANAGER_H
#define MUSICBGTHEMEMANAGER_H

#include "musicobject.h"
#include "musicsingletone.h"

#define M_ARTBG (MusicSingleton<MusicBgThemeManager>::createInstance())
#define MAX_INDEX 5

class MUSIC_CORE_EXPORT MusicBgThemeManager : public QObject
{
    Q_OBJECT
public:
    void setArtName(const QString &name);
    inline int getArtPhotoCount() const { return m_photos.count(); }
    QString getArtPhotoPath();
    QString getArtPhotoPathByIndex(int index = -1) const;

    QStringList getArtPhotoPaths() const;
    void setArtPhotoPaths(const QStringList &list);
    void sendUserSelectArtBg(int index);
    void setObject(QObject *object);

signals:
    void artHasChanged();

protected:
    MusicBgThemeManager();

    QStringList m_photos;
    int m_currentIndex;

    DECLARE_SINGLETON_CLASS(MusicBgThemeManager)
};

#endif // MUSICBGTHEMEMANAGER_H
