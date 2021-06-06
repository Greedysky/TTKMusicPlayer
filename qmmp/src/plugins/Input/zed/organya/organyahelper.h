/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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
 ================================================= */

#ifndef ORGANYAHELPER_H
#define ORGANYAHELPER_H

#include <liborganya/decoder.h>
#include <liborganya/organya.h>

#include <QMap>
#include <QFile>
#include <qmmp/qmmp.h>

typedef struct {
    org_decoder_t *input;
    int bitrate;
} organya_info;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class OrganyaHelper
{
public:
    explicit OrganyaHelper(const QString &path);
    ~OrganyaHelper();

    void deinit();

    bool initialize();
    int totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int sampleRate() const;
    int channels() const;
    int bitsPerSample() const;

    int read(unsigned char *buf, int size);

private:
    QString m_path;
    organya_info *m_info;

};

#endif
