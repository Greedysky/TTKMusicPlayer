#include "decoderymfactory.h"
#include "decoder_ym.h"

bool DecoderYmFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderYmFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("YM Plugin");
    properties.filters << "*.ym";
    properties.description = tr("ST-Sound, ATARI-ST Audio Files");
    properties.shortName = "ym";
    properties.noInput = true;
    properties.protocols << "file";
    return properties;
}

Decoder *DecoderYmFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderYm(path);
}

QList<TrackInfo*> DecoderYmFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    CYmMusic *music = new CYmMusic;
    if(!music->load(path.toLocal8Bit().constData()))
    {
        delete info;
        return QList<TrackInfo*>();
    }

    ymMusicInfo_t musicInfo;
    music->getMusicInfo(&musicInfo);

    if(parts & TrackInfo::MetaData)
    {
        char* title = strdup(musicInfo.pSongName);
        char* composer = strdup(musicInfo.pSongAuthor);
        char* comment = strdup(musicInfo.pSongComment);

        info->setValue(Qmmp::TITLE, QString::fromUtf8(title).trimmed());
        info->setValue(Qmmp::COMPOSER, QString::fromUtf8(composer).trimmed());
        info->setValue(Qmmp::COMMENT, QString::fromUtf8(comment).trimmed());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setDuration(musicInfo.musicTimeInMs);
        info->setValue(Qmmp::FORMAT_NAME, "ym");
    }

    delete music;
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderYmFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(ym, DecoderYmFactory)
#endif
