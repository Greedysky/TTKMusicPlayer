#include "musicequalizerdialog.h"
#include "ui_musicequalizerdialog.h"
#include "musicsettingmanager.h"
#include "musicmagicwidgetuiobject.h"
#include "musicconnectionpool.h"
#include "musicsoundeffectswidget.h"
#include "musicplayer.h"
#include "ttkclickedgroup.h"

static void makeBlockedValue(QSlider *slider, int value)
{
    slider->blockSignals(true);
    slider->setValue(value);
    slider->blockSignals(false);
}

MusicEqualizerDialog::MusicEqualizerDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicEqualizerDialog),
      m_enable(false)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    initialize();

    TTK::Widget::generateComboBoxFormat(m_ui->eqChoice);
    m_ui->eqChoice->addItems({tr("Custom"), tr("Default"), tr("Classical"), tr("Club"), tr("Dance"), tr("Bass"), tr("Soprano"),
                              tr("BassSoprano"), tr("Headset"), tr("Hall"), tr("Scene"), tr("Pop"), tr("Repaglinide"),
                              tr("Party"), tr("Rock"), tr("Oscar"), tr("Mild"), tr("SoftRock"), tr("Electronics")});
    connect(m_ui->eqChoice, SIGNAL(currentIndexChanged(int)), SLOT(equlizerTypeChanged(int)));

    m_ui->showEqButton->setStyleSheet(TTK::UI::EqualizerOff);

#ifdef Q_OS_UNIX
    m_ui->showEqButton->setFocusPolicy(Qt::NoFocus);
    m_ui->resetButton->setFocusPolicy(Qt::NoFocus);
#endif

    setControlEnabled(false);
    initEqualizeValue();
    readInformation();

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->connect(className(), MusicPlayer::className());
    G_CONNECTION_PTR->connect(className(), MusicSoundEffectsWidget::className());
}

MusicEqualizerDialog::~MusicEqualizerDialog()
{
    G_CONNECTION_PTR->removeValue(this);
    writeInformation();
    delete m_ui;
}

void MusicEqualizerDialog::setEnable()
{
    m_enable = !m_enable;
    Q_EMIT setEnabledEffect(m_enable);
    m_ui->showEqButton->setStyleSheet(!m_enable ? TTK::UI::EqualizerOff : TTK::UI::EqualizerOn);

    setControlEnabled(m_enable);
    if(m_enable)
    {
        parameterSubmit();
    }
}

void MusicEqualizerDialog::reset()
{
    m_ui->eqChoice->setCurrentIndex(1);
    makeBlockedValue(m_ui->bwVerticalSlider, 0);
    makeBlockedValue(m_ui->verticalSlider1,  0);
    makeBlockedValue(m_ui->verticalSlider2,  0);
    makeBlockedValue(m_ui->verticalSlider3,  0);
    makeBlockedValue(m_ui->verticalSlider4,  0);
    makeBlockedValue(m_ui->verticalSlider5,  0);
    makeBlockedValue(m_ui->verticalSlider6,  0);
    makeBlockedValue(m_ui->verticalSlider7,  0);
    makeBlockedValue(m_ui->verticalSlider8,  0);
    makeBlockedValue(m_ui->verticalSlider9,  0);
    makeBlockedValue(m_ui->verticalSlider10, 0);
}

void MusicEqualizerDialog::sliderValueChanged()
{
    m_ui->eqChoice->blockSignals(true);
    m_ui->eqChoice->setCurrentIndex(0);
    m_ui->eqChoice->blockSignals(false);
    parameterSubmit();
}

void MusicEqualizerDialog::equlizerTypeChanged(int index)
{
    if(index > 0)
    {
        index = (index - 1) * 10;
        makeBlockedValue(m_ui->verticalSlider1,  m_equalizeValue[index + 0]);
        makeBlockedValue(m_ui->verticalSlider2,  m_equalizeValue[index + 1]);
        makeBlockedValue(m_ui->verticalSlider3,  m_equalizeValue[index + 2]);
        makeBlockedValue(m_ui->verticalSlider4,  m_equalizeValue[index + 3]);
        makeBlockedValue(m_ui->verticalSlider5,  m_equalizeValue[index + 4]);
        makeBlockedValue(m_ui->verticalSlider6,  m_equalizeValue[index + 5]);
        makeBlockedValue(m_ui->verticalSlider7,  m_equalizeValue[index + 6]);
        makeBlockedValue(m_ui->verticalSlider8,  m_equalizeValue[index + 7]);
        makeBlockedValue(m_ui->verticalSlider9,  m_equalizeValue[index + 8]);
        makeBlockedValue(m_ui->verticalSlider10, m_equalizeValue[index + 9]);
    }
    parameterSubmit();
}

void MusicEqualizerDialog::initialize()
{
    TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
    connect(clickedGroup, SIGNAL(clicked(int)), SLOT(sliderValueChanged()));

    QList<TTKClickedSlider*> sliders;
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

    for(int i = 0; i < sliders.count(); ++i)
    {
        TTKClickedSlider *slider = sliders[i];
        slider->setRange(-15, 15);
        slider->setStyleSheet(TTK::UI::SliderStyle04);
        clickedGroup->mapped(slider);
        connect(slider, SIGNAL(valueChanged(int)), SLOT(sliderValueChanged()));
    }

    connect(m_ui->showEqButton, SIGNAL(clicked()), SLOT(setEnable()));
    connect(m_ui->resetButton, SIGNAL(clicked()), SLOT(reset()));
    m_ui->resetButton->setStyleSheet(TTK::UI::PushButtonStyle04);

#ifdef Q_OS_UNIX
    TTK::Widget::setLabelFontSize(m_ui->showPerArea_21, 9);
    TTK::Widget::setLabelFontSize(m_ui->showPerArea_22, 9);
    TTK::Widget::setLabelFontSize(m_ui->showPerArea_23, 9);
    TTK::Widget::setLabelFontSize(m_ui->showPerArea_24, 9);
    TTK::Widget::setLabelFontSize(m_ui->showPerArea_25, 9);
    TTK::Widget::setLabelFontSize(m_ui->showPerArea_26, 9);
    TTK::Widget::setLabelFontSize(m_ui->showPerArea_27, 9);
    TTK::Widget::setLabelFontSize(m_ui->showPerArea_28, 9);
    TTK::Widget::setLabelFontSize(m_ui->showPerArea_29, 9);
    TTK::Widget::setLabelFontSize(m_ui->showPerArea_30, 9);
    TTK::Widget::setLabelFontSize(m_ui->showPerArea_31, 9);
#endif
}

void MusicEqualizerDialog::setControlEnabled(bool enabled) const
{
    m_ui->bwVerticalSlider->setEnabled(enabled);
    m_ui->verticalSlider1->setEnabled(enabled);
    m_ui->verticalSlider2->setEnabled(enabled);
    m_ui->verticalSlider3->setEnabled(enabled);
    m_ui->verticalSlider4->setEnabled(enabled);
    m_ui->verticalSlider5->setEnabled(enabled);
    m_ui->verticalSlider6->setEnabled(enabled);
    m_ui->verticalSlider7->setEnabled(enabled);
    m_ui->verticalSlider8->setEnabled(enabled);
    m_ui->verticalSlider9->setEnabled(enabled);
    m_ui->verticalSlider10->setEnabled(enabled);
    m_ui->eqChoice->setEnabled(enabled);
    m_ui->resetButton->setEnabled(enabled);
}

void MusicEqualizerDialog::readInformation()
{
    if(G_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toBool())
    {
        m_ui->showEqButton->click();
    }

    const QStringList &eqValue = G_SETTING_PTR->value(MusicSettingManager::EqualizerValue).toString().split(",");
    if(eqValue.count() == 11)
    {
        if(G_SETTING_PTR->value(MusicSettingManager::EqualizerIndex).toInt() == 0)
        {
            makeBlockedValue(m_ui->bwVerticalSlider, eqValue[0].toInt());
            makeBlockedValue(m_ui->verticalSlider1,  eqValue[1].toInt());
            makeBlockedValue(m_ui->verticalSlider2,  eqValue[2].toInt());
            makeBlockedValue(m_ui->verticalSlider3,  eqValue[3].toInt());
            makeBlockedValue(m_ui->verticalSlider4,  eqValue[4].toInt());
            makeBlockedValue(m_ui->verticalSlider5,  eqValue[5].toInt());
            makeBlockedValue(m_ui->verticalSlider6,  eqValue[6].toInt());
            makeBlockedValue(m_ui->verticalSlider7,  eqValue[7].toInt());
            makeBlockedValue(m_ui->verticalSlider8,  eqValue[8].toInt());
            makeBlockedValue(m_ui->verticalSlider9,  eqValue[9].toInt());
            makeBlockedValue(m_ui->verticalSlider10, eqValue[10].toInt());
        }
        else
        {
           m_ui->eqChoice->setCurrentIndex(G_SETTING_PTR->value(MusicSettingManager::EqualizerIndex).toInt());
        }
    }
    else
    {
        reset();
    }
}

void MusicEqualizerDialog::writeInformation() const
{
    G_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnable, m_enable ? 1 : 0);
    G_SETTING_PTR->setValue(MusicSettingManager::EqualizerIndex, m_ui->eqChoice->currentIndex());
    G_SETTING_PTR->setValue(MusicSettingManager::EqualizerValue,
          QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11").arg(m_ui->bwVerticalSlider->value())
                 .arg(m_ui->verticalSlider1->value()).arg(m_ui->verticalSlider2->value()).arg(m_ui->verticalSlider3->value())
                 .arg(m_ui->verticalSlider4->value()).arg(m_ui->verticalSlider5->value()).arg(m_ui->verticalSlider6->value())
                 .arg(m_ui->verticalSlider7->value()).arg(m_ui->verticalSlider8->value()).arg(m_ui->verticalSlider9->value())
                 .arg(m_ui->verticalSlider10->value()));
}

void MusicEqualizerDialog::initEqualizeValue()
{
    m_equalizeValue.clear();
    m_equalizeValue << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 //Default
                    << 0 << 0 << 0 << 0 << 0 << 0 <<-4 <<-4 <<-4 <<-6 //Classical
                    << 0 << 0 << 5 << 3 << 3 << 3 << 2 << 0 << 0 << 0 //Club
                    << 6 << 4 << 1 << 0 << 0 <<-3 <<-4 <<-4 << 0 << 0 //Dance
                    <<-6 << 6 << 6 << 3 << 2 <<-2 <<-5 <<-6 <<-8 <<-8 //Bass
                    <<-6 <<-6 <<-6 <<-2 << 1 << 7 <<10 <<10 <<10 <<10 //BassSoprano
                    << 4 << 3 << 0 <<-4 <<-3 << 1 << 5 << 7 << 7 << 7 //Soprano
                    << 3 << 7 << 3 <<-2 <<-1 << 1 << 3 << 6 << 8 << 9 //Headset
                    << 6 << 6 << 3 << 3 << 0 <<-3 <<-3 <<-3 << 0 << 0 //Hall
                    <<-3 << 0 << 2 << 3 << 3 << 3 << 2 << 1 << 1 << 1 //Scene
                    <<-1 << 3 << 4 << 5 << 3 << 0 <<-1 <<-1 <<-1 <<-1 //Pop
                    << 0 << 0 << 0 <<-3 << 0 << 4 << 3 << 0 << 0 << 0 //Repaglinide
                    << 4 << 4 << 0 << 0 << 0 << 0 << 0 << 0 << 4 << 4 //Party
                    << 5 << 3 <<-3 <<-5 <<-2 << 2 << 5 << 7 << 7 << 7 //Rock
                    <<-2 <<-3 <<-2 << 0 << 2 << 3 << 5 << 6 << 7 << 6 //Oscar
                    << 3 << 1 << 0 <<-1 << 0 << 2 << 5 << 6 << 7 << 7 //Mild
                    << 2 << 2 << 1 << 0 <<-2 <<-3 <<-2 << 0 << 2 << 5 //SoftRock
                    << 5 << 3 << 0 <<-3 <<-3 << 0 << 5 << 6 << 6 << 5;//Electronics
}

void MusicEqualizerDialog::parameterSubmit()
{
    Q_EMIT setEqualizerEffect({m_ui->bwVerticalSlider->value(), m_ui->verticalSlider1->value(), m_ui->verticalSlider2->value(),
                               m_ui->verticalSlider3->value(), m_ui->verticalSlider4->value(), m_ui->verticalSlider5->value(),
                               m_ui->verticalSlider6->value(), m_ui->verticalSlider7->value(), m_ui->verticalSlider8->value(),
                               m_ui->verticalSlider9->value(), m_ui->verticalSlider10->value()});
}
