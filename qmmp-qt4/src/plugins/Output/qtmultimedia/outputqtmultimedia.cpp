/***************************************************************************
 *   Copyright (C) 2015 by Ivan Ponomarev                                  *
 *   ivantrue@gmail.com                                                    *
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

#include "outputqtmultimedia.h"

#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QSettings>
#include <QDebug>
#include <unistd.h>


OutputQtMultimedia::OutputQtMultimedia() : Output(), m_buffer(NULL), m_bytes_per_second(0)
{
}

OutputQtMultimedia::~OutputQtMultimedia()
{
}

bool OutputQtMultimedia::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
	QAudioFormat qformat;
	qformat.setCodec("audio/pcm");
	qformat.setFrequency(freq);
	qformat.setByteOrder(QAudioFormat::LittleEndian);
	qformat.setChannels(map.size());
	qformat.setSampleType(QAudioFormat::SignedInt);

	//Size of sample representation in input data. For 24-bit is 4, high byte is ignored.
	qint64 bytes_per_sample = 0;

	switch (format)
	{
	case Qmmp::PCM_S8:
		qformat.setSampleSize(8);
		bytes_per_sample = 1;
		break;
	case Qmmp::PCM_S16LE:
		qformat.setSampleSize(16);
		bytes_per_sample = 2;
		break;
	case Qmmp::PCM_S24LE:
		qformat.setSampleSize(24);
		bytes_per_sample = 4;
		break;
	case Qmmp::PCM_S32LE:
		qformat.setSampleSize(32);
		bytes_per_sample = 4;
		break;
	default:
		break;
	}

	if (!qformat.isValid())
		return false;

	m_bytes_per_second = bytes_per_sample * freq * qformat.channels();

	const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
	const QString saved_device_name = settings.value("QTMULTIMEDIA/device").toString();

	QAudioDeviceInfo device_info;
	if (!saved_device_name.isEmpty())
	{
		const QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
		foreach (const QAudioDeviceInfo &info, devices)
		{
			if (info.deviceName()==saved_device_name)
			{
				if (info.isFormatSupported(qformat))
				{
					device_info = info;
					break;
				}
				else
					qDebug() << "OutputQtMultimedia: Output device: " << saved_device_name << " is not supported";
			}
		}
	}

	if (device_info.isNull())
	{
		device_info = QAudioDeviceInfo::defaultOutputDevice();
		if (!device_info.isFormatSupported(qformat))
			return false;
	}

	qDebug() << "OutputQtMultimedia: Using output device: " << device_info.deviceName();

	m_output.reset(new QAudioOutput(device_info, qformat));
	m_buffer = m_output->start();

	configure(freq, map, format);
	return true;
}


qint64 OutputQtMultimedia::latency()
{
	return 0;
}

qint64 OutputQtMultimedia::writeAudio(unsigned char *data, qint64 maxSize)
{
	if (!m_output->bytesFree()) {
		//If the buffer is full, waiting for some bytes to be played:
		//trying to play maxSize bytes, but not more than half of buffer.
		usleep(qMin(maxSize, static_cast<qint64>(m_output->bufferSize() / 2)) * 1000000 / m_bytes_per_second);
	}
	return m_buffer->write((const char*)data, maxSize);
}

void OutputQtMultimedia::drain()
{
	m_buffer->waitForBytesWritten(-1);
}

void OutputQtMultimedia::reset()
{
	m_buffer->reset();
	m_buffer = m_output->start();
}

void OutputQtMultimedia::suspend()
{
	m_output->suspend();
}

void OutputQtMultimedia::resume()
{
	m_output->resume();
}
