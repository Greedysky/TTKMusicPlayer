#include "musicaudiorecordercore.h"
#include "musiccoreutils.h"

MusicAudioRecorderCore::MusicAudioRecorderCore(QObject *parent)
    : QObject(parent)
{
    m_mpAudioInputFile = nullptr;
    m_mpAudioOutputFile = nullptr;

    m_mpOutputFile = new QFile(this);
    m_mpOutputFile->setFileName( RECORD_FILE );

    m_mFormatFile.setSampleSize(16);
    m_mFormatFile.setSampleType(QAudioFormat::SignedInt);
    m_mFormatFile.setByteOrder(QAudioFormat::LittleEndian);
    m_mFormatFile.setCodec("audio/pcm");

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if(!info.isFormatSupported(m_mFormatFile))
    {
        M_LOGGER_WARN("input default mFormatFile not supported try to use nearest");
        m_mFormatFile = info.nearestFormat(m_mFormatFile);
    }
    QAudioDeviceInfo info1(QAudioDeviceInfo::defaultOutputDevice());
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
    QFile::remove(RECORD_FILE);
    QFile::remove(RECORD_IN_FILE);
    QFile::remove(RECORD_OUT_FILE);

    delete m_mpOutputFile;
    delete m_mpAudioInputFile;
    delete m_mpAudioOutputFile;
}

QString MusicAudioRecorderCore::getClassName()
{
    return staticMetaObject.className();
}

int MusicAudioRecorderCore::addWavHeader(const char *filename)
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
    DestionFileHeader.nFMTLength = 16;
    DestionFileHeader.nAudioFormat = 1;

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
    fp_s = fopen(MusicUtils::Core::toLocal8Bit(m_mpOutputFile->fileName()), "rb");
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

void MusicAudioRecorderCore::setFileName(const QString &name)
{
    m_mpOutputFile->setFileName(name);
}

QString MusicAudioRecorderCore::getFileName() const
{
    return m_mpOutputFile->fileName();
}

void MusicAudioRecorderCore::onRecordStart()
{
    m_mpOutputFile->open(QIODevice::WriteOnly | QIODevice::Truncate);

    m_mpAudioInputFile = new QAudioInput(m_mFormatFile, this);
    m_mpAudioInputFile->start(m_mpOutputFile);
}

void MusicAudioRecorderCore::onRecordPlay()
{
    m_mpOutputFile->open(QIODevice::ReadOnly | QIODevice::Truncate);

    m_mpAudioOutputFile = new QAudioOutput(m_mFormatFile, this);
    connect(m_mpAudioOutputFile, SIGNAL(stateChanged(QAudio::State)), SLOT(onStateChange(QAudio::State)));
    m_mpAudioOutputFile->start(m_mpOutputFile);

}

void MusicAudioRecorderCore::onRecordStop()
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
}

void MusicAudioRecorderCore::onStateChange(QAudio::State state)
{
    if(state == QAudio::IdleState)
    {
        onRecordStop();
    }
}
