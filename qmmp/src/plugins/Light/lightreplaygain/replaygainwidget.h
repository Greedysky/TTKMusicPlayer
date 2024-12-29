/***************************************************************************
 *   Copyright (C) 2013-2025 by Ilya Kotov                                 *
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

#ifndef REPLAYGAINWIDGET_H
#define REPLAYGAINWIDGET_H

#include <taglib/tstring.h>
#include <taglib/tstringlist.h>
#include <qmmp/lightfactory.h>
#include "ui_replaygainwidget.h"

class ReplayGainner;

struct ReplayGainInfoItem;

namespace TagLib
{
    namespace ID3v2
    {
        class Tag;
    }
    namespace APE
    {
        class Tag;
    }
    namespace Ogg
    {
        class XiphComment;
    }
    namespace MP4
    {
        class Tag;
    }
}

/*!
 * @author Greedysky <greedysky@163.com>
 */
class ReplayGainWidget : public Light
{
    Q_OBJECT
public:
    explicit ReplayGainWidget(QWidget *parent = nullptr);
    virtual ~ReplayGainWidget();

private slots:
    void calculateButtonClicked();
    void scanFinished(const QString &url);
    void writeButtonClicked();

private:
    virtual void open(const QString &path) override final;
    virtual void start() override final;
    virtual void stop() override final;

    ReplayGainner *findScannerByUrl(const QString &url);
    QString albumName(const QString &url);

    TagLib::String gainToString(double value);
    TagLib::String peakToString(double value);
    TagLib::StringList gainToStringList(double value);
    TagLib::StringList peakToStringList(double value);

    void writeAPETag(TagLib::APE::Tag *tag, ReplayGainInfoItem *item);
    void writeID3v2Tag(TagLib::ID3v2::Tag *tag, ReplayGainInfoItem *item);
    void writeVorbisComment(TagLib::Ogg::XiphComment *tag, ReplayGainInfoItem *item);
    void writeMP4Tag(TagLib::MP4::Tag *tag, ReplayGainInfoItem *item);

    int m_scannerIndex;
    Ui::ReplayGainWidget m_ui;
    QList<ReplayGainner *> m_scanners;
    QList<ReplayGainInfoItem*> m_replayGainItemList;

};

#endif
