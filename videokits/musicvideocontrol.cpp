#include "musicvideocontrol.h"
#include <QPushButton>
#include <QToolButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QWidgetAction>

MusicVideoControl::MusicVideoControl(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background:transparent");
    setFixedSize(520,40);
    setGeometry(0,415,520,40);

    m_timeSlider = new QSlider(Qt::Horizontal,this);
    m_menuButton = new QToolButton(this);
    m_playButton = new QPushButton(this);
    m_playNext = new QPushButton(this);
    m_playPrivious = new QPushButton(this);
    m_volumnButton = new QToolButton(this);
    m_volumnSlider = new QSlider(Qt::Vertical,this);
    m_volumnSlider->setRange(0,100);
    m_volumnSlider->setValue(100);

    m_playButton->setIcon(QIcon(":/video/play"));
    m_playNext->setIcon(QIcon(":/video/next"));
    m_playPrivious->setIcon(QIcon(":/video/privious"));
    m_volumnButton->setIcon(QIcon(":/video/volumn"));
    m_menuButton->setIcon(QIcon(":/video/menu"));

    m_playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_playNext->setCursor(QCursor(Qt::PointingHandCursor));
    m_playPrivious->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumnButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_menuButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_timeSlider->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumnSlider->setCursor(QCursor(Qt::PointingHandCursor));

    m_popupMenu.setStyleSheet(MusicObject::MusicSystemTrayMenu);
    m_popupVolumn.setStyleSheet(MusicObject::MusicSystemTrayMenu);
    m_timeSlider->setStyleSheet(MusicObject::MusicVolumeStyleHorizontal);
    m_volumnSlider->setStyleSheet(MusicObject::MusicVolumeStyleVertical);

    QBoxLayout *controlLayout = new QHBoxLayout(this);
    controlLayout->addWidget(m_menuButton);
    controlLayout->addWidget(m_playPrivious);
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(m_playNext);
    controlLayout->addWidget(m_volumnButton);
    controlLayout->addWidget(m_timeSlider);
    setLayout(controlLayout);

    m_popupMenu.addAction(tr("openFile"), this, SLOT(openFile()));
    m_popupMenu.addAction(tr("playList"), parent, SLOT(showPlayList()));
    m_menuButton->setMenu(&m_popupMenu);
    m_menuButton->setPopupMode(QToolButton::InstantPopup);

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
    connect(m_menuButton, SIGNAL(clicked()),this,
            SLOT(openFile()));
    connect(this,SIGNAL(addMedia(QStringList)),parent,
            SLOT(addMedia(QStringList)));
    connect(m_playNext,SIGNAL(clicked()),parent,
            SLOT(playNext()));
    connect(m_playPrivious,SIGNAL(clicked()),parent,
            SLOT(playPrivious()));
}

MusicVideoControl::~MusicVideoControl()
{
    delete m_volumnSlider;
    delete m_widgetAction;
    delete m_timeSlider;
    delete m_menuButton;
    delete m_playButton;
    delete m_playNext;
    delete m_playPrivious;
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

void MusicVideoControl::openFile()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Movie"),QDir::homePath());
    if(!fileNames.isEmpty())
    {
        emit addMedia(fileNames);
    }
}
