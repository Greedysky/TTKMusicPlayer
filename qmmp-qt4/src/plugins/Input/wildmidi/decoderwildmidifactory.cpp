/***************************************************************************
 *   Copyright (C) 2008-2016 by Ilya Kotov                                 *
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

#include <QApplication>
#include <QtPlugin>
#include "wildmidihelper.h"
#include "decoder_wildmidi.h"
#include "decoderwildmidifactory.h"

// DecoderWildMidiFactory

DecoderWildMidiFactory::DecoderWildMidiFactory()
{
    new WildMidiHelper(qApp);
}

bool DecoderWildMidiFactory::supports(const QString &source) const
{
    foreach(QString filter, properties().filters)
    {
        QRegExp regexp(filter, Qt::CaseInsensitive, QRegExp::Wildcard);
        if (regexp.exactMatch(source))
            return true;
    }
    return false;
}

bool DecoderWildMidiFactory::canDecode(QIODevice *input) const
{
    char buf[4];
    if(input->peek(buf, 4) != 4)
        return false;
#if defined(LIBWILDMIDI_VERSION) && (LIBWILDMIDI_VERSION >= 0x000400)
    return !memcmp(buf, "MThd", 4) || !memcmp(buf, "MUS", 3) || !memcmp(buf, "FORM", 4);
#else
    return !memcmp(buf, "MThd", 4);
#endif
}

const DecoderProperties DecoderWildMidiFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("WildMidi Plugin");
    properties.filters << "*.mid";
#if defined(LIBWILDMIDI_VERSION) && (LIBWILDMIDI_VERSION >= 0x000400)
    properties.filters << "*.mus" << "*.xmi";
#endif
    properties.description = tr("Midi Files");
    //properties.contentType = ;
    properties.shortName = "wildmidi";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.noInput = true;
    properties.protocols << "file";
    return properties;
}

Decoder *DecoderWildMidiFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderWildMidi(path);
}

QList<FileInfo *> DecoderWildMidiFactory::createPlayList(const QString &fileName, bool useMetaData, QStringList *)
{
    Q_UNUSED(useMetaData);
    QList <FileInfo*> list;
    FileInfo *info = new FileInfo(fileName);

    if(WildMidiHelper::instance()->initialize() && WildMidiHelper::instance()->sampleRate())
    {
        void *midi_ptr = WildMidi_Open (fileName.toLocal8Bit());
        if(midi_ptr)
        {
            WildMidiHelper::instance()->addPtr(midi_ptr);
            _WM_Info *wm_info = WildMidi_GetInfo(midi_ptr);
            info->setLength((qint64)wm_info->approx_total_samples
                            / WildMidiHelper::instance()->sampleRate());
            WildMidi_Close(midi_ptr);
            WildMidiHelper::instance()->removePtr(midi_ptr);
        }
    }
    list << info;
    return list;
}

MetaDataModel* DecoderWildMidiFactory::createMetaDataModel(const QString &path, QObject *parent)
{
    Q_UNUSED(path);
    Q_UNUSED(parent);
    return 0;
}

Q_EXPORT_PLUGIN2(wildmidi,DecoderWildMidiFactory)
