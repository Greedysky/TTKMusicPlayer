#include "xsfhelper.h"

#include <libxsf/file2sf.h>
#include <libxsf/filegsf.h>
#include <libxsf/fileusf.h>
#include <libxsf/filencsf.h>
#include <libxsf/filesnsf.h>
#include <libxsf/filemsu.h>

XSFHelper::XSFHelper(const QString &path, bool meta)
    : m_path(path),
      m_meta(meta)
{

}

XSFHelper::~XSFHelper()
{
    deinit();
}

void XSFHelper::deinit()
{
    delete m_input;
}

bool XSFHelper::initialize()
{
    const QString &suffix = m_path.toLower();
    if(suffix.endsWith(".2sf") || suffix.endsWith(".mini2sf")) m_input = new File2SFReader;
    else if(suffix.endsWith(".gsf") || suffix.endsWith(".minigsf")) m_input = new FileGSFReader;
    else if(suffix.endsWith(".usf") || suffix.endsWith(".miniusf")) m_input = new FileUSFReader;
    else if(suffix.endsWith(".ncsf") || suffix.endsWith(".minincsf")) m_input = new FileNCSFReader;
    else if(suffix.endsWith(".snsf") || suffix.endsWith(".minisnsf")) m_input = new FileSNSFReader;
    else if(suffix.endsWith(".pcm") || suffix.endsWith(".msu")) m_input = new FileMSUReader;

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
