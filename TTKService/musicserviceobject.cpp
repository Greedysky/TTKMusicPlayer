#include "musicserviceobject.h"
#include "musicversion.h"

#include <QApplication>

#define S_APPDATA_DIR_FULL        MusicObject::getAppDir() + APPDATA_DIR
#define S_DOWNLOADS_DIR_FULL      MusicObject::getAppDir() + DOWNLOADS_DIR

#define S_LRC_DIR_FULL            S_DOWNLOADS_DIR_FULL + LRC_DIR
#define S_MUSIC_DIR_FULL          S_DOWNLOADS_DIR_FULL + MUSIC_DIR
#define S_MOVIE_DIR_FULL          S_DOWNLOADS_DIR_FULL + MOVIE_DIR
#define S_TEMPORARY_DIR_FULL      S_DOWNLOADS_DIR_FULL + TEMPORARY_DIR
#define S_CACHE_DIR_FULL          S_DOWNLOADS_DIR_FULL + CACHE_DIR
#define S_ART_DIR_FULL            S_DOWNLOADS_DIR_FULL + ART_DIR
#define S_BACKGROUND_DIR_FULL     S_DOWNLOADS_DIR_FULL + BACKGROUND_DIR

#define S_COFIGPATH_FULL          S_APPDATA_DIR_FULL + COFIGPATH
#define S_NETDADIOPATH_FULL       S_APPDATA_DIR_FULL + NETDADIOPATH
#define S_MUSICPATH_FULL          S_APPDATA_DIR_FULL + MUSICPATH
#define S_DOWNLOADINFO_FULL       S_APPDATA_DIR_FULL + DOWNLOADINFO
#define S_MUSICSEARCH_FULL        S_APPDATA_DIR_FULL + MUSICSEARCH
#define S_DARABASEPATH_FULL       S_APPDATA_DIR_FULL + DARABASEPATH
#define S_USERPATH_FULL           S_APPDATA_DIR_FULL + USERPATH
#define S_BARRAGEPATH_FULL        S_APPDATA_DIR_FULL + BARRAGEPATH

#define S_THEME_DIR_FULL          MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/" + THEME_DIR
#define S_PLUGINS_DIR_FULL        MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/" + PLUGINS_DIR
#define S_LANGUAGE_DIR_FULL       MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/" + LANGUAGE_DIR
#define S_SOUNDPATH_FULL          MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/" + SOUNDPATH

class MusicServiceObjectPrivate : public TTKPrivate<MusicServiceObject>
{
public:
    MusicServiceObjectPrivate();
    ~MusicServiceObjectPrivate();

    QProcess *m_process;

};

MusicServiceObjectPrivate::MusicServiceObjectPrivate()
{
    m_process = nullptr;
}

MusicServiceObjectPrivate::~MusicServiceObjectPrivate()
{
    delete m_process;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///
///
MusicServiceObject::MusicServiceObject(QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(MusicServiceObject);

    d->m_process = new QProcess(this);
    connect(d->m_process, SIGNAL(finished(int)), qApp, SLOT(quit()));
}

void MusicServiceObject::checkValid()
{
    checkTheDirectoryExist();
    checkTheFileNeededExist();
}

void MusicServiceObject::run(int argc, char **argv)
{
    TTK_D(MusicServiceObject);
    QStringList list(APPNAME);
    if(argc == 3)
    {
        list << argv[1] << argv[2];
    }
    d->m_process->start(QString("%1/TTKRun.exe").arg(TTKMUSIC_VERSION_STR), list);
}

void MusicServiceObject::dirIsExist(const QString &name)
{
    QDir dir;
    if(!dir.exists(name))
    {
        dir.mkdir(name);
    }
}

void MusicServiceObject::checkTheDirectoryExist()
{
    dirIsExist(S_APPDATA_DIR_FULL);
    dirIsExist(S_DOWNLOADS_DIR_FULL);

    dirIsExist(S_LRC_DIR_FULL);
    dirIsExist(S_MUSIC_DIR_FULL);
    dirIsExist(S_MOVIE_DIR_FULL);
    dirIsExist(S_CACHE_DIR_FULL);
    dirIsExist(S_ART_DIR_FULL);
    dirIsExist(S_BACKGROUND_DIR_FULL);

    dirIsExist(S_THEME_DIR_FULL);
    dirIsExist(S_PLUGINS_DIR_FULL);
    dirIsExist(S_LANGUAGE_DIR_FULL);
}

void MusicServiceObject::checkTheFileNeededExist()
{
    if(!QFile::exists(S_COFIGPATH_FULL))
    {
        QFile::copy(":/data/musicconfig.xml", S_COFIGPATH_FULL);
    }
    if(!QFile::exists(S_NETDADIOPATH_FULL))
    {
        QFile::copy(":/data/musicradio.dll", S_NETDADIOPATH_FULL);
    }
    if(!QFile::exists(S_MUSICPATH_FULL))
    {
        QFile::copy(":/data/music.dll", S_MUSICPATH_FULL);
    }
    if(!QFile::exists(S_DOWNLOADINFO_FULL))
    {
        QFile::copy(":/data/musicdown.ttk", S_DOWNLOADINFO_FULL);
    }
    if(!QFile::exists(S_MUSICSEARCH_FULL))
    {
        QFile::copy(":/data/musichistory.ttk", S_MUSICSEARCH_FULL);
    }
    if(!QFile::exists(S_DARABASEPATH_FULL))
    {
        QFile::copy(":/data/musicuser.dll", S_DARABASEPATH_FULL);
    }
    if(!QFile::exists(S_USERPATH_FULL))
    {
        QFile::copy(":/data/musicuser.ttk", S_USERPATH_FULL);
    }
    if(!QFile::exists(S_BARRAGEPATH_FULL))
    {
        QFile::copy(":/data/musicbarrage.ttk", S_BARRAGEPATH_FULL);
    }
    if(!QFile::exists(S_SOUNDPATH_FULL))
    {
        QFile::copy(":/data/sound.wav", S_SOUNDPATH_FULL);
    }
}
