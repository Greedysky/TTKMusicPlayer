#include "sndfilemetadatamodel.h"
#include "decoder_sndfile.h"

SndFileMetaDataModel::SndFileMetaDataModel(const QString &path)
    : MetaDataModel(true),
      m_path(path)
{

}

QList<MetaDataItem> SndFileMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    SF_INFO snd_info;
    memset(&snd_info, 0, sizeof(snd_info));
    snd_info.format = 0;
#ifdef Q_OS_WIN
    SNDFILE *sndfile = sf_wchar_open(reinterpret_cast<LPCWSTR>(m_path.utf16()), SFM_READ, &snd_info);
#else
    SNDFILE *sndfile = sf_open(qPrintable(m_path), SFM_READ, &snd_info);
#endif
    if(!sndfile)
    {
        return ep;
    }

    const char *software = sf_get_string(sndfile, SF_STR_SOFTWARE);
    ep << MetaDataItem(tr("Software"), software ? QString::fromUtf8(software) : QString());
    const char *license = sf_get_string(sndfile, SF_STR_LICENSE);
    ep << MetaDataItem(tr("License"), license ? QString::fromUtf8(license) : QString());
    const char *copyright = sf_get_string(sndfile, SF_STR_COPYRIGHT);
    ep << MetaDataItem(tr("Copyright"), copyright ? QString::fromUtf8(copyright) : QString());

    sf_close(sndfile);
    return ep;
}
