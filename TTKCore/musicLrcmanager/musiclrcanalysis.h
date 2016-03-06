#ifndef MUSICLRCANALYSIS_H
#define MUSICLRCANALYSIS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicglobaldefine.h"

#define MIN_LRCCONTAIN_COUNT 11
#define CURRENT_LRC_PAINT 5

/*! @brief The class of the core lrc analysis.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcAnalysis : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        Null,               ///*no any operator*/
        OpenFileSuccess,    ///*open file success*/
        OpenFileFail,       ///*open file failed*/
        LrcEmpty            ///*current lrc empty*/
    };
    enum LrcFormat
    {
        Type01, ///*[xx:xx.xxx]*/
        Type02, ///*[xx:xx.xx]*/
        Type03, ///*[xx:xx.x]*/
        Type04, ///*[xx:xx:xxx]*/
        Type05, ///*[xx:xx:xx]*/
        Type06, ///*[xx:xx:x]*/
        Type07, ///*[xx:xx]*/
        Type08, ///*[xx.xx.xxx]*/
        Type09, ///*[xx.xx.xx]*/
        Type10, ///*[xx.xx.x]*/
        Type11, ///*[xx.xx:xxx]*/
        Type12, ///*[xx.xx:xx]*/
        Type13, ///*[xx.xx:x]*/
        Type14  ///*[xx.xx]*/
    };

    explicit MusicLrcAnalysis(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicLrcAnalysis();

    State transLrcFileToTime(const QString &lrcFileName);
    /*!
     * Analysis lrc file to map return the state.
     */
    qint64 setSongSpeedAndSlow(qint64 time);
    /*!
     * Set song speed and slow by given time, return new time.
     */
    void revertLrcTime(qint64 pos);
    /*!
     * Revert lrc time by pos, both + or - the same pos.
     */

    void setCurrentIndex(int index) { m_currentLrcIndex = index;}
    /*!
     * Set current middle index.
     */
    int getCurrentIndex() const { return m_currentLrcIndex;}
    /*!
     * Get current middle index.
     */
    QString getCurrentFileName() const { return m_currentLrcFileName;}
    /*!
     * Get current file name.
     */

    bool valid() const;
    /*!
     * Check current index is valid or not.
     */
    bool isEmpty() const;
    /*!
     * Check current container is empty or not.
     */
    int count() const;
    /*!
     * Check current container count.
     */
    QString getText(int index) const;
    /*!
     * Get current lrc text by index.
     */
    bool findText(qint64 current, qint64 total,
                  QString &pre, QString &last, qint64 &interval) const;
    /*!
     * Get current lrc and next lrc in container by current time.
     */
    qint64 findTime(int index) const;
    /*!
     * Get current time by index.
     */
    QString getAllLrcs() const;
    /*!
     * Get all lrcs from container.
     */

protected:
    void matchLrcLine(const QString &oneLine);
    /*!
     * Lrc analysis by match lrc line base.
     */
    void matchLrcLine(const QString &oneLine, const QString &cap,
                      const QString &first, const QString &second);
    /*!
     * Lrc analysis by match lrc line two[xx.(:)xx].
     */
    void matchLrcLine(const QString &oneLine, QString cap,
                      const QString &splite);
    /*!
     * Lrc analysis by match lrc line three[xx.xx.x(xx)]\[xx:xx:x(xx)].
     */
    void matchLrcLine(const QString &oneLine, const QString &cap,
                      const QString &first, const QString &second,
                      const QString &third);
    /*!
     * Lrc analysis by match lrc line three[xx.(:)xx.(:)x(xx)].
     */

    int m_currentLrcIndex;
    QStringList m_currentShowLrcContainer;
    QString m_currentLrcFileName;
    MIntStringMap m_lrcContainer;

};

#endif // MUSICLRCANALYSIS_H
