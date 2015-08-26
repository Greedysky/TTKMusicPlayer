#include "musicequalizerdialog.h"
#include "ui_musicequalizerdialog.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"

MusicEqualizerDialog::MusicEqualizerDialog(QWidget *parent) :
    MusicMoveDialogAbstract(parent),
    ui(new Ui::MusicEqualizerDialog)
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
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));

    m_eable = false;
    m_eqChoiceSelected = false;

    ui->bwVerticalSlider->setRange(-17,17);
    ui->verticalSlider1->setRange(-15,15);
    ui->verticalSlider2->setRange(-15,15);
    ui->verticalSlider3->setRange(-15,15);
    ui->verticalSlider4->setRange(-15,15);
    ui->verticalSlider5->setRange(-15,15);
    ui->verticalSlider6->setRange(-15,15);
    ui->verticalSlider7->setRange(-15,15);
    ui->verticalSlider8->setRange(-15,15);
    ui->verticalSlider9->setRange(-15,15);
    ui->verticalSlider10->setRange(-15,15);
    ui->bwVerticalSlider->setStyleSheet(MusicUIObject::MSliderStyle03);
    ui->verticalSlider1->setStyleSheet(MusicUIObject::MSliderStyle03);
    ui->verticalSlider2->setStyleSheet(MusicUIObject::MSliderStyle03);
    ui->verticalSlider3->setStyleSheet(MusicUIObject::MSliderStyle03);
    ui->verticalSlider4->setStyleSheet(MusicUIObject::MSliderStyle03);
    ui->verticalSlider5->setStyleSheet(MusicUIObject::MSliderStyle03);
    ui->verticalSlider6->setStyleSheet(MusicUIObject::MSliderStyle03);
    ui->verticalSlider7->setStyleSheet(MusicUIObject::MSliderStyle03);
    ui->verticalSlider8->setStyleSheet(MusicUIObject::MSliderStyle03);
    ui->verticalSlider9->setStyleSheet(MusicUIObject::MSliderStyle03);
    ui->verticalSlider10->setStyleSheet(MusicUIObject::MSliderStyle03);

    connect(ui->verticalSlider1,SIGNAL(valueChanged(int)),SLOT(verticalSlider1(int)));
    connect(ui->verticalSlider2,SIGNAL(valueChanged(int)),SLOT(verticalSlider2(int)));
    connect(ui->verticalSlider3,SIGNAL(valueChanged(int)),SLOT(verticalSlider3(int)));
    connect(ui->verticalSlider4,SIGNAL(valueChanged(int)),SLOT(verticalSlider4(int)));
    connect(ui->verticalSlider5,SIGNAL(valueChanged(int)),SLOT(verticalSlider5(int)));
    connect(ui->verticalSlider6,SIGNAL(valueChanged(int)),SLOT(verticalSlider6(int)));
    connect(ui->verticalSlider7,SIGNAL(valueChanged(int)),SLOT(verticalSlider7(int)));
    connect(ui->verticalSlider8,SIGNAL(valueChanged(int)),SLOT(verticalSlider8(int)));
    connect(ui->verticalSlider9,SIGNAL(valueChanged(int)),SLOT(verticalSlider9(int)));
    connect(ui->verticalSlider10,SIGNAL(valueChanged(int)),SLOT(verticalSlider10(int)));

    connect(ui->showEqButton,SIGNAL(clicked()),SLOT(setEqEnable()));
    connect(ui->resetButton,SIGNAL(clicked()),SLOT(resetEq()));
    ui->resetButton->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->funcButton_3->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->funcButton_4->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->funcButton_5->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->funcButton_6->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->funcButton_7->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->funcButton_8->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->funcButton_9->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->funcButton_10->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->funcButton_11->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->funcButton_12->setStyleSheet(MusicUIObject::MPushButtonStyle01);

    ui->checkBox_2->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->checkBox_3->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->checkBox_2->setCursor(QCursor(Qt::PointingHandCursor));
    ui->checkBox->setCursor(QCursor(Qt::PointingHandCursor));
    ui->checkBox_3->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->checkBox_2,SIGNAL(clicked(bool)),SLOT(setEchoEffect(bool)));
    connect(ui->checkBox_3,SIGNAL(clicked(bool)),SLOT(setMixChannelEffect(bool)));
    connect(ui->checkBox,SIGNAL(clicked(bool)),SLOT(setReverseEffect(bool)));

    connect(ui->funcButton_3,SIGNAL(clicked()),SLOT(setSideCutEffect()));
    connect(ui->funcButton_4,SIGNAL(clicked()),SLOT(setCenterCutEffect()));
    connect(ui->funcButton_5,SIGNAL(clicked()),SLOT(setRateDownEffect()));
    connect(ui->funcButton_6,SIGNAL(clicked()),SLOT(setFadeOutEffect()));
    connect(ui->funcButton_7,SIGNAL(clicked()),SLOT(setFadeInEffect()));
    connect(ui->funcButton_8,SIGNAL(clicked()),SLOT(setRateUpEffect()));
    connect(ui->funcButton_9,SIGNAL(clicked()),SLOT(setTempoUpEffect()));
    connect(ui->funcButton_10,SIGNAL(clicked()),SLOT(setTempoDownEffect()));
    connect(ui->funcButton_11,SIGNAL(clicked()),SLOT(setPitchDownEffect()));
    connect(ui->funcButton_12,SIGNAL(clicked()),SLOT(setPitchUpEffect()));

    ui->eqChoice->setStyleSheet(MusicUIObject::MComboBoxStyle03);
    ui->eqChoice->addItems(QStringList()<<tr("Custom")<<tr("Default")<<tr("Classical")
                  <<tr("Club")<<tr("Dance")<<tr("Bass")<<tr("Soprano")<<tr("BassSoprano")
                  <<tr("Headset")<<tr("Hall")<<tr("Scene")<<tr("Pop")<<tr("Repaglinide")
                  <<tr("Party")<<tr("Rock")<<tr("Oscar")<<tr("Mild")<<tr("SoftRock")
                  <<tr("Electronics"));
    connect(ui->eqChoice,SIGNAL(currentIndexChanged(int)),SLOT(eqChoiceIndexChanged(int)));

    ui->showEqButton->setIcon(QIcon(":/equalizer/off"));

    initEqualizeValue();
    readEqInformation();
    setControlEnable(false);
}

MusicEqualizerDialog::~MusicEqualizerDialog()
{
    writeEqInformation();
    delete ui;
}

void MusicEqualizerDialog::readEqInformation()
{
    if(M_SETTING.value(MusicSettingManager::EqualizerEnableChoiced).toInt())
    {
        ui->showEqButton->click();
    }
    QStringList eqValue = M_SETTING.value(MusicSettingManager::EqualizerValueChoiced).toString().split(',');
    if(eqValue.count() == 11)
    {
        if(M_SETTING.value(MusicSettingManager::EqualizerIndexChoiced).toInt() == 0)
        {
            ui->verticalSlider1->setValue(eqValue[1].toInt());
            ui->verticalSlider2->setValue(eqValue[2].toInt());
            ui->verticalSlider3->setValue(eqValue[3].toInt());
            ui->verticalSlider4->setValue(eqValue[4].toInt());
            ui->verticalSlider5->setValue(eqValue[5].toInt());
            ui->verticalSlider6->setValue(eqValue[6].toInt());
            ui->verticalSlider7->setValue(eqValue[7].toInt());
            ui->verticalSlider8->setValue(eqValue[8].toInt());
            ui->verticalSlider9->setValue(eqValue[9].toInt());
            ui->verticalSlider10->setValue(eqValue[10].toInt());
            ui->bwVerticalSlider->setValue(eqValue[0].toInt());
        }
        else
           ui->eqChoice->setCurrentIndex(M_SETTING.value(MusicSettingManager::EqualizerIndexChoiced).toInt());
    }
    else
        resetEq();

}

void MusicEqualizerDialog::writeEqInformation()
{
    M_SETTING.setValue(MusicSettingManager::EqualizerEnableChoiced, m_eable ? 1 : 0);
    M_SETTING.setValue(MusicSettingManager::EqualizerIndexChoiced, ui->eqChoice->currentIndex());
    M_SETTING.setValue(MusicSettingManager::EqualizerValueChoiced,
          QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11").arg(
          ui->bwVerticalSlider->value()).arg(ui->verticalSlider1->value()).arg(
          ui->verticalSlider2->value()).arg(ui->verticalSlider3->value()).arg(
          ui->verticalSlider4->value()).arg(ui->verticalSlider5->value()).arg(
          ui->verticalSlider6->value()).arg(ui->verticalSlider7->value()).arg(
          ui->verticalSlider8->value()).arg(ui->verticalSlider9->value()).arg(
          ui->verticalSlider10->value()));

}

void MusicEqualizerDialog::verticalSlider1(int)
{
    if(!m_eqChoiceSelected)
    {
        ui->eqChoice->setCurrentIndex(0);
    }
    emitParameter();
}

void MusicEqualizerDialog::verticalSlider2(int)
{
    if(!m_eqChoiceSelected)
    {
        ui->eqChoice->setCurrentIndex(0);
    }
    emitParameter();
}

void MusicEqualizerDialog::verticalSlider3(int)
{
    if(!m_eqChoiceSelected)
    {
        ui->eqChoice->setCurrentIndex(0);
    }
    emitParameter();
}

void MusicEqualizerDialog::verticalSlider4(int)
{
    if(!m_eqChoiceSelected)
    {
        ui->eqChoice->setCurrentIndex(0);
    }
    emitParameter();
}

void MusicEqualizerDialog::verticalSlider5(int)
{
    if(!m_eqChoiceSelected)
    {
        ui->eqChoice->setCurrentIndex(0);
    }
    emitParameter();
}

void MusicEqualizerDialog::verticalSlider6(int)
{
    if(!m_eqChoiceSelected)
    {
        ui->eqChoice->setCurrentIndex(0);
    }
    emitParameter();
}

void MusicEqualizerDialog::verticalSlider7(int)
{
    if(!m_eqChoiceSelected)
    {
        ui->eqChoice->setCurrentIndex(0);
    }
    emitParameter();
}

void MusicEqualizerDialog::verticalSlider8(int)
{
    if(!m_eqChoiceSelected)
    {
        ui->eqChoice->setCurrentIndex(0);
    }
    emitParameter();
}

void MusicEqualizerDialog::verticalSlider9(int)
{
    if(!m_eqChoiceSelected)
    {
        ui->eqChoice->setCurrentIndex(0);
    }
    emitParameter();
}

void MusicEqualizerDialog::verticalSlider10(int)
{
    if(!m_eqChoiceSelected)
    {
        ui->eqChoice->setCurrentIndex(0);
    }
    emitParameter();
}

void MusicEqualizerDialog::emitParameter()
{
    emit setEqEffect(MIntList()<<ui->verticalSlider1->value()
                     <<ui->verticalSlider2->value()<<ui->verticalSlider3->value()
                     <<ui->verticalSlider4->value()<<ui->verticalSlider5->value()
                     <<ui->verticalSlider6->value()<<ui->verticalSlider7->value()
                     <<ui->verticalSlider8->value()<<ui->verticalSlider9->value()
                     <<ui->verticalSlider10->value());
}

void MusicEqualizerDialog::setEqEnable()
{
    m_eable = !m_eable;
    emit setEnaleEffect(m_eable);
    ui->eqLabel->clear();
    ui->showEqButton->setIcon(QIcon(!m_eable ? ":/equalizer/off" : ":/equalizer/on"));

    setControlEnable(!m_eable);
    emitParameter();
}

void MusicEqualizerDialog::setControlEnable(bool)
{
    ui->bwVerticalSlider->setEnabled(m_eable);
    ui->verticalSlider1->setEnabled(m_eable);
    ui->verticalSlider2->setEnabled(m_eable);
    ui->verticalSlider3->setEnabled(m_eable);
    ui->verticalSlider4->setEnabled(m_eable);
    ui->verticalSlider5->setEnabled(m_eable);
    ui->verticalSlider6->setEnabled(m_eable);
    ui->verticalSlider7->setEnabled(m_eable);
    ui->verticalSlider8->setEnabled(m_eable);;
    ui->verticalSlider9->setEnabled(m_eable);
    ui->verticalSlider10->setEnabled(m_eable);
    ui->eqChoice->setEnabled(m_eable);
    ui->resetButton->setEnabled(m_eable);
}

void MusicEqualizerDialog::resetEq()
{
    emit resetEqEffect();
    ui->eqChoice->setCurrentIndex(1);
    ui->bwVerticalSlider->setValue(0);
    ui->verticalSlider1->setValue(0);
    ui->verticalSlider2->setValue(0);
    ui->verticalSlider3->setValue(0);
    ui->verticalSlider4->setValue(0);
    ui->verticalSlider5->setValue(0);
    ui->verticalSlider6->setValue(0);
    ui->verticalSlider7->setValue(0);
    ui->verticalSlider8->setValue(0);
    ui->verticalSlider9->setValue(0);
    ui->verticalSlider10->setValue(0);

}

void MusicEqualizerDialog::initEqualizeValue()
{
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
        index = (--index)*10;
        ui->verticalSlider1->setValue(m_equalizeValue[index + 0]);
        ui->verticalSlider2->setValue(m_equalizeValue[index + 1]);
        ui->verticalSlider3->setValue(m_equalizeValue[index + 2]);
        ui->verticalSlider4->setValue(m_equalizeValue[index + 3]);
        ui->verticalSlider5->setValue(m_equalizeValue[index + 4]);
        ui->verticalSlider6->setValue(m_equalizeValue[index + 5]);
        ui->verticalSlider7->setValue(m_equalizeValue[index + 6]);
        ui->verticalSlider8->setValue(m_equalizeValue[index + 7]);
        ui->verticalSlider9->setValue(m_equalizeValue[index + 8]);
        ui->verticalSlider10->setValue(m_equalizeValue[index + 9]);
    }
    m_eqChoiceSelected = false;
}

void MusicEqualizerDialog::setEchoEffect(bool)
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("EchoEffect"));
    emit setSpEqEffect(MusicObject::EQ_EchoEffect);
}

void MusicEqualizerDialog::setMixChannelEffect(bool)
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("MixChannelEffect"));
    emit setSpEqEffect(MusicObject::EQ_MixChannelEffect);
}

void MusicEqualizerDialog::setReverseEffect(bool)
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("ReverseEffect"));
    emit setSpEqEffect(MusicObject::EQ_ReverseEffect);
}

void MusicEqualizerDialog::setSideCutEffect()
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("SideCutEffect"));
    emit setSpEqEffect(MusicObject::EQ_SideCutEffect);
}

void MusicEqualizerDialog::setCenterCutEffect()
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("CenterCutEffect"));
    emit setSpEqEffect(MusicObject::EQ_CenterCutEffect);
}

void MusicEqualizerDialog::setRateUpEffect()
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("RateUpEffect"));
    emit setSpEqEffect(MusicObject::EQ_RateUpEffect);
}

void MusicEqualizerDialog::setRateDownEffect()
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("RateDownEffect"));
    emit setSpEqEffect(MusicObject::EQ_RateDownEffect);
}

void MusicEqualizerDialog::setPitchUpEffect()
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("PitchUpEffect"));
    emit setSpEqEffect(MusicObject::EQ_PitchUpEffect);
}

void MusicEqualizerDialog::setPitchDownEffect()
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("PitchDownEffect"));
    emit setSpEqEffect(MusicObject::EQ_PitchDownEffect);
}

void MusicEqualizerDialog::setTempoUpEffect()
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("TempoUpEffect"));
    emit setSpEqEffect(MusicObject::EQ_TempoUpEffect);
}

void MusicEqualizerDialog::setTempoDownEffect()
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("TempoDownEffect"));
    emit setSpEqEffect(MusicObject::EQ_TempoDownEffect);
}

void MusicEqualizerDialog::setFadeOutEffect()
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("FadeOutEffect"));
    emit setSpEqEffect(MusicObject::EQ_FadeOutEffect);
}

void MusicEqualizerDialog::setFadeInEffect()
{
    m_eable = true;
    setEqEnable();
    ui->eqLabel->setText(tr("FadeInEffect"));
    emit setSpEqEffect(MusicObject::EQ_FadeInEffect);
}
