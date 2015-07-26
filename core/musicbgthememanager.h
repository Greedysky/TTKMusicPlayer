#ifndef MUSICBGTHEMEMANAGER_H
#define MUSICBGTHEMEMANAGER_H

#include "musicobject.h"
#include "musicsingletone.h"

#define M_ARTBG (MusicSingleton<MusicBgThemeManager>::createInstance())
#define MAX_INDEX 5

class MUSIC_EXPORT MusicBgThemeManager : public QObject
{
    Q_OBJECT
public:
    void setArtName(const QString&);
    inline int getArtPhotoCount() { return m_photos.count(); }
    QString getArtPhotoPath();
    QString getArtPhotoPathByIndex(int index = -1);

    QStringList getArtPhotoPaths();
    void setArtPhotoPaths(const QStringList&);

    void sendUserSelectArtBg(int);

    void setObject(QObject*);

signals:
    void artHasChanged();

protected:
    MusicBgThemeManager();

    QStringList m_photos;
    int m_currentIndex;

    DECLARE_SINGLETON_CLASS(MusicBgThemeManager)
};

#endif // MUSICBGTHEMEMANAGER_H
