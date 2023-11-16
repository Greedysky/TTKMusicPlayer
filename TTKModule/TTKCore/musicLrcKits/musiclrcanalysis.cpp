#include "musiclrcanalysis.h"
#include "musiclrcfromkrc.h"
#include "musicstringutils.h"
#include "musicapplication.h"
#include "ttktime.h"

#include <qmath.h>

MusicLrcAnalysis::MusicLrcAnalysis(QObject *parent)
    : QObject(parent),
      m_lineMax(0),
      m_currentLrcIndex(0)
{

}

MusicLrcAnalysis::~MusicLrcAnalysis()
{

}

MusicLrcAnalysis::State MusicLrcAnalysis::setData(const QByteArray &data)
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
        return State::Failed;
    }

    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    if(m_lrcContainer.find(0) == m_lrcContainer.end())
    {
        m_lrcContainer.insert(0, {});
    }

    for(auto it = m_lrcContainer.constBegin(); it != m_lrcContainer.constEnd(); ++it)
    {
        m_currentShowLrcContainer << it.value();
    }

    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    return State::Success;
}

MusicLrcAnalysis::State MusicLrcAnalysis::setData(const TTKIntStringMap &data)
{
    clear();
    if(data.isEmpty())
    {
        return State::Failed;
    }

    m_lrcContainer = data;

    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    if(m_lrcContainer.find(0) == m_lrcContainer.end())
    {
        m_lrcContainer.insert(0, {});
    }

    for(auto it = m_lrcContainer.constBegin(); it != m_lrcContainer.constEnd(); ++it)
    {
        m_currentShowLrcContainer << it.value();
    }

    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    return State::Success;
}

MusicLrcAnalysis::State MusicLrcAnalysis::loadFromLrcFile(const QString &path)
{
    QFile file(m_currentFilePath = path);

    clear();
    if(!file.open(QIODevice::ReadOnly))
    {
        return State::Failed;
    }

    const State state = setData(file.readAll());
    file.close();

    return state;
}

MusicLrcAnalysis::State MusicLrcAnalysis::loadFromKrcFile(const QString &path)
{
    clear();
    m_currentFilePath = path;

    MusicLrcFromKrc krc;
    if(!krc.decode(path))
    {
        return State::Failed;
    }

    const QString &text = QString(krc.decodeString());
    //The lyrics by line into the lyrics list
    for(const QString &oneLine : text.split("\r\n"))
    {
        matchLrcLine(oneLine);
    }

    //If the lrcContainer is empty
    if(m_lrcContainer.isEmpty())
    {
        return State::Failed;
    }

    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    if(m_lrcContainer.find(0) == m_lrcContainer.end())
    {
        m_lrcContainer.insert(0, {});
    }

    for(auto it = m_lrcContainer.constBegin(); it != m_lrcContainer.constEnd(); ++it)
    {
        m_currentShowLrcContainer << it.value();
    }

    for(int i = 0; i < lineMiddle(); ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    return State::Success;
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

    Format type = Format::Type02;
    QRegExp regx;
    if(oneLine.contains(regx01))
    {
        type = Format::Type01;
        regx = regx01;
    }
    else if(oneLine.contains(regx02))
    {
        type = Format::Type02;
        regx = regx02;
    }
    else if(oneLine.contains(regx03))
    {
        type = Format::Type03;
        regx = regx03;
    }
    else if(oneLine.contains(regx04))
    {
        type = Format::Type04;
        regx = regx04;
    }
    else if(oneLine.contains(regx05))
    {
        type = Format::Type05;
        regx = regx05;
    }
    else if(oneLine.contains(regx06))
    {
        type = Format::Type06;
        regx = regx06;
    }
    else if(oneLine.contains(regx07))
    {
        type = Format::Type07;
        regx = regx07;
    }
    else if(oneLine.contains(regx08))
    {
        type = Format::Type08;
        regx = regx08;
    }
    else if(oneLine.contains(regx09))
    {
        type = Format::Type09;
        regx = regx09;
    }
    else if(oneLine.contains(regx10))
    {
        type = Format::Type10;
        regx = regx10;
    }
    else if(oneLine.contains(regx11))
    {
        type = Format::Type11;
        regx = regx11;
    }
    else if(oneLine.contains(regx12))
    {
        type = Format::Type12;
        regx = regx12;
    }
    else if(oneLine.contains(regx13))
    {
        type = Format::Type13;
        regx = regx13;
    }
    else
    {
        type = Format::Type14;
        regx = regx14;
    }

    QString temp = oneLine;
    temp.replace(regx, {});
    int pos = regx.indexIn(oneLine);
    while(pos != -1)
    {
        const QString &cap = regx.cap(0);
        //Return zeroth expression matching the content
        //The time tag into the time value, in milliseconds
        switch(type)
        {
            case Format::Type01: matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\.)", "\\d{3}(?=\\])"); break;
            case Format::Type02: matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\.)", "\\d{2}(?=\\])"); break;
            case Format::Type03: matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\.)", "\\d{1}(?=\\])"); break;
            case Format::Type04: matchLrcLine(temp, cap, ":"); break;
            case Format::Type05: matchLrcLine(temp, cap, ":"); break;
            case Format::Type06: matchLrcLine(temp, cap, ":"); break;
            case Format::Type07: matchLrcLine(temp, cap, "\\d{2}(?=:)", "\\d{2}(?=\\])"); break;
            case Format::Type08: matchLrcLine(temp, cap, "."); break;
            case Format::Type09: matchLrcLine(temp, cap, "."); break;
            case Format::Type10: matchLrcLine(temp, cap, "."); break;
            case Format::Type11: matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=:)", "\\d{3}(?=\\])"); break;
            case Format::Type12: matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=:)", "\\d{2}(?=\\])"); break;
            case Format::Type13: matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=:)", "\\d{1}(?=\\])"); break;
            case Format::Type14: matchLrcLine(temp, cap, "\\d{2}(?=\\.)", "\\d{2}(?=\\])"); break;
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
    const qint64 totalTime = minutes * TTK_DN_M2MS + seconds * TTK_DN_S2MS + milliseconds * pow(10, 3 - length);
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
    const qint64 totalTime = minutes * TTK_DN_M2MS + seconds * TTK_DN_S2MS;
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
    const qint64 totalTime = minutes * TTK_DN_M2MS + seconds * TTK_DN_S2MS + milliseconds * pow(10, 3 - length);
    m_lrcContainer.insert(totalTime, oneLine);
}

qint64 MusicLrcAnalysis::setSongTimeSpeed(qint64 time)
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

void MusicLrcAnalysis::revertTime(qint64 pos)
{
    TTKIntStringMap copy;
    for(auto it = m_lrcContainer.constBegin(); it != m_lrcContainer.constEnd(); ++it)
    {
        copy.insert(it.key() + pos, it.value());
    }
    m_lrcContainer = copy;
}

void MusicLrcAnalysis::saveData()
{
    QString data;
    data.append(QString("[by: %1]\n[offset:0]\n").arg(TTK_APP_NAME));

    for(auto it = m_lrcContainer.constBegin(); it != m_lrcContainer.constEnd(); ++it)
    {
        data.append(TTKTime::toString(it.key(), "[mm:ss.zzz]"));
        data.append(it.value() + "\n");
    }

    QFile file(m_currentFilePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    QTextStream outstream(&file);
    outstream.setCodec("UTF-8");
    outstream << data;
    QtStreamEndLine(outstream);
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
        return {};
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
        auto it = m_lrcContainer.constBegin();
        for(int i = 0; i < index + 1; ++i)
        {
            if(it != m_lrcContainer.constEnd())
            {
                ++it;
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

QString MusicLrcAnalysis::dataString() const
{
    QString v;
    for(const QString &s : m_lrcContainer.values())
    {
        v.append(s + "\n");
    }
    return v;
}

QStringList MusicLrcAnalysis::dataList() const
{
    return m_lrcContainer.values();
}
