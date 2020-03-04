#include "decoderalacfactory.h"
#include "decoder_alac.h"

bool DecoderALACFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderALACFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("ALAC Plugin");
    properties.filters << "*.alac";
    properties.description = tr("ALAC Files");
    properties.shortName = "alac";
    properties.hasSettings = false;
    properties.noInput = true;
    properties.protocols << "file";
    return properties;
}

Decoder *DecoderALACFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderALAC(path);
}

QList<TrackInfo *> DecoderALACFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    QList <TrackInfo *> list;
//    CYmMusic *music = new CYmMusic;
//    if(music->load(path.toLocal8Bit().constData()))
//    {
//        if(parts & (TrackInfo::MetaData | TrackInfo::Properties))
//        {
//            TrackInfo *info = new TrackInfo(path);
//            ymMusicInfo_t musicInfo;
//            music->getMusicInfo(&musicInfo);

//            char* title = strdup(musicInfo.pSongName);
//            char* composer = strdup(musicInfo.pSongAuthor);
//            char* comment = strdup(musicInfo.pSongComment);

//            info->setValue(Qmmp::TITLE, QString::fromUtf8(title).trimmed());
//            info->setValue(Qmmp::COMPOSER, QString::fromUtf8(composer).trimmed());
//            info->setValue(Qmmp::COMMENT, QString::fromUtf8(comment).trimmed());
//            info->setDuration(musicInfo.musicTimeInSec);
//        }
//    }

//    delete music;
//    return list;
}

MetaDataModel* DecoderALACFactory::createMetaDataModel(const QString &, bool)
{
    return nullptr;
}
