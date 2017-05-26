#include "musiclrcanalysis.h"
#ifndef MUSIC_MOBILE
#include "musiclrcfromkrc.h"
#endif
#include "musictime.h"
#include "musictranslationthread.h"

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

QString MusicLrcAnalysis::getClassName()
{
    return staticMetaObject.className();
}

MusicLrcAnalysis::State MusicLrcAnalysis::setLrcData(const QByteArray &data)
{
    m_currentLrcIndex = 0;
    m_lrcContainer.clear();
    m_currentShowLrcContainer.clear();

    QString getAllText = QString(data);
    foreach(const QString &oneLine, getAllText.split("\n"))
    {
        matchLrcLine(oneLine);
    }

    if (m_lrcContainer.isEmpty())
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

    MusicObject::MIntStringMapIterator it(m_lrcContainer);
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

MusicLrcAnalysis::State MusicLrcAnalysis::setLrcData(const MusicObject::MIntStringMap &data)
{
    if (data.isEmpty())
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

    MusicObject::MIntStringMapIterator it(m_lrcContainer);
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

    State state = setLrcData(file.readAll());
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

    QString getAllText = QString(krc.getDecodeString());
    //The lyrics by line into the lyrics list
    foreach(const QString &oneLine, getAllText.split("\r\n"))
    {
        matchLrcLine(oneLine);
    }

    //If the lrcContainer is empty
    if (m_lrcContainer.isEmpty())
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

    MusicObject::MIntStringMapIterator it(m_lrcContainer);
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
    qint64 totalTime = minutes * MT_M2MS + seconds * MT_S2MS + milliseconds * pow(10, 3 - length);
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
    qint64 totalTime = minutes * MT_M2MS + seconds * MT_S2MS;
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
    qint64 totalTime = minutes * MT_M2MS + seconds * MT_S2MS + milliseconds * pow(10, 3 - length);
    m_lrcContainer.insert(totalTime, oneLine);
}

qint64 MusicLrcAnalysis::setSongSpeedAndSlow(qint64 time)
{
    QList<qint64> keys(m_lrcContainer.keys());
    qint64 beforeTime = 0;
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
            if((m_currentLrcIndex = i - getMiddle() - 1) < 0 )
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
    MusicObject::MIntStringMapIterator it(m_lrcContainer);
    MusicObject::MIntStringMap copy;
    while(it.hasNext())
    {
        it.next();
        copy.insert(it.key() + pos, it.value());
    }
    m_lrcContainer = copy;
}

void MusicLrcAnalysis::saveLrcTimeChanged()
{
    MusicObject::MIntStringMapIterator it(m_lrcContainer);
    QByteArray data;
    data.append(QString("[by: TTKMusicPlayer]\n[offset:0]\n"));
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

bool MusicLrcAnalysis::valid() const
{
    return (!isEmpty()) &&
           (m_currentLrcIndex + m_lineMax <= m_currentShowLrcContainer.count());
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
    foreach(const qint64 &value, m_lrcContainer.keys())
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
        MusicObject::MIntStringMapIterator it(m_lrcContainer);
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
    foreach(const QString &s, m_lrcContainer.values())
    {
        clipString.append(s + "\n");
    }
    return clipString;
}

void MusicLrcAnalysis::getTranslatedLrc()
{
    if(m_translationThread == nullptr)
    {
        m_translationThread = new MusicTranslationThread(this);
        if(parent()->metaObject()->indexOfSlot("getTranslatedLrcFinished(QString)") != -1)
        {
            connect(m_translationThread, SIGNAL(downLoadDataChanged(QString)), parent(),
                                         SLOT(getTranslatedLrcFinished(QString)));
        }
    }

    QString data;
    foreach(const QString &s, m_lrcContainer.values())
    {
        data.append(s);
#ifdef Q_OS_UNIX
        data.append("\r");
#endif
    }

    m_translationThread->startToTranslation(MusicTranslationThread::Type_Auto,
                                            MusicTranslationThread::Type_Zh, data);
}
