#include "musiclrcanalysis.h"
#ifndef MUSIC_MOBILE
#include "musiclrcfromkrc.h"
#endif
#include <qmath.h>
#include "musictime.h"
#include "musicstringutils.h"
#include "musicapplication.h"
#include "musicdownloadqueryfactory.h"
#include "musictranslationthreadabstract.h"

MusicLrcAnalysis::MusicLrcAnalysis(QObject *parent)
    : QObject(parent)
{
    m_lineMax = 0;
    m_currentLrcIndex = 0;
    m_translationThread = nullptr;
}

MusicLrcAnalysis::~MusicLrcAnalysis()
{
    delete m_translationThread;
}

MusicLrcAnalysis::State MusicLrcAnalysis::setLrcData(const QByteArray &data)
{
    m_currentLrcIndex = 0;
    m_lrcContainer.clear();
    m_currentShowLrcContainer.clear();

    QStringList getAllText = QString(data).split("\n");
    if(data.left(9) == MUSIC_TTKLRCF) //plain txt check
    {
        getAllText[0].clear();
        const int perTime = MusicApplication::instance()->duration()/getAllText.count();
        foreach(const QString &oneLine, getAllText)
        {
            m_lrcContainer.insert(perTime*m_lrcContainer.count(), oneLine);
        }
    }
    else
    {
        foreach(const QString &oneLine, getAllText)
        {
            matchLrcLine(oneLine);
        }
    }

    if(m_lrcContainer.isEmpty())
    {
        return LrcEmpty;
    }

    for(int i=0; i<getMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }
    if(m_lrcContainer.find(0) == m_lrcContainer.end())
    {
        m_lrcContainer.insert(0, QString());
    }

    MIntStringMapIterator it(m_lrcContainer);
    while(it.hasNext())
    {
        it.next();
        m_currentShowLrcContainer << it.value();
    }
    for(int i=0; i<getMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    return OpenFileSuccess;
}

MusicLrcAnalysis::State MusicLrcAnalysis::setLrcData(const MIntStringMap &data)
{
    if(data.isEmpty())
    {
        return LrcEmpty;
    }

    m_lrcContainer = data;
    m_currentLrcIndex = 0;
    m_currentShowLrcContainer.clear();

    for(int i=0; i<getMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }
    if(m_lrcContainer.find(0) == m_lrcContainer.end())
    {
        m_lrcContainer.insert(0, QString());
    }

    MIntStringMapIterator it(m_lrcContainer);
    while(it.hasNext())
    {
        it.next();
        m_currentShowLrcContainer << it.value();
    }
    for(int i=0; i<getMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    return OpenFileSuccess;
}

MusicLrcAnalysis::State MusicLrcAnalysis::transLrcFileToTime(const QString &lrcFileName)
{
    QFile file(m_currentLrcFileName = lrcFileName);

    if(!file.open(QIODevice::ReadOnly))
    {
        return OpenFileFail;
    }

    const State state = setLrcData(file.readAll());
    file.close();

    return state;
}

MusicLrcAnalysis::State MusicLrcAnalysis::transKrcFileToTime(const QString &krcFileName)
{
#ifndef MUSIC_MOBILE
    m_lrcContainer.clear();
    m_currentShowLrcContainer.clear();
    m_currentLrcIndex = 0;
    m_currentLrcFileName = krcFileName;

    MusicLrcFromKrc krc;
    if(!krc.decode(krcFileName))
    {
        return OpenFileFail;
    }

    const QString &getAllText = QString(krc.getDecodeString());
    //The lyrics by line into the lyrics list
    foreach(const QString &oneLine, getAllText.split( MusicUtils::String::splitLineKey() ))
    {
        matchLrcLine(oneLine);
    }

    //If the lrcContainer is empty
    if(m_lrcContainer.isEmpty())
    {
        return LrcEmpty;
    }

    for(int i=0; i<getMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }
    if(m_lrcContainer.find(0) == m_lrcContainer.end())
    {
       m_lrcContainer.insert(0, QString());
    }

    MIntStringMapIterator it(m_lrcContainer);
    while(it.hasNext())
    {
        it.next();
        m_currentShowLrcContainer << it.value();
    }
    for(int i=0; i<getMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    return OpenFileSuccess;
#else
    Q_UNUSED(krcFileName);
    return OpenFileFail;
#endif
}

void MusicLrcAnalysis::matchLrcLine(const QString &oneLine)
{
    const QRegExp regx01("\\[\\d{2}:\\d{2}\\.\\d{3}\\]");
    const QRegExp regx02("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
    const QRegExp regx03("\\[\\d{2}:\\d{2}\\.\\d{1}\\]");
    const QRegExp regx04("\\[\\d{2}:\\d{2}:\\d{3}\\]");
    const QRegExp regx05("\\[\\d{2}:\\d{2}:\\d{2}\\]");
    const QRegExp regx06("\\[\\d{2}:\\d{2}:\\d{1}\\]");
    const QRegExp regx07("\\[\\d{2}:\\d{2}\\]");
    const QRegExp regx08("\\[\\d{2}\\.\\d{2}\\.\\d{3}\\]");
    const QRegExp regx09("\\[\\d{2}\\.\\d{2}\\.\\d{2}\\]");
    const QRegExp regx10("\\[\\d{2}\\.\\d{2}\\.\\d{1}\\]");
    const QRegExp regx11("\\[\\d{2}\\.\\d{2}:\\d{3}\\]");
    const QRegExp regx12("\\[\\d{2}\\.\\d{2}:\\d{2}\\]");
    const QRegExp regx13("\\[\\d{2}\\.\\d{2}:\\d{1}\\]");
    const QRegExp regx14("\\[\\d{2}\\.\\d{2}\\]");

    LrcFormat type = Type02;
    QRegExp regx;
    if(oneLine.contains(regx01))
    {
        type = Type01;
        regx = regx01;
    }
    else if(oneLine.contains(regx02))
    {
        type = Type02;
        regx = regx02;
    }
    else if(oneLine.contains(regx03))
    {
        type = Type03;
        regx = regx03;
    }
    else if(oneLine.contains(regx04))
    {
        type = Type04;
        regx = regx04;
    }
    else if(oneLine.contains(regx05))
    {
        type = Type05;
        regx = regx05;
    }
    else if(oneLine.contains(regx06))
    {
        type = Type06;
        regx = regx06;
    }
    else if(oneLine.contains(regx07))
    {
        type = Type07;
        regx = regx07;
    }
    else if(oneLine.contains(regx08))
    {
        type = Type08;
        regx = regx08;
    }
    else if(oneLine.contains(regx09))
    {
        type = Type09;
        regx = regx09;
    }
    else if(oneLine.contains(regx10))
    {
        type = Type10;
        regx = regx10;
    }
    else if(oneLine.contains(regx11))
    {
        type = Type11;
        regx = regx11;
    }
    else if(oneLine.contains(regx12))
    {
        type = Type12;
        regx = regx12;
    }
    else if(oneLine.contains(regx13))
    {
        type = Type13;
        regx = regx13;
    }
    else
    {
        type = Type14;
        regx = regx14;
    }

    QString temp = oneLine;
    temp.replace(regx, QString());
    int pos = regx.indexIn(oneLine, 0);
    while(pos != -1)
    {   //That match
        const QString &cap = regx.cap(0);
        //Return zeroth expression matching the content
        //The time tag into the time value, in milliseconds
        switch(type)
        {
            case Type01:
                    matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\.)", "\\d{3}(?=\\])");
                    break;
            case Type02:
                    matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\.)", "\\d{2}(?=\\])");
                    break;
            case Type03:
                    matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\.)", "\\d{1}(?=\\])");
                    break;
            case Type04:
                    matchLrcLine(temp, cap, ":");
                    break;
            case Type05:
                    matchLrcLine(temp, cap, ":");
                    break;
            case Type06:
                    matchLrcLine(temp, cap, ":");
                    break;
            case Type07:
                    matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\])");
                    break;
            case Type08:
                    matchLrcLine(temp, cap, ".");
                    break;
            case Type09:
                    matchLrcLine(temp, cap, ".");
                    break;
            case Type10:
                    matchLrcLine(temp, cap, ".");
                    break;
            case Type11:
                    matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=:)", "\\d{3}(?=\\])");
                    break;
            case Type12:
                    matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=:)", "\\d{2}(?=\\])");
                    break;
            case Type13:
                    matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=:)", "\\d{1}(?=\\])");
                    break;
            case Type14:
                    matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=\\])");
                    break;
            default: break;
        }
        pos += regx.matchedLength();
        pos = regx.indexIn(oneLine, pos); //Matching all
    }
}

void MusicLrcAnalysis::matchLrcLine(const QString &oneLine, const QString &cap, const QString &first, const QString &second, const QString &third)
{
    QRegExp regx;
    regx.setPattern(first);
    regx.indexIn(cap);

    const int minutes = regx.cap(0).toInt();
    regx.setPattern(second);
    regx.indexIn(cap);

    const int seconds = regx.cap(0).toInt();
    regx.setPattern(third);
    regx.indexIn(cap);

    const int milliseconds = regx.cap(0).toInt();
    const int length = QString::number(milliseconds).length();
    const qint64 totalTime = minutes * MT_M2MS + seconds * MT_S2MS + milliseconds * pow(10, 3 - length);
    m_lrcContainer.insert(totalTime, oneLine);
}

void MusicLrcAnalysis::matchLrcLine(const QString &oneLine, const QString &cap, const QString &first, const QString &second)
{
    QRegExp regx;
    regx.setPattern(first);
    regx.indexIn(cap);

    const int minutes = regx.cap(0).toInt();
    regx.setPattern(second);
    regx.indexIn(cap);

    const int seconds = regx.cap(0).toInt();
    const qint64 totalTime = minutes * MT_M2MS + seconds * MT_S2MS;
    m_lrcContainer.insert(totalTime, oneLine);
}

void MusicLrcAnalysis::matchLrcLine(const QString &oneLine, QString cap, const QString &splite)
{
    cap.chop(1);
    cap.remove(0, 1);
    const QStringList lists(cap.split(splite));
    if(lists.count() != 3)
    {
        return;
    }

    const int minutes = lists[0].toInt();
    const int seconds = lists[1].toInt();
    const int milliseconds = lists[2].toInt();
    const int length = QString::number(milliseconds).length();
    const qint64 totalTime = minutes * MT_M2MS + seconds * MT_S2MS + milliseconds * pow(10, 3 - length);
    m_lrcContainer.insert(totalTime, oneLine);
}

qint64 MusicLrcAnalysis::setSongSpeedChanged(qint64 time)
{
    const QList<qint64> keys(m_lrcContainer.keys());
    qint64 beforeTime = 0;
    int index = -1;

    if(!keys.isEmpty())
    {
        index = 0;
        beforeTime = keys[0];
    }
    for(int i=1; i<keys.count(); ++i)
    {
        index = i;
        const qint64 afterTime = keys[i];
        if(beforeTime <= time && time <= afterTime)
        {
            time = afterTime;
            break;
        }
        beforeTime = afterTime;
    }

    if((m_currentLrcIndex = index - 1) < 0)
    {
        m_currentLrcIndex = 0;
    }
//    for(int i=0; i<m_currentShowLrcContainer.count(); ++i)
//    {
//        if(m_currentShowLrcContainer[i] == m_lrcContainer.value(time))
//        {
//            if((m_currentLrcIndex = i - getMiddle() - 1) < 0 )
//            {
//                m_currentLrcIndex = 0;
//            }
//            break;
//        }
//    }
    return time;
}

void MusicLrcAnalysis::revertLrcTime(qint64 pos)
{
    MIntStringMapIterator it(m_lrcContainer);
    MIntStringMap copy;
    while(it.hasNext())
    {
        it.next();
        copy.insert(it.key() + pos, it.value());
    }
    m_lrcContainer = copy;
}

void MusicLrcAnalysis::saveLrcTimeChanged()
{
    MIntStringMapIterator it(m_lrcContainer);
    QByteArray data;
    data.append(QString("[by: %1]\n[offset:0]\n").arg(APP_NAME));
    while(it.hasNext())
    {
        it.next();
        data.append(MusicTime::toString(it.key(), MusicTime::All_Msec, "[mm:ss.zzz]"));
        data.append(it.value() + "\n");
    }

    QFile file(m_currentLrcFileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    QTextStream outstream(&file);
    outstream.setCodec("utf-8");
    outstream << data << endl;
    file.close();
}

bool MusicLrcAnalysis::isValid() const
{
    return (!isEmpty()) && (m_currentLrcIndex + m_lineMax <= m_currentShowLrcContainer.count());
}

bool MusicLrcAnalysis::isEmpty() const
{
    return m_lrcContainer.isEmpty();
}

int MusicLrcAnalysis::count() const
{
    return m_lrcContainer.count();
}

QString MusicLrcAnalysis::getText(int index) const
{
    index = m_currentLrcIndex + index;
    if(m_currentShowLrcContainer.count() <= index)
    {
        index = m_currentShowLrcContainer.count() - 1;
    }

    if(index <0 || index >= m_currentShowLrcContainer.count())
    {
        return QString();
    }
    return m_currentShowLrcContainer[index];
}

bool MusicLrcAnalysis::findText(qint64 current, qint64 total, QString &pre, QString &last, qint64 &interval) const
{
    if(isEmpty())
    {
        Q_UNUSED(current)
        Q_UNUSED(total)
        pre = last = QString();
        return false;
    }

    //After get the current time in the lyrics of the two time points
    qint64 previous = 0;
    qint64 later = 0;
    //Keys () method returns a list of lrcContainer
    foreach(const qint64 value, m_lrcContainer.keys())
    {
        if(current >= value)
        {
            previous = value;
        }
        else
        {
            later = value;
            break;
        }
    }
    //To the last line, set the later to song total time value
    if(later == 0)
    {
        later = total;
    }
    //The lyrics content corresponds to obtain the current time
    pre = m_lrcContainer.value(previous);
    last = m_lrcContainer.value(later);
    interval = later - previous;

    return true;
}

qint64 MusicLrcAnalysis::findTime(int index) const
{
    if(index + m_lineMax < m_currentShowLrcContainer.count())
    {
        MIntStringMapIterator it(m_lrcContainer);
        for(int i=0; i<index + 1; ++i)
        {
            if(it.hasNext())
            {
                it.next();
            }
        }
        return it.key();
    }
    else
    {
        return -1;
    }
}

qint64 MusicLrcAnalysis::findTime(const QStringList &ts) const
{
    if(ts.isEmpty())
    {
        return -1;
    }

    const QStringList copy(m_currentShowLrcContainer);
    for(int i=0; i<copy.count() - ts.count(); ++i)
    {
        if(copy.mid(i, ts.count()) == ts)
        {
            return findTime(i + getMiddle());
        }
    }
    return -1;
}

QStringList MusicLrcAnalysis::getAllLrcList() const
{
    return m_lrcContainer.values();
}

QString MusicLrcAnalysis::getAllLrcString() const
{
    QString clipString;
    foreach(const QString &s, m_lrcContainer.values())
    {
        clipString.append(s + "\n");
    }
    return clipString;
}

void MusicLrcAnalysis::getTranslatedLrc()
{
    delete m_translationThread;
    m_translationThread = M_DOWNLOAD_QUERY_PTR->getTranslationThread(this);
    if(parent()->metaObject()->indexOfSlot("getTranslatedLrcFinished(QString)") != -1)
    {
        connect(m_translationThread, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(getTranslatedLrcFinished(QString)));
    }

    QString data;
    foreach(const qint64 &key, m_lrcContainer.keys())
    {
        data.append(QString("[%1.000]").arg(MusicTime::msecTime2LabelJustified(key)) + m_lrcContainer.value(key));
#ifdef Q_OS_UNIX
        data.append("\r");
#endif
    }

    QVariantMap dtMap;
    dtMap["name"] = m_currentLrcFileName;
    m_translationThread->setRawData(dtMap);
    m_translationThread->startToDownload(data);
}
