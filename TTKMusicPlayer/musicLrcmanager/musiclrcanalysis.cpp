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
    QStringList lines = getAllText.split("\n");

    QRegExp reg1("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
    QRegExp reg2("\\[\\d{2}:\\d{2}\\.\\d{3}\\]");
    QRegExp reg3("\\[\\d{2}:\\d{2}\\]");
    foreach(QString oneLine, lines)
    {
        int type = -1;
        QRegExp regex;
        if(oneLine.contains(reg1))
        {
            type = 0;
            regex = reg1;
        }
        else if(oneLine.contains(reg2))
        {
            type = 1;
            regex = reg2;
        }
        else
        {
            type = 2;
            regex = reg3;
        }

        QString temp = oneLine;
        temp.replace(regex, QString());
        int pos = regex.indexIn(oneLine, 0);

        while(pos != -1)
        {   //That match
            QString cap = regex.cap(0);
            //Return zeroth expression matching the content
            //The time tag into the time value, in milliseconds
            QRegExp regexp;
            regexp.setPattern("\\d{2}(?=:)");
            regexp.indexIn(cap);
            int minute = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\.)");
            regexp.indexIn(cap);
            int second = regexp.cap(0).toInt();
            int millisecond = 0;
            if(type == 0)
            {
                regexp.setPattern("\\d{2}(?=\\])");
            }
            else if(type == 1)
            {
                regexp.setPattern("\\d{3}(?=\\])");
            }
            regexp.indexIn(cap);
            millisecond = regexp.cap(0).toInt();
            if(type == 2)
            {
                millisecond = 0;
            }
            qint64 totalTime = minute * 60000 + second * 1000 + millisecond * 10;
            //Insert into lrcContainer
            m_lrcContainer.insert(totalTime, temp);
            pos += regex.matchedLength();
            pos = regex.indexIn(oneLine, pos);//Matching all
        }
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

    return Null;
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

QString MusicLrcAnalysis::getText(int index) const
{
    return m_currentShowLrcContainer[m_currentLrcIndex + index];
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
