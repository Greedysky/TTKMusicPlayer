#include "musiclrcanalysis.h"
#include "musiclrcfromkrc.h"
#include "musicstringutils.h"
#include "musicapplication.h"
#include "musictime.h"

#include <qmath.h>

MusicLrcAnalysis::MusicLrcAnalysis(QObject *parent)
    : QObject(parent)
{
    m_lineMax = 0;
    m_currentLrcIndex = 0;
}

MusicLrcAnalysis::~MusicLrcAnalysis()
{

}

MusicLrcAnalysis::State MusicLrcAnalysis::setLrcData(const QByteArray &data)
{
    clear();

    QStringList text = QString(data).split("\n");
    if(data.left(9) == MUSIC_TTKLRCF) //plain txt check
    {
        text[0].clear();
        const int perTime = MusicApplication::instance()->duration() / text.count();
        for(const QString &oneLine : qAsConst(text))
        {
            m_lrcContainer.insert(perTime * m_lrcContainer.count(), oneLine);
        }
    }
    else
    {
        for(const QString &oneLine : qAsConst(text))
        {
            matchLrcLine(oneLine);
        }
    }

    if(m_lrcContainer.isEmpty())
    {
        return Failed;
    }

    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    if(m_lrcContainer.find(0) == m_lrcContainer.end())
    {
        m_lrcContainer.insert(0, QString());
    }

    TTKIntStringMapIter it(m_lrcContainer);
    while(it.hasNext())
    {
        it.next();
        m_currentShowLrcContainer << it.value();
    }

    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    return Success;
}

MusicLrcAnalysis::State MusicLrcAnalysis::setLrcData(const TTKIntStringMap &data)
{
    clear();
    if(data.isEmpty())
    {
        return Failed;
    }

    m_lrcContainer = data;

    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    if(m_lrcContainer.find(0) == m_lrcContainer.end())
    {
        m_lrcContainer.insert(0, QString());
    }

    TTKIntStringMapIter it(m_lrcContainer);
    while(it.hasNext())
    {
        it.next();
        m_currentShowLrcContainer << it.value();
    }

    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    return Success;
}

MusicLrcAnalysis::State MusicLrcAnalysis::readFromLrcFile(const QString &path)
{
    QFile file(m_currentFilePath = path);

    clear();
    if(!file.open(QIODevice::ReadOnly))
    {
        return Failed;
    }

    const State state = setLrcData(file.readAll());
    file.close();

    return state;
}

MusicLrcAnalysis::State MusicLrcAnalysis::readFromKrcFile(const QString &path)
{
    clear();
    m_currentFilePath = path;

    MusicLrcFromKrc krc;
    if(!krc.decode(path))
    {
        return Failed;
    }

    const QString &text = QString(krc.decodeString());
    //The lyrics by line into the lyrics list
    for(const QString &oneLine : text.split(MusicUtils::String::lineWrap()))
    {
        matchLrcLine(oneLine);
    }

    //If the lrcContainer is empty
    if(m_lrcContainer.isEmpty())
    {
        return Failed;
    }

    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    if(m_lrcContainer.find(0) == m_lrcContainer.end())
    {
       m_lrcContainer.insert(0, QString());
    }

    TTKIntStringMapIter it(m_lrcContainer);
    while(it.hasNext())
    {
        it.next();
        m_currentShowLrcContainer << it.value();
    }
    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    return Success;
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
    {
        const QString &cap = regx.cap(0);
        //Return zeroth expression matching the content
        //The time tag into the time value, in milliseconds
        switch(type)
        {
            case Type01: matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\.)", "\\d{3}(?=\\])"); break;
            case Type02: matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\.)", "\\d{2}(?=\\])"); break;
            case Type03: matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\.)", "\\d{1}(?=\\])"); break;
            case Type04: matchLrcLine(temp, cap, ":"); break;
            case Type05: matchLrcLine(temp, cap, ":"); break;
            case Type06: matchLrcLine(temp, cap, ":"); break;
            case Type07: matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\])"); break;
            case Type08: matchLrcLine(temp, cap, "."); break;
            case Type09: matchLrcLine(temp, cap, "."); break;
            case Type10: matchLrcLine(temp, cap, "."); break;
            case Type11: matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=:)", "\\d{3}(?=\\])"); break;
            case Type12: matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=:)", "\\d{2}(?=\\])"); break;
            case Type13: matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=:)", "\\d{1}(?=\\])"); break;
            case Type14: matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=\\])"); break;
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
    const QStringList list(cap.split(splite));
    if(list.count() != 3)
    {
        return;
    }

    const int minutes = list[0].toInt();
    const int seconds = list[1].toInt();
    const int milliseconds = list[2].toInt();
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

    for(int i = 1; i < keys.count(); ++i)
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
    return time;
}

void MusicLrcAnalysis::revertLrcTime(qint64 pos)
{
    TTKIntStringMapIter it(m_lrcContainer);
    TTKIntStringMap copy;
    while(it.hasNext())
    {
        it.next();
        copy.insert(it.key() + pos, it.value());
    }
    m_lrcContainer = copy;
}

void MusicLrcAnalysis::saveLrcData()
{
    TTKIntStringMapIter it(m_lrcContainer);
    QString data;
    data.append(QString("[by: %1]\n[offset:0]\n").arg(APP_NAME));
    while(it.hasNext())
    {
        it.next();
        data.append(MusicTime::toString(it.key(), MusicTime::AllMsec, "[mm:ss.zzz]"));
        data.append(it.value() + "\n");
    }

    QFile file(m_currentFilePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    QTextStream outstream(&file);
    outstream.setCodec("utf-8");
    outstream << data;
#if TTK_QT_VERSION_CHECK(5,15,0)
    outstream << Qt::endl;
#else
    outstream << endl;
#endif
    file.close();
}

void MusicLrcAnalysis::clear()
{
    m_currentLrcIndex = 0;
    m_lrcContainer.clear();
    m_currentShowLrcContainer.clear();
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

QString MusicLrcAnalysis::text(int index) const
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
    for(const qint64 value : m_lrcContainer.keys())
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
        TTKIntStringMapIter it(m_lrcContainer);
        for(int i = 0; i < index + 1; ++i)
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
    for(int i = 0; i < copy.count() - ts.count(); ++i)
    {
        if(copy.mid(i, ts.count()) == ts)
        {
            return findTime(i + lineMiddle());
        }
    }
    return -1;
}

QStringList MusicLrcAnalysis::generateLrcList() const
{
    return m_lrcContainer.values();
}

QString MusicLrcAnalysis::generateLrcString() const
{
    QString clipString;
    for(const QString &s : m_lrcContainer.values())
    {
        clipString.append(s + "\n");
    }
    return clipString;
}
