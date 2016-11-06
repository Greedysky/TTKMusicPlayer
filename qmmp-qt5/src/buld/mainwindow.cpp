#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "musicplayer.h"
#include "musicplaylist.h"
#include "../qmmp/visual.h"

#include <QSlider>

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

        m_sliders << slider;
        ui->equalizerLayout->addWidget(slider);
    }

    MusicPlaylist *list = new MusicPlaylist(this);

#ifdef Q_OS_UNIX
  #ifdef Q_OS_ANDROID
    list->appendMedia("/storage/emulated/0/QMMP/1.opus");
  #else
    list->appendMedia("/home/greedysky/qmmp_all/1.opus");
  #endif
#elif defined Q_OS_WIN
    list->appendMedia("D:/Qt/Workspace/qmmp_all/1.flac");
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
