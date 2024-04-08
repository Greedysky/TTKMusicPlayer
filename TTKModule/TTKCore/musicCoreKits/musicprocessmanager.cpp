#include "musicprocessmanager.h"
#include "musicapplication.h"
#include "ttkcommandline.h"

#define MEMORY_KEY TTK_STR_CAT(MUSIC_AUTHOR_NAME, TTK_DEFAULT_STR, TTK_APP_NAME)

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

    m_memory.create(512 * TTK_SN_KB2B);
    m_timer.start();
}

MusicProcessServer::~MusicProcessServer()
{
    m_timer.stop();
    m_memory.detach();
}

void MusicProcessServer::run(const QStringList &args)
{
    if(args.count() != 2)
    {
        return;
    }

    TTK_INFO_STREAM("Command line args: " << args);

    TTKCommandLineOption op1("-Open");
    TTKCommandLineOption op2("-List");

    TTKCommandLineParser parser;
    parser.addOption(op1);
    parser.addOption(op2);
    parser.process(args);

    if(parser.isSet(op1))
    {
        MusicApplication::instance()->importSongsByOutside(parser.value(op1), true);
    }
    else if(parser.isSet(op2))
    {
        MusicApplication::instance()->importSongsByOutside(parser.value(op2), false);
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
    memcpy(m_memory.data(), "\0", 1);
    m_memory.unlock();

    run(buffer.split(TTK_SPLITER));
}
