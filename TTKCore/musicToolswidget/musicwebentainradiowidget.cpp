#include "musicwebentainradiowidget.h"
#include "ui_musicwebentainradiowidget.h"
#include "musicwebentainradiodatebase.h"
#include "musiccoremplayer.h"

#include <QProcess>

MusicWebEntainRadioWidget::MusicWebEntainRadioWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicWebEntainRadioWidget), m_radio(nullptr)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_database = new MusicWebEntainRadioDatabase;
    m_database->connectDatabase();

    m_ui->playButton->setIcon(QIcon(":/functions/btn_play_hover"));
    m_ui->stopButton->setIcon(QIcon(":/functions/btn_pause_hover"));
    m_ui->colletButton->setIcon(QIcon(":/tiny/lb_star"));
    m_ui->discolletButton->setIcon(QIcon(":/tiny/lb_unstar"));

    m_ui->playButton->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    m_ui->stopButton->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    m_ui->colletButton->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    m_ui->discolletButton->setStyleSheet(MusicUIObject::MBackgroundStyle01);

    m_ui->playButton->setIconSize(QSize(31, 31));
    m_ui->stopButton->setIconSize(QSize(31, 31));
    m_ui->colletButton->setIconSize(QSize(31, 31));
    m_ui->discolletButton->setIconSize(QSize(31, 31));

    m_ui->playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->stopButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->colletButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->discolletButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->playButton->setToolTip(tr("Play"));
    m_ui->stopButton->setToolTip(tr("Stop"));
    m_ui->colletButton->setToolTip(tr("Colletion"));
    m_ui->discolletButton->setToolTip(tr("Discolletion"));

    m_ui->volumeSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    m_ui->volumeSlider->setRange(0, 100);
    m_ui->volumeSlider->setValue(100);

    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(radioPlay()));
    connect(m_ui->stopButton, SIGNAL(clicked()), SLOT(radioStop()));
    connect(m_ui->volumeSlider, SIGNAL(valueChanged(int)), SLOT(radioVolume(int)));
    connect(m_ui->colletButton, SIGNAL(clicked()), SLOT(radioColletButton()));
    connect(m_ui->discolletButton, SIGNAL(clicked()), SLOT(radioDiscolletButton()));

    m_ui->movieLabel->setPixmap(QPixmap(":/radio/lb_radiopng1").scaled(455, 60));
    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));
    m_timerCount = 1;
    m_collecticon = new QIcon(":/tiny/lb_star");
    m_discollecticon = new QIcon(":/tiny/lb_unstar");

    connect(m_ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
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
    delete m_ui;
}

QString MusicWebEntainRadioWidget::getClassName()
{
    return staticMetaObject.className();
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
    m_ui->movieLabel->setPixmap(QPixmap(":/radio/lb_radiopng" +
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
    m_radio->setMedia(MusicCoreMPlayer::RadioCategory, m_radioUrl);
    m_radio->setVolume(m_ui->volumeSlider->value());

    m_ui->stateLabel->setText(tr("Connecting..."));
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
    m_ui->listWidget->clear();
}

void MusicWebEntainRadioWidget::updateRadioList(const QString &category)
{
    clearAllItems();
    m_ui->listWidget->clear();
    QStringList fnames = m_database->getFavouriteNames();
    QStringList rnames = m_database->getRadioNames(category);
    foreach(const QString &name, rnames)
    {
        m_ui->listWidget->addItem(new QListWidgetItem(fnames.contains(name)
                                                    ? *m_collecticon
                                                    : *m_discollecticon, name));
    }
}

void MusicWebEntainRadioWidget::updateRecentList()
{
    clearAllItems();
    m_ui->listWidget->clear();
    QStringList fnames = m_database->getFavouriteNames();
    QStringList rnames = m_database->getRecentNames();
    foreach(const QString &name, rnames)
    {
        m_ui->listWidget->addItem(new QListWidgetItem(fnames.contains(name)
                                                    ? *m_collecticon
                                                    : *m_discollecticon, name));
    }
}

void MusicWebEntainRadioWidget::updateFavouriteList()
{
    clearAllItems();
    m_ui->listWidget->clear();
    foreach(const QString &name, m_database->getFavouriteNames())
    {
        m_ui->listWidget->addItem(new QListWidgetItem(*m_collecticon, name));
    }
}

void MusicWebEntainRadioWidget::itemHasDoubleClicked(QListWidgetItem *item)
{
    m_radioUrl = m_database->getRadioUrl(m_currentRadioName = item->text().trimmed());
    radioPlay();
}

void MusicWebEntainRadioWidget::radioStandardOutput()
{
    m_ui->stateLabel->setText(m_currentRadioName);
    m_database->radioRecentPlay(m_currentRadioName);
}

void MusicWebEntainRadioWidget::radioColletButton()
{
    QListWidgetItem* lItem = m_ui->listWidget->currentItem();
    if(!lItem)
    {
        return;
    }
    lItem->setIcon(*m_collecticon);
    m_database->radioCollection(lItem->text());
}

void MusicWebEntainRadioWidget::radioDiscolletButton()
{
    QListWidgetItem* lItem = m_ui->listWidget->currentItem();
    if(!lItem)
    {
        return;
    }
    lItem->setIcon(*m_discollecticon);
    m_database->radioDiscollection(lItem->text());
}

void MusicWebEntainRadioWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}
