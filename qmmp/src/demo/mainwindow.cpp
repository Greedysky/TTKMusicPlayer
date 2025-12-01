#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "musicplayer.h"
#include "musicplaylist.h"

#include "../qmmp/visual.h"
#include "../qmmp/visualfactory.h"

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

    for(int i = 0; i < 11; ++i)
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
    list->appendMedia("/home/greedysky/qmmp_all/files/mpeg/1.mp3");
#else
    list->appendMedia("D:/Workspace/qmmp_all/files/mpeg/1.mp3");
#endif
    m_sound->setPlaylist(list);

//    Visual::initialize(this);
//    for(VisualFactory *var : Visual::factories())
//    {
//        if(var->properties().shortName == "flowvumeter")
//        {
//            Visual::setEnabled(var, true);
//        }
//    }
//    const QList<Visual*> *vs = Visual::visuals();
//    if(!vs->isEmpty() && vs->last())
//    {
//        vs->last()->show();
//    }
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
