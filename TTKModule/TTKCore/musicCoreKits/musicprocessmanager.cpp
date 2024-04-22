#include "musicprocessmanager.h"
#include "musicapplication.h"
#include "ttkcommandline.h"

#define MEMORY_SIZE 512 * TTK_SN_KB2B
#define MEMORY_KEY  TTK_STR_CAT(TTK_AUTHOR_NAME, TTK_DEFAULT_STR, TTK_APP_NAME)

void MusicProcessClient::run(const QStringList &args) const
{
    QSharedMemory client;
    client.setKey(MEMORY_KEY);

    client.attach();

    if(client.size() <= 0)
    {
        return;
    }

    client.lock();
    const QString &data = args.join(TTK_SPLITER);
    memcpy(client.data(), data.toUtf8().constData(), data.length());
    client.unlock();
    client.detach();
}


MusicProcessServer::MusicProcessServer(QObject *parent)
    : QObject(parent)
{
    m_timer.setInterval(TTK_DN_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));

    m_memory.setKey(MEMORY_KEY);
    if(m_memory.isAttached())
    {
        m_memory.detach();
    }

    m_memory.create(MEMORY_SIZE);
    m_timer.start();
}

MusicProcessServer::~MusicProcessServer()
{
    m_timer.stop();
    m_memory.detach();
}

void MusicProcessServer::run(const QStringList &args) const
{
    TTK_INFO_STREAM("Command line args:" << args);
    if(args.isEmpty())
    {
        return;
    }

    TTKCommandLineOption op0("-h", "--help", "Show command line help options");
    TTKCommandLineOption op1("-List", "--file-to-list", "Add file to playlist");
    TTKCommandLineOption op2("-Open", "--file-to-list-play", "Add file to playlist and play it");
//    TTKCommandLineOption op3("-p", "--play", "Pause if playing, play otherwise");
//    TTKCommandLineOption op4("-s", "--stop", "Stop current song");
//    TTKCommandLineOption op5("--next", "Start playing next song");
//    TTKCommandLineOption op6("--previous", "Start playing previous song");
//    TTKCommandLineOption op7("--toggle-mute", "Mute/Restore volume");
//    TTKCommandLineOption op8("--volume", "Set playback volume (example: --volume 20)");
//    TTKCommandLineOption op9("--add-file", "Display add file dialog");
//    TTKCommandLineOption op10("--add-dir", "Display add directory dialog");
//    TTKCommandLineOption op11("--add-url", "Display add url path dialog");

    TTKCommandLineParser parser;
    parser.addOption(op0);
    parser.addOption(op1);
    parser.addOption(op2);
    parser.process(args);

    if(parser.isSet(op0))
    {
        parser.showHelp();
        return;
    }

    if(parser.isSet(op1))
    {
        MusicApplication::instance()->importSongsByOutside(parser.value(op1), false);
    }
    else if(parser.isSet(op2))
    {
        MusicApplication::instance()->importSongsByOutside(parser.value(op2), true);
    }
}

void MusicProcessServer::timeout()
{
    const QString &buffer = QString::fromUtf8((char*)m_memory.data());
    if(buffer.isEmpty())
    {
        return;
    }

    m_memory.lock();
    memset(m_memory.data(), 0, MEMORY_SIZE);
    m_memory.unlock();

    run(buffer.split(TTK_SPLITER));
}
