#include "musiclrcdefines.h"

MusicLrcDefines::MusicLrcDefines()
{
   m_interiorLrcSize << "14" << "16" << "18" << "20" << "22" << "24" << "26" << "28" << "36" << "48" << "72";
   m_desktopLrcSize << "24" << "25" << "26" << "27" << "28" << "29" << "30" << "31" << "32" << "33" << "34" << "35" << "36";
}

QStringList MusicLrcDefines::interiorLrcSize() const noexcept
{
    return m_interiorLrcSize;
}

QStringList MusicLrcDefines::desktopLrcSize() const noexcept
{
    return m_desktopLrcSize;
}

int MusicLrcDefines::findInteriorLrcIndex(int size) const
{
    return m_interiorLrcSize.indexOf(QString::number(size));
}

int MusicLrcDefines::findInteriorPreSize(int index) const
{
    if(index == -1 || index == 0)
    {
        return m_interiorLrcSize.front().toInt();
    }

    return m_interiorLrcSize[index - 1].toInt();
}

int MusicLrcDefines::findInteriorNextSize(int index) const
{
    if(index == -1 || index == m_interiorLrcSize.count() - 1)
    {
        return m_interiorLrcSize.back().toInt();
    }

    return m_interiorLrcSize[index + 1].toInt();
}

int MusicLrcDefines::findInteriorFirstSize() const
{
    return m_interiorLrcSize.front().toInt();
}

int MusicLrcDefines::findInteriorLastSize() const
{
    return m_interiorLrcSize.back().toInt();
}

int MusicLrcDefines::findDesktopLrcIndex(int size) const
{
    return m_desktopLrcSize.indexOf(QString::number(size));
}

int MusicLrcDefines::findDesktopPreSize(int index) const
{
    if(index == -1 || index == 0)
    {
        return m_desktopLrcSize.front().toInt();
    }

    return m_desktopLrcSize[index - 1].toInt();
}

int MusicLrcDefines::findDesktopNextSize(int index) const
{
    if(index == -1 || index == m_desktopLrcSize.count() - 1)
    {
        return m_desktopLrcSize.back().toInt();
    }

    return m_desktopLrcSize[index + 1].toInt();
}

int MusicLrcDefines::findDesktopFirstSize() const
{
    return m_desktopLrcSize.front().toInt();
}

int MusicLrcDefines::findDesktopLastSize() const
{
    return m_desktopLrcSize.back().toInt();
}
