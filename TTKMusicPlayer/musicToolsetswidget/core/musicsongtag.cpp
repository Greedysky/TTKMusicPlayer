#include "musicsongtag.h"
#include "musictime.h"
#include "metadatamanager.h"
#include "inputsource.h"

MusicSongTag::MusicSongTag()
{
    MetaDataManager::instance();
}

MusicSongTag::~MusicSongTag()
{
    MetaDataManager::destroy();
}

bool MusicSongTag::readFile(const QString &file)
{
    InputSource *input = InputSource::create(file);

    if(input == nullptr || !input->initialize())
    {
        return false;
    }

    QList<FileInfo*> infos = MetaDataManager::instance()->createPlayList(file);
    MetaDataModel *model = MetaDataManager::instance()->createMetaDataModel(file);
    if(!infos.isEmpty())
    {
        FileInfo* info = infos.first();
        m_parameters[TAG_LENGTH] = MusicTime::msecTime2LabelJustified(info->length()*1000);
        QMapIterator<Qmmp::MetaData, QString> mapIt(info->metaData());
        while(mapIt.hasNext())
        {
            mapIt.next();
            m_parameters[static_cast<MusicTag>(mapIt.key())] = mapIt.value();
        }
    }
    if(model)
    {
        QHash<QString, QString> audioProperties = model->audioProperties();
        QHash<QString, QString> description = model->descriptions();
        QHashIterator<QString, QString> audioIt(audioProperties);
        while(audioIt.hasNext())
        {
            audioIt.next();
            QString key = audioIt.key();
            if(key == "Format")
                m_parameters[TAG_Format] = audioIt.value();
            else if(key == "Sample rate")
                m_parameters[TAG_SampleRate] = audioIt.value();
            else if(key == "Mode")
                m_parameters[TAG_Mode] = audioIt.value();
            else if(key == "Bitrate")
                m_parameters[TAG_Bitrate] = audioIt.value();
            else if(key == "Protection")
                m_parameters[TAG_Protection] = audioIt.value();
            else if(key == "Original")
                m_parameters[TAG_Original] = audioIt.value();
            else if(key == "Copyright")
                m_parameters[TAG_Copyright] = audioIt.value();
        }
        if(!description.isEmpty())
        {
            m_parameters[TAG_Description] = description[description.keys().first()];
        }
    }
    return true;
}
