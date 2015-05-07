#include "musicxmlconfigmanager.h"
#include <QtXml/QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <QSettings>

MusicXMLConfigManager::MusicXMLConfigManager(QObject *parent) :
    QObject(parent)
{
    m_file = NULL;
    m_ddom = NULL;
}

MusicXMLConfigManager::~MusicXMLConfigManager()
{
    delete m_file;
    delete m_ddom;
}

bool MusicXMLConfigManager::readConfig(const QString& type)
{
    delete m_file;
    delete m_ddom;
    m_file = new QFile( type );
    m_ddom = new QDomDocument;
    if( !m_file->open(QIODevice::ReadOnly | QIODevice::Text) )
        return false;
    if( !m_ddom->setContent(m_file) )
    {
        m_file->close();
        delete m_file;
        m_file = NULL;
        return false;
    }
    return true;
}

void MusicXMLConfigManager::readMusicSongsConfig(QList<QStringList>& fileNamesList,
                                                 QList<QStringList>& fileUrlsList)
{
    QPair<QStringList,QStringList> onePara;
    onePara = readMusicFilePath("fileNormalPath");
    fileNamesList<<onePara.first;
    fileUrlsList<<onePara.second;
    onePara = readMusicFilePath("fileLovestPath");
    fileNamesList<<onePara.first;
    fileUrlsList<<onePara.second;
    onePara = readMusicFilePath("netFilePath");
    fileNamesList<<onePara.first;
    fileUrlsList<<onePara.second;

    //extend playlist init here
//    for(int i=3; i<8; ++i)
//    {
//        onePara = readMusicFilePath("extend" + QString::number(i - 2));
//        if(!onePara.first.isEmpty() && !onePara.second.isEmpty())
//        {
//            fileNamesList<<onePara.first;
//            fileUrlsList<<onePara.second;
//        }
//    }
}

void MusicXMLConfigManager::writeMusicSongsConfig(const QList<QStringList>& fileNamesList,
                                                  const QList<QStringList>& fileUrlsList)
{
    //Open wirte file
    delete m_file;
    delete m_ddom;
    m_file = new QFile( MUSICPATH );
    m_ddom = new QDomDocument;
    if( !m_file->open(QFile::WriteOnly | QFile::Text) )
      return;

    ///////////////////////////////////////////////////////
    m_ddom->appendChild(
        m_ddom->createProcessingInstruction("xml","version='1.0' encoding='UTF-8'") );
    QDomElement QMusicPlayer = m_ddom->createElement("QMusicPlayer");
    m_ddom->appendChild(QMusicPlayer);
    //Class A
    QDomElement fileNormalPath = m_ddom->createElement("fileNormalPath");
    fileNormalPath.setAttribute("count",fileNamesList[0].count());
    QMusicPlayer.appendChild(fileNormalPath);
    QDomElement fileLovestPath = m_ddom->createElement("fileLovestPath");
    fileLovestPath.setAttribute("count",fileNamesList[1].count());
    QMusicPlayer.appendChild(fileLovestPath);
    QDomElement netFilePath = m_ddom->createElement("netFilePath");
    netFilePath.setAttribute("count",fileNamesList[2].count());
    QMusicPlayer.appendChild(netFilePath);

    //extend playlist init here
//    for(int i=3; i<fileNamesList.count(); ++i)
//    {
//        QDomElement extend = m_ddom->createElement("extend" + QString::number(i - 2));
//        extend.setAttribute("count",fileNamesList[i].count());
//        QMusicPlayer.appendChild(extend);
//    }

    //Class B
    for(int i=0; i<fileNamesList[0].count(); ++i)
    {
        QDomElement value = m_ddom->createElement("value");
        value.setAttribute("name",fileNamesList[0][i]);
        QDomText valuetext = m_ddom->createTextNode(fileUrlsList[0][i]);
        value.appendChild(valuetext);
        fileNormalPath.appendChild(value);
    }
    for(int i=0; i<fileNamesList[1].count(); ++i)
    {
        QDomElement value = m_ddom->createElement("value");
        value.setAttribute("name",fileNamesList[1][i]);
        QDomText valuetext = m_ddom->createTextNode(fileUrlsList[1][i]);
        value.appendChild(valuetext);
        fileLovestPath.appendChild(value);
    }
    for(int i=0; i<fileNamesList[2].count(); ++i)
    {
        QDomElement value = m_ddom->createElement("value");
        value.setAttribute("name",fileNamesList[2][i]);
        QDomText valuetext = m_ddom->createTextNode(fileUrlsList[2][i]);
        value.appendChild(valuetext);
        netFilePath.appendChild(value);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out,4);
}

void MusicXMLConfigManager::writeXMLConfig()
{
    QSettings setting;
    int playModeChoiced = setting.value("PLAYMODECHOICED").toInt();
    int volumeChoiced = setting.value("VOLUMECHOICED").toInt();
    QString autoPlayChoiced = setting.value("AUTOPLAYCHOICED").toString();
    int play3DMusicChoiced = setting.value("PLAY3DMUSICCHOICED").toInt();
    int languageIndexChoiced = setting.value("CURRENTLANINDEXCHOICED").toInt();
    QString closeEventChoiced = setting.value("CLOSEEVENTCHOICED").toString();
    QStringList lastPlayIndexChoiced = setting.value("LASTPLAYINDEXCHOICED").toStringList();
    QString bgThemeChoiced = setting.value("BGTHEMECHOICED").toString();
    QString bgTransparentChoiced = setting.value("BGTRANSPARENTCHOICED").toString();
    QString showInlineLrcChoiced = setting.value("SHOWINLINELRCCHOICED").toString();
    QString showDesktopLrcChoiced = setting.value("SHOWDESKTOPLRCCHOICED").toString();
    int lrcColorChoiced = setting.value("LRCCOLORCHOICED").toInt();
    int lrcSizeChoiced = setting.value("LRCSIZECHOICED").toInt();
    int lrcTypeChoiced = setting.value("LRCTYPECHOICED").toInt();
    int lrcFamilyChoiced = setting.value("LRCFAMILYCHOICED").toInt();
    QColor lrcFgColorChoiced = setting.value("LRCFGCOLORCHOICED").value<QColor>();
    QColor lrcBgColorChoiced = setting.value("LRCBGCOLORCHOICED").value<QColor>();
    int lrcTransparentChoiced = setting.value("LRCCOLORTRANSCHOICED").toInt();
    int DLrcColorChoiced = setting.value("DLRCCOLORCHOICED").toInt();
    int DLrcSizeChoiced = setting.value("DLRCSIZECHOICED").toInt();
    int DLrcTypeChoiced = setting.value("DLRCTYPECHOICED").toInt();
    int DLrcFamilyChoiced = setting.value("DLRCFAMILYCHOICED").toInt();
    QColor DLrcFgColorChoiced = setting.value("DLRCFGCOLORCHOICED").value<QColor>();
    QColor DLrcBgColorChoiced = setting.value("DLRCBGCOLORCHOICED").value<QColor>();
    int DLrcTransparentChoiced = setting.value("DLRCCOLORTRANSCHOICED").toInt();
    int DLrcLockedChoiced = setting.value("DLRCLOCKEDCHOICED").toInt();
    QRect DLrcGeometry = setting.value("DLRCGEOMETRYCHOICED").toRect();
    int equalizerEnableChoiced = setting.value("EQUALIZERENABLECHOICED").toInt();
    QString equalizerValueChoiced = setting.value("EQUALIZERVALUECHOICED").toString();
    int equalizerIndexChoiced = setting.value("EQUALIZERINDEXCHOICED").toInt();
    int sEqualizerIndexChoiced = setting.value("SEQUALIZERINDEXCHOICED").toInt();

    ///////////////////////////////////////////////////////////////////////////
    int timeAutoIndexChoiced = setting.value("TIMERAUTOINDEXCHOICED").toInt();
    int timeAutoPlayChoiced = setting.value("TIMERAUTOPLAYCHOICED").toInt();
    int timeAutoPlayHourChoiced = setting.value("TIMERAUTOPLAYHOURCHOICED").toInt();
    int timeAutoPlaySecondChoiced = setting.value("TIMERAUTOPLAYSECONDCHOICED").toInt();
    int timeAutoPlayRepeatChoiced = setting.value("TIMERAUTOPLAYREPEATCHOICED").toInt();
    int timeAutoPlayItemIndexChoiced = setting.value("TIMERAUTOPLAYITEMINDEXCHOICED").toInt();
    int timeAutoPlaySongIndexChoiced = setting.value("TIMERAUTOPLAYSONGINDEXCHOICED").toInt();
    int timeAutoStopChoiced = setting.value("TIMERAUTOSTOPCHOICED").toInt();
    int timeAutoStopHourChoiced = setting.value("TIMERAUTOSTOPHOURCHOICED").toInt();
    int timeAutoStopSecondChoiced = setting.value("TIMERAUTOSTOPSECONDCHOICED").toInt();
    int timeAutoStopRepeatChoiced = setting.value("TIMERAUTOSTOPREPEATCHOICED").toInt();
    int timeAutoShutdownChoiced = setting.value("TIMERAUTOSHUTDOWNCHOICED").toInt();
    int timeAutoShutdownHourChoiced = setting.value("TIMERAUTOSHUTDOWNHOURCHOICED").toInt();
    int timeAutoShutdownSecondChoiced = setting.value("TIMERAUTOSHUTDOWNSECONDCHOICED").toInt();
    int timeAutoShutdownRepeatChoiced = setting.value("TIMERAUTOSHUTDOWNREPEATCHOICED").toInt();
    ///////////////////////////////////////////////////////////////////////////

    //Open wirte file
    m_file =new QFile( COFIGPATH );
    m_ddom = new QDomDocument;
    if( !m_file->open(QFile::WriteOnly | QFile::Text) )
      return;
    ///////////////////////////////////////////////////////
    m_ddom->appendChild(
        m_ddom->createProcessingInstruction("xml","version='1.0' encoding='UTF-8'") );
    QDomElement QMusicPlayer = m_ddom->createElement("QMusicPlayer");
    m_ddom->appendChild(QMusicPlayer);

    //Class A
    QDomElement music = m_ddom->createElement("music");
    QMusicPlayer.appendChild(music);
    QDomElement settings = m_ddom->createElement("settings");
    QMusicPlayer.appendChild(settings);
    QDomElement backgroundSkin = m_ddom->createElement("backgroundSkin");
    QMusicPlayer.appendChild(backgroundSkin);
    QDomElement showLrc = m_ddom->createElement("inlineLrc");
    QMusicPlayer.appendChild(showLrc);
    QDomElement showDLrc = m_ddom->createElement("desktopLrc");
    QMusicPlayer.appendChild(showDLrc);
    QDomElement equalizer = m_ddom->createElement("equalizer");
    QMusicPlayer.appendChild(equalizer);

    //Class B
    QDomElement playMode = m_ddom->createElement("playMode");
    playMode.setAttribute("value",playModeChoiced);
    music.appendChild(playMode);
    QDomElement playVolume = m_ddom->createElement("playVolume");
    playVolume.setAttribute("value",volumeChoiced);
    music.appendChild(playVolume);

    QDomElement play3DMusic = m_ddom->createElement("play3DMusic");
    play3DMusic.setAttribute("value",play3DMusicChoiced);
    settings.appendChild(play3DMusic);
    QDomElement language = m_ddom->createElement("language");
    language.setAttribute("value",languageIndexChoiced);
    settings.appendChild(language);
    QDomElement autoPlay = m_ddom->createElement("autoPlay");
    autoPlay.setAttribute("value",autoPlayChoiced);
    settings.appendChild(autoPlay);
    QDomElement closeEvent = m_ddom->createElement("closeEvent");
    closeEvent.setAttribute("value",closeEventChoiced);
    settings.appendChild(closeEvent);
    QDomElement lastPlayIndex = m_ddom->createElement("lastPlayIndex");
    lastPlayIndex.setAttribute("value",lastPlayIndexChoiced[0]);
    QDomText lastPlayIndextext = m_ddom->createTextNode(QString("%1,%2").arg(
                lastPlayIndexChoiced[1]).arg(lastPlayIndexChoiced[2]));
    lastPlayIndex.appendChild(lastPlayIndextext);
    settings.appendChild(lastPlayIndex);
    ///////////////////////////////////////////////////////////////////////////
    QDomElement timeAutoIndex = m_ddom->createElement("timeAutoIndex");
    timeAutoIndex.setAttribute("value",timeAutoIndexChoiced);
    settings.appendChild(timeAutoIndex);
    QDomElement timeAutoPlay = m_ddom->createElement("timeAutoPlay");
    timeAutoPlay.setAttribute("value",timeAutoPlayChoiced);
    settings.appendChild(timeAutoPlay);
    QDomElement timeAutoPlayHour = m_ddom->createElement("timeAutoPlayHour");
    timeAutoPlayHour.setAttribute("value",timeAutoPlayHourChoiced);
    settings.appendChild(timeAutoPlayHour);
    QDomElement timeAutoPlaySecond = m_ddom->createElement("timeAutoPlaySecond");
    timeAutoPlaySecond.setAttribute("value",timeAutoPlaySecondChoiced);
    settings.appendChild(timeAutoPlaySecond);
    QDomElement timeAutoPlayRepeat = m_ddom->createElement("timeAutoPlayRepeat");
    timeAutoPlayRepeat.setAttribute("value",timeAutoPlayRepeatChoiced);
    settings.appendChild(timeAutoPlayRepeat);
    QDomElement timeAutoPlayItemIndex = m_ddom->createElement("timeAutoPlayItemIndex");
    timeAutoPlayItemIndex.setAttribute("value",timeAutoPlayItemIndexChoiced);
    settings.appendChild(timeAutoPlayItemIndex);
    QDomElement timeAutoPlaySongIndex = m_ddom->createElement("timeAutoPlaySongIndex");
    timeAutoPlaySongIndex.setAttribute("value",timeAutoPlaySongIndexChoiced);
    settings.appendChild(timeAutoPlaySongIndex);
    QDomElement timeAutoStop = m_ddom->createElement("timeAutoStop");
    timeAutoStop.setAttribute("value",timeAutoStopChoiced);
    settings.appendChild(timeAutoStop);
    QDomElement timeAutoStopHour = m_ddom->createElement("timeAutoStopHour");
    timeAutoStopHour.setAttribute("value",timeAutoStopHourChoiced);
    settings.appendChild(timeAutoStopHour);
    QDomElement timeAutoStopSecond = m_ddom->createElement("timeAutoStopSecond");
    timeAutoStopSecond.setAttribute("value",timeAutoStopSecondChoiced);
    settings.appendChild(timeAutoStopSecond);
    QDomElement timeAutoStopRepeat = m_ddom->createElement("timeAutoStopRepeat");
    timeAutoStopRepeat.setAttribute("value",timeAutoStopRepeatChoiced);
    settings.appendChild(timeAutoStopRepeat);
    QDomElement timeAutoShutdown = m_ddom->createElement("timeAutoShutdown");
    timeAutoShutdown.setAttribute("value",timeAutoShutdownChoiced);
    settings.appendChild(timeAutoShutdown);
    QDomElement timeAutoShutdownHour = m_ddom->createElement("timeAutoShutdownHour");
    timeAutoShutdownHour.setAttribute("value",timeAutoShutdownHourChoiced);
    settings.appendChild(timeAutoShutdownHour);
    QDomElement timeAutoShutdownSecond = m_ddom->createElement("timeAutoShutdownSecond");
    timeAutoShutdownSecond.setAttribute("value",timeAutoShutdownSecondChoiced);
    settings.appendChild(timeAutoShutdownSecond);
    QDomElement timeAutoShutdownRepeat = m_ddom->createElement("timeAutoShutdownRepeat");
    timeAutoShutdownRepeat.setAttribute("value",timeAutoShutdownRepeatChoiced);
    settings.appendChild(timeAutoShutdownRepeat);
    ///////////////////////////////////////////////////////////////////////////

    QDomElement bgTheme = m_ddom->createElement("bgTheme");
    bgTheme.setAttribute("value",bgThemeChoiced);
    backgroundSkin.appendChild(bgTheme);
    QDomElement bgTransparent = m_ddom->createElement("bgTransparent");
    bgTransparent.setAttribute("value",bgTransparentChoiced);
    backgroundSkin.appendChild(bgTransparent);

    ///////////////////////////////////////////////
    QDomElement showInlineLrc = m_ddom->createElement("showInlineLrc");
    showInlineLrc.setAttribute("value",showInlineLrcChoiced);
    showLrc.appendChild(showInlineLrc);
    QDomElement lrcColor = m_ddom->createElement("lrcColor");
    lrcColor.setAttribute("value",lrcColorChoiced);
    showLrc.appendChild(lrcColor);
    QDomElement lrcSize = m_ddom->createElement("lrcSize");
    lrcSize.setAttribute("value",lrcSizeChoiced);
    showLrc.appendChild(lrcSize);
    QDomElement lrcFamily = m_ddom->createElement("lrcFamily");
    lrcFamily.setAttribute("value",lrcFamilyChoiced);
    showLrc.appendChild(lrcFamily);
    QDomElement lrcType = m_ddom->createElement("lrcType");
    lrcType.setAttribute("value",lrcTypeChoiced);
    showLrc.appendChild(lrcType);
    QDomElement lrcTransparent = m_ddom->createElement("lrcTransparent");
    lrcTransparent.setAttribute("value",lrcTransparentChoiced);
    showLrc.appendChild(lrcTransparent);
    QDomElement lrcFgColor = m_ddom->createElement("lrcFgColor");
    lrcFgColor.setAttribute("value",QString("%1,%2,%3").arg(lrcFgColorChoiced.red())
                          .arg(lrcFgColorChoiced.green()).arg(lrcFgColorChoiced.blue()));
    showLrc.appendChild(lrcFgColor);
    QDomElement lrcBgColor = m_ddom->createElement("lrcBgColor");
    lrcBgColor.setAttribute("value",QString("%1,%2,%3").arg(lrcBgColorChoiced.red())
                          .arg(lrcBgColorChoiced.green()).arg(lrcBgColorChoiced.blue()));
    showLrc.appendChild(lrcBgColor);

    ///////////////////////////////////////////////
    QDomElement showDesktopLrc = m_ddom->createElement("showDesktopLrc");
    showDesktopLrc.setAttribute("value",showDesktopLrcChoiced);
    showDLrc.appendChild(showDesktopLrc);
    QDomElement lrcDColor = m_ddom->createElement("lrcDColor");
    lrcDColor.setAttribute("value",DLrcColorChoiced);
    showDLrc.appendChild(lrcDColor);
    QDomElement lrcDSize = m_ddom->createElement("lrcDSize");
    lrcDSize.setAttribute("value",DLrcSizeChoiced);
    showDLrc.appendChild(lrcDSize);
    QDomElement lrcDFamily = m_ddom->createElement("lrcDFamily");
    lrcDFamily.setAttribute("value",DLrcFamilyChoiced);
    showDLrc.appendChild(lrcDFamily);
    QDomElement lrcDType = m_ddom->createElement("lrcDType");
    lrcDType.setAttribute("value",DLrcTypeChoiced);
    showDLrc.appendChild(lrcDType);
    QDomElement lrcDTransparent = m_ddom->createElement("lrcDTransparent");
    lrcDTransparent.setAttribute("value",DLrcTransparentChoiced);
    showDLrc.appendChild(lrcDTransparent);
    QDomElement lrcDFgColor = m_ddom->createElement("lrcDFgColor");
    lrcDFgColor.setAttribute("value",QString("%1,%2,%3").arg(DLrcFgColorChoiced.red())
                          .arg(DLrcFgColorChoiced.green()).arg(DLrcFgColorChoiced.blue()));
    showDLrc.appendChild(lrcDFgColor);
    QDomElement lrcDBgColor = m_ddom->createElement("lrcDBgColor");
    lrcDBgColor.setAttribute("value",QString("%1,%2,%3").arg(DLrcBgColorChoiced.red())
                          .arg(DLrcBgColorChoiced.green()).arg(DLrcBgColorChoiced.blue()));
    showDLrc.appendChild(lrcDBgColor);
    QDomElement lrcDLocked = m_ddom->createElement("lrcDLocked");
    lrcDLocked.setAttribute("value",DLrcLockedChoiced);
    showDLrc.appendChild(lrcDLocked);
    QDomElement lrcDGeometry = m_ddom->createElement("lrcDGeometry");
    lrcDGeometry.setAttribute("value",QString("%1,%2,%3,%4").arg(DLrcGeometry.left())
                           .arg(DLrcGeometry.top()).arg(DLrcGeometry.width())
                           .arg(DLrcGeometry.height()));
    showDLrc.appendChild(lrcDGeometry);

    QDomElement equalizerEnale = m_ddom->createElement("equalizerEnale");
    equalizerEnale.setAttribute("value",equalizerEnableChoiced);
    equalizer.appendChild(equalizerEnale);
    QDomElement equalizerIndex = m_ddom->createElement("equalizerIndex");
    equalizerIndex.setAttribute("value",equalizerIndexChoiced);
    equalizer.appendChild(equalizerIndex);
    QDomElement sEqualizerIndex = m_ddom->createElement("sEqualizerIndex");
    sEqualizerIndex.setAttribute("value",sEqualizerIndexChoiced);
    equalizer.appendChild(sEqualizerIndex);
    QDomElement equalizerValue = m_ddom->createElement("equalizerValue");
    equalizerValue.setAttribute("value",equalizerValueChoiced);
    equalizer.appendChild(equalizerValue);
    ///////////////////////////////////////////////

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out,4);
}

QPair<QStringList,QStringList> MusicXMLConfigManager::readMusicFilePath(const QString& value)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName(value).at(0).childNodes();
    QStringList name;
    QStringList url;
    for(int i=0; i<nodelist.count(); i++)
    {
       name.append(nodelist.at(i).toElement().attribute("name"));
       url.append(nodelist.at(i).toElement().text());
    }
    return QPair<QStringList,QStringList>(name,url);
}

void MusicXMLConfigManager::readSystemLastPlayIndexConfig(QStringList& key)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("lastPlayIndex");
    QDomElement element = nodelist.at(0).toElement();
    key<<element.attribute("value")<<element.text().split(',');
    Q_ASSERT(key.count() == 3);
    if(key.count() != 3) key.insert(0,"0");
}

QString MusicXMLConfigManager::readXmlByTagNameAndAttribute(const QString& tagName)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName(tagName);
    return nodelist.at(0).toElement().attribute("value");
}

QColor MusicXMLConfigManager::readColorConfig(const QString& value)
{
    QStringList rgb = readXmlByTagNameAndAttribute(value).split(',');
    if(rgb.count() != 3)
        return QColor();
    return QColor(rgb[0].toInt(),rgb[1].toInt(),rgb[2].toInt());
}

QRect MusicXMLConfigManager::readShowDLrcGeometry()
{
    QStringList geometry = readXmlByTagNameAndAttribute("lrcDGeometry").split(',');
    if(geometry.count() != 4)
        return QRect();
    return QRect(geometry[0].toInt(),geometry[1].toInt(),
                 geometry[2].toInt(),geometry[3].toInt() );
}

void MusicXMLConfigManager::readTimeAutoConfig()
{
    QSettings setting;
    setting.setValue("TIMERAUTOINDEXCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoIndex").toInt());
    setting.setValue("TIMERAUTOPLAYCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoPlay").toInt());
    setting.setValue("TIMERAUTOPLAYHOURCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoPlayHour").toInt());
    setting.setValue("TIMERAUTOPLAYSECONDCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoPlaySecond").toInt());
    setting.setValue("TIMERAUTOPLAYREPEATCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoPlayRepeat").toInt());
    setting.setValue("TIMERAUTOPLAYITEMINDEXCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoPlayItemIndex").toInt());
    setting.setValue("TIMERAUTOPLAYSONGINDEXCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoPlaySongIndex").toInt());
    setting.setValue("TIMERAUTOSTOPCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoStop").toInt());
    setting.setValue("TIMERAUTOSTOPHOURCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoStopHour").toInt());
    setting.setValue("TIMERAUTOSTOPSECONDCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoStopSecond").toInt());
    setting.setValue("TIMERAUTOSTOPREPEATCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoStopRepeat").toInt());
    setting.setValue("TIMERAUTOSHUTDOWNCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoShutdown").toInt());
    setting.setValue("TIMERAUTOSHUTDOWNHOURCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoShutdownHour").toInt());
    setting.setValue("TIMERAUTOSHUTDOWNSECONDCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoShutdownSecond").toInt());
    setting.setValue("TIMERAUTOSHUTDOWNREPEATCHOICED",
                     readXmlByTagNameAndAttribute("timeAutoShutdownRepeat").toInt());
}
