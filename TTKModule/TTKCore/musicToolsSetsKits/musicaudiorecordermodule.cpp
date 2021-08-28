#include "musicaudiorecordermodule.h"
#include "musiccodecutils.h"
#include "musicobject.h"

#define OPEN_FILE_ERROR     -1
#define SAVE_FILE_ERROR     -2
#define WRITE_FILE_ERROR    -3
#define REWRITE_FILE_ERROR  -4

MusicAudioRecorderModule::MusicAudioRecorderModule(QObject *parent)
    : QObject(parent)
{
    m_inputVolume = 0;
    m_audioInputFile = nullptr;
    m_audioOutputFile = nullptr;

    m_outputFile = new QFile(this);
    m_outputFile->setFileName(MUSIC_RECORD_FILE);
    m_formatFile.setChannelCount(1);
    m_formatFile.setSampleSize(16);
    m_formatFile.setSampleRate(8000);
    m_formatFile.setSampleType(QAudioFormat::SignedInt);
    m_formatFile.setByteOrder(QAudioFormat::LittleEndian);
    m_formatFile.setCodec("audio/pcm");

    const QAudioDeviceInfo input_info(QAudioDeviceInfo::defaultInputDevice());
    if(!input_info.isFormatSupported(m_formatFile))
    {
        TTK_LOGGER_WARN("input default format file not supported try to use nearest");
        m_formatFile = input_info.nearestFormat(m_formatFile);
    }

    const QAudioDeviceInfo output_info(QAudioDeviceInfo::defaultOutputDevice());
    if(!output_info.isFormatSupported(m_formatFile))
    {
        TTK_LOGGER_WARN("output default format file not supported - trying to use nearest");
        TTK_LOGGER_WARN("output no support input format file.");
    }

    if(m_formatFile.sampleSize() != 16)
    {
        TTK_LOGGER_INFO(QString("audio device doesn't support 16 bit support %d bit samples, example cannot run %1").arg(m_formatFile.sampleSize()));
    }
}

MusicAudioRecorderModule::~MusicAudioRecorderModule()
{
    QFile::remove(MUSIC_RECORD_FILE);
    QFile::remove(MUSIC_RECORD_IN_FILE);
    QFile::remove(MUSIC_RECORD_OUT_FILE);

    delete m_outputFile;
    delete m_audioInputFile;
    delete m_audioOutputFile;
}

int MusicAudioRecorderModule::addWavHeader(const char *fileName) const
{
    WAVHEADER fileHeader;
    fileHeader.RIFFNAME[0] = 'R';
    fileHeader.RIFFNAME[1] = 'I';
    fileHeader.RIFFNAME[2] = 'F';
    fileHeader.RIFFNAME[3] = 'F';

    fileHeader.WAVNAME[0] = 'W';
    fileHeader.WAVNAME[1] = 'A';
    fileHeader.WAVNAME[2] = 'V';
    fileHeader.WAVNAME[3] = 'E';

    fileHeader.FMTNAME[0] = 'f';
    fileHeader.FMTNAME[1] = 'm';
    fileHeader.FMTNAME[2] = 't';
    fileHeader.FMTNAME[3] = 0x20;
    fileHeader.FMTLENGTH = 16;
    fileHeader.AUDIOFORMAT = 1;

    fileHeader.DATANAME[0] = 'd';
    fileHeader.DATANAME[1] = 'a';
    fileHeader.DATANAME[2] = 't';
    fileHeader.DATANAME[3] = 'a';
    fileHeader.BITSPERSAMPLE = 16;
    fileHeader.BYTESPERSAMPLE = 2;
    fileHeader.SAMPLERATE = 8000;
    fileHeader.BYTESPERSECOND = 16000;
    fileHeader.CHANNLENUMBER = 1;

    int fileLen = 0;
    const int headerSize = sizeof(fileHeader);

    FILE *input = nullptr;
    FILE *output = nullptr;
    if((input = fopen(qPrintable(m_outputFile->fileName()), "rb")) == nullptr)
    {
        return OPEN_FILE_ERROR;
    }

    if((output = fopen(qPrintable(fileName), "wb+")) == nullptr)
    {
        return SAVE_FILE_ERROR;
    }

    if(fwrite(&fileHeader, 1, headerSize, output) != headerSize)
    {
        fclose(input);
        fclose(output);
        return WRITE_FILE_ERROR;
    }

    while(!feof(input))
    {
        char readBuf[4096];
        const int nRead = fread(readBuf, 1, 4096, input);
        if(nRead > 0)
        {
            fwrite(readBuf, 1, nRead, output);
        }

        fileLen += nRead;
    }

    fseek(output, 0L, SEEK_SET);
    fileHeader.RIFFLENGTH = fileLen - 8 + headerSize;
    fileHeader.DATALENGTH = fileLen;

    if(fwrite(&fileHeader, 1, headerSize, output) != headerSize)
    {
        fclose(input);
        fclose(output);
        return REWRITE_FILE_ERROR;
    }

    fclose(input);
    fclose(output);

    return fileLen;
}

void MusicAudioRecorderModule::setVolume(int volume)
{
    m_inputVolume = volume;
#if TTK_QT_VERSION_CHECK(5,0,0)
    if(m_audioInputFile)
    {
        m_audioInputFile->setVolume(volume);
    }
#endif
}

int MusicAudioRecorderModule::volume() const
{
    return m_inputVolume;
}

void MusicAudioRecorderModule::setFileName(const QString &name)
{
    m_outputFile->setFileName(name);
}

QString MusicAudioRecorderModule::getFileName() const
{
    return m_outputFile->fileName();
}

bool MusicAudioRecorderModule::error() const
{
    if(!m_audioInputFile)
    {
        return true;
    }

    return m_audioInputFile->error() != QAudio::NoError;
}

void MusicAudioRecorderModule::onRecordStart()
{
    if(!m_outputFile->isOpen())
    {
        m_outputFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
        m_audioInputFile = new QAudioInput(m_formatFile, this);
    }

    if(m_audioInputFile->error() != QAudio::NoError)
    {
        TTK_LOGGER_ERROR("audio input open error");
        return;
    }
#if TTK_QT_VERSION_CHECK(5,0,0)
    m_audioInputFile->setVolume(m_inputVolume);
#endif
    m_audioInputFile->start(m_outputFile);
}

void MusicAudioRecorderModule::onRecordStop()
{
    if(m_audioInputFile)
    {
        m_audioInputFile->stop();
        delete m_audioInputFile;
        m_audioInputFile = nullptr;
    }

    if(m_audioOutputFile)
    {
        m_audioOutputFile->stop();
        delete m_audioOutputFile;
        m_audioOutputFile = nullptr;
    }

    m_outputFile->close();
}
