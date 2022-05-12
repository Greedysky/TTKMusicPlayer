#include "decoderuadefactory.h"
#include "uademetadatamodel.h"
#include "decoder_uade.h"
#include "uadehelper.h"

bool DecoderUADEFactory::canDecode(QIODevice *input) const
{
    QFile *file = static_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    return UADEHelper::instance()->initialize(file->fileName());
}

DecoderProperties DecoderUADEFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("UADE Plugin");
    properties.shortName = "uade";
    properties.filters << "*.aam" << "*.amc" << "*.aon" << "*.aps" << "*.ash" << "*.ast" << "*.avp";
    properties.filters << "*.bd" << "*.bds" << "*.bsi" << "*.bss" << "*.bye";
    properties.filters << "*.cm" << "*.core" << "*.cust";
    properties.filters << "*.dat" << "*.dh" << "*.dl" << "*.dln" << "*.dm" << "*.dm2" << "*.dmu" << "*.doda" << "*.dsc" << "*.dsr" << "*.dss" << "*.dum" << "*.dw" << "*.dz";
    properties.filters << "*.ea" << "*.ems" << "*.emsv6" << "*.ex";
    properties.filters << "*.fp" << "*.fred" << "*.fw";
    properties.filters << "*.glue" << "*.gmc" << "*.gray";
    properties.filters << "*.hd" << "*.hip" << "*.hip7" << "*.hipc" << "*.hot";
    properties.filters << "*.is" << "*.is20";
    properties.filters << "*.jam" << "*.jcb" << "*.jcbo" << "*.jd" << "*.jmf" << "*.jo" << "*.jpo" << "*.jt";
    properties.filters << "*.kh" << "*.kim" << "*.kris";
    properties.filters << "*.lion" << "*.lme";
    properties.filters << "*.ma" << "*.mc" << "*.mcmd" << "*.mco" << "*.md" << "*.mii" << "*.mk2" << "*.mkii" << "*.ml" << "*.mm8" << "*.mmdc" << "*.mok" << "*.mon" << "*.mosh" << "*.mso" << "*.mug" << "*.mw";
    properties.filters << "*.ntp";
    properties.filters << "*.osp";
    properties.filters << "*.pap" << "*.pn" << "*.ps" << "*.psa" << "*.psf" << "*.pt" << "*.puma" << "*.pvp";
    properties.filters << "*.rh" << "*.rho" << "*.riff" << "*.rmc";
    properties.filters << "*.s7g" << "*.sa" << "*.sas"  << "*.sb" << "*.sc" << "*.scn" << "*.scr" << "*.sct" << "*.scumm" << "*.sdr" << "*.sg" << "*.sid1" << "*.sid2" << "*.sm" << "*.sm3" << "*.smn" << "*.smpro" << "*.sng" << "*.snk" << "*.soc" << "*.sog" << "*.spl" << "*.sqt" << "*.ss" << "*.sun" << "*.syn" << "*.synmod";
    properties.filters << "*.tf" << "*.thx" << "*.tits" << "*.tme" << "*.tro" << "*.tw";
    properties.filters << "*.ufo";
    properties.filters << "*.vss";
    properties.filters << "*.wb";
    properties.description = "Unix Amiga Delitracker Emulator File";
    properties.protocols << "file" << "uade";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderUADEFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderUADE(path);
}

QList<TrackInfo*> DecoderUADEFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
{
    if(path.contains("://")) //is it one track?
    {
        QString filePath = path;
        filePath.remove("uade://");
        filePath.remove(RegularWrapper("#\\d+$"));

        const int track = path.section("#", -1).toInt();
        QList<TrackInfo*> playlist = createPlayList(filePath, parts, ignoredPaths);
        if(playlist.isEmpty() || track <= 0 || track > playlist.count())
        {
            qDeleteAll(playlist);
            playlist.clear();
            return playlist;
        }

        TrackInfo *info = playlist.takeAt(track - 1);
        qDeleteAll(playlist);
        playlist.clear();
        return playlist << info;
    }
    else
    {
        if(ignoredPaths)
        {
            ignoredPaths->push_back(path);
        }
    }

    return UADEHelper::instance()->createPlayList(path, parts);
}

MetaDataModel* DecoderUADEFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new UADEMetaDataModel(path);
}

void DecoderUADEFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(uade, DecoderUADEFactory)
#endif
