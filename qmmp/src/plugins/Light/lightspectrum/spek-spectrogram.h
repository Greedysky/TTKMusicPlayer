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

#ifndef LIGHTSPECTRUM_H
#define LIGHTSPECTRUM_H

#include <memory>
#include <qmmp/lightfactory.h>
#include <qmmp/visualpalette.h>

#include "spek-pipeline.h"

class Audio;
class FFT;
class SpekHaveSampleEvent;
struct spek_pipeline;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class LightSpectrum : public Light
{
    Q_OBJECT
public:
    explicit LightSpectrum(QWidget *parent = nullptr);
    virtual ~LightSpectrum();

    virtual void open(const QString &path) override;
    virtual void start() override;
    virtual void stop() override;

    inline QImage *getPaintImage() { return &m_image; }
    inline int getURange() const { return m_urange; }
    inline int getLRange() const { return m_lrange; }
    inline VisualPalette::Palette getPalette() const { return m_palette; }

private slots:
    void typeChanged(QAction *action);
    void mediaUrlChanged();

private:
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void resizeEvent(QResizeEvent *e) override;
    virtual void contextMenuEvent(QContextMenuEvent *e) override;

    void paint(QPainter *dc);
    void create_palette();

    std::unique_ptr<Audio> m_audio;
    std::unique_ptr<FFT> m_fft;
    spek_pipeline *m_pipeline = nullptr;
    int m_streams = 0;
    int m_stream = 0;
    int m_channels = 0;
    int m_channel = 0;
    enum WindowFunction m_window_function = WINDOW_DEFAULT;
    QString m_path;
    QString m_desc;
    double m_duration = 0.0;
    int m_sample_rate = 0;
    VisualPalette::Palette m_palette = VisualPalette::PALETTE_SPECTRUM;
    QImage m_palette_image;
    QImage m_image;
    int m_prev_width = -1;
    int m_fft_bits;
    int m_urange;
    int m_lrange;

};

#endif
