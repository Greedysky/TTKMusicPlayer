#include "musiclrchelper.h"

MusicLrcHelper::MusicLrcHelper()
{
   m_interiorLrcSize << "14" << "16" << "18" << "20" << "22" << "24" << "26" << "28" << "36" << "48" << "72";
   m_desktopLrcSize << "24" << "25" << "26" << "27" << "28" << "29" << "30" << "31" << "32" << "33" << "34" << "35" << "36";
}

QStringList MusicLrcHelper::interiorLrcSize() const noexcept
{
    return m_interiorLrcSize;
}

QStringList MusicLrcHelper::desktopLrcSize() const noexcept
{
    return m_desktopLrcSize;
}

int MusicLrcHelper::findInteriorLrcIndex(int size) const
{
    return m_interiorLrcSize.indexOf(QString::number(size));
}

int MusicLrcHelper::findInteriorPreSize(int index) const
{
    if(index == -1 || index == 0)
    {
        return m_interiorLrcSize.first().toInt();
    }

    return m_interiorLrcSize[index - 1].toInt();
}

int MusicLrcHelper::findInteriorNextSize(int index) const
{
    if(index == -1 || index == m_interiorLrcSize.count() - 1)
    {
        return m_interiorLrcSize.last().toInt();
    }

    return m_interiorLrcSize[index + 1].toInt();
}

int MusicLrcHelper::findInteriorFirstSize() const
{
    return m_interiorLrcSize.first().toInt();
}

int MusicLrcHelper::findInteriorLastSize() const
{
    return m_interiorLrcSize.last().toInt();
}

int MusicLrcHelper::findDesktopLrcIndex(int size) const
{
    return m_desktopLrcSize.indexOf(QString::number(size));
}

int MusicLrcHelper::findDesktopPreSize(int index) const
{
    if(index == -1 || index == 0)
    {
        return m_desktopLrcSize.first().toInt();
    }

    return m_desktopLrcSize[index - 1].toInt();
}

int MusicLrcHelper::findDesktopNextSize(int index) const
{
    if(index == -1 || index == m_desktopLrcSize.count() - 1)
    {
        return m_desktopLrcSize.last().toInt();
    }

    return m_desktopLrcSize[index + 1].toInt();
}

int MusicLrcHelper::findDesktopFirstSize() const
{
    return m_desktopLrcSize.first().toInt();
}

int MusicLrcHelper::findDesktopLastSize() const
{
    return m_desktopLrcSize.last().toInt();
}
