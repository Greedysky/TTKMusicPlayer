#include "musicvideocontrol.h"
#include "musicconnectionpool.h"

#include <QPushButton>
#include <QToolButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QWidgetAction>

MusicVideoControl::MusicVideoControl(bool popup, QWidget *parent)
    : QWidget(parent), m_widgetPopup(popup)
{
    setStyleSheet(MusicUIObject::MCustomStyle01);
    setFixedSize(520, 40);
    move(0, 375);

    m_timeSlider = new QSlider(Qt::Horizontal,this);
    m_menuButton = new QToolButton(this);
    m_playButton = new QPushButton(this);
    m_inSideButton = new QPushButton(this);
    m_fullButton = new QPushButton(this);
    m_qualityButton = new QPushButton(tr("NormalMV"), this);
    m_downloadButton = new QPushButton(tr("DownloadMV"), this);

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
    m_timeSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    m_volumnSlider->setStyleSheet(MusicUIObject::MSliderStyle02);

    QHBoxLayout *controlLayout = new QHBoxLayout(this);
    controlLayout->addWidget(m_menuButton);
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(m_volumnButton);
    controlLayout->addWidget(m_timeSlider);
    controlLayout->addWidget(m_qualityButton);
    controlLayout->addWidget(m_downloadButton);
    controlLayout->addWidget(m_inSideButton);
    controlLayout->addWidget(m_fullButton);
    setLayout(controlLayout);

    QWidgetAction *widgetAction = new QWidgetAction(this);
    widgetAction->setDefaultWidget(m_volumnSlider);
    m_popupVolumn.addAction(widgetAction);
    m_volumnButton->setMenu(&m_popupVolumn);
    m_volumnButton->setPopupMode(QToolButton::InstantPopup);

    m_mvNormal = m_popupQuality.addAction(tr("NormalMV"));
    m_mvHd = m_popupQuality.addAction(tr("HdMV"));
    connect(&m_popupQuality, SIGNAL(triggered(QAction*)), SLOT(menuActionTriggered(QAction*)));
    m_qualityButton->setMenu(&m_popupQuality);

    connect(m_timeSlider, SIGNAL(sliderMoved(int)), parent,
                          SLOT(setPosition(int)));
    connect(m_volumnSlider, SIGNAL(valueChanged(int)), parent,
                            SLOT(volumnChanged(int)));
    connect(m_playButton, SIGNAL(clicked()), parent, SLOT(play()));
    connect(m_inSideButton, SIGNAL(clicked()), SLOT(inSideButtonClicked()));
    connect(m_fullButton, SIGNAL(clicked()), SLOT(fullButtonClicked()));
    connect(m_downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));

    M_CONNECTION->setValue("MusicVideoControl", this);
    M_CONNECTION->connect("MusicVideoControl", "MusicRightAreaWidget");
    M_CONNECTION->connect("MusicVideoControl", "MusicVideoTableWidget");

}

MusicVideoControl::~MusicVideoControl()
{
    M_CONNECTION->disConnect("MusicVideoControl");
    delete m_volumnSlider;
    delete m_timeSlider;
    delete m_menuButton;
    delete m_playButton;
    delete m_volumnButton;
    delete m_inSideButton;
    delete m_fullButton;
    delete m_qualityButton;
    delete m_downloadButton;
}

void MusicVideoControl::setValue(qint64 position) const
{
    m_timeSlider->setValue(position);
}

void MusicVideoControl::durationChanged(qint64 duration) const
{
    m_timeSlider->setRange(0, duration);
}

void MusicVideoControl::setButtonStyle(bool style) const
{
    m_playButton->setIcon(QIcon( style ? ":/video/play" : ":/video/pause"));
}

void MusicVideoControl::mediaChanged(const QString &url)
{
    SongUrlFormats data;
    emit getMusicMvInfo(data);

    for(int i=0; i<data.count(); ++i)
    {
        if(data[i].m_url == url)
        {
            m_qualityButton->setText( (data[i].m_format == "500") ? tr("NormalMV") : tr("HdMV"));
        }
    }
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

void MusicVideoControl::downloadButtonClicked()
{
    int quality;
    quality = m_qualityButton->text() == tr("NormalMV") ? 500 : 750;
    emit downloadLocalByQuality( quality );
}

void MusicVideoControl::menuActionTriggered(QAction *action)
{
    SongUrlFormats data;
    emit getMusicMvInfo(data);
    if(action->text() == tr("NormalMV"))
    {
        m_qualityButton->setText(tr("NormalMV"));
        emit mvURLChanged((data.first().m_format == "500") ? data.first().m_url : data.last().m_url);
    }
    else
    {
        m_qualityButton->setText(tr("HdMV"));
        emit mvURLChanged((data.first().m_format == "750") ? data.first().m_url : data.last().m_url);
    }
}

void MusicVideoControl::setQualityActionState()
{
    SongUrlFormats data;
    emit getMusicMvInfo(data);

    m_mvNormal->setEnabled(true);
    m_mvHd->setEnabled(true);
    m_downloadButton->setEnabled(true);

    if(data.isEmpty())
    {
        m_mvNormal->setEnabled(false);
        m_mvHd->setEnabled(false);
        m_downloadButton->setEnabled(false);
    }
    else if(data.count() == 1)
    {
        data.first().m_format == "500" ? m_mvHd->setEnabled(false) : m_mvNormal->setEnabled(false);
    }
}
