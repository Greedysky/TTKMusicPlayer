#include "musicwebradiowidget.h"
#include "ui_musicwebradiowidget.h"
#include "musicwebradiodatebase.h"
#include "musicbgthememanager.h"

#include <QProcess>

MusicWebRadioWidget::MusicWebRadioWidget(QWidget *parent) :
    MusicAbstractMoveWidget(parent),m_radio(NULL),
    ui(new Ui::MusicWebRadioWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    m_database = new MusicWebRadioDatabase;
    m_database->connectDatabase();

    ui->playButton->setIcon(QIcon(":/image/play"));
    ui->stopButton->setIcon(QIcon(":/image/stop"));
    ui->colletButton->setIcon(QIcon(":/radio/collect"));
    ui->discolletButton->setIcon(QIcon(":/radio/discollect"));
    ui->playButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->stopButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->colletButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->discolletButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->playButton->setIconSize(QSize(31,31));
    ui->stopButton->setIconSize(QSize(31,31));
    ui->colletButton->setIconSize(QSize(31,31));
    ui->discolletButton->setIconSize(QSize(31,31));
    ui->playButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->stopButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->colletButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->discolletButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->playButton->setToolTip(tr("Play"));
    ui->stopButton->setToolTip(tr("Stop"));
    ui->colletButton->setToolTip(tr("Colletion"));
    ui->discolletButton->setToolTip(tr("Discolletion"));
    ui->volumnSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->volumnSlider->setRange(0,100);
    ui->volumnSlider->setValue(100);

    connect(ui->playButton,SIGNAL(clicked()),SLOT(radioPlay()));
    connect(ui->stopButton,SIGNAL(clicked()),SLOT(radioStop()));
    connect(ui->volumnSlider,SIGNAL(valueChanged(int)),SLOT(radioVolume(int)));
    connect(ui->colletButton,SIGNAL(clicked()),SLOT(radioColletButton()));
    connect(ui->discolletButton,SIGNAL(clicked()),SLOT(radioDiscolletButton()));

    ui->movieLabel->setPixmap(QPixmap(":/radio/radiopng1").scaled(455,60));
    connect(&m_timer,SIGNAL(timeout()),SLOT(timeout()));
    m_timerCount = 1;
    m_collecticon = new QIcon(":/radio/collect");
    m_discollecticon = new QIcon(":/radio/discollect");

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                            SLOT(itemHasDoubleClicked(QListWidgetItem*)));

}

MusicWebRadioWidget::~MusicWebRadioWidget()
{
    clearAllItems();
    m_timer.stop();
    m_database->disConnectDatabase();
    delete m_collecticon;
    delete m_discollecticon;
    delete m_radio;
    delete m_database;
    delete ui;
}

void MusicWebRadioWidget::closeEvent(QCloseEvent *event)
{
    m_timer.stop();
    delete m_radio;
    m_radio = NULL;
    QWidget::closeEvent(event);
}

void MusicWebRadioWidget::timeout()
{
    ++m_timerCount;
    ui->movieLabel->setPixmap(QPixmap(":/radio/radiopng" +
                              QString::number(m_timerCount)).scaled(455,60));
    if(m_timerCount >= 4)
    {
        m_timerCount = 0;
    }
}

void MusicWebRadioWidget::radioPlay()
{
    if(m_radioUrl.isEmpty())
    {
        return;
    }

    delete m_radio;
    m_radio = new QProcess(this);

    QStringList arguments;
    arguments<<"-slave"<<"-quiet"<<"-vo"<<"directx:noaccel"<<m_radioUrl;
    m_radio->start(MAKE_RADIO, arguments);
    connect(m_radio,SIGNAL(readyReadStandardOutput()),SLOT(radioStandardOutput()));
    m_radio->write(QString("volume " + QString::number(ui->volumnSlider->value()) + " 1\n").toUtf8());
    ui->stateLabel->setText(tr("Connecting..."));
    m_timer.start(100);
}

void MusicWebRadioWidget::radioStop()
{
    if(m_radio)
    {
        m_radio->write("pause\n");
        m_timer.stop();
    }
}

void MusicWebRadioWidget::radioVolume(int num)
{
    if(m_radio)
    {
        m_radio->write(QString("volume " + QString::number(num) + " 1\n").toUtf8());
    }
}

void MusicWebRadioWidget::clearAllItems()
{
    ui->listWidget->clear();
}

void MusicWebRadioWidget::updateRadioList(const QString& category)
{
    clearAllItems();
    ui->listWidget->clear();
    QStringList fnames = m_database->getFavouriteNames();
    QStringList rnames = m_database->getRadioNames(category);
    for(int i=0; i<rnames.count(); ++i)
    {
        if(fnames.contains(rnames[i]))
        {
            ui->listWidget->addItem(new QListWidgetItem(*m_collecticon,rnames[i]));
        }
        else
        {
            ui->listWidget->addItem(new QListWidgetItem(*m_discollecticon,rnames[i]));
        }
    }
}

void MusicWebRadioWidget::updateRecentList()
{
    clearAllItems();
    ui->listWidget->clear();
    QStringList fnames = m_database->getFavouriteNames();
    QStringList rnames = m_database->getRecentNames();
    for(int i=0; i<rnames.count(); ++i)
    {
        if(fnames.contains(rnames[i]))
        {
            ui->listWidget->addItem(new QListWidgetItem(*m_collecticon,rnames[i]));
        }
        else
        {
            ui->listWidget->addItem(new QListWidgetItem(*m_discollecticon,rnames[i]));
        }
    }
}

void MusicWebRadioWidget::updateFavouriteList()
{
    clearAllItems();
    ui->listWidget->clear();
    QStringList fnames = m_database->getFavouriteNames();
    for(int i=0; i<fnames.count(); ++i)
    {
        ui->listWidget->addItem(new QListWidgetItem(*m_collecticon,fnames[i]));
    }
}

void MusicWebRadioWidget::itemHasDoubleClicked(QListWidgetItem *item)
{
    m_radioUrl = m_database->getRadioUrl(m_currentRadioName = item->text().trimmed());
    radioPlay();
}

void MusicWebRadioWidget::radioStandardOutput()
{
    while(m_radio->canReadLine())
    {
        QString message(m_radio->readLine());
        QStringList messagelist = message.split(" ");
        if(messagelist[0] == "Starting")
        {
            ui->stateLabel->setText(m_currentRadioName);
            m_database->radioRecentPlay(m_currentRadioName);
        }
    }
}

void MusicWebRadioWidget::radioColletButton()
{
    QListWidgetItem* lItem = ui->listWidget->currentItem();
    if(!lItem)
    {
        return;
    }
    lItem->setIcon(*m_collecticon);
    m_database->radioCollection(lItem->text());
}

void MusicWebRadioWidget::radioDiscolletButton()
{
    QListWidgetItem* lItem = ui->listWidget->currentItem();
    if(!lItem)
    {
        return;
    }
    lItem->setIcon(*m_discollecticon);
    m_database->radioDiscollection(lItem->text());
}

void MusicWebRadioWidget::show()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    MusicAbstractMoveWidget::show();
}
