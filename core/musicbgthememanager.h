#ifndef MUSICBGTHEMEMANAGER_H
#define MUSICBGTHEMEMANAGER_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicsingletone.h"

#define M_BG_MANAGER (MusicSingleton<MusicBgThemeManager>::createInstance())
#define MAX_INDEX 5

class MUSIC_CORE_EXPORT MusicBgThemeManager : public QObject
{
    Q_OBJECT
public:
    void setArtName(const QString &name);
    void clearArtName();

    inline int getArtPhotoCount() const { return m_photos.count(); }
    QString getArtPhotoPath();
    QString getArtPhotoPathByIndex(int index = -1) const;

    QStringList getArtPhotoPaths() const;
    void setArtPhotoPaths(const QStringList &list);
    void sendUserSelectArtBg(int index);
    void setObject(QObject *object) const ;

    void setMBackground(const QString &path) { m_MBackground = path;}
    QString getMBackground() const {return m_MBackground;}

signals:
    void artHasChanged();

protected:
    MusicBgThemeManager();

    QString m_currentArtName;
    QString m_MBackground;
    QStringList m_photos;
    int m_currentIndex;

    DECLARE_SINGLETON_CLASS(MusicBgThemeManager)
};

#endif // MUSICBGTHEMEMANAGER_H
