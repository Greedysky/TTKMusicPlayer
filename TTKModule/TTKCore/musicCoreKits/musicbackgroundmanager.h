#ifndef MUSICBACKGROUNDMANAGER_H
#define MUSICBACKGROUNDMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

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
    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current artist name.
     */
    void setArtName(const QString &name);
    /*!
     * Remove current artist name.
     */
    void clearArtName();

    /*!
     * Get artist photo current index.
     */
    inline int getCurrentIndex() const { return m_currentIndex; }
    /*!
     * Set artist photo current index.
     */
    inline void setCurrentIndex(int index) { m_currentIndex = index; }
    /*!
     * Get artist photo count.
     */
    inline int count() const { return m_photos.count(); }
    /*!
     * Get artist photo count is 0.
     */
    inline bool isEmpty() const { return m_photos.isEmpty(); }

    /*!
     * Set artist photo current index + 1.
     */
    void indexIncrease();
    /*!
     * Set artist photo current index - 1.
     */
    void indexDecrease();

    /*!
     * Get artist photo path.
     */
    QString getArtPhotoPath();
    /*!
     * Get artist photo path no index.
     */
    QString getArtPhotoPathNoIndex();
    /*!
     * Get artist photo path by given index.
     */
    QString getArtPhotoPathByIndex(int index = -1) const;

    /*!
     * Get artist photo path list.
     */
    QStringList getArtPhotoPathList() const;
    /*!
     * Set artist photo path by given path list.
     */
    void setArtPhotoPathList(const QStringList &list);
    /*!
     * Set user select current index.
     */
    void setUserSelectArtIndex(int index);

    /*!
     * Add observer.
     */
    void addObserver(QObject *object);
    /*!
     * Remove observer.
     */
    void removeObserver(QObject *object);
    /*!
     * Set artist background picture by path.
     */
    void setMBackground(const QString &path);
    /*!
     * Get artist background picture path.
     */
    QString getMBackground() const;

    /*!
     * Get artist background picture path.
     */
    void backgroundHasChanged();

Q_SIGNALS:
    /*!
     * Background image changed.
     */
    void backgroundChanged();
    /*!
     * New art has been set emit.
     */
    void artHasChanged();
    /*!
     * User selected index changed emit.
     */
    void userSelectIndexChanged();

protected:
    /*!
     * Object contsructor.
     */
    MusicBackgroundManager();

    int m_currentIndex;
    QStringList m_photos;
    QList<QObject*> m_observer;
    QString m_currentArtName, m_background;

    DECLARE_SINGLETON_CLASS(MusicBackgroundManager)
};

#endif // MUSICBACKGROUNDMANAGER_H
