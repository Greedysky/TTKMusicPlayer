/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#ifndef SPECTRUMSPEK_H
#define SPECTRUMSPEK_H

#include <memory>
#include <qmmp/spekfactory.h>

#include "spek-palette.h"
#include "spek-pipeline.h"

class Audio;
class FFT;
class SpekHaveSampleEvent;
struct spek_pipeline;

class SpectrumSpek : public Spek
{
    Q_OBJECT
public:
    explicit SpectrumSpek(QWidget *parent = 0);
    ~SpectrumSpek();

    virtual void open(const QString &path) override;
    virtual void start() override;
    virtual void stop() override;

    inline QImage *getPaintImage() { return &image; }
    inline int getURange() const { return urange; }
    inline int getLRange() const { return lrange; }
    Palette getPalette() const { return palette; }

private slots:
    void typeChanged(QAction *action);
    void urlChanged();

private:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    void paint(QPainter *dc);
    void create_palette();

    std::unique_ptr<Audio> audio;
    std::unique_ptr<FFT> fft;
    spek_pipeline *pipeline;
    int streams;
    int stream;
    int channels;
    int channel;
    enum window_function window_function;
    QString path;
    QString desc;
    double duration;
    int sample_rate;
    Palette palette;
    QImage palette_image;
    QImage image;
    int prev_width;
    int fft_bits;
    int urange;
    int lrange;


};

#endif
