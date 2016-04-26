#include "musicwebentainradiowidget.h"
#include "ui_musicwebentainradiowidget.h"
#include "musicwebentainradiodatebase.h"
#include "musicbackgroundmanager.h"
#include "musiccoremplayer.h"

#include <QProcess>

MusicWebEntainRadioWidget::MusicWebEntainRadioWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicWebEntainRadioWidget), m_radio(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_database = new MusicWebEntainRadioDatabase;
    m_database->connectDatabase();

    ui->playButton->setIcon(QIcon(":/image/play"));
    ui->stopButton->setIcon(QIcon(":/image/stop"));
    ui->colletButton->setIcon(QIcon(":/radio/collect"));
    ui->discolletButton->setIcon(QIcon(":/radio/discollect"));
    ui->playButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->stopButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->colletButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->discolletButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->playButton->setIconSize(QSize(31, 31));
    ui->stopButton->setIconSize(QSize(31, 31));
    ui->colletButton->setIconSize(QSize(31, 31));
    ui->discolletButton->setIconSize(QSize(31, 31));
    ui->playButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->stopButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->colletButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->discolletButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->playButton->setToolTip(tr("Play"));
    ui->stopButton->setToolTip(tr("Stop"));
    ui->colletButton->setToolTip(tr("Colletion"));
    ui->discolletButton->setToolTip(tr("Discolletion"));
    ui->volumeSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(100);

    connect(ui->playButton, SIGNAL(clicked()), SLOT(radioPlay()));
    connect(ui->stopButton, SIGNAL(clicked()), SLOT(radioStop()));
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), SLOT(radioVolume(int)));
    connect(ui->colletButton, SIGNAL(clicked()), SLOT(radioColletButton()));
    connect(ui->discolletButton, SIGNAL(clicked()), SLOT(radioDiscolletButton()));

    ui->movieLabel->setPixmap(QPixmap(":/radio/radiopng1").scaled(455, 60));
    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));
    m_timerCount = 1;
    m_collecticon = new QIcon(":/radio/collect");
    m_discollecticon = new QIcon(":/radio/discollect");

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                            SLOT(itemHasDoubleClicked(QListWidgetItem*)));

}

MusicWebEntainRadioWidget::~MusicWebEntainRadioWidget()
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

void MusicWebEntainRadioWidget::closeEvent(QCloseEvent *event)
{
    m_timer.stop();
    delete m_radio;
    m_radio = nullptr;
    QWidget::closeEvent(event);
}

void MusicWebEntainRadioWidget::timeout()
{
    ++m_timerCount;
    ui->movieLabel->setPixmap(QPixmap(":/radio/radiopng" +
                              QString::number(m_timerCount)).scaled(455, 60));
    if(m_timerCount >= 4)
    {
        m_timerCount = 0;
    }
}

void MusicWebEntainRadioWidget::radioPlay()
{
    if(m_radioUrl.isEmpty())
    {
        return;
    }

    delete m_radio;
    m_radio = new MusicCoreMPlayer(this);
    connect(m_radio, SIGNAL(radioChanged()), SLOT(radioStandardOutput()));
    m_radio->setMedia(MusicCoreMPlayer::RadioCategory, m_radioUrl, -1);
    m_radio->setVolume(ui->volumeSlider->value());

    ui->stateLabel->setText(tr("Connecting..."));
    m_timer.start(100);
}

void MusicWebEntainRadioWidget::radioStop()
{
    if(m_radio)
    {
        m_radio->stop();
        m_timer.stop();
    }
}

void MusicWebEntainRadioWidget::radioVolume(int num)
{
    if(m_radio)
    {
        m_radio->setVolume(num);
    }
}

void MusicWebEntainRadioWidget::clearAllItems()
{
    ui->listWidget->clear();
}

void MusicWebEntainRadioWidget::updateRadioList(const QString &category)
{
    clearAllItems();
    ui->listWidget->clear();
    QStringList fnames = m_database->getFavouriteNames();
    QStringList rnames = m_database->getRadioNames(category);
    foreach(QString name, rnames)
    {
        ui->listWidget->addItem(new QListWidgetItem(fnames.contains(name)
                                                    ? *m_collecticon
                                                    : *m_discollecticon, name));
    }
}

void MusicWebEntainRadioWidget::updateRecentList()
{
    clearAllItems();
    ui->listWidget->clear();
    QStringList fnames = m_database->getFavouriteNames();
    QStringList rnames = m_database->getRecentNames();
    foreach(QString name, rnames)
    {
        ui->listWidget->addItem(new QListWidgetItem(fnames.contains(name)
                                                    ? *m_collecticon
                                                    : *m_discollecticon, name));
    }
}

void MusicWebEntainRadioWidget::updateFavouriteList()
{
    clearAllItems();
    ui->listWidget->clear();
    foreach(QString name, m_database->getFavouriteNames())
    {
        ui->listWidget->addItem(new QListWidgetItem(*m_collecticon, name));
    }
}

void MusicWebEntainRadioWidget::itemHasDoubleClicked(QListWidgetItem *item)
{
    m_radioUrl = m_database->getRadioUrl(m_currentRadioName = item->text().trimmed());
    radioPlay();
}

void MusicWebEntainRadioWidget::radioStandardOutput()
{
    ui->stateLabel->setText(m_currentRadioName);
    m_database->radioRecentPlay(m_currentRadioName);
}

void MusicWebEntainRadioWidget::radioColletButton()
{
    QListWidgetItem* lItem = ui->listWidget->currentItem();
    if(!lItem)
    {
        return;
    }
    lItem->setIcon(*m_collecticon);
    m_database->radioCollection(lItem->text());
}

void MusicWebEntainRadioWidget::radioDiscolletButton()
{
    QListWidgetItem* lItem = ui->listWidget->currentItem();
    if(!lItem)
    {
        return;
    }
    lItem->setIcon(*m_discollecticon);
    m_database->radioDiscollection(lItem->text());
}

void MusicWebEntainRadioWidget::show()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    MusicAbstractMoveWidget::show();
}
