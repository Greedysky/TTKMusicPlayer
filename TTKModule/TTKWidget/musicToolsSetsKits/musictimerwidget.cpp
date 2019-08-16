#include "musictimerwidget.h"
#include "ui_musictimerwidget.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicnumberdefine.h"
#include "musicapplicationobject.h"
#include "musicotherdefine.h"
#include "musictoolsetsuiobject.h"

#include <QButtonGroup>
#include <QAbstractItemView>
#include <QStyledItemDelegate>

MusicTimerWidget::MusicTimerWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicTimerWidget)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->timerToPlay->setStyleSheet(MusicUIObject::MKGTimeToPlay);
    m_ui->timerToStop->setStyleSheet(MusicUIObject::MKGTimeToStop);
    m_ui->timerToShutdown->setStyleSheet(MusicUIObject::MKGTimeToDown);
    m_ui->confirm->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->cancel->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->timerToPlay->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->timerToStop->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->timerToShutdown->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->confirm->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancel->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_ui->confirm, SIGNAL(clicked()), SLOT(commitTheResults()));
    connect(m_ui->cancel, SIGNAL(clicked()), SLOT(close()));

#ifdef Q_OS_UNIX
    m_ui->timerToPlay->setFocusPolicy(Qt::NoFocus);
    m_ui->timerToStop->setFocusPolicy(Qt::NoFocus);
    m_ui->timerToShutdown->setFocusPolicy(Qt::NoFocus);
    m_ui->confirm->setFocusPolicy(Qt::NoFocus);
    m_ui->cancel->setFocusPolicy(Qt::NoFocus);
#endif

    initComboParameter();
    initFirstWidget();
    initSecondWidget();
    initThreeWidget();
    initParemeter();

    QButtonGroup *group0 = new QButtonGroup(this);
    group0->addButton(m_ui->timerToPlay, 0);
    group0->addButton(m_ui->timerToStop, 1);
    group0->addButton(m_ui->timerToShutdown, 2);
    connect(group0, SIGNAL(buttonClicked(int)), SLOT(changeWidgetIndex(int)));

    QButtonGroup *group1 = new QButtonGroup(this);
    group1->addButton(m_ui->noSetRadioButton1, 0);
    group1->addButton(m_ui->setRadioButton1, 1);
    connect(group1, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

    QButtonGroup *group2 = new QButtonGroup(this);
    group2->addButton(m_ui->noSetRadioButton2, 2);
    group2->addButton(m_ui->setRadioButton2, 3);
    connect(group2, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

    QButtonGroup *group3 = new QButtonGroup(this);
    group3->addButton(m_ui->noSetRadioButton3, 4);
    group3->addButton(m_ui->setRadioButton3, 5);
    connect(group3, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));

    changeWidgetIndex(0);
}

MusicTimerWidget::~MusicTimerWidget()
{
    delete m_ui;
}

void MusicTimerWidget::initParemeter()
{
    if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayChoiced).toInt() == 1)
    {
        m_ui->noSetRadioButton1->setChecked(true);
        setEnabledFirstControl(false);
    }
    else
    {
        m_ui->setRadioButton1->setChecked(true);
    }
    m_ui->hourComboBox1->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayHourChoiced).toInt());
    m_ui->secComboBox1->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySecondChoiced).toInt());
    m_ui->repeatComboBox1->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayRepeatChoiced).toInt());
    m_ui->plistComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayItemIndexChoiced).toInt());

    if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopChoiced).toInt() == 1)
    {
        m_ui->noSetRadioButton2->setChecked(true);
        setEnabledSecondControl(false);
    }
    else
    {
        m_ui->setRadioButton2->setChecked(true);
    }
    m_ui->hourComboBox2->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopHourChoiced).toInt());
    m_ui->secComboBox2->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopSecondChoiced).toInt());
    m_ui->repeatComboBox2->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopRepeatChoiced).toInt());

    if(M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownChoiced).toInt() == 1)
    {
        m_ui->noSetRadioButton3->setChecked(true);
        setEnabledThreeControl(false);
    }
    else
    {
        m_ui->setRadioButton3->setChecked(true);
    }
    m_ui->hourComboBox3->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownHourChoiced).toInt());
    m_ui->secComboBox3->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownSecondChoiced).toInt());
    m_ui->repeatComboBox3->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownRepeatChoiced).toInt());
}

void MusicTimerWidget::writeParemeter() const
{
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoIndexChoiced, m_ui->stackedWidget->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayChoiced, m_ui->noSetRadioButton1->isChecked() ? 1 : 0);
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayHourChoiced, m_ui->hourComboBox1->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlaySecondChoiced, m_ui->secComboBox1->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayRepeatChoiced, m_ui->repeatComboBox1->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayItemIndexChoiced, m_ui->plistComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlaySongIndexChoiced, m_ui->psongComboBox->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopChoiced, m_ui->noSetRadioButton2->isChecked() ? 1 : 0);
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopHourChoiced, m_ui->hourComboBox2->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopSecondChoiced, m_ui->secComboBox2->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopRepeatChoiced, m_ui->repeatComboBox2->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownChoiced, m_ui->noSetRadioButton3->isChecked() ? 1 : 0);
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownHourChoiced, m_ui->hourComboBox3->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownSecondChoiced, m_ui->secComboBox3->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownRepeatChoiced, m_ui->repeatComboBox3->currentIndex());
}

void MusicTimerWidget::setSongStringList(const QStringList &list)
{
    m_ui->psongComboBox->addItems(list);
    m_ui->psongComboBox->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySongIndexChoiced).toInt());
}

void MusicTimerWidget::initComboParameter()
{
    for(int i=0; i<MT_D; ++i)
    {
        m_hour << tr("%1H").arg(i);
    }
    for(int i=0; i<MT_H; ++i)
    {
        m_second << tr("%1S").arg(i);
    }
    m_repeat << tr("once") << tr("evMonth") << tr("evWeek") << tr("evDay");
}

void MusicTimerWidget::changeWidgetIndex(int index)
{
    m_ui->timerToPlay->setStyleSheet(MusicUIObject::MKGTimeToPlay);
    m_ui->timerToStop->setStyleSheet(MusicUIObject::MKGTimeToStop);
    m_ui->timerToShutdown->setStyleSheet(MusicUIObject::MKGTimeToDown);

    switch(index)
    {
        case 0: m_ui->timerToPlay->setStyleSheet(MusicUIObject::MKGTimeToPlayClicked); break;
        case 1: m_ui->timerToStop->setStyleSheet(MusicUIObject::MKGTimeToStopClicked); break;
        case 2: m_ui->timerToShutdown->setStyleSheet(MusicUIObject::MKGTimeToDownClicked); break;
        default: break;
    }

    m_ui->stackedWidget->setCurrentIndex(index);
}

void MusicTimerWidget::initFirstWidget()
{
    m_ui->hourComboBox1->addItems(m_hour);
    m_ui->secComboBox1->addItems(m_second);
    m_ui->repeatComboBox1->addItems(m_repeat);
    m_ui->plistComboBox->addItem(tr("defualt"));
    m_ui->noSetRadioButton1->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->setRadioButton1->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->hourComboBox1->setItemDelegate(new QStyledItemDelegate(m_ui->hourComboBox1));
    m_ui->hourComboBox1->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->hourComboBox1->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->secComboBox1->setItemDelegate(new QStyledItemDelegate(m_ui->secComboBox1));
    m_ui->secComboBox1->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->secComboBox1->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->repeatComboBox1->setItemDelegate(new QStyledItemDelegate(m_ui->repeatComboBox1));
    m_ui->repeatComboBox1->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->repeatComboBox1->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->plistComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->plistComboBox));
    m_ui->plistComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->plistComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->psongComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->psongComboBox));
    m_ui->psongComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->psongComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
#ifdef Q_OS_UNIX
    m_ui->noSetRadioButton1->setFocusPolicy(Qt::NoFocus);
    m_ui->setRadioButton1->setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicTimerWidget::initSecondWidget()
{
    m_ui->hourComboBox2->addItems(m_hour);
    m_ui->secComboBox2->addItems(m_second);
    m_ui->repeatComboBox2->addItems(m_repeat);
    m_ui->noSetRadioButton2->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->setRadioButton2->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->hourComboBox2->setItemDelegate(new QStyledItemDelegate(m_ui->hourComboBox2));
    m_ui->hourComboBox2->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->hourComboBox2->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->secComboBox2->setItemDelegate(new QStyledItemDelegate(m_ui->secComboBox2));
    m_ui->secComboBox2->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->secComboBox2->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->repeatComboBox2->setItemDelegate(new QStyledItemDelegate(m_ui->repeatComboBox2));
    m_ui->repeatComboBox2->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->repeatComboBox2->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
#ifdef Q_OS_UNIX
    m_ui->noSetRadioButton2->setFocusPolicy(Qt::NoFocus);
    m_ui->setRadioButton2->setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicTimerWidget::initThreeWidget()
{
    m_ui->hourComboBox3->addItems(m_hour);
    m_ui->secComboBox3->addItems(m_second);
    m_ui->repeatComboBox3->addItems(m_repeat);
    m_ui->noSetRadioButton3->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->setRadioButton3->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    m_ui->hourComboBox3->setItemDelegate(new QStyledItemDelegate(m_ui->hourComboBox3));
    m_ui->hourComboBox3->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->hourComboBox3->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->secComboBox3->setItemDelegate(new QStyledItemDelegate(m_ui->secComboBox3));
    m_ui->secComboBox3->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->secComboBox3->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->repeatComboBox3->setItemDelegate(new QStyledItemDelegate(m_ui->repeatComboBox3));
    m_ui->repeatComboBox3->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->repeatComboBox3->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
#ifdef Q_OS_UNIX
    m_ui->noSetRadioButton3->setFocusPolicy(Qt::NoFocus);
    m_ui->setRadioButton3->setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicTimerWidget::commitTheResults()
{
    writeParemeter();
    MusicApplicationObject::instance()->musicToolSetsParameter();
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
    m_ui->hourComboBox1->setEnabled(enable);
    m_ui->secComboBox1->setEnabled(enable);
    m_ui->repeatComboBox1->setEnabled(enable);
    m_ui->plistComboBox->setEnabled(enable);
    m_ui->psongComboBox->setEnabled(enable);
}

void MusicTimerWidget::setEnabledSecondControl(bool enable)
{
    m_ui->hourComboBox2->setEnabled(enable);
    m_ui->secComboBox2->setEnabled(enable);
    m_ui->repeatComboBox2->setEnabled(enable);
}

void MusicTimerWidget::setEnabledThreeControl(bool enable)
{
    m_ui->hourComboBox3->setEnabled(enable);
    m_ui->secComboBox3->setEnabled(enable);
    m_ui->repeatComboBox3->setEnabled(enable);
}

int MusicTimerWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
