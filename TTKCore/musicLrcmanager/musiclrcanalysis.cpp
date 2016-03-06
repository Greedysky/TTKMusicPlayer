#include "musiclrcanalysis.h"

MusicLrcAnalysis::MusicLrcAnalysis(QObject *parent)
    : QObject(parent)
{
    m_currentLrcIndex = 0;
}

MusicLrcAnalysis::~MusicLrcAnalysis()
{

}

MusicLrcAnalysis::State MusicLrcAnalysis::transLrcFileToTime(const QString &lrcFileName)
{
    m_lrcContainer.clear();///Clear the original map
    m_currentShowLrcContainer.clear();///Clear the original lrc
    QFile file(m_currentLrcFileName = lrcFileName); ///Open the lyrics file

    if(!file.open(QIODevice::ReadOnly))
    {
        return OpenFileFail;
    }

    QString getAllText = QString(file.readAll());
    file.close();
    //The lyrics by line into the lyrics list
    foreach(QString oneLine, getAllText.split("\n"))
    {
        matchLrcLine(oneLine);
    }

    //If the lrcContainer is empty
    if (m_lrcContainer.isEmpty())
    {
        return LrcEmpty;
    }

    m_currentShowLrcContainer.clear();
    m_currentLrcIndex = 0;

    for(int i=0; i<MIN_LRCCONTAIN_COUNT/2; ++i)
    {
        m_currentShowLrcContainer << QString();
    }
    if(m_lrcContainer.find(0) == m_lrcContainer.end())
    {
       m_lrcContainer.insert(0, QString());
    }

    MIntStringMapIt it(m_lrcContainer);
    while(it.hasNext())
    {
        it.next();
        m_currentShowLrcContainer << it.value();
    }
    for(int i=0; i<MIN_LRCCONTAIN_COUNT/2; ++i)
    {
        m_currentShowLrcContainer << QString();
    }

    return OpenFileSuccess;
}

void MusicLrcAnalysis::matchLrcLine(const QString &oneLine)
{
    QRegExp reg01("\\[\\d{2}:\\d{2}\\.\\d{3}\\]");
    QRegExp reg02("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
    QRegExp reg03("\\[\\d{2}:\\d{2}\\.\\d{1}\\]");
    QRegExp reg04("\\[\\d{2}:\\d{2}:\\d{3}\\]");
    QRegExp reg05("\\[\\d{2}:\\d{2}:\\d{2}\\]");
    QRegExp reg06("\\[\\d{2}:\\d{2}:\\d{1}\\]");
    QRegExp reg07("\\[\\d{2}:\\d{2}\\]");
    QRegExp reg08("\\[\\d{2}\\.\\d{2}\\.\\d{3}\\]");
    QRegExp reg09("\\[\\d{2}\\.\\d{2}\\.\\d{2}\\]");
    QRegExp reg10("\\[\\d{2}\\.\\d{2}\\.\\d{1}\\]");
    QRegExp reg11("\\[\\d{2}\\.\\d{2}:\\d{3}\\]");
    QRegExp reg12("\\[\\d{2}\\.\\d{2}:\\d{2}\\]");
    QRegExp reg13("\\[\\d{2}\\.\\d{2}:\\d{1}\\]");
    QRegExp reg14("\\[\\d{2}\\.\\d{2}\\]");

    LrcFormat type = Type02;
    QRegExp regex;
    if(oneLine.contains(reg01))
    {
        type = Type01;
        regex = reg01;
    }
    else if(oneLine.contains(reg02))
    {
        type = Type02;
        regex = reg02;
    }
    else if(oneLine.contains(reg03))
    {
        type = Type03;
        regex = reg03;
    }
    else if(oneLine.contains(reg04))
    {
        type = Type04;
        regex = reg04;
    }
    else if(oneLine.contains(reg05))
    {
        type = Type05;
        regex = reg05;
    }
    else if(oneLine.contains(reg06))
    {
        type = Type06;
        regex = reg06;
    }
    else if(oneLine.contains(reg07))
    {
        type = Type07;
        regex = reg07;
    }
    else if(oneLine.contains(reg08))
    {
        type = Type08;
        regex = reg08;
    }
    else if(oneLine.contains(reg09))
    {
        type = Type09;
        regex = reg09;
    }
    else if(oneLine.contains(reg10))
    {
        type = Type10;
        regex = reg10;
    }
    else if(oneLine.contains(reg11))
    {
        type = Type11;
        regex = reg11;
    }
    else if(oneLine.contains(reg12))
    {
        type = Type12;
        regex = reg12;
    }
    else if(oneLine.contains(reg13))
    {
        type = Type13;
        regex = reg13;
    }
    else
    {
        type = Type14;
        regex = reg14;
    }
    /////////////////////////////////////////
    QString temp = oneLine;
    temp.replace(regex, QString());
    int pos = regex.indexIn(oneLine, 0);
    while(pos != -1)
    {   //That match
        QString cap = regex.cap(0);
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
        }
        pos += regex.matchedLength();
        pos = regex.indexIn(oneLine, pos); //Matching all
    }
}

void MusicLrcAnalysis::matchLrcLine(const QString &oneLine, const QString &cap,
                                    const QString &first, const QString &second, const QString &third)
{
    QRegExp regexp;
    regexp.setPattern(first);
    regexp.indexIn(cap);
    int minutes = regexp.cap(0).toInt();
    regexp.setPattern(second);
    regexp.indexIn(cap);
    int seconds = regexp.cap(0).toInt();
    regexp.setPattern(third);
    regexp.indexIn(cap);
    int milliseconds = regexp.cap(0).toInt();
    int length = QString::number(milliseconds).length();
    qint64 totalTime = minutes * 60000 + seconds * 1000 + milliseconds * pow(10, 3 - length);
    m_lrcContainer.insert(totalTime, oneLine);
}

void MusicLrcAnalysis::matchLrcLine(const QString &oneLine, const QString &cap, const QString &first,
                                    const QString &second)
{
    QRegExp regexp;
    regexp.setPattern(first);
    regexp.indexIn(cap);
    int minutes = regexp.cap(0).toInt();
    regexp.setPattern(second);
    regexp.indexIn(cap);
    int seconds = regexp.cap(0).toInt();
    qint64 totalTime = minutes * 60000 + seconds * 1000;
    m_lrcContainer.insert(totalTime, oneLine);
}

void MusicLrcAnalysis::matchLrcLine(const QString &oneLine, QString cap,
                                    const QString &splite)
{
    cap.chop(1);
    cap.remove(0, 1);
    QStringList lists(cap.split(splite));
    if(lists.count() != 3)
    {
        return;
    }
    int minutes = lists[0].toInt();
    int seconds = lists[1].toInt();
    int milliseconds = lists[2].toInt();
    int length = QString::number(milliseconds).length();
    qint64 totalTime = minutes * 60000 + seconds * 1000 + milliseconds * pow(10, 3 - length);
    m_lrcContainer.insert(totalTime, oneLine);
}

qint64 MusicLrcAnalysis::setSongSpeedAndSlow(qint64 time)
{
    QList<qint64> keys(m_lrcContainer.keys());
    qint64 beforeTime;
    if(!keys.isEmpty())
    {
        beforeTime = keys[0];
    }
    for(int i=1; i<keys.count(); ++i)
    {
        qint64 afterTime = keys[i];
        if(beforeTime <= time && time <= afterTime)
        {
            time = afterTime;
            break;
        }
        beforeTime = afterTime;
    }

    for(int i=0; i<m_currentShowLrcContainer.count(); ++i)
    {
        if(m_currentShowLrcContainer[i] == m_lrcContainer.value(time))
        {
            if((m_currentLrcIndex = i - CURRENT_LRC_PAINT - 1) < 0 )
            {
                m_currentLrcIndex = 0;
            }
            break;
        }
    }
    return time;
}

void MusicLrcAnalysis::revertLrcTime(qint64 pos)
{
    MIntStringMapIt it(m_lrcContainer);
    MIntStringMap copy;
    while(it.hasNext())
    {
        it.next();
        copy.insert(it.key() + pos, it.value());
    }
    m_lrcContainer = copy;
}

bool MusicLrcAnalysis::valid() const
{
    return (!isEmpty()) &&
           (m_currentLrcIndex + MIN_LRCCONTAIN_COUNT <= m_currentShowLrcContainer.count());
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
    return m_currentShowLrcContainer[index];
}

bool MusicLrcAnalysis::findText(qint64 current, qint64 total,
                                QString &pre, QString &last, qint64 &interval) const
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
    foreach (qint64 value, m_lrcContainer.keys())
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
    if(index + MIN_LRCCONTAIN_COUNT < m_currentShowLrcContainer.count())
    {
        MIntStringMapIt it(m_lrcContainer);
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

QString MusicLrcAnalysis::getAllLrcs() const
{
    QString clipString;
    foreach(QString s, m_lrcContainer.values())
    {
        clipString.append(s);
    }
    return clipString;
}
