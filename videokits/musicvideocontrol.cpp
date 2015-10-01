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

    m_volumnButton = new QToolButton(this);
    m_volumnSlider = new QSlider(Qt::Vertical,this);
    m_volumnSlider->setRange(0,100);
    m_volumnSlider->setValue(100);

    m_playButton->setIcon(QIcon(":/video/play"));
    m_volumnButton->setIcon(QIcon(":/video/volumn"));
    m_menuButton->setIcon(QIcon(":/video/menu"));

    m_inSideButton->setText(popup ? tr("InlineMode") : tr("PopupMode"));
    m_fullButton->setText(tr("FullScreenMode"));
    m_fullButton->setEnabled( popup );

    m_inSideButton->setStyleSheet(MusicUIObject::MPushButtonStyle17);
    m_fullButton->setStyleSheet(MusicUIObject::MPushButtonStyle17);

    m_playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumnButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_menuButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_timeSlider->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumnSlider->setCursor(QCursor(Qt::PointingHandCursor));
    m_inSideButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_fullButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_popupVolumn.setStyleSheet(MusicUIObject::MMenuStyle01);
    m_timeSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    m_volumnSlider->setStyleSheet(MusicUIObject::MSliderStyle02);

    QHBoxLayout *controlLayout = new QHBoxLayout(this);
    controlLayout->addWidget(m_menuButton);
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(m_volumnButton);
    controlLayout->addWidget(m_timeSlider);
    controlLayout->addWidget(m_inSideButton);
    controlLayout->addWidget(m_fullButton);
    setLayout(controlLayout);

    m_widgetAction = new QWidgetAction(this);
    m_widgetAction->setDefaultWidget(m_volumnSlider);
    m_popupVolumn.addAction(m_widgetAction);
    m_volumnButton->setMenu(&m_popupVolumn);
    m_volumnButton->setPopupMode(QToolButton::InstantPopup);

    connect(m_timeSlider, SIGNAL(sliderMoved(int)), parent,
                          SLOT(setPosition(int)));
    connect(m_volumnSlider, SIGNAL(valueChanged(int)), parent,
                            SLOT(volumnChanged(int)));
    connect(m_playButton, SIGNAL(clicked()), parent, SLOT(play()));
    connect(m_inSideButton, SIGNAL(clicked()), SLOT(inSideButtonClicked()));
    connect(m_fullButton, SIGNAL(clicked()), SLOT(fullButtonClicked()));

    M_Connection->setValue("MusicVideoControl", this);
    M_Connection->connect("MusicVideoControl", "MusicRightAreaWidget");
}

MusicVideoControl::~MusicVideoControl()
{
    M_Connection->disConnect("MusicVideoControl");
    delete m_volumnSlider;
    delete m_widgetAction;
    delete m_timeSlider;
    delete m_menuButton;
    delete m_playButton;
    delete m_volumnButton;
    delete m_inSideButton;
    delete m_fullButton;
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
