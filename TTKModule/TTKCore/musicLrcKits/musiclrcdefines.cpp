#include "musiclrcdefines.h"

MusicLrcDefines::MusicLrcDefines()
{
   m_inlineLrcSize << "14" << "16" << "18" << "20" << "22" << "24"
                   << "26" << "28" << "36" << "48" << "72";

   m_desktopLrcSize << "24" << "25" << "26" << "27" << "28" << "29" << "30"
                    << "31" << "32" << "33" << "34" << "35" << "36";
}

QStringList MusicLrcDefines::getInlineLrcSize() const
{
    return m_inlineLrcSize;
}

QStringList MusicLrcDefines::getDesktopLrcSize() const
{
    return m_desktopLrcSize;
}

int MusicLrcDefines::findInlineLrcIndex(int size) const
{
    return m_inlineLrcSize.indexOf(QString::number(size));
}

int MusicLrcDefines::findInlinePreSize(int index) const
{
    if(index == -1 || index == 0)
    {
        return m_inlineLrcSize.first().toInt();
    }

    return m_inlineLrcSize[index - 1].toInt();
}

int MusicLrcDefines::findInlineNextSize(int index) const
{
    if(index == -1 || index == m_inlineLrcSize.count() - 1)
    {
        return m_inlineLrcSize.last().toInt();
    }

    return m_inlineLrcSize[index + 1].toInt();
}

int MusicLrcDefines::findInlineFirstSize() const
{
    return m_inlineLrcSize.first().toInt();
}

int MusicLrcDefines::findInlineLastSize() const
{
    return m_inlineLrcSize.last().toInt();
}

int MusicLrcDefines::findDesktopLrcIndex(int size) const
{
    return m_desktopLrcSize.indexOf(QString::number(size));
}

int MusicLrcDefines::findDesktopPreSize(int index) const
{
    if(index == -1 || index == 0)
    {
        return m_desktopLrcSize.first().toInt();
    }

    return m_desktopLrcSize[index - 1].toInt();
}

int MusicLrcDefines::findDesktopNextSize(int index) const
{
    if(index == -1 || index == m_desktopLrcSize.count() - 1)
    {
        return m_desktopLrcSize.last().toInt();
    }

    return m_desktopLrcSize[index + 1].toInt();
}

int MusicLrcDefines::findDesktopFirstSize() const
{
    return m_desktopLrcSize.first().toInt();
}

int MusicLrcDefines::findDesktopLastSize() const
{
    return m_desktopLrcSize.last().toInt();
}
