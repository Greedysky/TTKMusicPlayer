#include "musicsoundeffectswidget.h"
#include "ui_musicsoundeffectswidget.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicuiobject.h"
#include "musicplayer.h"
#include "musicwidgetheaders.h"
#include "musicqmmputils.h"

#include <QStyledItemDelegate>

MusicSoundEffectsItemWidget::MusicSoundEffectsItemWidget(Type type, QWidget *parent)
    : QWidget(parent)
{
    m_type = type;
    m_enable = false;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);

    m_textLabel = new QLabel(" " + transformQStringFromEnum(m_type), this);
    m_textLabel->setObjectName("Background");
    m_textLabel->setStyleSheet(QString("#Background{%1}").arg(MusicUIObject::MQSSBackgroundStyle08) +
                               MusicUIObject::MQSSSpinBoxStyle01 +
                               MusicUIObject::MQSSSliderStyle06 +
                               MusicUIObject::MQSSCheckBoxStyle01 +
                               MusicUIObject::MQSSComboBoxStyle01 +
                               MusicUIObject::MQSSPushButtonStyle15);

    QWidget *func = new QWidget(this);
    func->setStyleSheet(MusicUIObject::MQSSBackgroundStyle04);
    QHBoxLayout *funcLayout = new QHBoxLayout(func);
    funcLayout->setContentsMargins(0, 5, 5, 0);
    funcLayout->setSpacing(0);

    m_settingButton = new QPushButton(func);
    m_settingButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle01);
    m_settingButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_settingButton->setFixedWidth(40);
    m_settingButton->setText(tr("PSet"));
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
    setFixedHeight(60);
}

MusicSoundEffectsItemWidget::~MusicSoundEffectsItemWidget()
{
    delete m_textLabel;
    delete m_openButton;
    delete m_settingButton;
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
    MusicUtils::QMMP::showEffectSetting(plugin, m_textLabel);
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
        case MonoToStereo: plugin = "monotostereo"; break;
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
    m_ui->stateComboBox->addItems(QStringList() << tr("OperatorAll") << tr("All Off"));
    connect(m_ui->stateComboBox, SIGNAL(currentIndexChanged(int)), SLOT(stateComboBoxChanged(int)));

    m_ui->eqButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->eqButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->eqEffectButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->eqEffectButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->eqButton->setFocusPolicy(Qt::NoFocus);
    m_ui->eqEffectButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->scrollArea->setWidgetResizable(true);
    m_ui->scrollArea->setFrameShape(QFrame::NoFrame);
    m_ui->scrollArea->setFrameShadow(QFrame::Plain);
    m_ui->scrollArea->setAlignment(Qt::AlignVCenter);
    m_ui->scrollArea->setWidget(m_ui->effectContainer);
    m_ui->scrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);

    m_items.push_back(new MusicSoundEffectsItemWidget(MusicSoundEffectsItemWidget::BS2B, this));
    m_items.push_back(new MusicSoundEffectsItemWidget(MusicSoundEffectsItemWidget::Crossfade, this));
    m_items.push_back(new MusicSoundEffectsItemWidget(MusicSoundEffectsItemWidget::Stereo, this));
    m_items.push_back(new MusicSoundEffectsItemWidget(MusicSoundEffectsItemWidget::Soxr, this));
    m_items.push_back(new MusicSoundEffectsItemWidget(MusicSoundEffectsItemWidget::SrcConverter, this));
    m_items.push_back(new MusicSoundEffectsItemWidget(MusicSoundEffectsItemWidget::MonoToStereo, this));
    m_items.push_back(new MusicSoundEffectsItemWidget(MusicSoundEffectsItemWidget::Mono, this));
#ifdef Q_OS_UNIX
    m_items.push_back(new MusicSoundEffectsItemWidget(MusicSoundEffectsItemWidget::LADSPA, this));
#endif

    QVBoxLayout *layout = new QVBoxLayout(m_ui->effectContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    m_ui->effectContainer->setLayout(layout);

    for(MusicSoundEffectsItemWidget *item : m_items)
    {
        layout->addWidget(item);
    }

    readSoundEffect();

    G_CONNECTION_PTR->setValue(getClassName(), this);
    G_CONNECTION_PTR->poolConnect(getClassName(), MusicPlayer::getClassName());
}

MusicSoundEffectsWidget::~MusicSoundEffectsWidget()
{
    G_CONNECTION_PTR->removeValue(getClassName());
    writeSoundEffect();
    qDeleteAll(m_items);
    delete m_ui;
}

void MusicSoundEffectsWidget::setInputObject(QObject *object)
{
    if(G_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt())
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
    const int state = !G_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt();
    equalizerButtonChanged(state);

    G_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnable, state);

    Q_EMIT setEqInformation();
}

void MusicSoundEffectsWidget::stateComboBoxChanged(int index)
{
    m_ui->stateComboBox->blockSignals(true);
    m_ui->stateComboBox->setCurrentIndex(0);
    m_ui->stateComboBox->blockSignals(false);

    if(index == 1)
    {
        for(MusicSoundEffectsItemWidget *item : m_items)
        {
            item->setPluginEnabled(false);
        }
    }
}

int MusicSoundEffectsWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSoundEffectsWidget::readSoundEffect()
{
    m_items[0]->setPluginEnabled(G_SETTING_PTR->value(MusicSettingManager::EnhancedBS2B).toInt());
    m_items[1]->setPluginEnabled(G_SETTING_PTR->value(MusicSettingManager::EnhancedCrossfade).toInt());
    m_items[2]->setPluginEnabled(G_SETTING_PTR->value(MusicSettingManager::EnhancedStereo).toInt());
    m_items[3]->setPluginEnabled(G_SETTING_PTR->value(MusicSettingManager::EnhancedSOX).toInt());
    m_items[4]->setPluginEnabled(G_SETTING_PTR->value(MusicSettingManager::EnhancedSRC).toInt());
    m_items[5]->setPluginEnabled(G_SETTING_PTR->value(MusicSettingManager::EnhancedMonoToStereo).toInt());
    m_items[6]->setPluginEnabled(G_SETTING_PTR->value(MusicSettingManager::EnhancedMono).toInt());
#ifdef Q_OS_UNIX
    m_items[7]->setPluginEnabled(G_SETTING_PTR->value(MusicSettingManager::EnhancedLADSPA).toInt());
#endif
}

void MusicSoundEffectsWidget::writeSoundEffect()
{
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedBS2B, m_items[0]->pluginEnabled());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedCrossfade, m_items[1]->pluginEnabled());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedStereo, m_items[2]->pluginEnabled());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedSOX, m_items[3]->pluginEnabled());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedSRC, m_items[4]->pluginEnabled());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedMonoToStereo, m_items[5]->pluginEnabled());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedMono, m_items[6]->pluginEnabled());
#ifdef Q_OS_UNIX
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedLADSPA, m_items[7]->pluginEnabled());
#endif
}
