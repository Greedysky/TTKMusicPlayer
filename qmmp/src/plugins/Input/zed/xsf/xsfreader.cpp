#include "xsfreader.h"

FileReader *XSFReader::makeReader(const QString &path)
{
    const QString &suffix = path.toLower();
    if(suffix.endsWith(".2sf") || suffix.endsWith(".mini2sf"))
    {
        return new File2SFReader;
    }
    else if(suffix.endsWith(".gsf") || suffix.endsWith(".minigsf"))
    {
        return new FileGSFReader;
    }
    else if(suffix.endsWith(".usf") || suffix.endsWith(".miniusf"))
    {
        return new FileUSFReader;
    }
    else if(suffix.endsWith(".ncsf") || suffix.endsWith(".minincsf"))
    {
        return new FileNCSFReader;
    }
    else if(suffix.endsWith(".snsf") || suffix.endsWith(".minisnsf"))
    {
        return new FileSNSFReader;
    }
    return nullptr;
}
