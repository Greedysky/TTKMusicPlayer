#ifndef MUSICBACKGROUNDMANAGER_H
#define MUSICBACKGROUNDMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    inline void setCurrentIndex(int index) { m_currentIndex = index; }
    /*!
     * Set artist photo current index.
     */
    inline int count() const { return m_photos.count(); }
    /*!
     * Get artist photo count.
     */
    inline bool isEmpty() const { return m_photos.isEmpty(); }
    /*!
     * Get artist photo count is 0.
     */

    void indexIncrease();
    /*!
     * Set artist photo current index + 1.
     */
    void indexDecrease();
    /*!
     * Set artist photo current index - 1.
     */

    QString getArtPhotoPath();
    /*!
     * Get artist photo path.
     */
    QString getArtPhotoPathNoIndex();
    /*!
     * Get artist photo path no index.
     */
    QString getArtPhotoPathByIndex(int index = -1) const;
    /*!
     * Get artist photo path by given index.
     */

    QStringList getArtPhotoPathList() const;
    /*!
     * Get artist photo path list.
     */
    void setArtPhotoPathList(const QStringList &list);
    /*!
     * Set artist photo path by given path list.
     */
    void setUserSelectArtIndex(int index);
    /*!
     * Set user select current index.
     */

    void addObserver(QObject *object);
    /*!
     * Add observer.
     */
    void removeObserver(QObject *object);
    /*!
     * Remove observer.
     */
    void setMBackground(const QString &path);
    /*!
     * Set artist background picture by path.
     */
    QString getMBackground() const;
    /*!
     * Get artist background picture path.
     */

    void backgroundHasChanged();
    /*!
     * Get artist background picture path.
     */

Q_SIGNALS:
    void backgroundChanged();
    /*!
     * Background image changed.
     */
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

    int m_currentIndex;
    QStringList m_photos;
    QList<QObject*> m_observer;
    QString m_currentArtName, m_background;

    DECLARE_SINGLETON_CLASS(MusicBackgroundManager)
};

#endif // MUSICBACKGROUNDMANAGER_H
