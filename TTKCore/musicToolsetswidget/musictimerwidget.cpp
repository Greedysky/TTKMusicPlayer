#include "musictimerwidget.h"
#include "ui_musictimerwidget.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicbackgroundmanager.h"
#include "musicconnectionpool.h"

#include <QStyledItemDelegate>
#include <QButtonGroup>

MusicTimerWidget::MusicTimerWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicTimerWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    connect(ui->timerToPlay, SIGNAL(clicked()), SLOT(changeFirstWidget()));
    connect(ui->timerToStop, SIGNAL(clicked()), SLOT(changeSecondWidget()));
    connect(ui->timerToShutdown, SIGNAL(clicked()), SLOT(changeThreeWidget()));

    ui->timerToPlay->setIcon(QIcon(":/control/timerPlay"));
    ui->timerToStop->setIcon(QIcon(":/control/timerStop"));
    ui->timerToShutdown->setIcon(QIcon(":/control/timerDown"));
    ui->timerToPlay->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->timerToStop->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->timerToShutdown->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->confirm->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->cancel->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->timerToPlay->setCursor(QCursor(Qt::PointingHandCursor));
    ui->timerToStop->setCursor(QCursor(Qt::PointingHandCursor));
    ui->timerToShutdown->setCursor(QCursor(Qt::PointingHandCursor));
    ui->confirm->setCursor(QCursor(Qt::PointingHandCursor));
    ui->cancel->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->timerToPlay, SIGNAL(clicked()), SLOT(changeFirstWidget()));
    connect(ui->timerToStop, SIGNAL(clicked()), SLOT(changeSecondWidget()));
    connect(ui->timerToShutdown, SIGNAL(clicked()), SLOT(changeThreeWidget()));
    connect(ui->confirm, SIGNAL(clicked()), SLOT(commitTheResults()));
    connect(ui->cancel, SIGNAL(clicked()), SLOT(close()));

    initComboParameter();
    initFirstWidget();
    initSecondWidget();
    initThreeWidget();
    initParemeter();

    QButtonGroup *group1 = new QButtonGroup(this);
    group1->addButton(ui->noSetRadioButton1, 0);
    group1->addButton(ui->setRadioButton1, 1);
    connect(group1, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

    QButtonGroup *group2 = new QButtonGroup(this);
    group2->addButton(ui->noSetRadioButton2, 2);
    group2->addButton(ui->setRadioButton2, 3);
    connect(group2, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

    QButtonGroup *group3 = new QButtonGroup(this);
    group3->addButton(ui->noSetRadioButton3, 4);
    group3->addButton(ui->setRadioButton3, 5);
    connect(group3, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

    M_CONNECTION->setValue("MusicTimerWidget", this);
    M_CONNECTION->poolConnect("MusicTimerWidget", "MusicApplicationObject");
    M_CONNECTION->poolConnect("MusicTimerWidget", "MusicApplication");
}

MusicTimerWidget::~MusicTimerWidget()
{
    M_CONNECTION->poolDisConnect("MusicTimerWidget");
    delete ui;
}

void MusicTimerWidget::initParemeter()
{
    if(M_SETTING->value(MusicSettingManager::TimerAutoPlayChoiced).toInt() == 1)
    {
        ui->noSetRadioButton1->setChecked(true);
        setEnabledFirstControl(false);
    }
    else
    {
        ui->setRadioButton1->setChecked(true);
    }
    ui->hourComboBox1->setCurrentIndex(M_SETTING->value(MusicSettingManager::TimerAutoPlayHourChoiced).toInt());
    ui->secComboBox1->setCurrentIndex(M_SETTING->value(MusicSettingManager::TimerAutoPlaySecondChoiced).toInt());
    ui->repeatComboBox1->setCurrentIndex(M_SETTING->value(MusicSettingManager::TimerAutoPlayRepeatChoiced).toInt());
    ui->plistComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::TimerAutoPlayItemIndexChoiced).toInt());

    if(M_SETTING->value(MusicSettingManager::TimerAutoStopChoiced).toInt() == 1)
    {
        ui->noSetRadioButton2->setChecked(true);
        setEnabledSecondControl(false);
    }
    else
    {
        ui->setRadioButton2->setChecked(true);
    }
    ui->hourComboBox2->setCurrentIndex(M_SETTING->value(MusicSettingManager::TimerAutoStopHourChoiced).toInt());
    ui->secComboBox2->setCurrentIndex(M_SETTING->value(MusicSettingManager::TimerAutoStopSecondChoiced).toInt());
    ui->repeatComboBox2->setCurrentIndex(M_SETTING->value(MusicSettingManager::TimerAutoStopRepeatChoiced).toInt());

    if(M_SETTING->value(MusicSettingManager::TimerAutoShutdownChoiced).toInt() == 1)
    {
        ui->noSetRadioButton3->setChecked(true);
        setEnabledThreeControl(false);
    }
    else
    {
        ui->setRadioButton3->setChecked(true);
    }
    ui->hourComboBox3->setCurrentIndex(M_SETTING->value(MusicSettingManager::TimerAutoShutdownHourChoiced).toInt());
    ui->secComboBox3->setCurrentIndex(M_SETTING->value(MusicSettingManager::TimerAutoShutdownSecondChoiced).toInt());
    ui->repeatComboBox3->setCurrentIndex(M_SETTING->value(MusicSettingManager::TimerAutoShutdownRepeatChoiced).toInt());
}

void MusicTimerWidget::writeParemeter() const
{
    M_SETTING->setValue(MusicSettingManager::TimerAutoIndexChoiced,
                     ui->stackedWidget->currentIndex());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayChoiced,
                     ui->noSetRadioButton1->isChecked() ? 1 : 0);
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayHourChoiced,
                     ui->hourComboBox1->currentIndex());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlaySecondChoiced,
                     ui->secComboBox1->currentIndex());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayRepeatChoiced,
                     ui->repeatComboBox1->currentIndex());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayItemIndexChoiced,
                     ui->plistComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlaySongIndexChoiced,
                     ui->psongComboBox->currentIndex());
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopChoiced,
                     ui->noSetRadioButton2->isChecked() ? 1 : 0);
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopHourChoiced,
                     ui->hourComboBox2->currentIndex());
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopSecondChoiced,
                     ui->secComboBox2->currentIndex());
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopRepeatChoiced,
                     ui->repeatComboBox2->currentIndex());
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownChoiced,
                     ui->noSetRadioButton3->isChecked() ? 1 : 0);
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownHourChoiced,
                     ui->hourComboBox3->currentIndex());
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownSecondChoiced,
                     ui->secComboBox3->currentIndex());
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownRepeatChoiced,
                     ui->repeatComboBox3->currentIndex());
}

void MusicTimerWidget::setSongStringList(const QStringList &list)
{
    ui->psongComboBox->addItems(list);
    ui->psongComboBox->setCurrentIndex(M_SETTING->value(MusicSettingManager::TimerAutoPlaySongIndexChoiced).toInt());
}

void MusicTimerWidget::initComboParameter()
{
    for(int i=0; i<24; ++i)
    {
        m_hour << tr("%1H").arg(i);
    }
    for(int i=0; i<60; ++i)
    {
        m_second << tr("%1S").arg(i);
    }
    m_repeat << tr("once") << tr("evMonth") << tr("evWeek") << tr("evDay");
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
    ui->hourComboBox1->addItems(m_hour);
    ui->secComboBox1->addItems(m_second);
    ui->repeatComboBox1->addItems(m_repeat);
    ui->plistComboBox->addItem(tr("defualt"));
    ui->noSetRadioButton1->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->setRadioButton1->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->hourComboBox1->setItemDelegate(new QStyledItemDelegate(ui->hourComboBox1));
    ui->hourComboBox1->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->hourComboBox1->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->secComboBox1->setItemDelegate(new QStyledItemDelegate(ui->secComboBox1));
    ui->secComboBox1->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->secComboBox1->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->repeatComboBox1->setItemDelegate(new QStyledItemDelegate(ui->repeatComboBox1));
    ui->repeatComboBox1->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->repeatComboBox1->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->plistComboBox->setItemDelegate(new QStyledItemDelegate(ui->plistComboBox));
    ui->plistComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->plistComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->psongComboBox->setItemDelegate(new QStyledItemDelegate(ui->psongComboBox));
    ui->psongComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->psongComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
}

void MusicTimerWidget::initSecondWidget()
{
    ui->hourComboBox2->addItems(m_hour);
    ui->secComboBox2->addItems(m_second);
    ui->repeatComboBox2->addItems(m_repeat);
    ui->noSetRadioButton2->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->setRadioButton2->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->hourComboBox2->setItemDelegate(new QStyledItemDelegate(ui->hourComboBox2));
    ui->hourComboBox2->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->hourComboBox2->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->secComboBox2->setItemDelegate(new QStyledItemDelegate(ui->secComboBox2));
    ui->secComboBox2->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->secComboBox2->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->repeatComboBox2->setItemDelegate(new QStyledItemDelegate(ui->repeatComboBox2));
    ui->repeatComboBox2->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->repeatComboBox2->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
}

void MusicTimerWidget::initThreeWidget()
{
    ui->hourComboBox3->addItems(m_hour);
    ui->secComboBox3->addItems(m_second);
    ui->repeatComboBox3->addItems(m_repeat);
    ui->noSetRadioButton3->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->setRadioButton3->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    ui->hourComboBox3->setItemDelegate(new QStyledItemDelegate(ui->hourComboBox3));
    ui->hourComboBox3->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->hourComboBox3->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->secComboBox3->setItemDelegate(new QStyledItemDelegate(ui->secComboBox3));
    ui->secComboBox3->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->secComboBox3->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->repeatComboBox3->setItemDelegate(new QStyledItemDelegate(ui->repeatComboBox3));
    ui->repeatComboBox3->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->repeatComboBox3->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
}

void MusicTimerWidget::commitTheResults()
{
    writeParemeter();
    emit timerParameterChanged();
    close();
}

void MusicTimerWidget::buttonClicked(int index)
{
    switch(index)
    {
        case 0: setEnabledFirstControl(false); break;
        case 1: setEnabledFirstControl(true); break;
        case 2: setEnabledSecondControl(false); break;
        case 3: setEnabledSecondControl(true); break;
        case 4: setEnabledThreeControl(false); break;
        case 5: setEnabledThreeControl(true); break;
        default: break;
    }
}

void MusicTimerWidget::setEnabledFirstControl(bool enable)
{
    ui->hourComboBox1->setEnabled(enable);
    ui->secComboBox1->setEnabled(enable);
    ui->repeatComboBox1->setEnabled(enable);
    ui->plistComboBox->setEnabled(enable);
    ui->psongComboBox->setEnabled(enable);
}

void MusicTimerWidget::setEnabledSecondControl(bool enable)
{
    ui->hourComboBox2->setEnabled(enable);
    ui->secComboBox2->setEnabled(enable);
    ui->repeatComboBox2->setEnabled(enable);
}

void MusicTimerWidget::setEnabledThreeControl(bool enable)
{
    ui->hourComboBox3->setEnabled(enable);
    ui->secComboBox3->setEnabled(enable);
    ui->repeatComboBox3->setEnabled(enable);
}

int MusicTimerWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
