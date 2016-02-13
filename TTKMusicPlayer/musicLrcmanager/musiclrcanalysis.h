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

class MUSIC_LRC_EXPORT MusicLrcAnalysis : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        Null,
        OpenFileSucess,
        OpenFileFail,
        LrcEmpty
    };
    enum LrcFormat
    {
        Type01,//[xx:xx.xxx]
        Type02,//[xx:xx.xx]
        Type03,//[xx:xx.x]
        Type04,//[xx:xx:xxx]
        Type05,//[xx:xx:xx]
        Type06,//[xx:xx:x]
        Type07,//[xx:xx]
        Type08,//[xx.xx.xxx]
        Type09,//[xx.xx.xx]
        Type10,//[xx.xx.x]
        Type11,//[xx.xx:xxx]
        Type12,//[xx.xx:xx]
        Type13,//[xx.xx:x]
        Type14 //[xx.xx]
    };

    explicit MusicLrcAnalysis(QObject *parent = 0);
    ~MusicLrcAnalysis();

    State transLrcFileToTime(const QString &lrcFileName);
    qint64 setSongSpeedAndSlow(qint64 time);
    void revertLrcTime(qint64 pos);

    void setCurrentIndex(int index) { m_currentLrcIndex = index;}
    int getCurrentIndex() const { return m_currentLrcIndex;}
    QString getCurrentFileName() const { return m_currentLrcFileName;}

    bool valid() const;
    bool isEmpty() const;
    QString getText(int index) const;
    bool findText(qint64 current, qint64 total,
                  QString &pre, QString &last, qint64 &interval) const;
    qint64 findTime(int index) const;

    QString getAllLrcs() const;

protected:
    void matchLrcLine(const QString &oneLine);
    void matchLrcLine(const QString &oneLine, const QString &cap,
                      const QString &first, const QString &second);
    void matchLrcLine(const QString &oneLine, QString cap,
                      const QString &splite);
    void matchLrcLine(const QString &oneLine, const QString &cap,
                      const QString &first, const QString &second,
                      const QString &third);

    int m_currentLrcIndex;
    QStringList m_currentShowLrcContainer;
    QString m_currentLrcFileName;
    MIntStringMap m_lrcContainer;

};

#endif // MUSICLRCANALYSIS_H
