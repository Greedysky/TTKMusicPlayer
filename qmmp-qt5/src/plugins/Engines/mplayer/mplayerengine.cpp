/***************************************************************************
 *   Copyright (C) 2008-2015 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QObject>
#include <QFile>
#include <QApplication>
#include <QAction>
#include <QKeyEvent>
#include <QMenu>
#include <QRegExp>
#include <QSettings>
#include <qmmp/fileinfo.h>
#include <qmmp/inputsource.h>
#include "mplayerengine.h"

//#define MPLAYER_DEBUG

static QRegExp rx_av("^[AV]: *([0-9,:.-]+)");
static QRegExp rx_pause("^(.*)=(.*)PAUSE(.*)");
static QRegExp rx_end("^(.*)End of file(.*)");
static QRegExp rx_quit("^(.*)Quit(.*)");
static QRegExp rx_audio("^AUDIO: *([0-9,.]+) *Hz.*([0-9,.]+) *ch.*([0-9]+).* ([0-9,.]+) *kbit.*");


FileInfo *MplayerInfo::createFileInfo(const QString &path)
{
    QRegExp rx_id_length("^ID_LENGTH=([0-9,.]+)*");
    QStringList args;
    args << "-slave";
    args << "-identify";
    args << "-frames";
    args << "0";
    args << "-vo";
    args << "null";
    args << "-ao";
    args << "null";
    args << path;
    QProcess mplayer_process;
    mplayer_process.start("mplayer", args);
    mplayer_process.waitForFinished(1500);
    mplayer_process.kill();
    QString str = QString::fromLocal8Bit(mplayer_process.readAll()).trimmed();
    FileInfo *info = new FileInfo(path);
    QStringList lines = str.split("\n");
    foreach(QString line, lines)
    {
        if (rx_id_length.indexIn(line) > -1)
            info->setLength((qint64) rx_id_length.cap(1).toDouble());
    }
#ifdef MPLAYER_DEBUG
    qDebug("%s",qPrintable(str));
#endif
    return info;
}

QStringList MplayerInfo::filters()
{
    QStringList filters;
    filters << "*.avi" << "*.mpg" << "*.mpeg" << "*.divx" << "*.qt" << "*.mov" << "*.wmv" << "*.asf"
            << "*.flv" << "*.3gp" << "*.mkv" << "*.mp4" << "*.webm";
    return filters;
}

MplayerEngine::MplayerEngine(QObject *parent)
        : AbstractEngine(parent)
{
    m_source = 0;
    m_bitrate = 0;
    m_samplerate = 0;
    m_channels = 0;
    m_bitsPerSample = 0;
    m_length = 0;
    m_currentTime = 0;
    m_muted = false;
    m_user_stop = false;
    m_process = 0;
}

MplayerEngine::~MplayerEngine()
{
    qDebug("%s",__FUNCTION__);
    if(m_process)
        m_process->kill();
    while(!m_sources.isEmpty())
        m_sources.dequeue()->deleteLater();
}

bool MplayerEngine::play()
{
    m_user_stop = false;
    if(m_process && m_process->state() != QProcess::NotRunning)
        return false;
    startMplayerProcess();
    return true;
}

bool MplayerEngine::enqueue(InputSource *source)
{
    QStringList filters = MplayerInfo::filters();
    bool supports = false;
    foreach(QString filter, filters)
    {
        QRegExp regexp(filter, Qt::CaseInsensitive, QRegExp::Wildcard);
        supports = regexp.exactMatch(source->url());
        if(supports)
            break;
    }
    if(!supports)
        return false;

    if(!m_process || m_process->state() == QProcess::NotRunning)
        m_source = source;
    else
        m_sources.enqueue(source);
    return true;
}

bool MplayerEngine::initialize()
{
    FileInfo *info = MplayerInfo::createFileInfo(m_source->url());
    m_length = info->length();
    delete info;
    m_args.clear();
    m_args << "-slave";
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    QString ao_str = settings.value("mplayer/ao","default").toString();
    QString vo_str = settings.value("mplayer/vo","default").toString();
    if (ao_str != "default")
        m_args << "-ao" << ao_str;
    if (vo_str != "default")
        m_args << "-vo" << vo_str;

    if (settings.value("autosync", false).toBool())
        m_args << "-autosync" << QString("%1").arg(settings.value("autosync_factor", 100).toInt());

    m_args << settings.value("cmd_options").toString().split(" ", QString::SkipEmptyParts);

    if(m_source->offset() > 0)
        m_args << "-ss" << QString("%1").arg(m_source->offset()/1000);
    m_args << m_source->url();
    return true;
}

void MplayerEngine::seek(qint64 pos)
{
    if (m_process && m_process->state() == QProcess::Running)
        m_process->write(QString("seek %1\n").arg(pos/1000 - m_currentTime).toLocal8Bit ());
}

void MplayerEngine::stop()
{
    while(!m_sources.isEmpty())
        m_sources.dequeue()->deleteLater();
    if(m_process && m_process->state() == QProcess::Running)
    {
        m_user_stop = true;
        m_process->write("quit\n");
        m_process->waitForFinished(3500);
        m_process->kill();
        StateHandler::instance()->dispatch(Qmmp::Stopped);
    }
}

void MplayerEngine::pause()
{
    if(m_process)
        m_process->write("pause\n");
}

void MplayerEngine::setMuted(bool muted)
{
    if(m_process && m_process->state() == QProcess::Running)
    {
        m_process->write(muted ? "mute 1\n" : "mute 0\n");
    }
}

void MplayerEngine::readStdOut()
{
    QString line = QString::fromLocal8Bit(m_process->readAll ()).trimmed();
    QStringList lines = line.split("\n");
    foreach(line, lines)
    {
        if (rx_av.indexIn(line) > -1)
        {
            StateHandler::instance()->dispatch(Qmmp::Playing);
            m_currentTime = (qint64) rx_av.cap(1).toDouble();
            StateHandler::instance()->dispatch(m_currentTime * 1000,
                                               m_bitrate,
                                               m_samplerate,
                                               m_bitsPerSample,
                                               m_channels);
        }
        else if (rx_pause.indexIn(line) > -1)
        {
            StateHandler::instance()->dispatch(Qmmp::Paused);
        }
        else if (rx_end.indexIn(line) > -1)
        {
            if (m_process->state() == QProcess::Running)
                m_process->waitForFinished(3500);
            StateHandler::instance()->sendFinished();
            if(!m_sources.isEmpty())
            {
                StateHandler::instance()->dispatch(Qmmp::Stopped);
                m_source = m_sources.dequeue();
                startMplayerProcess();
            }
            else
            {
                StateHandler::instance()->dispatch(Qmmp::Stopped);
                return;
            }
        }
        else if (rx_quit.indexIn(line) > -1 && !m_user_stop)
        {
            if (m_process->state() == QProcess::Running)
            {
                m_process->waitForFinished(1500);
                m_process->kill();
            }
            StateHandler::instance()->dispatch(Qmmp::Stopped);
        }
        else if (rx_audio.indexIn(line) > -1)
        {
            m_samplerate = rx_audio.cap(1).toInt();
            m_channels = rx_audio.cap(2).toInt();
            m_bitsPerSample = rx_audio.cap(3).toDouble();
            m_bitrate = rx_audio.cap(4).toDouble();
        }
#ifdef MPLAYER_DEBUG
        else
            qDebug("%s",qPrintable(line));
#endif
    }
}

void MplayerEngine::onError(QProcess::ProcessError error)
{
    if(error == QProcess::FailedToStart || error == QProcess::FailedToStart)
        StateHandler::instance()->dispatch(Qmmp::FatalError);
    qWarning("MplayerEngine: process error: %d", error);
}

void MplayerEngine::startMplayerProcess()
{
    initialize();
    if(m_process)
        delete m_process;
    m_process = new QProcess(this);
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(readStdOut()));
    connect(m_process, SIGNAL(error(QProcess::ProcessError)), SLOT(onError(QProcess::ProcessError)));
    m_process->start ("mplayer", m_args);
    StateHandler::instance()->dispatch(Qmmp::Playing);
    StateHandler::instance()->dispatch(m_length * 1000);
    FileInfo *info = MplayerInfo::createFileInfo(m_source->url());
    StateHandler::instance()->dispatch(info->metaData());
    delete info;
    m_source->deleteLater();
    m_source = 0;
    m_currentTime = 0;
    if(m_muted)
        setMuted(true);
}
