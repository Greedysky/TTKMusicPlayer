#ifndef MUSICBACKGROUNDMANAGER_H
#define MUSICBACKGROUNDMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicsingleton.h"

#define M_BACKGROUND_PTR (MusicSingleton<MusicBackgroundManager>::createInstance())

#define MAX_INDEX 5

/*! @brief The class of the manager of dealing with artist pictures.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicBackgroundManager : public QObject
{
    Q_OBJECT
public:
    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setArtName(const QString &name);
    /*!
     * Set current artist name.
     */
    void clearArtName();
    /*!
     * Remove current artist name.
     */

    inline int getCurrentIndex() const { return m_currentIndex; }
    /*!
     * Get artist photo current index.
     */
    inline int getArtPhotoCount() const { return m_photos.count(); }
    /*!
     * Get artist photo count.
     */
    QString getArtPhotoPath();
    /*!
     * Get artist photo path.
     */
    QString getArtPhotoPathByIndex(int index = -1) const;
    /*!
     * Get artist photo path by given index.
     */

    QStringList getArtPhotoPaths() const;
    /*!
     * Get artist photo path list.
     */
    void setArtPhotoPaths(const QStringList &list);
    /*!
     * Set artist photo path by given path list.
     */
    void setUserSelectArtIndex(int index);
    /*!
     * Set user select current index.
     */

    void setMBackground(const QString &path) { m_background = path;}
    /*!
     * Set artist background picture by path.
     */
    QString getMBackground() const {return m_background;}
    /*!
     * Get artist background picture path.
     */

Q_SIGNALS:
    void artHasChanged();
    /*!
     * New art has been set emit.
     */
    void userSelectIndexChanged();
    /*!
     * User selected index changed emit.
     */

protected:
    MusicBackgroundManager();
    /*!
     * Object contsructor.
     */
    ~MusicBackgroundManager() = default;

    QString m_currentArtName;
    QString m_background;
    QStringList m_photos;
    int m_currentIndex;

    DECLARE_SINGLETON_CLASS(MusicBackgroundManager)
};

#endif // MUSICBACKGROUNDMANAGER_H
