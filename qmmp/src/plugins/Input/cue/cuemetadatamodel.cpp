#include <qmmp/metadatamanager.h>
#include "cuefile.h"
#include "cuemetadatamodel.h"

CUEMetaDataModel::CUEMetaDataModel(const QString &url)
    : MetaDataModel(true)
{
    m_cueFile = new CueFile(url);
    if(m_cueFile->count() == 0)
    {
        qWarning("CUEMetaDataModel: invalid cue file");
        return;
    }
    int track = url.section("#", -1).toInt();
    m_path = m_cueFile->dataFilePath(track);
}

CUEMetaDataModel::~CUEMetaDataModel()
{
    delete m_cueFile;
}

QList<MetaDataItem> CUEMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    MetaDataModel *model = MetaDataManager::instance()->createMetaDataModel(m_path, true);
    if(model)
    {
        ep = model->extraProperties();
        delete model;
    }
    return ep;
}

QString CUEMetaDataModel::coverPath() const
{
    return MetaDataManager::instance()->findCoverFile(m_path);
}
