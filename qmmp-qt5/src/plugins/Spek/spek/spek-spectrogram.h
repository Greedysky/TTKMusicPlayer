#ifndef SPEKSPECTROGRAM_H
#define SPEKSPECTROGRAM_H

#include <memory>
#include <qmmp/spekfactory.h>

#include "spek-palette.h"
#include "spek-pipeline.h"

class Audio;
class FFT;
class SpekHaveSampleEvent;
struct spek_pipeline;

class SpekSpectrogram : public Spek
{
    Q_OBJECT
public:
    explicit SpekSpectrogram(QWidget *parent = 0);
    ~SpekSpectrogram();

    virtual void open(const QString& path) override;
    virtual void start() override;
    virtual void stop() override;

    inline QImage *getPaintImage() { return &image; }
    inline int getURange() const { return urange; }
    inline int getLRange() const { return lrange; }
    Palette getPalette() const { return palette; }

private slots:
    void typeChanged(QAction *action);

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
