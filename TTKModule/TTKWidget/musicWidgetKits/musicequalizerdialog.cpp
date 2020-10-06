#include "musicequalizerdialog.h"
#include "ui_musicequalizerdialog.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicmagicwidgetuiobject.h"
#include "musicconnectionpool.h"
#include "musicsoundeffectswidget.h"
#include "musicwidgetutils.h"
#include "musicplayer.h"
#include "musicclickedgroup.h"

#include <QAbstractItemView>
#include <QStyledItemDelegate>

MusicEqualizerDialog::MusicEqualizerDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicEqualizerDialog)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_eable = false;
    m_eqChoiceSelected = false;

    initialize();

    m_ui->eqChoice->setItemDelegate(new QStyledItemDelegate(m_ui->eqChoice));
    m_ui->eqChoice->setStyleSheet(MusicUIObject::MQSSComboBoxStyle01 + MusicUIObject::MQSSItemView01);
    m_ui->eqChoice->view()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);
    m_ui->eqChoice->addItems(QStringList() << tr("Custom") << tr("Default") << tr("Classical")
                                           << tr("Club") << tr("Dance") << tr("Bass") << tr("Soprano") << tr("BassSoprano")
                                           << tr("Headset") << tr("Hall") << tr("Scene") << tr("Pop") << tr("Repaglinide")
                                           << tr("Party") << tr("Rock") << tr("Oscar") << tr("Mild") << tr("SoftRock")
                                           << tr("Electronics"));
    connect(m_ui->eqChoice, SIGNAL(currentIndexChanged(int)), SLOT(eqChoiceIndexChanged(int)));

    m_ui->showEqButton->setStyleSheet(MusicUIObject::MQSSEqualizerOff);

#ifdef Q_OS_UNIX
    m_ui->showEqButton->setFocusPolicy(Qt::NoFocus);
    m_ui->resetButton->setFocusPolicy(Qt::NoFocus);
#endif

    setControlEnabled(false);
    initEqualizeValue();
    readEqInformation();

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicPlayer::getClassName());
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSoundEffectsWidget::getClassName());
}

MusicEqualizerDialog::~MusicEqualizerDialog()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    writeEqInformation();
    delete m_ui;
}

void MusicEqualizerDialog::initialize()
{
    MusicClickedGroup *clickedGroup = new MusicClickedGroup(this);
    connect(clickedGroup, SIGNAL(clicked(int)), SLOT(verticalSliderChanged(int)));

    QList<MusicClickedSlider*> sliders;
    sliders << m_ui->verticalSlider1;
    sliders << m_ui->verticalSlider2;
    sliders << m_ui->verticalSlider3;
    sliders << m_ui->verticalSlider4;
    sliders << m_ui->verticalSlider5;
    sliders << m_ui->verticalSlider6;
    sliders << m_ui->verticalSlider7;
    sliders << m_ui->verticalSlider8;
    sliders << m_ui->verticalSlider9;
    sliders << m_ui->verticalSlider10;
    sliders << m_ui->bwVerticalSlider;

    for(int i=0; i<sliders.count(); ++i)
    {
        MusicClickedSlider *slider = sliders[i];
        slider->setRange(-15, 15);
        slider->setStyleSheet(MusicUIObject::MQSSSliderStyle04);
        clickedGroup->mapped(slider);
    }

    connect(m_ui->showEqButton, SIGNAL(clicked()), SLOT(setEqEnable()));
    connect(m_ui->resetButton, SIGNAL(clicked()), SLOT(resetEq()));
    m_ui->resetButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);

#ifdef Q_OS_UNIX
    MusicUtils::Widget::setLabelFontSize(m_ui->showPerArea_21, 9);
    MusicUtils::Widget::setLabelFontSize(m_ui->showPerArea_22, 9);
    MusicUtils::Widget::setLabelFontSize(m_ui->showPerArea_23, 9);
    MusicUtils::Widget::setLabelFontSize(m_ui->showPerArea_24, 9);
    MusicUtils::Widget::setLabelFontSize(m_ui->showPerArea_25, 9);
    MusicUtils::Widget::setLabelFontSize(m_ui->showPerArea_26, 9);
    MusicUtils::Widget::setLabelFontSize(m_ui->showPerArea_27, 9);
    MusicUtils::Widget::setLabelFontSize(m_ui->showPerArea_28, 9);
    MusicUtils::Widget::setLabelFontSize(m_ui->showPerArea_29, 9);
    MusicUtils::Widget::setLabelFontSize(m_ui->showPerArea_30, 9);
    MusicUtils::Widget::setLabelFontSize(m_ui->showPerArea_31, 9);
#endif
}

void MusicEqualizerDialog::readEqInformation()
{
    if(M_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt())
    {
        m_ui->showEqButton->click();
    }

    const QStringList &eqValue = M_SETTING_PTR->value(MusicSettingManager::EqualizerValue).toString().split(",");
    if(eqValue.count() == 11)
    {
        if(M_SETTING_PTR->value(MusicSettingManager::EqualizerIndex).toInt() == 0)
        {
            m_ui->verticalSlider1->setValue(eqValue[1].toInt());
            m_ui->verticalSlider2->setValue(eqValue[2].toInt());
            m_ui->verticalSlider3->setValue(eqValue[3].toInt());
            m_ui->verticalSlider4->setValue(eqValue[4].toInt());
            m_ui->verticalSlider5->setValue(eqValue[5].toInt());
            m_ui->verticalSlider6->setValue(eqValue[6].toInt());
            m_ui->verticalSlider7->setValue(eqValue[7].toInt());
            m_ui->verticalSlider8->setValue(eqValue[8].toInt());
            m_ui->verticalSlider9->setValue(eqValue[9].toInt());
            m_ui->verticalSlider10->setValue(eqValue[10].toInt());
            m_ui->bwVerticalSlider->setValue(eqValue[0].toInt());
        }
        else
        {
           m_ui->eqChoice->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::EqualizerIndex).toInt());
        }
    }
    else
    {
        resetEq();
    }
}

void MusicEqualizerDialog::writeEqInformation() const
{
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnable, m_eable ? 1 : 0);
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerIndex, m_ui->eqChoice->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerValue,
          QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11").arg(
          m_ui->bwVerticalSlider->value()).arg(m_ui->verticalSlider1->value()).arg(
          m_ui->verticalSlider2->value()).arg(m_ui->verticalSlider3->value()).arg(
          m_ui->verticalSlider4->value()).arg(m_ui->verticalSlider5->value()).arg(
          m_ui->verticalSlider6->value()).arg(m_ui->verticalSlider7->value()).arg(
          m_ui->verticalSlider8->value()).arg(m_ui->verticalSlider9->value()).arg(
          m_ui->verticalSlider10->value()));
}

void MusicEqualizerDialog::verticalSliderChanged(int)
{
    if(!m_eqChoiceSelected)
    {
        m_ui->eqChoice->setCurrentIndex(0);
    }
    parameterSubmit();
}

void MusicEqualizerDialog::parameterSubmit()
{
    Q_EMIT setEqEffect(TTKIntList() << m_ui->bwVerticalSlider->value() << m_ui->verticalSlider1->value()
                                  << m_ui->verticalSlider2->value() << m_ui->verticalSlider3->value()
                                  << m_ui->verticalSlider4->value() << m_ui->verticalSlider5->value()
                                  << m_ui->verticalSlider6->value() << m_ui->verticalSlider7->value()
                                  << m_ui->verticalSlider8->value() << m_ui->verticalSlider9->value()
                                  << m_ui->verticalSlider10->value());
}

void MusicEqualizerDialog::setEqEnable()
{
    m_eable = !m_eable;
    Q_EMIT setEnabledEffect(m_eable);
    m_ui->showEqButton->setStyleSheet(!m_eable ? MusicUIObject::MQSSEqualizerOff : MusicUIObject::MQSSEqualizerOn);

    setControlEnabled(m_eable);
    if(m_eable)
    {
        parameterSubmit();
    }
}

void MusicEqualizerDialog::setControlEnabled(bool enable) const
{
    m_ui->bwVerticalSlider->setEnabled(enable);
    m_ui->verticalSlider1->setEnabled(enable);
    m_ui->verticalSlider2->setEnabled(enable);
    m_ui->verticalSlider3->setEnabled(enable);
    m_ui->verticalSlider4->setEnabled(enable);
    m_ui->verticalSlider5->setEnabled(enable);
    m_ui->verticalSlider6->setEnabled(enable);
    m_ui->verticalSlider7->setEnabled(enable);
    m_ui->verticalSlider8->setEnabled(enable);
    m_ui->verticalSlider9->setEnabled(enable);
    m_ui->verticalSlider10->setEnabled(enable);
    m_ui->eqChoice->setEnabled(enable);
    m_ui->resetButton->setEnabled(enable);
}

void MusicEqualizerDialog::resetEq()
{
    m_ui->eqChoice->setCurrentIndex(1);
    m_ui->bwVerticalSlider->setValue(0);
    m_ui->verticalSlider1->setValue(0);
    m_ui->verticalSlider2->setValue(0);
    m_ui->verticalSlider3->setValue(0);
    m_ui->verticalSlider4->setValue(0);
    m_ui->verticalSlider5->setValue(0);
    m_ui->verticalSlider6->setValue(0);
    m_ui->verticalSlider7->setValue(0);
    m_ui->verticalSlider8->setValue(0);
    m_ui->verticalSlider9->setValue(0);
    m_ui->verticalSlider10->setValue(0);

}

void MusicEqualizerDialog::initEqualizeValue()
{
    m_equalizeValue.clear();
    m_equalizeValue<< 0<< 0<< 0<< 0<< 0<< 0<< 0<< 0<< 0<< 0 //Default
                   << 0<< 0<< 0<< 0<< 0<< 0<<-4<<-4<<-4<<-6 //Classical
                   << 0<< 0<< 5<< 3<< 3<< 3<< 2<< 0<< 0<< 0 //Club
                   << 6<< 4<< 1<< 0<< 0<<-3<<-4<<-4<< 0<< 0 //Dance
                   <<-6<< 6<< 6<< 3<< 2<<-2<<-5<<-6<<-8<<-8 //Bass
                   <<-6<<-6<<-6<<-2<< 1<< 7<<10<<10<<10<<10 //BassSoprano
                   << 4<< 3<< 0<<-4<<-3<< 1<< 5<< 7<< 7<< 7 //Soprano
                   << 3<< 7<< 3<<-2<<-1<< 1<< 3<< 6<< 8<< 9 //Headset
                   << 6<< 6<< 3<< 3<< 0<<-3<<-3<<-3<< 0<< 0 //Hall
                   <<-3<< 0<< 2<< 3<< 3<< 3<< 2<< 1<< 1<< 1 //Scene
                   <<-1<< 3<< 4<< 5<< 3<< 0<<-1<<-1<<-1<<-1 //Pop
                   << 0<< 0<< 0<<-3<< 0<< 4<< 3<< 0<< 0<< 0 //Repaglinide
                   << 4<< 4<< 0<< 0<< 0<< 0<< 0<< 0<< 4<< 4 //Party
                   << 5<< 3<<-3<<-5<<-2<< 2<< 5<< 7<< 7<< 7 //Rock
                   <<-2<<-3<<-2<< 0<< 2<< 3<< 5<< 6<< 7<< 6 //Oscar
                   << 3<< 1<< 0<<-1<< 0<< 2<< 5<< 6<< 7<< 7 //Mild
                   << 2<< 2<< 1<< 0<<-2<<-3<<-2<< 0<< 2<< 5 //SoftRock
                   << 5<< 3<< 0<<-3<<-3<< 0<< 5<< 6<< 6<< 5;//Electronics
}

void MusicEqualizerDialog::eqChoiceIndexChanged(int index)
{
    m_eqChoiceSelected = true;
    if(index > 0)
    {
        index = (index - 1) * 10;
        m_ui->verticalSlider1->setValue(m_equalizeValue[index + 0]);
        m_ui->verticalSlider2->setValue(m_equalizeValue[index + 1]);
        m_ui->verticalSlider3->setValue(m_equalizeValue[index + 2]);
        m_ui->verticalSlider4->setValue(m_equalizeValue[index + 3]);
        m_ui->verticalSlider5->setValue(m_equalizeValue[index + 4]);
        m_ui->verticalSlider6->setValue(m_equalizeValue[index + 5]);
        m_ui->verticalSlider7->setValue(m_equalizeValue[index + 6]);
        m_ui->verticalSlider8->setValue(m_equalizeValue[index + 7]);
        m_ui->verticalSlider9->setValue(m_equalizeValue[index + 8]);
        m_ui->verticalSlider10->setValue(m_equalizeValue[index + 9]);
    }
    m_eqChoiceSelected = false;
}

int MusicEqualizerDialog::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
