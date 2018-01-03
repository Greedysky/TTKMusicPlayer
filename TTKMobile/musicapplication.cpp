#include "musicapplication.h"

#include "ttkmusicutils.h"
#include "ttknetworkhelper.h"
#include "ttkmusicconfigmanager.h"
#include <QQmlContext>

MusicApplication::MusicApplication(QQmlContext *parent)
    : QObject(parent)
{
    m_ttkUtils =  new TTKMusicUtils(this);
    m_networkHelper = new TTKNetworkHelper(this);

    parent->setContextProperty("TTK_APP", this);
    parent->setContextProperty("TTK_UTILS", m_ttkUtils);
    parent->setContextProperty("TTK_NETWORK", m_networkHelper);

    readXMLConfigFromText();
}

MusicApplication::~MusicApplication()
{
    writeXMLConfigToText();
    delete m_ttkUtils;
    delete m_networkHelper;
}

bool MusicApplication::checkMusicListCurrentIndex() const
{
    return true;
}

QString MusicApplication::getCurrentFileName() const
{
    return QString();
}

void MusicApplication::musicLoadCurrentSongLrc()
{

}

void MusicApplication::readXMLConfigFromText()
{
    TTKMusicConfigManager xml;
    if(!xml.readXMLConfig(COFIGPATH_FULL))
    {
        return;
    }

    xml.readOtherLoadConfig();
    m_networkHelper->setCurrentServer();
}

void MusicApplication::writeXMLConfigToText()
{
    TTKMusicConfigManager xml;
    xml.writeXMLConfig();
}
