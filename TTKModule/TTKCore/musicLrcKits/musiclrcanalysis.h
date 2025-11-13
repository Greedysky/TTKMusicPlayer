#ifndef MUSICLRCANALYSIS_H
#define MUSICLRCANALYSIS_H

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

#include "musicglobaldefine.h"

static constexpr int MUSIC_LRC_INTERIOR_MAX_LINE = 11;

/*! @brief The class of the core lrc analysis.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcAnalysis : public QObject
{
    Q_OBJECT
public:
    enum class State
    {
        Success,    /*!< open file success */
        Failed      /*!< open file failed */
    };

    enum class Format
    {
        Type01,     /*!< [xx:xx.xxx] */
        Type02,     /*!< [xx:xx.xx] */
        Type03,     /*!< [xx:xx.x] */
        Type04,     /*!< [xx:xx:xxx] */
        Type05,     /*!< [xx:xx:xx] */
        Type06,     /*!< [xx:xx:x] */
        Type07,     /*!< [xx:xx] */
        Type08,     /*!< [xx.xx.xxx] */
        Type09,     /*!< [xx.xx.xx] */
        Type10,     /*!< [xx.xx.x] */
        Type11,     /*!< [xx.xx:xxx] */
        Type12,     /*!< [xx.xx:xx] */
        Type13,     /*!< [xx.xx:x] */
        Type14      /*!< [xx.xx] */
    };

    /*!
     * Object constructor.
     */
    explicit MusicLrcAnalysis(QObject *parent = nullptr);

    /*!
     * Set current line maximum value.
     */
    inline void setLineMax(int max) noexcept { m_lineMax = max; }
    /*!
     * Get current line maximum value.
     */
    inline int lineMax() const noexcept { return m_lineMax; }
    /*!
     * Get current line middle number.
     */
    inline int lineMiddle() const noexcept { return m_lineMax / 2; }

    /*!
     * Save lrc data to current lrc file.
     */
    void saveData();
    /*!
     * Set lrc container data from other raw data.
     */
    State setData(const TTKIntStringMap &data);

    /*!
     * Analysis lyric file to map return the state.
     */
    State loadFromFile(const QString &path);

    /*!
     * Set lrc time by pos, both + or - the same pos.
     */
    void setTimePosition(qint64 pos);
    /*!
     * Find song time position by given time, return new time.
     */
    qint64 findTimePosition(qint64 time);

    /*!
     * Set current middle index.
     */
    inline void setCurrentIndex(int index) noexcept { m_currentIndex = index; }
    /*!
     * Get current middle index.
     */
    inline int currentIndex() const noexcept { return m_currentIndex; }
    /*!
     * Set current file path.
     */
    inline void setCurrentFilePath(const QString &name) noexcept { m_currentFilePath = name; }
    /*!
     * Get current file path.
     */
    inline QString currentFilePath() const noexcept { return m_currentFilePath; }

    /*!
     * Clear current lrc cache.
     */
    void clear() noexcept;
    /*!
     * Check current index is valid or not.
     */
    bool isValid() const noexcept;
    /*!
     * Check current container is empty or not.
     */
    bool isEmpty() const noexcept;
    /*!
     * Check current container count.
     */
    int count() const noexcept;

    /*!
     * Get current lrc text by index.
     */
    QString text(int index) const;
    /*!
     * Get current lrc and next lrc in container by current time.
     */
    bool findText(qint64 current, qint64 total, QString &pre, QString &last, qint64 &interval) const;
    /*!
     * Get current time by index.
     */
    qint64 findTime(int index) const;
    /*!
     * Get current time by texts.
     */
    qint64 findTime(const QStringList &ts) const;

    /*!
     * Get all lrc from container.
     */
    QString dataString() const;
    /*!
     * Get all lrc from container.
     */
    QStringList dataList() const;

private:
    /*!
     * Init parameters.
     */
    State initialize();
    /*!
     * Lrc analysis by match lrc line base.
     */
    void matchLrcLine(const QString &oneLine);
    /*!
     * Lrc analysis by match lrc line two[xx.(:)xx].
     */
    void matchLrcLine(const QString &oneLine, const QString &cap, const QString &first, const QString &second);
    /*!
     * Lrc analysis by match lrc line three[xx.xx.x(xx)]\[xx:xx:x(xx)].
     */
    void matchLrcLine(const QString &oneLine, QString cap, const QString &splite);
    /*!
     * Lrc analysis by match lrc line three[xx.(:)xx.(:)x(xx)].
     */
    void matchLrcLine(const QString &oneLine, const QString &cap, const QString &first, const QString &second, const QString &third);

    int m_lineMax, m_currentIndex;
    QString m_currentFilePath;
    TTKIntStringMap m_lrcContainer;
    QStringList m_currentShowLrcContainer;

};

#endif // MUSICLRCANALYSIS_H
