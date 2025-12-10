#include "xmpmetadatamodel.h"

XMPMetaDataModel::XMPMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    m_ctx = xmp_create_context();
    const int err = xmp_load_module(m_ctx, QmmpPrintable(path));
    if(err != 0)
    {
        xmp_free_context(m_ctx);
        m_ctx = nullptr;
        qWarning("XMPMetaDataModel: unable to load module file, error = %d", err);
    }
}

XMPMetaDataModel::~XMPMetaDataModel()
{
    if(m_ctx)
    {
        xmp_release_module(m_ctx);
        xmp_free_context(m_ctx);
    }
}

QList<MetaDataItem> XMPMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    if(!m_ctx)
    {
        return ep;
    }

    xmp_module_info mi;
    xmp_get_module_info(m_ctx, &mi);

    ep << MetaDataItem(tr("Volume scale"), mi.vol_base);
    ep << MetaDataItem(tr("Number of patterns"), mi.mod->pat);
    ep << MetaDataItem(tr("Number of tracks"), mi.mod->trk);
    ep << MetaDataItem(tr("Tracks per pattern"), mi.mod->chn);
    ep << MetaDataItem(tr("Number of instruments"), mi.mod->ins);
    ep << MetaDataItem(tr("Number of samples"),mi.mod->smp);
    ep << MetaDataItem(tr("Initial speed"), mi.mod->spd);
    ep << MetaDataItem(tr("Initial BPM"), mi.mod->bpm);
    ep << MetaDataItem(tr("Module length in patterns"), mi.mod->len);
    return ep;
}

QList<MetaDataItem> XMPMetaDataModel::descriptions() const
{
    QList<MetaDataItem> desc;
    if(!m_ctx)
    {
        return desc;
    }

    xmp_module_info mi;
    xmp_get_module_info(m_ctx, &mi);

    QString value;
    for(int i = 0; i < mi.mod->smp; ++i)
    {
        value += QString::fromUtf8(mi.mod->xxs[i].name) + '\n';
    }

    if(!value.isEmpty())
    {
        desc << MetaDataItem(tr("Samples"), value);
        value.clear();
    }

    for(int i = 0; i < mi.mod->ins; ++i)
    {
        value += QString::fromUtf8(mi.mod->xxi[i].name) + '\n';
    }

    if(!value.isEmpty())
    {
        desc << MetaDataItem(tr("Instruments"), value);
    }
    return desc;
}
