#include <qmmp/metadatamanager.h>
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
    QFile file(m_cueFilePath);
    file.open(QIODevice::ReadOnly);
    return QString::fromUtf8(file.readAll().constData());
}

void CUEMetaDataModel::setCue(const QString &content)
{
    QFile file(m_cueFilePath);
    file.open(QIODevice::WriteOnly);
    file.write(content.toUtf8());
}

void CUEMetaDataModel::removeCue()
{
    QFile::remove(m_cueFilePath);
}
