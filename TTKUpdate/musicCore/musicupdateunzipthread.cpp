#include "musicupdateunzipthread.h"
#include "gojobzip.h"

#include <QDir>
#include <QDebug>

MusicUpdateUnzipThread::MusicUpdateUnzipThread(QObject *parent)
    : QThread(parent)
{
    m_run = true;
}

MusicUpdateUnzipThread::~MusicUpdateUnzipThread()
{
    stopAndQuitThread();
}

QString MusicUpdateUnzipThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicUpdateUnzipThread::run()
{
    PackZip zip;
    if(parent()->metaObject()->indexOfSlot("process(float,QString)") != -1)
    {
        connect(&zip, SIGNAL(process(float,QString)),
                parent(), SLOT(process(float,QString)));
    }

    QDir checkOutput(m_output);
    checkOutput.mkpath(m_output);

    if(checkOutput.exists() && zip.isFile(m_input))
    {
        QString dis = checkOutput.absolutePath();
        if(!dis.endsWith("/"))
        {
            dis.append("/");
        }
;
        if(zip.unZipTo(m_input, dis))
        {
            qDebug() << "UnZip " << m_input << " is ready.\n";
        }
        else
        {
            qDebug() << "UnZip " << m_input << " file or dir not found.\n";
        }
    }else
    {
        qDebug() << "UnZip " << m_input << " file or dir not found.\n";
    }
}

void MusicUpdateUnzipThread::start()
{
    m_run = true;
    QThread::start();
}

void MusicUpdateUnzipThread::setFilePath(const QString &in, const QString &out)
{
    m_input = in;
    m_output = out;
}

void MusicUpdateUnzipThread::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}
