#include "musicequalizerdialog.h"
#include "ui_musicequalizerdialog.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicmagicwidgetuiobject.h"
#include "musicconnectionpool.h"
#include "musicsoundeffectswidget.h"
#include "musicwidgetutils.h"
#include "musicplayer.h"

#include <QSignalMapper>
#include <QStyledItemDelegate>

MusicEqualizerDialog::MusicEqualizerDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicEqualizerDialog)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_eable = false;
    m_eqChoiceSelected = false;

    init();

    ui->eqChoice->setItemDelegate(new QStyledItemDelegate(ui->eqChoice));
    ui->eqChoice->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->eqChoice->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->eqChoice->addItems(QStringList() << tr("Custom") << tr("Default") << tr("Classical")
                  << tr("Club") << tr("Dance") << tr("Bass") << tr("Soprano") << tr("BassSoprano")
                  << tr("Headset") << tr("Hall") << tr("Scene") << tr("Pop") << tr("Repaglinide")
                  << tr("Party") << tr("Rock") << tr("Oscar") << tr("Mild") << tr("SoftRock")
                  << tr("Electronics"));
    connect(ui->eqChoice, SIGNAL(currentIndexChanged(int)), SLOT(eqChoiceIndexChanged(int)));

    ui->showEqButton->setStyleSheet(MusicUIObject::MKGEqualizerOff);

    setControlEnable(false);
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
    delete m_signalMapper;
    delete ui;
}

QString MusicEqualizerDialog::getClassName()
{
    return staticMetaObject.className();
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
    ui->resetButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

#ifdef Q_OS_UNIX
    MusicUtils::Widget::setLabelFontSize(ui->showPerArea_21, 9);
    MusicUtils::Widget::setLabelFontSize(ui->showPerArea_22, 9);
    MusicUtils::Widget::setLabelFontSize(ui->showPerArea_23, 9);
    MusicUtils::Widget::setLabelFontSize(ui->showPerArea_24, 9);
    MusicUtils::Widget::setLabelFontSize(ui->showPerArea_25, 9);
    MusicUtils::Widget::setLabelFontSize(ui->showPerArea_26, 9);
    MusicUtils::Widget::setLabelFontSize(ui->showPerArea_27, 9);
    MusicUtils::Widget::setLabelFontSize(ui->showPerArea_28, 9);
    MusicUtils::Widget::setLabelFontSize(ui->showPerArea_29, 9);
    MusicUtils::Widget::setLabelFontSize(ui->showPerArea_30, 9);
    MusicUtils::Widget::setLabelFontSize(ui->showPerArea_31, 9);
#endif
}

void MusicEqualizerDialog::initSlider(QSlider *slider, int index)
{
    slider->setRange(-15, 15);
    slider->setStyleSheet(MusicUIObject::MSliderStyle04);
    connect(slider, SIGNAL(valueChanged(int)), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(slider, index);
}

void MusicEqualizerDialog::readEqInformation()
{
    if(M_SETTING_PTR->value(MusicSettingManager::EqualizerEnableChoiced).toInt())
    {
        ui->showEqButton->click();
    }
    QStringList eqValue = M_SETTING_PTR->value(MusicSettingManager::EqualizerValueChoiced).toString().split(',');
    if(eqValue.count() == 11)
    {
        if(M_SETTING_PTR->value(MusicSettingManager::EqualizerIndexChoiced).toInt() == 0)
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
           ui->eqChoice->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::EqualizerIndexChoiced).toInt());
        }
    }
    else
    {
        resetEq();
    }
}

void MusicEqualizerDialog::writeEqInformation() const
{
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnableChoiced, m_eable ? 1 : 0);
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerIndexChoiced, ui->eqChoice->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerValueChoiced,
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
    emit setEqEffect(MusicObject::MIntList() << ui->bwVerticalSlider->value() << ui->verticalSlider1->value()
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
    ui->showEqButton->setStyleSheet(!m_eable ? MusicUIObject::MKGEqualizerOff : MusicUIObject::MKGEqualizerOn);

    setControlEnable(m_eable);
    if(m_eable)
    {
        emitParameter();
    }
}

void MusicEqualizerDialog::setControlEnable(bool enable) const
{
    ui->bwVerticalSlider->setEnabled(enable);
    ui->verticalSlider1->setEnabled(enable);
    ui->verticalSlider2->setEnabled(enable);
    ui->verticalSlider3->setEnabled(enable);
    ui->verticalSlider4->setEnabled(enable);
    ui->verticalSlider5->setEnabled(enable);
    ui->verticalSlider6->setEnabled(enable);
    ui->verticalSlider7->setEnabled(enable);
    ui->verticalSlider8->setEnabled(enable);;
    ui->verticalSlider9->setEnabled(enable);
    ui->verticalSlider10->setEnabled(enable);
    ui->eqChoice->setEnabled(enable);
    ui->resetButton->setEnabled(enable);
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

int MusicEqualizerDialog::exec()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
