#include "musicrunobject.h"
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
#define S_NETRADIOPATH_FULL       S_APPDATA_DIR_FULL + NETRADIOPATH
#define S_MUSICPATH_FULL          S_APPDATA_DIR_FULL + MUSICPATH
#define S_NORMALDOWNPATH_FULL     S_APPDATA_DIR_FULL + NORMALDOWNPATH
#define S_CLOUDDOWNPATH_FULL      S_APPDATA_DIR_FULL + CLOUDDOWNPATH
#define S_MUSICSEARCH_FULL        S_APPDATA_DIR_FULL + MUSICSEARCH
#define S_DARABASEPATH_FULL       S_APPDATA_DIR_FULL + DARABASEPATH
#define S_USERPATH_FULL           S_APPDATA_DIR_FULL + USERPATH
#define S_BARRAGEPATH_FULL        S_APPDATA_DIR_FULL + BARRAGEPATH

#define S_THEME_DIR_FULL          MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/" + THEME_DIR
#define S_PLUGINS_DIR_FULL        MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/" + PLUGINS_DIR
#define S_LANGUAGE_DIR_FULL       MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/" + LANGUAGE_DIR
#define S_SOUNDPATH_FULL          MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/" + SOUNDPATH

#ifdef Q_OS_WIN
#define S_TTKSERVICE_FULL         MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/TTKService.exe"
#else
#define S_TTKSERVICE_FULL         MusicObject::getAppDir() + TTKMUSIC_VERSION_STR + "/TTKService"
#endif

class MusicRunObjectPrivate : public TTKPrivate<MusicRunObject>
{
public:
    MusicRunObjectPrivate();
    ~MusicRunObjectPrivate();

    QProcess *m_process;

};

MusicRunObjectPrivate::MusicRunObjectPrivate()
{
    m_process = nullptr;
}

MusicRunObjectPrivate::~MusicRunObjectPrivate()
{
    delete m_process;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///
///
MusicRunObject::MusicRunObject(QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(MusicRunObject);

    d->m_process = new QProcess(this);
    connect(d->m_process, SIGNAL(finished(int)),SLOT(finished(int)));
}

void MusicRunObject::checkValid()
{
    checkTheDirectoryExist();
    checkTheFileNeededExist();
}

void MusicRunObject::run(int argc, char **argv)
{
    TTK_D(MusicRunObject);
    QStringList list(APPNAME);
    if(argc == 3)
    {
        list << argv[1] << argv[2];
    }
    d->m_process->start(S_TTKSERVICE_FULL, list);
}

void MusicRunObject::finished(int code)
{
     if(code == 0)
     {
         qDebug() << "Application exit success!";
     }
     else
     {
         qDebug() << "Application run error, please run TTKService instead!";
     }

     qApp->quit();
}

void MusicRunObject::dirIsExist(const QString &name)
{
    QDir dir;
    if(!dir.exists(name))
    {
        dir.mkdir(name);
    }
}

void MusicRunObject::checkTheDirectoryExist()
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

void MusicRunObject::checkTheFileNeededExist()
{
    if(!QFile::exists(S_COFIGPATH_FULL))
    {
        QFile::copy(":/data/musicconfig.xml", S_COFIGPATH_FULL);
    }
    if(!QFile::exists(S_NETRADIOPATH_FULL))
    {
        QFile::copy(":/data/musicradio.dll", S_NETRADIOPATH_FULL);
    }
    if(!QFile::exists(S_MUSICPATH_FULL))
    {
        QFile::copy(":/data/music.lis", S_MUSICPATH_FULL);
    }
    if(!QFile::exists(S_NORMALDOWNPATH_FULL))
    {
        QFile::copy(":/data/musicdown.ttk", S_NORMALDOWNPATH_FULL);
    }
    if(!QFile::exists(S_CLOUDDOWNPATH_FULL))
    {
        QFile::copy(":/data/musicdown.ttk", S_CLOUDDOWNPATH_FULL);
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
