#include "musicvideoqualitypopwidget.h"
#include "musicvideouiobject.h"
#include "musicconnectionpool.h"
#include "musicvideosearchtablewidget.h"

MusicVideoQualityPopWidget::MusicVideoQualityPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();

    setFixedSize(44, 20);
    setStyleSheet(MusicUIObject::MQSSVideoBtnSDMode);

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->poolConnect(className(), MusicVideoSearchTableWidget::className());
}

MusicVideoQualityPopWidget::~MusicVideoQualityPopWidget()
{
    G_CONNECTION_PTR->removeValue(className());
    delete m_actionGroup;
}

void MusicVideoQualityPopWidget::setQualityActionState()
{
    QList<QAction*> actions = m_actionGroup->actions();
    if(actions.count() >= 4)
    {
        actions[0]->setEnabled(findExistByBitrate(MB_250));
        actions[1]->setEnabled(findExistByBitrate(MB_500));
        actions[2]->setEnabled(findExistByBitrate(MB_750));
        actions[3]->setEnabled(findExistByBitrate(MB_1000));
    }
}

void MusicVideoQualityPopWidget::setQualityText(const QString &url)
{
    QString style = MusicUIObject::MQSSVideoBtnSDMode;
    switch(findMVBitrateByUrl(url))
    {
        case MB_250: style = MusicUIObject::MQSSVideoBtnSTMode; break;
        case MB_500: style = MusicUIObject::MQSSVideoBtnSDMode; break;
        case MB_750: style = MusicUIObject::MQSSVideoBtnHDMode; break;
        case MB_1000: style = MusicUIObject::MQSSVideoBtnSQMode; break;
        default: break;
    }
    setStyleSheet(style);
}

void MusicVideoQualityPopWidget::movieQualityChoiced(QAction *action)
{
    QString url;
    switch(action->data().toInt())
    {
        case MusicObject::NoneQuality: url = findMVUrlByBitrate(MB_250); break;
        case MusicObject::StandardQuality: url = findMVUrlByBitrate(MB_500); break;
        case MusicObject::HighQuality: url = findMVUrlByBitrate(MB_750); break;
        case MusicObject::SuperQuality: url = findMVUrlByBitrate(MB_1000); break;
        default: break;
    }
    setQualityText(url);
    Q_EMIT mediaUrlChanged(url);
}

void MusicVideoQualityPopWidget::initialize()
{
    setTranslucentBackground();
    m_actionGroup = new QActionGroup(this);
    connect(m_actionGroup, SIGNAL(triggered(QAction*)), SLOT(movieQualityChoiced(QAction*)));

    m_containWidget->setFixedSize(140, 125);
    m_menu->removeAction(m_menu->actions().front());
    m_actionGroup->addAction(m_menu->addAction(tr("ST")))->setData(MusicObject::NoneQuality);
    m_actionGroup->addAction(m_menu->addAction(tr("SD")))->setData(MusicObject::StandardQuality);
    m_actionGroup->addAction(m_menu->addAction(tr("HD")))->setData(MusicObject::HighQuality);
    m_actionGroup->addAction(m_menu->addAction(tr("SQ")))->setData(MusicObject::SuperQuality);

    setQualityActionState();
}

QString MusicVideoQualityPopWidget::findMVUrlByBitrate(int bitrate)
{
    MusicObject::MusicSongPropertyList props;
    Q_EMIT queryMusicMediaInfo(props);

    for(const MusicObject::MusicSongProperty &prop : qAsConst(props))
    {
        if(prop.m_bitrate == bitrate)
        {
            return prop.m_url;
        }
    }
    return QString();
}

int MusicVideoQualityPopWidget::findMVBitrateByUrl(const QString &url)
{
    MusicObject::MusicSongPropertyList props;
    Q_EMIT queryMusicMediaInfo(props);

    for(const MusicObject::MusicSongProperty &prop : qAsConst(props))
    {
        const QString &aurl = prop.m_url;
        if(aurl == url)
        {
            return prop.m_bitrate;
        }
    }
    return 0;
}

bool MusicVideoQualityPopWidget::findExistByBitrate(int bitrate)
{
    MusicObject::MusicSongPropertyList props;
    Q_EMIT queryMusicMediaInfo(props);

    for(const MusicObject::MusicSongProperty &prop : qAsConst(props))
    {
        if(prop.m_bitrate == bitrate)
        {
            return true;
        }
    }
    return false;
}
