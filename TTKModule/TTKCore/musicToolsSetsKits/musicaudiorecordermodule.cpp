#include "musicaudiorecordermodule.h"

#define OPEN_FILE_ERROR     -1
#define SAVE_FILE_ERROR     -2
#define WRITE_FILE_ERROR    -3
#define REWRITE_FILE_ERROR  -4

struct WavHeader
{
    char riffName[4];
    ulong riffLength;
    char wavName[4];
    char fmtName[4];
    ulong fmtLength;
    ushort audioFormat;
    ushort channleNumber;
    ulong sampleRate;
    ulong bytesPperSecond;
    ushort bytesPerSample;
    ushort bitsPerSample;
    char dataName[4];
    ulong dataLength;
};


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
        TTK_LOGGER_WARN("Input default format file not supported try to use nearest");
        m_formatFile = input_info.nearestFormat(m_formatFile);
    }

    const QAudioDeviceInfo output_info(QAudioDeviceInfo::defaultOutputDevice());
    if(!output_info.isFormatSupported(m_formatFile))
    {
        TTK_LOGGER_WARN("Output default format file not supported - trying to use nearest");
        TTK_LOGGER_WARN("Output no support input format file.");
    }

    if(m_formatFile.sampleSize() != 16)
    {
        TTK_LOGGER_INFO(QString("Audio device doesn't support 16 bit support %d bit samples, example cannot run %1").arg(m_formatFile.sampleSize()));
    }
}

MusicAudioRecorderModule::~MusicAudioRecorderModule()
{
    QFile::remove(MUSIC_RECORD_FILE);
    QFile::remove(MUSIC_RECORD_DATA_FILE);

    delete m_outputFile;
    delete m_audioInputFile;
    delete m_audioOutputFile;
}

int MusicAudioRecorderModule::addWavHeader(const char *fileName) const
{
    WavHeader fileHeader;
    fileHeader.riffName[0] = 'R';
    fileHeader.riffName[1] = 'I';
    fileHeader.riffName[2] = 'F';
    fileHeader.riffName[3] = 'F';

    fileHeader.wavName[0] = 'W';
    fileHeader.wavName[1] = 'A';
    fileHeader.wavName[2] = 'V';
    fileHeader.wavName[3] = 'E';

    fileHeader.fmtName[0] = 'f';
    fileHeader.fmtName[1] = 'm';
    fileHeader.fmtName[2] = 't';
    fileHeader.fmtName[3] = 0x20;
    fileHeader.fmtLength = 16;
    fileHeader.audioFormat = 1;

    fileHeader.dataName[0] = 'd';
    fileHeader.dataName[1] = 'a';
    fileHeader.dataName[2] = 't';
    fileHeader.dataName[3] = 'a';
    fileHeader.bitsPerSample = 16;
    fileHeader.bytesPerSample = 2;
    fileHeader.sampleRate = 8000;
    fileHeader.bytesPperSecond = 16000;
    fileHeader.channleNumber = 1;

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
    fileHeader.riffLength = fileLen - 8 + headerSize;
    fileHeader.dataLength = fileLen;

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

QString MusicAudioRecorderModule::fileName() const
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
        TTK_LOGGER_ERROR("Audio input open error");
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
