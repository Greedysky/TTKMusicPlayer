/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2022 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef DECODER_OPENMPT_H
#define DECODER_OPENMPT_H

#include "openmpthelper.h"
#include <qmmp/decoder.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class DecoderOpenMPT : public Decoder
{
public:
    DecoderOpenMPT(QIODevice *input);
    virtual ~DecoderOpenMPT();

    static DecoderOpenMPT *instance();

    void readSettings();

    // Standard Decoder API
    virtual bool initialize() override;
    virtual qint64 totalTime() const override;
    virtual int bitrate() const override;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override;
    virtual void seek(qint64 time) override;

private:
    OpenMPTHelper *m_helper = nullptr;

    static DecoderOpenMPT *m_instance;
};

#endif
