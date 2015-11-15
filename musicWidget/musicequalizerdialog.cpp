#include "musicequalizerdialog.h"
#include "ui_musicequalizerdialog.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicbgthememanager.h"
#include "musicconnectionpool.h"

#include <QSignalMapper>
#include <QButtonGroup>

MusicEqualizerDialog::MusicEqualizerDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicEqualizerDialog)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_eable = false;
    m_eqChoiceSelected = false;

    init();

    ui->eqChoice->setStyleSheet(MusicUIObject::MComboBoxStyle01);
    ui->eqChoice->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->eqChoice->addItems(QStringList() << tr("Custom") << tr("Default") << tr("Classical")
                  << tr("Club") << tr("Dance") << tr("Bass") << tr("Soprano") << tr("BassSoprano")
                  << tr("Headset") << tr("Hall") << tr("Scene") << tr("Pop") << tr("Repaglinide")
                  << tr("Party") << tr("Rock") << tr("Oscar") << tr("Mild") << tr("SoftRock")
                  << tr("Electronics"));
    connect(ui->eqChoice, SIGNAL(currentIndexChanged(int)), SLOT(eqChoiceIndexChanged(int)));

    ui->showEqButton->setIcon(QIcon(":/equalizer/off"));

    initEqualizeValue();
    readEqInformation();
    setControlEnable(false);

    M_CONNECTION->setValue("MusicEqualizerDialog", this);
    M_CONNECTION->connect("MusicEqualizerDialog", "MusicPlayer");
}

MusicEqualizerDialog::~MusicEqualizerDialog()
{
    M_CONNECTION->disConnect("MusicEqualizerDialog");
    writeEqInformation();
    delete m_signalMapper;
    delete ui;
}

void MusicEqualizerDialog::init()
{
    m_signalMapper = new QSignalMapper(this);
    initSlider(ui->verticalSlider1, 0);
    initSlider(ui->verticalSlider2, 1);
    initSlider(ui->verticalSlider3, 2);
    initSlider(ui->verticalSlider4, 3);
    initSlider(ui->verticalSlider5, 4);
    initSlider(ui->verticalSlider6, 5);
    initSlider(ui->verticalSlider7, 6);
    initSlider(ui->verticalSlider8, 7);
    initSlider(ui->verticalSlider9, 8);
    initSlider(ui->verticalSlider10, 9);
    initSlider(ui->bwVerticalSlider, 10);
    connect(m_signalMapper, SIGNAL(mapped(int)), SLOT(verticalSliderChanged(int)));

    connect(ui->showEqButton, SIGNAL(clicked()), SLOT(setEqEnable()));
    connect(ui->resetButton, SIGNAL(clicked()), SLOT(resetEq()));
    ui->resetButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->funcButton_3->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->funcButton_4->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->funcButton_5->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->funcButton_6->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->funcButton_7->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->funcButton_8->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->funcButton_9->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->funcButton_10->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->funcButton_11->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->funcButton_12->setStyleSheet(MusicUIObject::MPushButtonStyle05);

    ui->checkBox_2->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->checkBox_3->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->checkBox_2->setCursor(QCursor(Qt::PointingHandCursor));
    ui->checkBox->setCursor(QCursor(Qt::PointingHandCursor));
    ui->checkBox_3->setCursor(QCursor(Qt::PointingHandCursor));

    QButtonGroup *checkGroup = new QButtonGroup(this);
    checkGroup->addButton(ui->checkBox_2, 0);
    checkGroup->addButton(ui->checkBox_3, 1);
    checkGroup->addButton(ui->checkBox, 2);
    checkGroup->setExclusive(false);
    connect(checkGroup, SIGNAL(buttonClicked(int)), SLOT(setMixedEffect(int)));

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->funcButton_3, 0);
    buttonGroup->addButton(ui->funcButton_4, 1);
    buttonGroup->addButton(ui->funcButton_5, 2);
    buttonGroup->addButton(ui->funcButton_6, 3);
    buttonGroup->addButton(ui->funcButton_7, 4);
    buttonGroup->addButton(ui->funcButton_8, 5);
    buttonGroup->addButton(ui->funcButton_9, 6);
    buttonGroup->addButton(ui->funcButton_10, 7);
    buttonGroup->addButton(ui->funcButton_11, 8);
    buttonGroup->addButton(ui->funcButton_12, 9);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(setEqualizerEffect(int)));
}

void MusicEqualizerDialog::initSlider(QSlider *slider, int index)
{
    slider->setRange(-15, 15);
    slider->setStyleSheet(MusicUIObject::MSliderStyle03);
    connect(slider, SIGNAL(valueChanged(int)), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(slider, index);
}

void MusicEqualizerDialog::readEqInformation()
{
    if(M_SETTING->value(MusicSettingManager::EqualizerEnableChoiced).toInt())
    {
        ui->showEqButton->click();
    }
    QStringList eqValue = M_SETTING->value(MusicSettingManager::EqualizerValueChoiced).toString().split(',');
    if(eqValue.count() == 11)
    {
        if(M_SETTING->value(MusicSettingManager::EqualizerIndexChoiced).toInt() == 0)
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
        {
           ui->eqChoice->setCurrentIndex(M_SETTING->value(MusicSettingManager::EqualizerIndexChoiced).toInt());
        }
    }
    else
    {
        resetEq();
    }
}

void MusicEqualizerDialog::writeEqInformation() const
{
    M_SETTING->setValue(MusicSettingManager::EqualizerEnableChoiced, m_eable ? 1 : 0);
    M_SETTING->setValue(MusicSettingManager::EqualizerIndexChoiced, ui->eqChoice->currentIndex());
    M_SETTING->setValue(MusicSettingManager::EqualizerValueChoiced,
          QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11").arg(
          ui->bwVerticalSlider->value()).arg(ui->verticalSlider1->value()).arg(
          ui->verticalSlider2->value()).arg(ui->verticalSlider3->value()).arg(
          ui->verticalSlider4->value()).arg(ui->verticalSlider5->value()).arg(
          ui->verticalSlider6->value()).arg(ui->verticalSlider7->value()).arg(
          ui->verticalSlider8->value()).arg(ui->verticalSlider9->value()).arg(
          ui->verticalSlider10->value()));

}

void MusicEqualizerDialog::verticalSliderChanged(int)
{
    if(!m_eqChoiceSelected)
    {
        ui->eqChoice->setCurrentIndex(0);
    }
    emitParameter();
}

void MusicEqualizerDialog::emitParameter()
{
    emit setEqEffect(MIntList() << ui->verticalSlider1->value()
                     << ui->verticalSlider2->value() << ui->verticalSlider3->value()
                     << ui->verticalSlider4->value() << ui->verticalSlider5->value()
                     << ui->verticalSlider6->value() << ui->verticalSlider7->value()
                     << ui->verticalSlider8->value() << ui->verticalSlider9->value()
                     << ui->verticalSlider10->value());
}

void MusicEqualizerDialog::setEqEnable()
{
    m_eable = !m_eable;
    emit setEnaleEffect(m_eable);
    ui->showEqButton->setIcon(QIcon(!m_eable ? ":/equalizer/off" : ":/equalizer/on"));

    setControlEnable(!m_eable);
    emitParameter();
}

void MusicEqualizerDialog::setControlEnable(bool) const
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
        index = (index - 1)*10;
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

void MusicEqualizerDialog::setMixedEffect(int index)
{
    m_eable = true;
    setEqEnable();
    switch(index)
    {
        case 0:
            emit setSpEqEffect(MusicObject::EQ_EchoEffect);
            break;
        case 1:
            emit setSpEqEffect(MusicObject::EQ_MixChannelEffect);
            break;
        case 2:
            emit setSpEqEffect(MusicObject::EQ_ReverseEffect);
            break;
        default: break;
    }
}

void MusicEqualizerDialog::setEqualizerEffect(int index)
{
    m_eable = true;
    setEqEnable();
    switch(index)
    {
        case 0:
            emit setSpEqEffect(MusicObject::EQ_SideCutEffect);
            break;
        case 1:
            emit setSpEqEffect(MusicObject::EQ_CenterCutEffect);
            break;
        case 2:
            emit setSpEqEffect(MusicObject::EQ_RateDownEffect);
            break;
        case 3:
            emit setSpEqEffect(MusicObject::EQ_FadeOutEffect);
            break;
        case 4:
            emit setSpEqEffect(MusicObject::EQ_FadeInEffect);
            break;
        case 5:
            emit setSpEqEffect(MusicObject::EQ_RateUpEffect);
            break;
        case 6:
            emit setSpEqEffect(MusicObject::EQ_TempoUpEffect);
            break;
        case 7:
            emit setSpEqEffect(MusicObject::EQ_TempoDownEffect);
            break;
        case 8:
            emit setSpEqEffect(MusicObject::EQ_PitchDownEffect);
            break;
        case 9:
            emit setSpEqEffect(MusicObject::EQ_PitchUpEffect);
            break;
        default:break;
    }
}

int MusicEqualizerDialog::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
