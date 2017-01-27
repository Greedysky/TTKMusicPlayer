#ifndef MUSICSONGCHECKTOOLSUNIT_H
#define MUSICSONGCHECKTOOLSUNIT_H

#include "musicobject.h"
#include "musicglobaldefine.h"

typedef struct MUSIC_TOOLSET_EXPORT SongCheckToolsRename
{
    QString m_locaName;
    QString m_RecommendName;

    SongCheckToolsRename(const QString &locaName, const QString &recommendName)
    {
        m_locaName = locaName;
        m_RecommendName = recommendName;
    }
}SongCheckToolsRename;
TTK_DECLARE_LISTS(SongCheckToolsRename)


#endif // MUSICSONGCHECKTOOLSUNIT_H
