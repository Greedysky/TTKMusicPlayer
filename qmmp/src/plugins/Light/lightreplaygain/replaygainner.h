/***************************************************************************
 *   Copyright (C) 2013-2024 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
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

#ifndef REPLAYGAINNER_H
#define REPLAYGAINNER_H

#include <QRunnable>
#include <QMutex>
#include <qmmp/decoder.h>
#include <qmmp/inputsource.h>
#include "gain_analysis.h"

/*!
 * @author Greedysky <greedysky@163.com>
 */
class ReplayGainner : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ReplayGainner();
    ~ReplayGainner();

    bool prepare(const QString &url);
    void stop();

    bool hasValues() const;

    QMap<Qmmp::ReplayGainKey, double> oldReplayGainInfo() const;
    double gain() const;
    double peak() const;
    QString url() const;
    GainHandle_t *handle();

signals:
    void progress(int percent);
    void finished(const QString &url);

private:
    virtual void run() override final;
    void deinit();

    InputSource *m_source = nullptr;
    Decoder *m_decoder = nullptr;
    QMutex m_mutex;
    bool m_stop = false, m_hasValues = false;
    QString m_url;
    double m_gain = 0.0;
    double m_peak = 0.0;
    GainHandle_t *m_handle = nullptr;

};

#endif
