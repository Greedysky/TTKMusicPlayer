#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "musicplayer.h"
#include "musicplaylist.h"
#include "../qmmp/visual.h"

#include <QSlider>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_sound = new MusicPlayer(this);

    connect(ui->playButton, SIGNAL(clicked(bool)), m_sound, SLOT(play()));
    connect(ui->pauseButton, SIGNAL(clicked(bool)), m_sound, SLOT(pause()));
    connect(ui->stopButton, SIGNAL(clicked(bool)), m_sound, SLOT(stop()));
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), SLOT(setVolume(int)));
    connect(ui->timeSlider, SIGNAL(sliderMoved(int)), SLOT(setSeek(int)));
    connect(m_sound, SIGNAL(positionChanged(qint64)), SLOT(setTimeValue(qint64)));

    for(int i=0; i<11; ++i)
    {
        QSlider *slider = new QSlider(this);
        slider->setRange(-20, 20);
        slider->setTickPosition(QSlider::TicksLeft);
        slider->setTickInterval(10);
        connect(slider, SIGNAL(valueChanged(int)), SLOT(applySettings()));

        m_sliders << slider;
        ui->equalizerLayout->addWidget(slider);
    }

    MusicPlaylist *list = new MusicPlaylist(this);
#ifdef Q_OS_UNIX
    list->appendMedia("/home/greedysky/qmmp_all/1.mp3");
#else
    list->appendMedia("D:\\Workspace\\qmmp_all\\1.flac");
#endif
    m_sound->setPlaylist(list);
}

MainWindow::~MainWindow()
{
    delete ui;
    qDeleteAll(m_sliders);
    delete m_sound;
}

void MainWindow::setVolume(int value)
{
    m_sound->setVolume(value);
}

void MainWindow::setSeek(int value)
{
    m_sound->setPosition(value);
}

void MainWindow::setTimeValue(qint64 value)
{
    ui->timeSlider->setRange(0, m_sound->duration());
    ui->timeSlider->setValue(value);
}

void MainWindow::applySettings()
{
//    EqSettings settings(EqSettings::EQ_BANDS_10);
//    settings.setPreamp(m_sliders[0]->value());
//    settings.setEnabled(true);

//    for(int i = 0; i < EqSettings::EQ_BANDS_10; ++i)
//    {
//        settings.setGain(i, m_sliders[i+1]->value());
//    }
//    m_sound->setEqSettings(settings);
}
