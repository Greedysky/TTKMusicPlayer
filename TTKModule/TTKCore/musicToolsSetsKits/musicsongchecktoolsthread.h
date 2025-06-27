#ifndef MUSICSONGCHECKTOOLSTHREAD_H
#define MUSICSONGCHECKTOOLSTHREAD_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
 ***************************************************************************/

#include "ttkabstractthread.h"
#include "musicsongchecktoolsunit.h"

/*! @brief The class of the song check tools rename thread.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongCheckToolsRenameThread : public TTKAbstractThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongCheckToolsRenameThread)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongCheckToolsRenameThread(QObject *parent = nullptr);

    /*!
     * Set music song check tool mode.
     */
    inline void setMode(TTK::Mode mode) noexcept { m_operateMode = mode; }
    /*!
     * Get music song check tool mode.
     */
    inline TTK::Mode mode() const noexcept { return m_operateMode; }
    /*!
     * Set item list.
     */
    inline void setItemList(const TTKIntList &items) noexcept { m_ids = items; }

    /*!
     * Set find file path by given path.
     */
    void setRenameSongs(MusicSongList *songs) noexcept;

Q_SIGNALS:
    /*!
     * Rename check finished.
     */
    void finished(const MusicSongCheckToolsRenameList &items);

private:
    /*!
     * Thread run now.
     */
    virtual void run() override final;

    TTKIntList m_ids;
    MusicSongList *m_items;
    TTK::Mode m_operateMode;
    MusicSongCheckToolsRenameList m_datas;

};


/*! @brief The class of the song check tools duplicate thread.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongCheckToolsDuplicateThread : public TTKAbstractThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongCheckToolsDuplicateThread)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongCheckToolsDuplicateThread(QObject *parent = nullptr);

    /*!
     * Set music song check tool mode.
     */
    inline void setMode(TTK::Mode mode) noexcept { m_operateMode = mode; }
    /*!
     * Get music song check tool mode.
     */
    inline TTK::Mode mode() const noexcept { return m_operateMode; }
    /*!
     * Set item list.
     */
    inline void setItemList(const TTKIntList &items) noexcept { m_ids = items; }

    /*!
     * Set find file path by given path.
     */
    void setDuplicateSongs(MusicSongList *songs) noexcept;

Q_SIGNALS:
    /*!
     * Duplicate check finished.
     */
    void finished(const MusicSongCheckToolsDuplicateList &items);

private:
    /*!
     * Thread run now.
     */
    virtual void run() override final;

    TTKIntList m_ids;
    MusicSongList *m_items;
    TTK::Mode m_operateMode;
    MusicSongCheckToolsDuplicateList m_datas;

};


/*! @brief The class of the song check tools quality thread.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongCheckToolsQualityThread : public TTKAbstractThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongCheckToolsQualityThread)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongCheckToolsQualityThread(QObject *parent = nullptr);

    /*!
     * Set find file path by given path.
     */
    void setQualitySongs(MusicSongList *songs) noexcept;

Q_SIGNALS:
    /*!
     * Quality check finished.
     */
    void finished(const MusicSongCheckToolsQualityList &items);

private:
    /*!
     * Thread run now.
     */
    virtual void run() override final;

    MusicSongList *m_items;

};

#endif // MUSICSONGCHECKTOOLSTHREAD_H
