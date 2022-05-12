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
       qWarning("XSFHelper: unable to open file");
       return false;
    }
    return true;
}
