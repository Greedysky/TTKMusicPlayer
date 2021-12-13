#include <QSettings>
#include <QTextCodec>
#include <qmmp/metadatamanager.h>
#ifdef WITH_ENCA
#  include <enca.h>
#endif
#include "cuefile.h"
#include "cuemetadatamodel.h"

CUEMetaDataModel::CUEMetaDataModel(bool readOnly, const QString &url)
    : MetaDataModel(readOnly, IsCueEditable)
{
    CueFile file(url);
    if(file.isEmpty())
    {
        qWarning("CUEMetaDataModel: invalid cue file");
        return;
    }

    const int track = url.section("#", -1).toInt();
    m_dataFilePath = file.dataFilePath(track);
    m_cueFilePath = file.cueFilePath();
}

CUEMetaDataModel::~CUEMetaDataModel()
{

}

QList<MetaDataItem> CUEMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    MetaDataModel *model = MetaDataManager::instance()->createMetaDataModel(m_dataFilePath, true);
    if(model)
    {
        ep = model->extraProperties();
        delete model;
    }
    return ep;
}

QString CUEMetaDataModel::coverPath() const
{
    return MetaDataManager::instance()->findCoverFile(m_dataFilePath);
}

QString CUEMetaDataModel::cue() const
{
    if(m_codec)
    {
        m_codec = nullptr;
    }

    QFile file(m_cueFilePath);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("CUE");
#ifdef WITH_ENCA
    EncaAnalyser analyser = nullptr;
    if(settings.value("use_enca", false).toBool())
    {
        analyser = enca_analyser_alloc(settings.value("enca_lang").toByteArray ().constData());

        if(analyser)
        {
            enca_set_threshold(analyser, 1.38);
            EncaEncoding encoding = enca_analyse(analyser, (uchar *)data.constData(), data.size());
            if(encoding.charset != ENCA_CS_UNKNOWN)
            {
                m_codec = QTextCodec::codecForName(enca_charset_name(encoding.charset,ENCA_NAME_STYLE_ENCA));
            }
            enca_analyser_free(analyser);
        }
    }
#endif
    if(!m_codec)
        m_codec = QTextCodec::codecForName(settings.value("encoding", "UTF-8").toByteArray());
    settings.endGroup();

    return m_codec->toUnicode(data);
}

void CUEMetaDataModel::setCue(const QString &content)
{
    if(!m_codec)
    {
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        m_codec = QTextCodec::codecForName(settings.value("CUE/encoding", "UTF-8").toByteArray());
    }

    QFile file(m_cueFilePath);
    file.open(QIODevice::WriteOnly);
    file.write(m_codec->fromUnicode(content));
}

void CUEMetaDataModel::removeCue()
{
    QFile::remove(m_cueFilePath);
}