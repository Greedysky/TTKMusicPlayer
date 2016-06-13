#include "musicupdatemainwindow.h"
#include "musicprogresswidget.h"
#include "gojobzip.h"

#include <QDebug>
#include <QDir>
#include <QThread>

MusicUpdateMainWindow::MusicUpdateMainWindow(QWidget *parent)
    : MusicAbstractMoveWidget(parent)
{
    resize(500, 500);
    m_progressBar = new MusicProgressWidget(this);
}

MusicUpdateMainWindow::~MusicUpdateMainWindow()
{
    delete m_progressBar;
}

void MusicUpdateMainWindow::setFilePath(const QString &in, const QString &out)
{
#if defined Q_OS_WIN && defined MUSIC_QT_5
    QThread::sleep(10);
#else
    sleep(10);
#endif
    m_progressBar->show();
    m_progressBar->setTitle(tr("Copy File Mode"));
    m_progressBar->setRange(0, 100);

    PackZip zip;
    connect(&zip, SIGNAL(process(float,QString)), SLOT(process(float,QString)));

    QDir checkOutput(out);
    checkOutput.mkpath(out);

    if(checkOutput.exists() && zip.isFile(in))
    {
        QString dis = checkOutput.absolutePath();
        if(!dis.endsWith("/"))
        {
            dis.append("/");
        }

        bool uz = zip.unZipTo(in, dis);
        if(uz)
        {
            qDebug() << "UnZip " << in << " is ready.\n";
        }
        else
        {
           qDebug() << "UnZip " << in << " file or dir not found.\n";
        }
    }else
    {
        qDebug() << "UnZip " << in << " file or dir not found.\n";
    }

    delete m_progressBar;
    m_progressBar = nullptr;
}

void MusicUpdateMainWindow::process(float percent, const QString &file)
{
    if(m_progressBar)
    {
        m_progressBar->setValue(percent);
        m_progressBar->setTitle(file);
    }
}
