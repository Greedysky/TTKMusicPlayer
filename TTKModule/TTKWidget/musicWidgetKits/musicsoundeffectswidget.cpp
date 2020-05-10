#include "musicsoundeffectswidget.h"
#include "ui_musicsoundeffectswidget.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicuiobject.h"
#include "musicplayer.h"
#include "musicwidgetheaders.h"
#include "musicqmmputils.h"

#include <QStyledItemDelegate>

MusicSoundEffectsItemWidget::MusicSoundEffectsItemWidget(QWidget *parent)
    : QWidget(parent)
{
    m_type = Null;
    m_enable = false;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);

    m_textLabel = new QLabel(this);
    m_textLabel->setStyleSheet(MusicUIObject::MQSSBackgroundStyle08);

    setText(tr("null"));

    QWidget *func = new QWidget(this);
    func->setStyleSheet(MusicUIObject::MQSSBackgroundStyle04);
    QHBoxLayout *funcLayout = new QHBoxLayout(func);
    funcLayout->setContentsMargins(0, 5, 5, 0);
    funcLayout->setSpacing(0);

    m_settingButton = new QPushButton(func);
    m_settingButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle01);
    m_settingButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_settingButton->setFixedWidth(40);
    m_settingButton->setText(tr("Sets"));
    m_settingButton->setEnabled(false);
    connect(m_settingButton, SIGNAL(clicked()), SLOT(soundEffectValueChanged()));

    m_openButton = new QPushButton(func);
    m_openButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle01);
    m_openButton->setIcon(QIcon(":/tiny/btn_effect_on"));
    m_openButton->setToolTip(tr("On"));
    m_openButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_openButton->setFixedSize(16, 16);
    connect(m_openButton, SIGNAL(clicked()), SLOT(setPluginEnabled()));

#ifdef Q_OS_UNIX
    m_settingButton->setFocusPolicy(Qt::NoFocus);
    m_openButton->setFocusPolicy(Qt::NoFocus);
#endif

    QLabel *iconLabel = new QLabel(func);
    iconLabel->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    iconLabel->setPixmap(QPixmap(":/tiny/lb_arrow_down_normal"));
    iconLabel->setFixedSize(16, 16);

    funcLayout->addWidget(m_settingButton);
    funcLayout->addWidget(iconLabel);
    funcLayout->addStretch(1);
    funcLayout->addWidget(m_openButton);

    layout->addWidget(m_textLabel);
    layout->addWidget(func);

    setLayout(layout);
}

MusicSoundEffectsItemWidget::~MusicSoundEffectsItemWidget()
{
    delete m_textLabel;
    delete m_openButton;
    delete m_settingButton;
}

void MusicSoundEffectsItemWidget::setText(const QString &text)
{
    m_textLabel->setText(" " + text);
}

void MusicSoundEffectsItemWidget::setType(Type type)
{
    m_type = type;
}

void MusicSoundEffectsItemWidget::setPluginEnabled(bool enable)
{
    m_enable = !enable;
    setPluginEnabled();
}

bool MusicSoundEffectsItemWidget::pluginEnabled() const
{
    return m_enable;
}

void MusicSoundEffectsItemWidget::soundEffectChanged(Type type, bool enable)
{
    const QString plugin(transformQStringFromEnum(type));
    MusicUtils::QMMP::enabledEffectPlugin(plugin, enable);
}

void MusicSoundEffectsItemWidget::setPluginEnabled()
{
    if(!m_enable)
    {
        m_enable = true;
        m_openButton->setIcon(QIcon(":/tiny/btn_effect_off"));
        soundEffectChanged(m_type, true);

        const QString plugin(transformQStringFromEnum(m_type));
        m_settingButton->setEnabled(MusicUtils::QMMP::effectHasSetting(plugin));
        m_openButton->setToolTip(tr("Off"));
    }
    else
    {
        m_enable = false;
        m_openButton->setIcon(QIcon(":/tiny/btn_effect_on"));
        soundEffectChanged(m_type, false);
        m_settingButton->setEnabled(false);
        m_openButton->setToolTip(tr("On"));
    }
}

void MusicSoundEffectsItemWidget::soundEffectValueChanged()
{
    const QString plugin(transformQStringFromEnum(m_type));
    MusicUtils::QMMP::showEffectSetting(plugin, this);
}

QString MusicSoundEffectsItemWidget::transformQStringFromEnum(Type type)
{
    QString plugin;
    switch(type)
    {
        case BS2B:         plugin = "bs2b"; break;
        case Crossfade:    plugin = "crossfade"; break;
        case Stereo:       plugin = "stereo"; break;
        case LADSPA:       plugin = "ladspa"; break;
        case Soxr:         plugin = "soxr"; break;
        case SrcConverter: plugin = "srconverter"; break;
        case MonoStereo:   plugin = "monotostereo"; break;
        case Mono:         plugin = "mono"; break;
        default:           plugin = "unknow"; break;
    }
    return plugin;
}



MusicSoundEffectsWidget::MusicSoundEffectsWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSoundEffectsWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->stateComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->stateComboBox));
    m_ui->stateComboBox->setStyleSheet(MusicUIObject::MQSSComboBoxStyle01 + MusicUIObject::MQSSItemView01);
    m_ui->stateComboBox->view()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);
    m_ui->stateComboBox->addItems(QStringList() << tr("OperatorAll") << tr("All On") << tr("All Off"));
    connect(m_ui->stateComboBox, SIGNAL(currentIndexChanged(int)), SLOT(stateComboBoxChanged(int)));

    m_ui->eqButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->eqButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->eqEffectButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->eqEffectButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->eqButton->setFocusPolicy(Qt::NoFocus);
    m_ui->eqEffectButton->setFocusPolicy(Qt::NoFocus);
#endif

    //
    m_ui->BS2BWidget->setText("BS2B");
    m_ui->BS2BWidget->setType(MusicSoundEffectsItemWidget::BS2B);

    m_ui->CrossfadeWidget->setText("Crossfade");
    m_ui->CrossfadeWidget->setType(MusicSoundEffectsItemWidget::Crossfade);

    m_ui->StereoWidget->setText("Stereo");
    m_ui->StereoWidget->setType(MusicSoundEffectsItemWidget::Stereo);

    m_ui->SOXWidget->setText("Soxr");
    m_ui->SOXWidget->setType(MusicSoundEffectsItemWidget::Soxr);

    m_ui->SRCWidget->setText("SrcConverter");
    m_ui->SRCWidget->setType(MusicSoundEffectsItemWidget::SrcConverter);

    m_ui->MonoStereoWidget->setText("MonoStereo");
    m_ui->MonoStereoWidget->setType(MusicSoundEffectsItemWidget::MonoStereo);

    m_ui->MonoWidget->setText("Mono");
    m_ui->MonoWidget->setType(MusicSoundEffectsItemWidget::Mono);

#ifdef Q_OS_UNIX
    m_ui->LADSPAWidget->setText("LADSPA");
    m_ui->LADSPAWidget->setType(MusicSoundEffectsItemWidget::LADSPA);
#endif

    readSoundEffect();

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicPlayer::getClassName());
}

MusicSoundEffectsWidget::~MusicSoundEffectsWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    writeSoundEffect();

    delete m_ui;
}

void MusicSoundEffectsWidget::setInputObject(QObject *object)
{
    if(M_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt())
    {
        m_ui->eqButton->setText(tr("Off"));
    }
    else
    {
        m_ui->eqButton->setText(tr("On"));
    }

    connect(m_ui->eqButton, SIGNAL(clicked()), SLOT(equalizerButtonChanged()));
    connect(m_ui->eqEffectButton, SIGNAL(clicked()), object, SLOT(musicSetEqualizer()));
}

void MusicSoundEffectsWidget::equalizerButtonChanged(bool state)
{
    m_ui->eqButton->setText(state ? tr("Off") : tr("On"));
}

void MusicSoundEffectsWidget::equalizerButtonChanged()
{
    const int state = !M_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt();
    equalizerButtonChanged(state);

    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnable, state);

    Q_EMIT setEqInformation();
}

void MusicSoundEffectsWidget::stateComboBoxChanged(int index)
{
    m_ui->stateComboBox->blockSignals(true);
    m_ui->stateComboBox->setCurrentIndex(0);
    m_ui->stateComboBox->blockSignals(false);

    if(index == 1)
    {
        m_ui->BS2BWidget->setPluginEnabled(true);
        m_ui->CrossfadeWidget->setPluginEnabled(true);
        m_ui->StereoWidget->setPluginEnabled(true);
        m_ui->SOXWidget->setPluginEnabled(true);
        m_ui->SRCWidget->setPluginEnabled(true);
        m_ui->MonoStereoWidget->setPluginEnabled(true);
        m_ui->MonoWidget->setPluginEnabled(true);
#ifdef Q_OS_UNIX
        m_ui->LADSPAWidget->setPluginEnabled(true);
#endif
    }
    else if(index == 2)
    {
        m_ui->BS2BWidget->setPluginEnabled(false);
        m_ui->CrossfadeWidget->setPluginEnabled(false);
        m_ui->StereoWidget->setPluginEnabled(false);
        m_ui->SOXWidget->setPluginEnabled(false);
        m_ui->SRCWidget->setPluginEnabled(false);
        m_ui->MonoStereoWidget->setPluginEnabled(false);
        m_ui->MonoWidget->setPluginEnabled(false);
#ifdef Q_OS_UNIX
        m_ui->LADSPAWidget->setPluginEnabled(false);
#endif
    }
}

int MusicSoundEffectsWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSoundEffectsWidget::readSoundEffect()
{
    m_ui->BS2BWidget->setPluginEnabled(M_SETTING_PTR->value(MusicSettingManager::EnhancedBS2B).toInt());
    m_ui->CrossfadeWidget->setPluginEnabled(M_SETTING_PTR->value(MusicSettingManager::EnhancedCrossfade).toInt());
    m_ui->StereoWidget->setPluginEnabled(M_SETTING_PTR->value(MusicSettingManager::EnhancedStereo).toInt());
    m_ui->SOXWidget->setPluginEnabled(M_SETTING_PTR->value(MusicSettingManager::EnhancedSOX).toInt());
    m_ui->SRCWidget->setPluginEnabled(M_SETTING_PTR->value(MusicSettingManager::EnhancedSRC).toInt());
    m_ui->MonoStereoWidget->setPluginEnabled(M_SETTING_PTR->value(MusicSettingManager::EnhancedMonoStereo).toInt());
    m_ui->MonoWidget->setPluginEnabled(M_SETTING_PTR->value(MusicSettingManager::EnhancedMono).toInt());
#ifdef Q_OS_UNIX
    m_ui->LADSPAWidget->setPluginEnabled(M_SETTING_PTR->value(MusicSettingManager::EnhancedLADSPA).toInt());
#endif
}

void MusicSoundEffectsWidget::writeSoundEffect()
{
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedBS2B, m_ui->BS2BWidget->pluginEnabled());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedCrossfade, m_ui->CrossfadeWidget->pluginEnabled());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedStereo, m_ui->StereoWidget->pluginEnabled());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedSOX, m_ui->SOXWidget->pluginEnabled());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedSRC, m_ui->SRCWidget->pluginEnabled());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedMonoStereo, m_ui->MonoStereoWidget->pluginEnabled());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedMono, m_ui->MonoWidget->pluginEnabled());
#ifdef Q_OS_UNIX
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedLADSPA, m_ui->LADSPAWidget->pluginEnabled());
#endif
}
