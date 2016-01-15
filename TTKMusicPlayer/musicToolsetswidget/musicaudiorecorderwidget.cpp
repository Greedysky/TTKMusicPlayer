#include "musicaudiorecorderwidget.h"
#include "ui_musicaudiorecorderwidget.h"
#include "musictime.h"
#include "musicbgthememanager.h"
#include "musicmessagebox.h"

#include <QMovie>
#include <QFileDialog>

MusicAudioRecorderWidget::MusicAudioRecorderWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicAudioRecorderWidget), m_mBuffer(BufferSize, 0)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    ui->startButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->startButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->startButton->setToolTip(tr("startAudio"));
    ui->stopButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->stopButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->stopButton->setToolTip(tr("stopAudio"));
    ui->playButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->playButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->playButton->setToolTip(tr("playAudio"));
    ui->saveButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->saveButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->saveButton->setToolTip(tr("saveButton"));

    connect(ui->startButton, SIGNAL(clicked()), SLOT(onRecordStart()));
    connect(ui->stopButton, SIGNAL(clicked()), SLOT(onRecordStop()));
    connect(ui->playButton, SIGNAL(clicked()), SLOT(onRecordPlay()));
    connect(ui->saveButton, SIGNAL(clicked()), SLOT(onRecordSave()));

    ui->checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->checkBox_2->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->checkBox_3->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->checkBox_4->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->checkBox_5->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->checkBox->setCursor(QCursor(Qt::PointingHandCursor));
    ui->checkBox_2->setCursor(QCursor(Qt::PointingHandCursor));
    ui->checkBox_3->setCursor(QCursor(Qt::PointingHandCursor));
    ui->checkBox_4->setCursor(QCursor(Qt::PointingHandCursor));
    ui->checkBox_5->setCursor(QCursor(Qt::PointingHandCursor));

    connect(&m_timer, SIGNAL(timeout()), SLOT(onTimerout()));
    ui->stopButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    m_movie = nullptr;
    m_time = 0;

    m_miMaxValue = 0;
    ui->horizontalSlider->setValue(100);
    m_miVolume = 100;
    m_mpOutputFile = nullptr;
    m_mpAudioInputFile = nullptr;
    m_mpAudioOutputFile = nullptr;

    m_mpAudioInputSound = nullptr;
    m_mpAudioOutputSound = nullptr;

    m_mpInputDevSound = nullptr;
    m_mpInputDevSound = nullptr;

    m_mpOutputFile = new QFile(this);
    m_mpOutputFile->setFileName("record.raw");

    m_mFormatFile.setSampleSize(16);
    m_mFormatFile.setSampleType(QAudioFormat::SignedInt);
    m_mFormatFile.setByteOrder(QAudioFormat::LittleEndian);
    m_mFormatFile.setCodec("audio/pcm");

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if(!info.isFormatSupported(m_mFormatFile))
    {
        M_LOGGERS("input default mFormatFile not supported try to use nearest");
        m_mFormatFile = info.nearestFormat(m_mFormatFile);
    }
    QAudioDeviceInfo info1(QAudioDeviceInfo::defaultOutputDevice());
    if(!info1.isFormatSupported(m_mFormatFile))
    {
        M_LOGGERS("output default mFormatFile not supported - trying to use nearest");
        //           mFormatFile = info.nearestFormat(mFormatSound);
        M_LOGGERS("output no support input mFormatFile.");
        return;
    }
    if(m_mFormatFile.sampleSize() != 16)
    {
        M_LOGGER << "audio device doesn't support 16 bit support %d bit samples, example cannot run"
                 << m_mFormatFile.sampleSize() << LOG_END;
        m_mpAudioInputFile = 0;
        return;
    }

    m_mpAudioInputFile = nullptr;
    m_mpAudioOutputFile = nullptr;

    initMonitor();
}

MusicAudioRecorderWidget::~MusicAudioRecorderWidget()
{
    delete m_mpOutputFile;
    delete m_movie;
    delete ui;
}

void MusicAudioRecorderWidget::onTimerout()
{
    QString text = MusicTime::normalTime2Label( ++m_time );
    ui->timer->setText( text );
}

void MusicAudioRecorderWidget::initMonitor()
{
    m_mFormatSound.setSampleSize(16); //set sample sze to 16 bit
    m_mFormatSound.setSampleType(QAudioFormat::UnSignedInt ); //Sample type as usigned integer sample
    m_mFormatSound.setByteOrder(QAudioFormat::LittleEndian); //Byte order
    m_mFormatSound.setCodec("audio/pcm"); //set codec as simple audio/pcm

    QAudioDeviceInfo infoIn(QAudioDeviceInfo::defaultInputDevice());
    if (!infoIn.isFormatSupported(m_mFormatSound))
    {
        //Default format not supported - trying to use nearest
        m_mFormatSound = infoIn.nearestFormat(m_mFormatSound);
    }

    QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());
    if (!infoOut.isFormatSupported(m_mFormatSound))
    {
        //Default format not supported - trying to use nearest
        m_mFormatSound = infoOut.nearestFormat(m_mFormatSound);
    }

    createAudioInput();
    createAudioOutput();

    m_mpOutputDevSound = m_mpAudioOutputSound->start();
    m_mpInputDevSound = m_mpAudioInputSound->start();
    connect(m_mpInputDevSound, SIGNAL(readyRead()), SLOT(onReadMore()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), SLOT(onSliderValueChanged(int)));
}

void MusicAudioRecorderWidget::onRecordStart()
{
    m_mpOutputFile->open(QIODevice::WriteOnly | QIODevice::Truncate);

    m_mpAudioInputFile = new QAudioInput(m_mFormatFile, this);
    m_mpAudioInputFile->start(m_mpOutputFile);

    ui->stopButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    m_timer.start(1000);

    if(m_movie == nullptr)
    {
        m_movie = new QMovie(":/image/rate", QByteArray(), this);
    }
    ui->timer_2->setMovie(m_movie);
    m_movie->start();
}

void MusicAudioRecorderWidget::onRecordPlay()
{
    m_mpOutputFile->open(QIODevice::ReadOnly | QIODevice::Truncate);

    m_mpAudioOutputFile = new QAudioOutput(m_mFormatFile, this);
    connect(m_mpAudioOutputFile, SIGNAL(stateChanged(QAudio::State)), SLOT(onStateChange(QAudio::State)));
    m_mpAudioOutputFile->start(m_mpOutputFile);

    m_movie->start();
}

void MusicAudioRecorderWidget::onRecordStop()
{
    if(m_mpAudioInputFile != nullptr)
    {
        m_mpAudioInputFile->stop();
        delete m_mpAudioInputFile;
        m_mpAudioInputFile = nullptr;
    }

    if(m_mpAudioOutputFile != nullptr)
    {
        m_mpAudioOutputFile->stop();
        delete m_mpAudioOutputFile;
        m_mpAudioOutputFile = nullptr;
    }

    m_mpOutputFile->close();

    m_timer.stop();
    m_movie->stop();
    m_time = 0;
    ui->playButton->setEnabled(true);
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->saveButton->setEnabled(true);
}

void MusicAudioRecorderWidget::onRecordSave()
{
    QString filename = QFileDialog::getSaveFileName( this,
        tr("choose a filename to save under"), QDir::currentPath(), "Wav(*.wav)");
    if(!filename.isEmpty())
    {
        addWavHeader(filename.toLatin1().data());
    }
}

void MusicAudioRecorderWidget::onStateChange(QAudio::State state)
{
    if(state == QAudio::IdleState)
    {
        onRecordStop();
    }
}

int MusicAudioRecorderWidget::addWavHeader(char *filename)
{
    HEADER DestionFileHeader;
    DestionFileHeader.RIFFNAME[0] = 'R';
    DestionFileHeader.RIFFNAME[1] = 'I';
    DestionFileHeader.RIFFNAME[2] = 'F';
    DestionFileHeader.RIFFNAME[3] = 'F';

    DestionFileHeader.WAVNAME[0] = 'W';
    DestionFileHeader.WAVNAME[1] = 'A';
    DestionFileHeader.WAVNAME[2] = 'V';
    DestionFileHeader.WAVNAME[3] = 'E';

    DestionFileHeader.FMTNAME[0] = 'f';
    DestionFileHeader.FMTNAME[1] = 'm';
    DestionFileHeader.FMTNAME[2] = 't';
    DestionFileHeader.FMTNAME[3] = 0x20;
    DestionFileHeader.nFMTLength = 16;  //
    DestionFileHeader.nAudioFormat = 1; //

    DestionFileHeader.DATANAME[0] = 'd';
    DestionFileHeader.DATANAME[1] = 'a';
    DestionFileHeader.DATANAME[2] = 't';
    DestionFileHeader.DATANAME[3] = 'a';
    DestionFileHeader.nBitsPerSample = 16;
    DestionFileHeader.nBytesPerSample = 2;
    DestionFileHeader.nSampleRate = 8000;
    DestionFileHeader.nBytesPerSecond = 16000;
    DestionFileHeader.nChannleNumber = 1;

    int nFileLen = 0;
    int nSize = sizeof(DestionFileHeader);

    FILE *fp_s = nullptr;
    FILE *fp_d = nullptr;
    fp_s = fopen("record.raw", "rb");
    if (fp_s == nullptr)
    {
        return -1;
    }
    fp_d = fopen(filename, "wb+");
    if (fp_d == nullptr)
    {
        return -2;
    }
    int nWrite = fwrite(&DestionFileHeader, 1, nSize, fp_d);
    if (nWrite != nSize)
    {
        fclose(fp_s);
        fclose(fp_d);
        return -3;
    }
    while( !feof(fp_s))
    {
        char readBuf[4096];
        int nRead = fread(readBuf, 1, 4096, fp_s);
        if (nRead > 0)
        {
            fwrite(readBuf, 1, nRead, fp_d);
        }

        nFileLen += nRead;
    }
    fseek(fp_d, 0L, SEEK_SET);
    DestionFileHeader.nRIFFLength = nFileLen - 8 + nSize;
    DestionFileHeader.nDataLength = nFileLen;
    nWrite = fwrite(&DestionFileHeader, 1, nSize, fp_d);
    if (nWrite != nSize)
    {
        fclose(fp_s);
        fclose(fp_d);
        return -4;
    }
    fclose(fp_s);
    fclose(fp_d);
    return nFileLen;
}

void MusicAudioRecorderWidget::createAudioInput()
{
    if (m_mpInputDevSound != 0)
    {
        disconnect(m_mpInputDevSound, 0, this, 0);
        m_mpInputDevSound = 0;
    }
    QAudioDeviceInfo inputDevice(QAudioDeviceInfo::defaultInputDevice());
    m_mpAudioInputSound = new QAudioInput(inputDevice, m_mFormatSound, this);
}

void MusicAudioRecorderWidget::createAudioOutput()
{
    QAudioDeviceInfo outputDevice(QAudioDeviceInfo::defaultOutputDevice());
    m_mpAudioOutputSound = new QAudioOutput(outputDevice, m_mFormatSound, this);
}

int MusicAudioRecorderWidget::applyVolumeToSample(short iSample)
{
    //Calculate volume, Volume limited to  max 30000 and min -30000
    return std::max(std::min(((iSample * m_miVolume) / 50), 30000), -30000);
}

void MusicAudioRecorderWidget::onSliderValueChanged(int value)
{
    m_miVolume = value;
}

void MusicAudioRecorderWidget::onReadMore()
{
    //Return if audio input is nullptr
    if(!m_mpAudioInputSound)
    {
        return;
    }
    //Check the number of samples in input buffer
    qint64 len = m_mpAudioInputSound->bytesReady();
    //Limit sample size
    if(len > 4096)
    {
        len = 4096;
    }
    //Read sound samples from input device to buffer
    qint64 l = m_mpInputDevSound->read(m_mBuffer.data(), len);
    if(l > 0)
    {
        //Assign sound samples to short array
        short* resultingData = (short*)m_mBuffer.data();
        short *outdata=resultingData;
        outdata[ 0 ] = resultingData [ 0 ];
        int iIndex;
        if(false)
        {
            //Remove noise using Low Pass filter algortm[Simple algorithm used to remove noise]
            for ( iIndex=1; iIndex < len; iIndex++ )
            {
                outdata[ iIndex ] = 0.333 * resultingData[iIndex ] + ( 1.0 - 0.333 ) * outdata[ iIndex-1 ];
            }
        }
        m_miMaxValue = 0;
        for ( iIndex=0; iIndex < len; iIndex++ )
        {
            //Cange volume to each integer data in a sample
            int value = applyVolumeToSample( outdata[ iIndex ]);
            outdata[ iIndex ] = value;
            m_miMaxValue = m_miMaxValue >= value ? m_miMaxValue : value;
        }
        //write modified sond sample to outputdevice for playback audio
        m_mpOutputDevSound->write((char*)outdata, len);
        QTimer::singleShot(1000, this, SLOT(onTimeOut()));
    }
}

void MusicAudioRecorderWidget::onTimeOut()
{
    ui->progress->setValue(m_miMaxValue);
}

int MusicAudioRecorderWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
