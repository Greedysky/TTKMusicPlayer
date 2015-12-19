#include "musicvideocontrol.h"
#include "musicconnectionpool.h"
#include "musicdownloadwidget.h"
#include "musicmovinglabelslider.h"

#include <QPushButton>
#include <QToolButton>
#include <QBoxLayout>
#include <QWidgetAction>

MusicVideoControl::MusicVideoControl(bool popup, QWidget *parent)
    : QWidget(parent), m_widgetPopup(popup)
{
    m_timeSlider = new MusicMovingLabelSlider(Qt::Horizontal, this);
    m_menuButton = new QToolButton(this);
    m_playButton = new QPushButton(this);
    m_inSideButton = new QPushButton(this);
    m_fullButton = new QPushButton(this);
    m_qualityButton = new QPushButton(tr("NormalMV"), this);
    m_downloadButton = new QPushButton(tr("DownloadMV"), this);
    m_barrageWidget = new QLabel(this);

    m_volumnButton = new QToolButton(this);
    m_volumnSlider = new QSlider(Qt::Vertical,this);
    m_volumnSlider->setRange(0, 100);
    m_volumnSlider->setValue(100);

    m_playButton->setIcon(QIcon(":/video/play"));
    m_volumnButton->setIcon(QIcon(":/video/volumn"));
    m_menuButton->setIcon(QIcon(":/video/menu"));

    m_inSideButton->setText(popup ? tr("InlineMode") : tr("PopupMode"));
    m_fullButton->setText(tr("FullScreenMode"));
    m_fullButton->setEnabled( popup );

    m_inSideButton->setStyleSheet(MusicUIObject::MPushButtonStyle17);
    m_fullButton->setStyleSheet(MusicUIObject::MPushButtonStyle17);
    m_qualityButton->setStyleSheet(MusicUIObject::MPushButtonStyle17);
    m_downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle17);

    m_playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumnButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_menuButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_timeSlider->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumnSlider->setCursor(QCursor(Qt::PointingHandCursor));
    m_inSideButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_fullButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_qualityButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_popupVolumn.setStyleSheet(MusicUIObject::MMenuStyle01);
    m_popupQuality.setStyleSheet(MusicUIObject::MMenuStyle01);
    m_timeSlider->setStyleSheet(MusicUIObject::MSliderStyle04);
    m_volumnSlider->setStyleSheet(MusicUIObject::MSliderStyle02);

    QVBoxLayout *controlVLayout = new QVBoxLayout(this);
    controlVLayout->setSpacing(0);
    controlVLayout->setContentsMargins(0, 0, 0, 0);
    QWidget *controlBWidget = new QWidget(this);
    QHBoxLayout *controlBLayout = new QHBoxLayout(controlBWidget);
    controlBLayout->setContentsMargins(9, 0, 9, 0);
    controlBLayout->addWidget(m_menuButton);
    controlBLayout->addWidget(m_playButton);
    controlBLayout->addWidget(m_volumnButton);
    controlBLayout->addWidget(m_barrageWidget, 1);
    controlBLayout->addWidget(m_qualityButton);
    controlBLayout->addWidget(m_downloadButton);
    controlBLayout->addWidget(m_inSideButton);
    controlBLayout->addWidget(m_fullButton);
    controlBWidget->setLayout(controlBLayout);
    controlVLayout->addWidget(m_timeSlider);
    controlVLayout->addWidget(controlBWidget);
    setLayout(controlVLayout);

    QWidgetAction *widgetAction = new QWidgetAction(this);
    widgetAction->setDefaultWidget(m_volumnSlider);
    m_popupVolumn.addAction(widgetAction);
    m_volumnButton->setMenu(&m_popupVolumn);
    m_volumnButton->setPopupMode(QToolButton::InstantPopup);

    m_mvSd = m_popupQuality.addAction(tr("SdMV"));
    m_mvHd = m_popupQuality.addAction(tr("HdMV"));
    m_mvSq = m_popupQuality.addAction(tr("SqMV"));
    connect(&m_popupQuality, SIGNAL(triggered(QAction*)), SLOT(menuActionTriggered(QAction*)));
    m_qualityButton->setMenu(&m_popupQuality);

    connect(m_timeSlider, SIGNAL(sliderReleased()), SLOT(sliderReleased()));
    connect(this, SIGNAL(sliderValueChanged(int)), parent, SLOT(setPosition(int)));
    connect(m_volumnSlider, SIGNAL(valueChanged(int)), parent, SLOT(volumnChanged(int)));
    connect(m_playButton, SIGNAL(clicked()), parent, SLOT(play()));
    connect(m_inSideButton, SIGNAL(clicked()), SLOT(inSideButtonClicked()));
    connect(m_fullButton, SIGNAL(clicked()), SLOT(fullButtonClicked()));
    connect(m_downloadButton, SIGNAL(clicked()), SIGNAL(downloadLocalByControl()));

    M_CONNECTION->setValue("MusicVideoControl", this);
    M_CONNECTION->poolConnect("MusicVideoControl", "MusicRightAreaWidget");
    M_CONNECTION->poolConnect("MusicVideoControl", "MusicVideoTableWidget");

}

MusicVideoControl::~MusicVideoControl()
{
    M_CONNECTION->poolDisConnect("MusicVideoControl");
    delete m_volumnSlider;
    delete m_timeSlider;
    delete m_menuButton;
    delete m_playButton;
    delete m_volumnButton;
    delete m_barrageWidget;
    delete m_inSideButton;
    delete m_fullButton;
    delete m_qualityButton;
    delete m_downloadButton;
}

void MusicVideoControl::setValue(qint64 position) const
{
    m_timeSlider->setValue(position*1000);
}

void MusicVideoControl::durationChanged(qint64 duration) const
{
    m_timeSlider->setRange(0, duration*1000);
}

void MusicVideoControl::setButtonStyle(bool style) const
{
    m_playButton->setIcon(QIcon( style ? ":/video/play" : ":/video/pause"));
}

void MusicVideoControl::mediaChanged(const QString &url)
{
    switch(findMVBitrateByUrl(url))
    {
        case 500:   m_qualityButton->setText(tr("SdMV")); break;
        case 750:   m_qualityButton->setText(tr("HdMV")); break;
        case 1000:  m_qualityButton->setText(tr("SqMV")); break;
        default: break;
    }
}

void MusicVideoControl::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    m_timeSlider->setFixedWidth(w);
}

void MusicVideoControl::show()
{
    QWidget::show();
    setQualityActionState();
}

void MusicVideoControl::inSideButtonClicked()
{
    emit musicVideoSetPopup( !m_widgetPopup );
}

void MusicVideoControl::fullButtonClicked()
{
    m_fullButton->setText(m_fullButton->text() == tr("NormalMode") ?
                           tr("FullScreenMode") : tr("NormalMode"));
    emit musicVideoFullscreen( m_fullButton->text() == tr("NormalMode") );
}

void MusicVideoControl::menuActionTriggered(QAction *action)
{
    if(action->text() == tr("SdMV"))
    {
        m_qualityButton->setText(tr("SdMV"));
        emit mvURLChanged( findMVUrlByBitrate(500) );
    }
    else if(action->text() == tr("HdMV"))
    {
        m_qualityButton->setText(tr("HdMV"));
        emit mvURLChanged( findMVUrlByBitrate(750) );
    }
    else if(action->text() == tr("SqMV"))
    {
        m_qualityButton->setText(tr("SqMV"));
        emit mvURLChanged( findMVUrlByBitrate(1000) );
    }
}

void MusicVideoControl::sliderReleased()
{
    emit sliderValueChanged(m_timeSlider->value()/1000);
}

QString MusicVideoControl::findMVUrlByBitrate(int bitrate)
{
    MusicSongAttributes data;
    emit getMusicMvInfo(data);
    foreach(MusicSongAttribute attr, data)
    {
        if(attr.m_bitrate == bitrate)
        {
            return attr.m_url;
        }
    }
    return QString();
}

int MusicVideoControl::findMVBitrateByUrl(const QString &url)
{
    MusicSongAttributes data;
    emit getMusicMvInfo(data);
    foreach(MusicSongAttribute attr, data)
    {
        if(attr.m_url == url)
        {
            return attr.m_bitrate;
        }
    }
    return 0;
}

bool MusicVideoControl::findExsitByBitrate(int bitrate)
{
    MusicSongAttributes data;
    emit getMusicMvInfo(data);
    foreach(MusicSongAttribute attr, data)
    {
        if(attr.m_bitrate == bitrate)
        {
            return true;
        }
    }
    return false;
}

void MusicVideoControl::setQualityActionState()
{
    MusicSongAttributes data;
    emit getMusicMvInfo(data);

    m_mvSd->setEnabled( findExsitByBitrate(500) );
    m_mvHd->setEnabled( findExsitByBitrate(750) );
    m_mvSq->setEnabled( findExsitByBitrate(1000) );

    m_downloadButton->setEnabled( m_mvSd->isEnabled() || m_mvHd->isEnabled() ||
                                  m_mvSq->isEnabled() );
}
