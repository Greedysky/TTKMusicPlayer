#ifndef MUSICLRCANALYSIS_H
#define MUSICLRCANALYSIS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
#include "musicglobaldefine.h"

#define MUSIC_TTKLRCF               "[TTKLRCF]"
#define MUSIC_LRC_INLINE_MAX_LINE   11

class MusicTranslationThreadAbstract;

/*! @brief The class of the core lrc analysis.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcAnalysis : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcAnalysis)
public:
    enum State
    {
        Null,               /*!< no any operator*/
        OpenFileSuccess,    /*!< open file success*/
        OpenFileFail,       /*!< open file failed*/
        LrcEmpty            /*!< current lrc empty*/
    };

    enum LrcFormat
    {
        Type01, /*!< [xx:xx.xxx]*/
        Type02, /*!< [xx:xx.xx]*/
        Type03, /*!< [xx:xx.x]*/
        Type04, /*!< [xx:xx:xxx]*/
        Type05, /*!< [xx:xx:xx]*/
        Type06, /*!< [xx:xx:x]*/
        Type07, /*!< [xx:xx]*/
        Type08, /*!< [xx.xx.xxx]*/
        Type09, /*!< [xx.xx.xx]*/
        Type10, /*!< [xx.xx.x]*/
        Type11, /*!< [xx.xx:xxx]*/
        Type12, /*!< [xx.xx:xx]*/
        Type13, /*!< [xx.xx:x]*/
        Type14  /*!< [xx.xx]*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicLrcAnalysis(QObject *parent = nullptr);

    ~MusicLrcAnalysis();

    /*!
     * Set current line maximum value.
     */
    inline void setLineMax(int max) { m_lineMax = max;}
    /*!
     * Get current line maximum value.
     */
    inline int getLineMax() const { return m_lineMax;}
    /*!
     * Get current line middle number.
     */
    inline int getMiddle() const { return m_lineMax/2;}

    /*!
     * Set lrc container data from other raw data.
     */
    State setLrcData(const QByteArray &data);
    /*!
     * Set lrc container data from other raw data.
     */
    State setLrcData(const MIntStringMap &data);
    /*!
     * Analysis lrc file to map return the state.
     */
    State transLrcFileToTime(const QString &lrcFileName);
    /*!
     * Analysis krc file to map return the state.
     */
    State transKrcFileToTime(const QString &krcFileName);

    /*!
     * Set song speed by given time, return new time.
     */
    qint64 setSongSpeedChanged(qint64 time);
    /*!
     * Revert lrc time by pos, both + or - the same pos.
     */
    void revertLrcTime(qint64 pos);
    /*!
     * Save lrc time changed to current lrc file.
     */
    void saveLrcTimeChanged();

    /*!
     * Set current middle index.
     */
    void setCurrentIndex(int index) { m_currentLrcIndex = index;}
    /*!
     * Get current middle index.
     */
    int getCurrentIndex() const { return m_currentLrcIndex;}
    /*!
     * Set current file name.
     */
    inline void setCurrentFileName(const QString &name) { m_currentLrcFileName = name;}
    /*!
     * Get current file name.
     */
    inline QString getCurrentFileName() const { return m_currentLrcFileName;}

    /*!
     * Check current index is valid or not.
     */
    bool isValid() const;
    /*!
     * Check current container is empty or not.
     */
    bool isEmpty() const;
    /*!
     * Check current container count.
     */
    int count() const;
    /*!
     * Get current lrc text by index.
     */
    QString getText(int index) const;
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
     * Get all lrcs from container.
     */
    QStringList getAllLrcList() const;
    /*!
     * Get all lrcs from container.
     */
    QString getAllLrcString() const;

public Q_SLOTS:
    /*!
     * Get all lrcs from tr container.
     */
    void getTranslatedLrc();

protected:
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

    int m_lineMax, m_currentLrcIndex;
    QString m_currentLrcFileName;
    MIntStringMap m_lrcContainer;
    QStringList m_currentShowLrcContainer;
    MusicTranslationThreadAbstract *m_translationThread;

};

#endif // MUSICLRCANALYSIS_H
