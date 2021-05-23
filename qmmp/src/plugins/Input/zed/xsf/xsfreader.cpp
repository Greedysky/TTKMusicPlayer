#include "xsfreader.h"

FileReader* XSFReader::makeReader(const QString &path)
{
    const QString &suffix = path.toLower();
    if(suffix.endsWith(".2sf") || suffix.endsWith(".mini2sf"))
    {
        return new DsFileReader;
    }
    else if(suffix.endsWith(".gsf") || suffix.endsWith(".minigsf"))
    {
        return new GsFileReader;
    }
    else if(suffix.endsWith(".usf") || suffix.endsWith(".miniusf"))
    {
        return new UsFileReader;
    }
    return nullptr;
}
