#include "musicvideoqualitypopwidget.h"
#include "musicvideouiobject.h"
#include "musicvideosearchtablewidget.h"
#include "musicconnectionpool.h"

MusicVideoQualityPopWidget::MusicVideoQualityPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();

    setFixedSize(44, 20);
    setStyleSheet(TTK::UI::VideoBtnSTMode);

    TTK_CONNECTIONPOOL_REGISTER(this);
    TTK_CONNECTIONPOOL_BIND_TO(MusicVideoSearchTableWidget);
}

MusicVideoQualityPopWidget::~MusicVideoQualityPopWidget()
{
    TTK_CONNECTIONPOOL_UNREGISTER(this);
    delete m_qualityActions;
}

void MusicVideoQualityPopWidget::setQualityActionState()
{
    QList<QAction*> actions = m_qualityActions->actions();
    if(actions.count() >= 4)
    {
        actions[0]->setEnabled(bitrateIsValid(TTK_BN_250));
        actions[1]->setEnabled(bitrateIsValid(TTK_BN_500));
        actions[2]->setEnabled(bitrateIsValid(TTK_BN_750));
        actions[3]->setEnabled(bitrateIsValid(TTK_BN_1000));
    }
}

void MusicVideoQualityPopWidget::setQualityText(const QString &url)
{
    QString style = TTK::UI::VideoBtnSTMode;
    switch(findBitrateByUrl(url))
    {
        case TTK_BN_250: style = TTK::UI::VideoBtnSTMode; break;
        case TTK_BN_500: style = TTK::UI::VideoBtnSDMode; break;
        case TTK_BN_750: style = TTK::UI::VideoBtnHDMode; break;
        case TTK_BN_1000: style = TTK::UI::VideoBtnSQMode; break;
        default: break;
    }

    setStyleSheet(style);
}

void MusicVideoQualityPopWidget::movieQualityChoiced(QAction *action)
{
    QString url;
    switch(TTKStaticCast(TTK::QueryQuality, action->data().toInt()))
    {
        case TTK::QueryQuality::Standard: url = findUrlByBitrate(TTK_BN_250); break;
        case TTK::QueryQuality::High: url = findUrlByBitrate(TTK_BN_500); break;
        case TTK::QueryQuality::Super: url = findUrlByBitrate(TTK_BN_750); break;
        case TTK::QueryQuality::Lossless: url = findUrlByBitrate(TTK_BN_1000); break;
        default: break;
    }

    setQualityText(url);
    Q_EMIT mediaUrlChanged(url);
}

void MusicVideoQualityPopWidget::initialize()
{
    setTranslucentBackground();

    m_qualityActions = new QActionGroup(this);
    connect(m_qualityActions, SIGNAL(triggered(QAction*)), SLOT(movieQualityChoiced(QAction*)));

    m_containWidget->setFixedSize(60, 125);
    m_menu->setStyleSheet(TTK::UI::MenuStyle03);
    m_menu->removeAction(m_menu->actions().first());

    m_qualityActions->addAction(m_menu->addAction(tr("ST")))->setData(TTKStaticCast(int, TTK::QueryQuality::Standard));
    m_qualityActions->addAction(m_menu->addAction(tr("SD")))->setData(TTKStaticCast(int, TTK::QueryQuality::High));
    m_qualityActions->addAction(m_menu->addAction(tr("HD")))->setData(TTKStaticCast(int, TTK::QueryQuality::Super));
    m_qualityActions->addAction(m_menu->addAction(tr("SQ")))->setData(TTKStaticCast(int, TTK::QueryQuality::Lossless));

    setQualityActionState();
}

QString MusicVideoQualityPopWidget::findUrlByBitrate(int bitrate)
{
    TTK::MusicSongPropertyList props;
    Q_EMIT queryMediaProps(props);

    for(const TTK::MusicSongProperty &prop : qAsConst(props))
    {
        if(prop.m_bitrate == bitrate)
        {
            return prop.m_url;
        }
    }
    return {};
}

int MusicVideoQualityPopWidget::findBitrateByUrl(const QString &url)
{
    TTK::MusicSongPropertyList props;
    Q_EMIT queryMediaProps(props);

    for(const TTK::MusicSongProperty &prop : qAsConst(props))
    {
        const QString &aurl = prop.m_url;
        if(aurl == url)
        {
            return prop.m_bitrate;
        }
    }
    return 0;
}

bool MusicVideoQualityPopWidget::bitrateIsValid(int bitrate)
{
    TTK::MusicSongPropertyList props;
    Q_EMIT queryMediaProps(props);

    for(const TTK::MusicSongProperty &prop : qAsConst(props))
    {
        if(prop.m_bitrate == bitrate)
        {
            return true;
        }
    }
    return false;
}
