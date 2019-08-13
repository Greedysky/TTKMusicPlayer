#include "musicaudiorecordercore.h"
#include "musicmessagebox.h"
#include "musiccodecutils.h"
#include "musicobject.h"

#define OPEN_FILE_ERROR     -1
#define SAVE_FILE_ERROR     -2
#define WRITE_FILE_ERROR    -3
#define REWRITE_FILE_ERROR  -4

MusicAudioRecorderCore::MusicAudioRecorderCore(QObject *parent)
    : QObject(parent)
{
    m_inputVolume = 0;

    m_mpAudioInputFile = nullptr;
    m_mpAudioOutputFile = nullptr;

    m_mpOutputFile = new QFile(this);
    m_mpOutputFile->setFileName( MUSIC_RECORD_FILE );

    m_mFormatFile.setSampleSize(16);
    m_mFormatFile.setSampleType(QAudioFormat::SignedInt);
    m_mFormatFile.setByteOrder(QAudioFormat::LittleEndian);
    m_mFormatFile.setCodec("audio/pcm");

    const QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if(!info.isFormatSupported(m_mFormatFile))
    {
        M_LOGGER_WARN("input default mFormatFile not supported try to use nearest");
        m_mFormatFile = info.nearestFormat(m_mFormatFile);
    }

    const QAudioDeviceInfo info1(QAudioDeviceInfo::defaultOutputDevice());
    if(!info1.isFormatSupported(m_mFormatFile))
    {
        M_LOGGER_WARN("output default mFormatFile not supported - trying to use nearest");
        M_LOGGER_WARN("output no support input mFormatFile.");
    }

    if(m_mFormatFile.sampleSize() != 16)
    {
        M_LOGGER_INFO(QString("audio device doesn't support 16 bit support %d bit samples, example cannot run %1")
                      .arg(m_mFormatFile.sampleSize()));
    }
}

MusicAudioRecorderCore::~MusicAudioRecorderCore()
{
    QFile::remove(MUSIC_RECORD_FILE);
    QFile::remove(MUSIC_RECORD_IN_FILE);
    QFile::remove(MUSIC_RECORD_OUT_FILE);

    delete m_mpOutputFile;
    delete m_mpAudioInputFile;
    delete m_mpAudioOutputFile;
}

int MusicAudioRecorderCore::addWavHeader(const char *filename)
{
    HEADER destionFileHeader;
    destionFileHeader.RIFFNAME[0] = 'R';
    destionFileHeader.RIFFNAME[1] = 'I';
    destionFileHeader.RIFFNAME[2] = 'F';
    destionFileHeader.RIFFNAME[3] = 'F';

    destionFileHeader.WAVNAME[0] = 'W';
    destionFileHeader.WAVNAME[1] = 'A';
    destionFileHeader.WAVNAME[2] = 'V';
    destionFileHeader.WAVNAME[3] = 'E';

    destionFileHeader.FMTNAME[0] = 'f';
    destionFileHeader.FMTNAME[1] = 'm';
    destionFileHeader.FMTNAME[2] = 't';
    destionFileHeader.FMTNAME[3] = 0x20;
    destionFileHeader.nFMTLength = 16;
    destionFileHeader.nAudioFormat = 1;

    destionFileHeader.DATANAME[0] = 'd';
    destionFileHeader.DATANAME[1] = 'a';
    destionFileHeader.DATANAME[2] = 't';
    destionFileHeader.DATANAME[3] = 'a';
    destionFileHeader.nBitsPerSample = 16;
    destionFileHeader.nBytesPerSample = 2;
    destionFileHeader.nSampleRate = 8000;
    destionFileHeader.nBytesPerSecond = 16000;
    destionFileHeader.nChannleNumber = 1;

    int nFileLen = 0;
    int nSize = sizeof(destionFileHeader);

    FILE *fpInput = nullptr;
    FILE *fpOutput = nullptr;
    if((fpInput = fopen(MusicUtils::Codec::toLocal8Bit(m_mpOutputFile->fileName()), "rb")) == nullptr)
    {
        return OPEN_FILE_ERROR;
    }

    if((fpOutput = fopen(MusicUtils::Codec::toLocal8Bit(filename), "wb+")) == nullptr)
    {
        return SAVE_FILE_ERROR;
    }

    int nWrite = fwrite(&destionFileHeader, 1, nSize, fpOutput);
    if(nWrite != nSize)
    {
        fclose(fpInput);
        fclose(fpOutput);
        return WRITE_FILE_ERROR;
    }

    while(!feof(fpInput))
    {
        char readBuf[4096];
        const int nRead = fread(readBuf, 1, 4096, fpInput);
        if(nRead > 0)
        {
            fwrite(readBuf, 1, nRead, fpOutput);
        }

        nFileLen += nRead;
    }

    fseek(fpOutput, 0L, SEEK_SET);
    destionFileHeader.nRIFFLength = nFileLen - 8 + nSize;
    destionFileHeader.nDataLength = nFileLen;

    nWrite = fwrite(&destionFileHeader, 1, nSize, fpOutput);
    if(nWrite != nSize)
    {
        fclose(fpInput);
        fclose(fpOutput);
        return REWRITE_FILE_ERROR;
    }

    fclose(fpInput);
    fclose(fpOutput);

    return nFileLen;
}

void MusicAudioRecorderCore::setVolume(int volume)
{
    m_inputVolume = volume;
#ifdef TTK_GREATER_NEW
    if(m_mpAudioInputFile)
    {
        m_mpAudioInputFile->setVolume(volume);
    }
#endif
}

int MusicAudioRecorderCore::volume() const
{
    return m_inputVolume;
}

void MusicAudioRecorderCore::setFileName(const QString &name)
{
    m_mpOutputFile->setFileName(name);
}

QString MusicAudioRecorderCore::getFileName() const
{
    return m_mpOutputFile->fileName();
}

bool MusicAudioRecorderCore::error() const
{
    if(!m_mpAudioInputFile)
    {
        return true;
    }

    return (m_mpAudioInputFile->error() != QAudio::NoError);
}

void MusicAudioRecorderCore::onRecordStart()
{
    if(!m_mpOutputFile->isOpen())
    {
        m_mpOutputFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
        m_mpAudioInputFile = new QAudioInput(m_mFormatFile, this);
    }

    if(m_mpAudioInputFile->error() != QAudio::NoError)
    {
        MusicMessageBox message;
        message.setText(tr("Audio Input Open Error"));
        message.exec();
        return;
    }
#ifdef TTK_GREATER_NEW
    m_mpAudioInputFile->setVolume(m_inputVolume);
#endif
    m_mpAudioInputFile->start(m_mpOutputFile);
}

void MusicAudioRecorderCore::onRecordPlay()
{
    m_mpOutputFile->open(QIODevice::ReadOnly | QIODevice::Truncate);

    m_mpAudioOutputFile = new QAudioOutput(m_mFormatFile, this);
    if(m_mpAudioOutputFile->error() != QAudio::NoError)
    {
        MusicMessageBox message;
        message.setText(tr("Audio Output Open Error"));
        message.exec();
        return;
    }
    connect(m_mpAudioOutputFile, SIGNAL(stateChanged(QAudio::State)), SLOT(onStateChange(QAudio::State)));
    m_mpAudioOutputFile->start(m_mpOutputFile);

}

void MusicAudioRecorderCore::onRecordStop()
{
    if(m_mpAudioInputFile)
    {
        m_mpAudioInputFile->stop();
        delete m_mpAudioInputFile;
        m_mpAudioInputFile = nullptr;
    }

    if(m_mpAudioOutputFile)
    {
        m_mpAudioOutputFile->stop();
        delete m_mpAudioOutputFile;
        m_mpAudioOutputFile = nullptr;
    }

    m_mpOutputFile->close();
}

void MusicAudioRecorderCore::onStateChange(QAudio::State state)
{
    if(state == QAudio::IdleState)
    {
        onRecordStop();
    }
}
