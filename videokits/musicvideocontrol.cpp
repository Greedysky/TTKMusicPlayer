#include "musicvideocontrol.h"
#include <QPushButton>
#include <QToolButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QWidgetAction>

MusicVideoControl::MusicVideoControl(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background:transparent");
    setFixedSize(520,40);
    setGeometry(0,375,520,40);

    m_timeSlider = new QSlider(Qt::Horizontal,this);
    m_menuButton = new QToolButton(this);
    m_playButton = new QPushButton(this);
    m_volumnButton = new QToolButton(this);
    m_volumnSlider = new QSlider(Qt::Vertical,this);
    m_volumnSlider->setRange(0,100);
    m_volumnSlider->setValue(100);

    m_playButton->setIcon(QIcon(":/video/play"));
    m_volumnButton->setIcon(QIcon(":/video/volumn"));
    m_menuButton->setIcon(QIcon(":/video/menu"));

    m_playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumnButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_menuButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_timeSlider->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumnSlider->setCursor(QCursor(Qt::PointingHandCursor));

    m_popupVolumn.setStyleSheet(MusicUIObject::MMenuStyle01);
    m_timeSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    m_volumnSlider->setStyleSheet(MusicUIObject::MSliderStyle02);

    QHBoxLayout *controlLayout = new QHBoxLayout(this);
    controlLayout->addWidget(m_menuButton);
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(m_volumnButton);
    controlLayout->addWidget(m_timeSlider);
    setLayout(controlLayout);

    m_widgetAction = new QWidgetAction(this);
    m_widgetAction->setDefaultWidget(m_volumnSlider);
    m_popupVolumn.addAction(m_widgetAction);
    m_volumnButton->setMenu(&m_popupVolumn);
    m_volumnButton->setPopupMode(QToolButton::InstantPopup);

    connect(m_timeSlider, SIGNAL(sliderMoved(int)),
            parent, SLOT(setPosition(int)));
    connect(m_volumnSlider, SIGNAL(valueChanged(int)),
            parent, SLOT(volumnChanged(int)));
    connect(m_playButton, SIGNAL(clicked()),parent,
            SLOT(play()));
}

MusicVideoControl::~MusicVideoControl()
{
    delete m_volumnSlider;
    delete m_widgetAction;
    delete m_timeSlider;
    delete m_menuButton;
    delete m_playButton;
    delete m_volumnButton;
}

void MusicVideoControl::setValue(qint64 position)
{
    m_timeSlider->setValue(position);
}

void MusicVideoControl::durationChanged(qint64 duration)
{
    m_timeSlider->setRange(0, duration);
}

void MusicVideoControl::setButtonStyle(bool style)
{
    if(style)
        m_playButton->setIcon(QIcon(":/video/play"));
    else
        m_playButton->setIcon(QIcon(":/video/pause"));
}
