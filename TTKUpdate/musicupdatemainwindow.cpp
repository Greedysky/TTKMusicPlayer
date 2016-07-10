#include "musicupdatemainwindow.h"
#include "musicupdateunzipthread.h"
#include "musicmessagebox.h"

#include <QApplication>
#if defined Q_OS_UNIX || defined Q_CC_MINGW
# include <unistd.h>
#endif

MusicUpdateMainWindow::MusicUpdateMainWindow(QWidget *parent)
    : MusicProgressWidget(parent)
{
    setTitle(tr("Unzip File Mode"));
    setRange(0, 100);

    m_thread = new MusicUpdateUnzipThread(this);
    connect(m_thread, SIGNAL(finished()), SLOT(finished()));
}

MusicUpdateMainWindow::~MusicUpdateMainWindow()
{
    delete m_thread;
}

QString MusicUpdateMainWindow::getClassName()
{
    return staticMetaObject.className();
}

void MusicUpdateMainWindow::setFilePath(const QString &in, const QString &out)
{
#if defined Q_OS_WIN && defined MUSIC_GREATER_NEW
    QThread::sleep(3);
#else
    sleep(3);
#endif
    m_thread->setFilePath(in, out);
    m_thread->start();
}

void MusicUpdateMainWindow::process(float percent, const QString &file)
{
    setValue(percent*100);
    setTitle(file);
}

void MusicUpdateMainWindow::finished()
{
    MusicMessageBox message;
    message.setText(tr("unzip finished, please restart!"));
    message.exec();

    qApp->quit();
}
