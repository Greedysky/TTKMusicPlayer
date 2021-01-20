#include "musicvideoqualitypopwidget.h"
#include "musicvideouiobject.h"
#include "musicuiobject.h"
#include "musicconnectionpool.h"
#include "musicvideosearchtablewidget.h"

MusicVideoQualityPopWidget::MusicVideoQualityPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();

    setFixedSize(44, 20);
    setStyleSheet(MusicUIObject::MQSSVideoBtnSDMode);

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicVideoSearchTableWidget::getClassName());
}

MusicVideoQualityPopWidget::~MusicVideoQualityPopWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
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
        case MB_250:   style = MusicUIObject::MQSSVideoBtnSTMode; break;
        case MB_500:   style = MusicUIObject::MQSSVideoBtnSDMode; break;
        case MB_750:   style = MusicUIObject::MQSSVideoBtnHDMode; break;
        case MB_1000:  style = MusicUIObject::MQSSVideoBtnSQMode; break;
        default: break;
    }
    setStyleSheet(style);
}

void MusicVideoQualityPopWidget::movieQualityChoiced(QAction *action)
{
    QString url;
    switch(action->data().toInt())
    {
        case 0: url = findMVUrlByBitrate(MB_250); break;
        case 1: url = findMVUrlByBitrate(MB_500); break;
        case 2: url = findMVUrlByBitrate(MB_750); break;
        case 3: url = findMVUrlByBitrate(MB_1000); break;
        default: break;
    }
    setQualityText(url);
    Q_EMIT mediaUrlChanged(url);
}

void MusicVideoQualityPopWidget::initWidget()
{
    setTranslucentBackground();
    m_actionGroup = new QActionGroup(this);
    connect(m_actionGroup, SIGNAL(triggered(QAction*)), SLOT(movieQualityChoiced(QAction*)));

    m_containWidget->setFixedSize(140, 125);
    m_menu->removeAction(m_menu->actions().first());
    m_actionGroup->addAction(m_menu->addAction(tr("StMV")))->setData(0);
    m_actionGroup->addAction(m_menu->addAction(tr("SdMV")))->setData(1);
    m_actionGroup->addAction(m_menu->addAction(tr("HdMV")))->setData(2);
    m_actionGroup->addAction(m_menu->addAction(tr("SqMV")))->setData(3);

    setQualityActionState();
}

QString MusicVideoQualityPopWidget::findMVUrlByBitrate(int bitrate)
{
    MusicObject::MusicSongAttributes data;
    Q_EMIT getMusicMediaInfo(data);

    for(const MusicObject::MusicSongAttribute &attr : qAsConst(data))
    {
        if(attr.m_bitrate == bitrate)
        {
            return attr.m_url;
        }
    }
    return QString();
}

int MusicVideoQualityPopWidget::findMVBitrateByUrl(const QString &url)
{
    MusicObject::MusicSongAttributes data;
    Q_EMIT getMusicMediaInfo(data);

    for(const MusicObject::MusicSongAttribute &attr : qAsConst(data))
    {
        const QString &aurl = attr.m_url;
        if(aurl == url)
        {
            return attr.m_bitrate;
        }
    }
    return 0;
}

bool MusicVideoQualityPopWidget::findExistByBitrate(int bitrate)
{
    MusicObject::MusicSongAttributes data;
    Q_EMIT getMusicMediaInfo(data);

    for(const MusicObject::MusicSongAttribute &attr : qAsConst(data))
    {
        if(attr.m_bitrate == bitrate)
        {
            return true;
        }
    }
    return false;
}
