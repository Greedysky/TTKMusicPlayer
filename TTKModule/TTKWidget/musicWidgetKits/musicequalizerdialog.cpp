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
#include <QAbstractItemView>
#include <QStyledItemDelegate>

MusicEqualizerDialog::MusicEqualizerDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicEqualizerDialog)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_eable = false;
    m_eqChoiceSelected = false;

    init();

    m_ui->eqChoice->setItemDelegate(new QStyledItemDelegate(m_ui->eqChoice));
    m_ui->eqChoice->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->eqChoice->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->eqChoice->addItems(QStringList() << tr("Custom") << tr("Default") << tr("Classical")
                                           << tr("Club") << tr("Dance") << tr("Bass") << tr("Soprano") << tr("BassSoprano")
                                           << tr("Headset") << tr("Hall") << tr("Scene") << tr("Pop") << tr("Repaglinide")
                                           << tr("Party") << tr("Rock") << tr("Oscar") << tr("Mild") << tr("SoftRock")
                                           << tr("Electronics"));
    connect(m_ui->eqChoice, SIGNAL(currentIndexChanged(int)), SLOT(eqChoiceIndexChanged(int)));

    m_ui->showEqButton->setStyleSheet(MusicUIObject::MKGEqualizerOff);

#ifdef Q_OS_UNIX
    m_ui->showEqButton->setFocusPolicy(Qt::NoFocus);
    m_ui->resetButton->setFocusPolicy(Qt::NoFocus);
#endif

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
    delete m_ui;
}

void MusicEqualizerDialog::init()
{
    m_signalMapper = new QSignalMapper(this);
    initSlider(m_ui->verticalSlider1, 0);
    initSlider(m_ui->verticalSlider2, 1);
    initSlider(m_ui->verticalSlider3, 2);
    initSlider(m_ui->verticalSlider4, 3);
    initSlider(m_ui->verticalSlider5, 4);
    initSlider(m_ui->verticalSlider6, 5);
    initSlider(m_ui->verticalSlider7, 6);
    initSlider(m_ui->verticalSlider8, 7);
    initSlider(m_ui->verticalSlider9, 8);
    initSlider(m_ui->verticalSlider10, 9);
    initSlider(m_ui->bwVerticalSlider, 10);
    connect(m_signalMapper, SIGNAL(mapped(int)), SLOT(verticalSliderChanged(int)));

    connect(m_ui->showEqButton, SIGNAL(clicked()), SLOT(setEqEnable()));
    connect(m_ui->resetButton, SIGNAL(clicked()), SLOT(resetEq()));
    m_ui->resetButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

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
        m_ui->showEqButton->click();
    }

    const QStringList &eqValue = M_SETTING_PTR->value(MusicSettingManager::EqualizerValueChoiced).toString().split(',');
    if(eqValue.count() == 11)
    {
        if(M_SETTING_PTR->value(MusicSettingManager::EqualizerIndexChoiced).toInt() == 0)
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
           m_ui->eqChoice->setCurrentIndex(M_SETTING_PTR->value(MusicSettingManager::EqualizerIndexChoiced).toInt());
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
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerIndexChoiced, m_ui->eqChoice->currentIndex());
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerValueChoiced,
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
    emitParameter();
}

void MusicEqualizerDialog::emitParameter()
{
    emit setEqEffect(MIntList() << m_ui->bwVerticalSlider->value() << m_ui->verticalSlider1->value()
                                << m_ui->verticalSlider2->value() << m_ui->verticalSlider3->value()
                                << m_ui->verticalSlider4->value() << m_ui->verticalSlider5->value()
                                << m_ui->verticalSlider6->value() << m_ui->verticalSlider7->value()
                                << m_ui->verticalSlider8->value() << m_ui->verticalSlider9->value()
                                << m_ui->verticalSlider10->value());
}

void MusicEqualizerDialog::setEqEnable()
{
    m_eable = !m_eable;
    emit setEnaleEffect(m_eable);
    m_ui->showEqButton->setStyleSheet(!m_eable ? MusicUIObject::MKGEqualizerOff : MusicUIObject::MKGEqualizerOn);

    setControlEnable(m_eable);
    if(m_eable)
    {
        emitParameter();
    }
}

void MusicEqualizerDialog::setControlEnable(bool enable) const
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
        index = (index - 1)*10;
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
