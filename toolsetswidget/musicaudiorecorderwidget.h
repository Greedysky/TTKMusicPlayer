#ifndef MUSICAUDIORECORDERWIDGET_H
#define MUSICAUDIORECORDERWIDGET_H

#include <QDialog>
#include <QMouseEvent>
#include <QTimer>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include "../core/musicobject.h"
#include "../core/musicmovedialogabstract.h"

#define BufferSize 14096

class QMovie;

namespace Ui {
class MusicAudioRecorderWidget;
}

class MUSIC_EXPORT MusicAudioRecorderWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT
struct HEADER
{
    char RIFFNAME[4];
    unsigned long nRIFFLength;
    char WAVNAME[4];
    char FMTNAME[4];
    unsigned long nFMTLength;
    unsigned short nAudioFormat;
    unsigned short nChannleNumber;
    unsigned long nSampleRate;
    unsigned long nBytesPerSecond;
    unsigned short nBytesPerSample;
    unsigned short    nBitsPerSample;
    char    DATANAME[4];
    unsigned long   nDataLength;
};

public:
    explicit MusicAudioRecorderWidget(QWidget *parent = 0);
    ~MusicAudioRecorderWidget();

signals:

public slots:
    void onRecordStart();
    void onRecordStop();
    void onRecordPlay();
    void onRecordSave();
    void onStateChange(QAudio::State s);
    void onReadMore();
    void onSliderValueChanged(int);
    void onTimeOut();
    void onTimerout();

protected:
    int addWavHeader(char *);
    int applyVolumeToSample(short iSample);
    void initMonitor();
    void createAudioInput();
    void createAudioOutput();

    Ui::MusicAudioRecorderWidget *ui;
    QTimer m_timer;
    qint64 m_time;
    QMovie *m_movie;

    int m_miVolume;
    int m_miMaxValue;
    QAudioFormat m_mFormatFile;
    QFile* m_mpOutputFile;
    QAudioInput* m_mpAudioInputFile;
    QAudioOutput* m_mpAudioOutputFile;
    QAudioFormat m_mFormatSound;
    QAudioInput* m_mpAudioInputSound;
    QAudioOutput* m_mpAudioOutputSound;
    QIODevice* m_mpInputDevSound;
    QIODevice* m_mpOutputDevSound;
    QByteArray m_mBuffer;

};

#endif // MUSICAUDIORECORDERWIDGET_H
