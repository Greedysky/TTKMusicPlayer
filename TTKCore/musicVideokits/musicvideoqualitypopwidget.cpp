#include "musicvideoqualitypopwidget.h"
#include "musicvideouiobject.h"
#include "musicuiobject.h"
#include "musicnumberdefine.h"
#include "musicconnectionpool.h"
#include "musicvideotablewidget.h"

MusicVideoQualityPopWidget::MusicVideoQualityPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();

    setFixedSize(44, 20);
    setStyleSheet(MusicUIObject::MKGVideoBtnSDMode);

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicVideoTableWidget::getClassName());

}

MusicVideoQualityPopWidget::~MusicVideoQualityPopWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_actionGroup;
}

QString MusicVideoQualityPopWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicVideoQualityPopWidget::setQualityActionState()
{
    QList<QAction*> actions = m_actionGroup->actions();
    if(actions.count() >= 3)
    {
        actions[0]->setEnabled( findExistByBitrate(MB_500) );
        actions[1]->setEnabled( findExistByBitrate(MB_750) );
        actions[2]->setEnabled( findExistByBitrate(MB_1000) );
    }
}

void MusicVideoQualityPopWidget::setQualityText(const QString &url)
{
    QString style = MusicUIObject::MKGVideoBtnSDMode;
    switch(findMVBitrateByUrl(url))
    {
        case MB_500:   style = MusicUIObject::MKGVideoBtnSDMode; break;
        case MB_750:   style = MusicUIObject::MKGVideoBtnHDMode; break;
        case MB_1000:  style = MusicUIObject::MKGVideoBtnSQMode; break;
        default: break;
    }
    setStyleSheet( style );
}

void MusicVideoQualityPopWidget::movieQualityChoiced(QAction *action)
{
    QString url;
    switch( action->data().toInt() )
    {
        case 0: url = findMVUrlByBitrate(MB_500); break;
        case 1: url = findMVUrlByBitrate(MB_750); break;
        case 2: url = findMVUrlByBitrate(MB_1000); break;
    }
    setQualityText( url );
    emit mvURLChanged( url );
}

void MusicVideoQualityPopWidget::initWidget()
{
    m_menu->setWindowFlags(m_menu->windowFlags() | Qt::FramelessWindowHint);
    m_menu->setAttribute(Qt::WA_TranslucentBackground);
    m_menu->setStyleSheet(MusicUIObject::MMenuStyle03);

    m_actionGroup = new QActionGroup(this);
    connect(m_actionGroup, SIGNAL(triggered(QAction*)), SLOT(movieQualityChoiced(QAction*)));

    m_containWidget->setFixedSize(140, 100);
    m_menu->removeAction(m_menu->actions().first());
    m_actionGroup->addAction(m_menu->addAction(tr("SdMV")))->setData(0);
    m_actionGroup->addAction(m_menu->addAction(tr("HdMV")))->setData(1);
    m_actionGroup->addAction(m_menu->addAction(tr("SqMV")))->setData(2);
}

QString MusicVideoQualityPopWidget::findMVUrlByBitrate(int bitrate)
{
    MusicObject::MusicSongAttributes data;
    emit getMusicMvInfo(data);
    foreach(const MusicObject::MusicSongAttribute &attr, data)
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
    emit getMusicMvInfo(data);
    foreach(const MusicObject::MusicSongAttribute &attr, data)
    {
        if(attr.m_url == url)
        {
            return attr.m_bitrate;
        }
    }
    return 0;
}

bool MusicVideoQualityPopWidget::findExistByBitrate(int bitrate)
{
    MusicObject::MusicSongAttributes data;
    emit getMusicMvInfo(data);
    foreach(const MusicObject::MusicSongAttribute &attr, data)
    {
        if(attr.m_bitrate == bitrate)
        {
            return true;
        }
    }
    return false;
}
