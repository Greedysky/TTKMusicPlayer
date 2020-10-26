#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QSettings>
#include <QTextStream>
#include <QTextCodec>
#include <qmmp/decoder.h>
#include <qmmp/metadatamanager.h>
#ifdef WITH_ENCA
#include <enca.h>
#endif
#include "cuefile.h"

CueFile::CueFile(const QString &path)
    : CueParser()
{
    QString filePath = path;
    if(path.contains("://"))
    {
        filePath.remove("cue://");
        filePath.remove(RegularWrapper("#\\d+$"));
    }

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug("CueFile: error: %s", qPrintable(file.errorString()));
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("CUE");
    m_dirty = settings.value("dirty_cue", false).toBool();
    QTextCodec *codec = nullptr;
#ifdef WITH_ENCA
    EncaAnalyser analyser = nullptr;
    if(settings.value("use_enca", false).toBool())
    {
        analyser = enca_analyser_alloc(settings.value("enca_lang").toByteArray().constData());

        if(analyser)
        {
            enca_set_threshold(analyser, 1.38);
            EncaEncoding encoding = enca_analyse(analyser, (uchar *)data.constData(), data.size());
            file.reset();
            if(encoding.charset != ENCA_CS_UNKNOWN)
            {
                codec = QTextCodec::codecForName(enca_charset_name(encoding.charset,ENCA_NAME_STYLE_ENCA));
                //qDebug("CUEParser: detected charset: %s",
                  //     enca_charset_name(encoding.charset,ENCA_NAME_STYLE_ENCA));
            }
            enca_analyser_free(analyser);
        }
    }
#endif
    if(!codec)
        codec = QTextCodec::codecForName(settings.value("encoding","UTF-8").toByteArray());
    if(!codec)
        codec = QTextCodec::codecForName("UTF-8");
    settings.endGroup();
    //qDebug("CUEParser: using %s encoding", codec->name().constData());
    loadData(data, codec);
    setUrl("cue", filePath);
    for(const QString &dataFileName : files())
    {
        QString dataFilePath = getDirtyPath(filePath, QFileInfo(filePath).dir().filePath(dataFileName));
        m_dataFiles.insert(dataFileName, dataFilePath);
        QList<TrackInfo*> pl = MetaDataManager::instance()->createPlayList(dataFilePath, TrackInfo::Properties);
        if(!pl.isEmpty())
        {
            setProperties(dataFileName, pl.first()->properties());
            setDuration(dataFileName, pl.first()->duration());
            qDeleteAll(pl);
            pl.clear();
        }
    }

    for(const QString &p : m_dataFiles.values())
    {
        if(!QFile::exists(p))
        {
            qDebug("CueFile: unable to find file: %s", qPrintable(p));
            clear();
            return;
        }
    }
}

CueFile::~CueFile()
{

}

QString CueFile::dataFilePath(int track) const
{
    return m_dataFiles.value(file(track));
}

QStringList CueFile::dataFilePaths() const
{
    return m_dataFiles.values();
}

QStringList CueFile::splitLine(const QString &line)
{
    //qDebug("raw string = %s",qPrintable(line));
    QStringList list;
    QString buf = line.trimmed();
    if(buf.isEmpty())
        return list;
    while(!buf.isEmpty())
    {
        //qDebug(qPrintable(buf));
        if(buf.startsWith('"'))
        {
            int end = buf.indexOf('"',1);
            if(end == -1) //ignore invalid line
            {
                list.clear();
                qWarning("CUEParser: unable to parse line: %s",qPrintable(line));
                return list;
            }
            list << buf.mid(1, end - 1);
            buf.remove(0, end+1);
        }
        else
        {
            int end = buf.indexOf(' ', 0);
            if(end < 0)
                end = buf.size();
            list << buf.mid(0, end);
            buf.remove(0, end);
        }
        buf = buf.trimmed();
    }
    return list;
}

QString CueFile::getDirtyPath(const QString &cue_path, const QString &path)
{    
    if((QFile::exists(path) && Decoder::findByFilePath(path)))
        return path;

    QStringList candidates;
    QDirIterator it(QFileInfo(path).dir().path(), QDir::Files);
    while(it.hasNext())
    {
        it.next();

        QString f = it.filePath();

        if(it.fileName().toLower() == QFileInfo(path).fileName().toLower() && Decoder::findByFilePath(f))
            return it.filePath();

        if(m_dirty && (f != cue_path) && Decoder::findByFilePath(f))
            candidates.push_back(f);
    }

    if(candidates.isEmpty())
        return path;
    else if(candidates.count() == 1)
        return candidates.first();

    int dot = cue_path.lastIndexOf('.');
    if(dot != -1)
    {
        QRegExp r(QRegExp::escape(cue_path.left(dot)) + "\\.[^\\.]+$");

        int index = candidates.indexOf(r);
        int rindex = candidates.lastIndexOf(r);

        if((index != -1) && (index == rindex))
            return candidates[index];
    }
    dot = path.lastIndexOf('.');
    if(dot != -1)
    {
        QRegExp r(QRegExp::escape(path.left(dot)) + "\\.[^\\.]+$");

        int index = candidates.indexOf(r);
        int rindex = candidates.lastIndexOf(r);

        if((index != -1) && (index == rindex))
            return candidates[index];
    }

    return path;
}
