#include "xsfhelper.h"

#include <libxsf/file2sf.h>
#include <libxsf/filegsf.h>
#include <libxsf/fileusf.h>
#include <libxsf/filencsf.h>
#include <libxsf/filesnsf.h>
#include <libxsf/filemsu.h>

static FileReader *generateFileReader(const QString &path)
{
    const QString &suffix = path.toLower();
    if(suffix.endsWith(".2sf") || suffix.endsWith(".mini2sf")) return new File2SFReader;
    else if(suffix.endsWith(".gsf") || suffix.endsWith(".minigsf")) return new FileGSFReader;
    else if(suffix.endsWith(".usf") || suffix.endsWith(".miniusf")) return new FileUSFReader;
    else if(suffix.endsWith(".ncsf") || suffix.endsWith(".minincsf")) return new FileNCSFReader;
    else if(suffix.endsWith(".snsf") || suffix.endsWith(".minisnsf")) return new FileSNSFReader;
    else if(suffix.endsWith(".pcm") || suffix.endsWith(".msu")) return new FileMSUReader;
    else return nullptr;
}


XSFHelper::XSFHelper(const QString &path)
    : m_path(path)
{

}

XSFHelper::~XSFHelper()
{
    deinit();
}

void XSFHelper::metaMode(bool meta)
{
    m_meta = meta;
}

void XSFHelper::deinit()
{
    delete m_input;
}

bool XSFHelper::initialize()
{
    m_input = generateFileReader(m_path);
    if(!m_input)
    {
        qWarning("XSFHelper: load file suffix error");
        return false;
    }

    if(!m_input->load(QmmpPrintable(m_path), m_meta))
    {
       qWarning("XSFHelper: load error");
       return false;
    }
    return true;
}

QMap<Qmmp::MetaData, QString> XSFHelper::readMetaData() const
{
    QMap<Qmmp::MetaData, QString> metaData;
    if(!m_input)
    {
        return metaData;
    }

    const file_meta meta(m_input->get_meta_map());
    for(auto itr = meta.begin(); itr != meta.end(); ++itr)
    {
        if(itr->first == "title") metaData.insert(Qmmp::TITLE, QString::fromStdString(itr->second));
        else if(itr->first == "artist") metaData.insert(Qmmp::ARTIST, QString::fromStdString(itr->second));
        else if(itr->first == "album") metaData.insert(Qmmp::ALBUM, QString::fromStdString(itr->second));
        else if(itr->first == "year") metaData.insert(Qmmp::YEAR, QString::fromStdString(itr->second));
        else if(itr->first == "genre") metaData.insert(Qmmp::GENRE, QString::fromStdString(itr->second));
        else if(itr->first == "copyright") metaData.insert(Qmmp::COMMENT, QString::fromStdString(itr->second));
    }
    return metaData;
}
