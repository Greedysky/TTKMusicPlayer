#include "musictimerwidget.h"
#include "ui_musictimerwidget.h"
#include <QSettings>

MusicTimerWidget::MusicTimerWidget(QWidget *parent)
        : MusicMoveDialogAbstract(parent),
        ui(new Ui::MusicTimerWidget)
{
    ui->setupUi(this);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),4,4);
    setMask(bmp);
    //set window radius

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    connect(ui->timerToPlay,SIGNAL(clicked()),SLOT(changeFirstWidget()));
    connect(ui->timerToStop,SIGNAL(clicked()),SLOT(changeSecondWidget()));
    connect(ui->timerToShutdown,SIGNAL(clicked()),SLOT(changeThreeWidget()));

    ui->timerToPlay->setIcon(QIcon(":/control/timerPlay"));
    ui->timerToStop->setIcon(QIcon(":/control/timerStop"));
    ui->timerToShutdown->setIcon(QIcon(":/control/timerDown"));
    ui->timerToPlay->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->timerToStop->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->timerToShutdown->setStyleSheet(MusicObject::MusicSettingMainButton);
    ui->confirm->setStyleSheet(MusicObject::MusicSettingSubmitButton);
    ui->cancel->setStyleSheet(MusicObject::MusicSettingSubmitButton);
    ui->timerToPlay->setCursor(QCursor(Qt::PointingHandCursor));
    ui->timerToStop->setCursor(QCursor(Qt::PointingHandCursor));
    ui->timerToShutdown->setCursor(QCursor(Qt::PointingHandCursor));
    ui->confirm->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancel->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->timerToPlay,SIGNAL(clicked()),SLOT(changeFirstWidget()));
    connect(ui->timerToStop,SIGNAL(clicked()),SLOT(changeSecondWidget()));
    connect(ui->timerToShutdown,SIGNAL(clicked()),SLOT(changeThreeWidget()));
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(commitTheResults()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(close()));

    initComboParameter();
    initFirstWidget();
    initSecondWidget();
    initThreeWidget();
    initParemeter();
}

MusicTimerWidget::~MusicTimerWidget()
{
    delete ui;
}

void MusicTimerWidget::initParemeter()
{
    QSettings setting;
    if(setting.value("TIMERAUTOPLAYCHOICED").toInt() == 1)
    {
        ui->noSetRadioButton1->setChecked(true);
        setEnabledControlFalse1();
    }
    else
        ui->setRadioButton1->setChecked(true);
    ui->hourComboBox1->setCurrentIndex(setting.value("TIMERAUTOPLAYHOURCHOICED").toInt());
    ui->secComboBox1->setCurrentIndex(setting.value("TIMERAUTOPLAYSECONDCHOICED").toInt());
    ui->repeatComboBox1->setCurrentIndex(setting.value("TIMERAUTOPLAYREPEATCHOICED").toInt());
    ui->plistComboBox->setCurrentIndex(setting.value("TIMERAUTOPLAYITEMINDEXCHOICED").toInt());

    if(setting.value("TIMERAUTOSTOPCHOICED").toInt() == 1)
    {
        ui->noSetRadioButton2->setChecked(true);
        setEnabledControlFalse2();
    }
    else
        ui->setRadioButton2->setChecked(true);
    ui->hourComboBox2->setCurrentIndex(setting.value("TIMERAUTOSTOPHOURCHOICED").toInt());
    ui->secComboBox2->setCurrentIndex(setting.value("TIMERAUTOSTOPSECONDCHOICED").toInt());
    ui->repeatComboBox2->setCurrentIndex(setting.value("TIMERAUTOSTOPREPEATCHOICED").toInt());

    if(setting.value("TIMERAUTOSHUTDOWNCHOICED").toInt() == 1)
    {
        ui->noSetRadioButton3->setChecked(true);
        setEnabledControlFalse3();
    }
    else
        ui->setRadioButton3->setChecked(true);
    ui->hourComboBox3->setCurrentIndex(setting.value("TIMERAUTOSHUTDOWNHOURCHOICED").toInt());
    ui->secComboBox3->setCurrentIndex(setting.value("TIMERAUTOSHUTDOWNSECONDCHOICED").toInt());
    ui->repeatComboBox3->setCurrentIndex(setting.value("TIMERAUTOSHUTDOWNREPEATCHOICED").toInt());
}

void MusicTimerWidget::writeParemeter()
{
    QSettings setting;
    setting.setValue("TIMERAUTOINDEXCHOICED",
                     ui->stackedWidget->currentIndex());
    setting.setValue("TIMERAUTOPLAYCHOICED",
                     ui->noSetRadioButton1->isChecked() ? 1 : 0);
    setting.setValue("TIMERAUTOPLAYHOURCHOICED",
                     ui->hourComboBox1->currentIndex());
    setting.setValue("TIMERAUTOPLAYSECONDCHOICED",
                     ui->secComboBox1->currentIndex());
    setting.setValue("TIMERAUTOPLAYREPEATCHOICED",
                     ui->repeatComboBox1->currentIndex());
    setting.setValue("TIMERAUTOPLAYITEMINDEXCHOICED",
                     ui->plistComboBox->currentIndex());
    setting.setValue("TIMERAUTOPLAYSONGINDEXCHOICED",
                     ui->psongComboBox->currentIndex());
    setting.setValue("TIMERAUTOSTOPCHOICED",
                     ui->noSetRadioButton2->isChecked() ? 1 : 0);
    setting.setValue("TIMERAUTOSTOPHOURCHOICED",
                     ui->hourComboBox2->currentIndex());
    setting.setValue("TIMERAUTOSTOPSECONDCHOICED",
                     ui->secComboBox2->currentIndex());
    setting.setValue("TIMERAUTOSTOPREPEATCHOICED",
                     ui->repeatComboBox2->currentIndex());
    setting.setValue("TIMERAUTOSHUTDOWNCHOICED",
                     ui->noSetRadioButton3->isChecked() ? 1 : 0);
    setting.setValue("TIMERAUTOSHUTDOWNHOURCHOICED",
                     ui->hourComboBox3->currentIndex());
    setting.setValue("TIMERAUTOSHUTDOWNSECONDCHOICED",
                     ui->secComboBox3->currentIndex());
    setting.setValue("TIMERAUTOSHUTDOWNREPEATCHOICED",
                     ui->repeatComboBox3->currentIndex());
    setting.sync();
}

void MusicTimerWidget::setSongStringList(const QStringList& list)
{
    ui->psongComboBox->addItems(list);
    ui->psongComboBox->setCurrentIndex(QSettings().value("TIMERAUTOPLAYSONGINDEXCHOICED").toInt());
}

void MusicTimerWidget::initComboParameter()
{
    for(int i=0; i<24; ++i)
        m_hour<<tr("%1H").arg(i);
    for(int i=0; i<60; ++i)
        m_second<<tr("%1S").arg(i);
    m_repeat<<tr("once")<<tr("evMonth")<<tr("evWeek")<<tr("evDay");
}

void MusicTimerWidget::changeFirstWidget()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MusicTimerWidget::changeSecondWidget()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MusicTimerWidget::changeThreeWidget()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MusicTimerWidget::initFirstWidget()
{
//    ui->noSetRadioButton1->setChecked(true);
    ui->hourComboBox1->addItems(m_hour);
    ui->secComboBox1->addItems(m_second);
    ui->repeatComboBox1->addItems(m_repeat);
    ui->plistComboBox->addItem(tr("defualt"));
    ui->noSetRadioButton1->setStyleSheet(MusicObject::MusicSettingRadioButton);
    ui->setRadioButton1->setStyleSheet(MusicObject::MusicSettingRadioButton);
    ui->hourComboBox1->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    ui->secComboBox1->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    ui->repeatComboBox1->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    ui->plistComboBox->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    ui->psongComboBox->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    connect(ui->noSetRadioButton1,SIGNAL(clicked()),SLOT(setEnabledControlFalse1()));
    connect(ui->setRadioButton1,SIGNAL(clicked()),SLOT(setEnabledControlTrue1()));
}

void MusicTimerWidget::initSecondWidget()
{
//    ui->noSetRadioButton2->setChecked(true);
    ui->hourComboBox2->addItems(m_hour);
    ui->secComboBox2->addItems(m_second);
    ui->repeatComboBox2->addItems(m_repeat);
    ui->noSetRadioButton2->setStyleSheet(MusicObject::MusicSettingRadioButton);
    ui->setRadioButton2->setStyleSheet(MusicObject::MusicSettingRadioButton);
    ui->hourComboBox2->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    ui->secComboBox2->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    ui->repeatComboBox2->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    connect(ui->noSetRadioButton2,SIGNAL(clicked()),SLOT(setEnabledControlFalse2()));
    connect(ui->setRadioButton2,SIGNAL(clicked()),SLOT(setEnabledControlTrue2()));
}

void MusicTimerWidget::initThreeWidget()
{
//    ui->noSetRadioButton3->setChecked(true);
    ui->hourComboBox3->addItems(m_hour);
    ui->secComboBox3->addItems(m_second);
    ui->repeatComboBox3->addItems(m_repeat);
    ui->noSetRadioButton3->setStyleSheet(MusicObject::MusicSettingRadioButton);
    ui->setRadioButton3->setStyleSheet(MusicObject::MusicSettingRadioButton);
    ui->hourComboBox3->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    ui->secComboBox3->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    ui->repeatComboBox3->setStyleSheet(MusicObject::MusicEqComboboxEditStyle);
    connect(ui->noSetRadioButton3,SIGNAL(clicked()),SLOT(setEnabledControlFalse3()));
    connect(ui->setRadioButton3,SIGNAL(clicked()),SLOT(setEnabledControlTrue3()));
}

void MusicTimerWidget::commitTheResults()
{
    writeParemeter();
    emit timerParameterChanged();
    close();
}

void MusicTimerWidget::setEnabledControlFalse1()
{
    ui->hourComboBox1->setEnabled(false);
    ui->secComboBox1->setEnabled(false);
    ui->repeatComboBox1->setEnabled(false);
    ui->plistComboBox->setEnabled(false);
    ui->psongComboBox->setEnabled(false);
}

void MusicTimerWidget::setEnabledControlTrue1()
{
    ui->hourComboBox1->setEnabled(true);
    ui->secComboBox1->setEnabled(true);
    ui->repeatComboBox1->setEnabled(true);
    ui->plistComboBox->setEnabled(true);
    ui->psongComboBox->setEnabled(true);
}

void MusicTimerWidget::setEnabledControlFalse2()
{
    ui->hourComboBox2->setEnabled(false);
    ui->secComboBox2->setEnabled(false);
    ui->repeatComboBox2->setEnabled(false);
}

void MusicTimerWidget::setEnabledControlTrue2()
{
    ui->hourComboBox2->setEnabled(true);
    ui->secComboBox2->setEnabled(true);
    ui->repeatComboBox2->setEnabled(true);
}

void MusicTimerWidget::setEnabledControlFalse3()
{
    ui->hourComboBox3->setEnabled(false);
    ui->secComboBox3->setEnabled(false);
    ui->repeatComboBox3->setEnabled(false);
}

void MusicTimerWidget::setEnabledControlTrue3()
{
    ui->hourComboBox3->setEnabled(true);
    ui->secComboBox3->setEnabled(true);
    ui->repeatComboBox3->setEnabled(true);
}
