#include "musicsoundeffectswidget.h"
#include "ui_musicsoundeffectswidget.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicplayer.h"
#include "musicwidgetheaders.h"

MusicSoundEffectsItemWidget::MusicSoundEffectsItemWidget(const MusicPluginProperty &property, QWidget *parent)
    : QWidget(parent)
{
    m_property = property;
    m_enable = false;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);

    m_textLabel = new QLabel(" " + property.m_name, this);
    m_textLabel->setObjectName("Background");
    m_textLabel->setStyleSheet(QString("#Background{%1}").arg(MusicUIObject::MQSSBackgroundStyle08) +
                               MusicUIObject::MQSSSpinBoxStyle01 +
                               MusicUIObject::MQSSSliderStyle06 +
                               MusicUIObject::MQSSCheckBoxStyle01 +
                               MusicUIObject::MQSSComboBoxStyle01 +
                               MusicUIObject::MQSSPushButtonStyle13);

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

void MusicSoundEffectsItemWidget::soundEffectChanged(const QString &name, bool enable)
{
    MusicUtils::QMMP::enabledEffectPlugin(name, enable);
}

void MusicSoundEffectsItemWidget::setPluginEnabled()
{
    if(!m_enable)
    {
        m_enable = true;
        m_openButton->setIcon(QIcon(":/tiny/btn_effect_off"));
        soundEffectChanged(m_property.m_type, true);

        m_settingButton->setEnabled(m_property.m_setting);
        m_openButton->setToolTip(tr("Off"));
    }
    else
    {
        m_enable = false;
        m_openButton->setIcon(QIcon(":/tiny/btn_effect_on"));
        soundEffectChanged(m_property.m_type, false);
        m_settingButton->setEnabled(false);
        m_openButton->setToolTip(tr("On"));
    }
}

void MusicSoundEffectsItemWidget::soundEffectValueChanged()
{
    MusicUtils::QMMP::showEffectSetting(m_property.m_type, m_textLabel);
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

    MusicUtils::Widget::generateComboBoxFormat(m_ui->stateComboBox);
    m_ui->stateComboBox->addItems({tr("OperatorAll"), tr("All Off")});
    connect(m_ui->stateComboBox, SIGNAL(currentIndexChanged(int)), SLOT(stateComboBoxChanged(int)));

    m_ui->eqButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->eqButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->eqEffectButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->eqEffectButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->eqButton->setFocusPolicy(Qt::NoFocus);
    m_ui->eqEffectButton->setFocusPolicy(Qt::NoFocus);
#endif

    MusicUtils::Widget::generateVScrollAreaFormat(m_ui->scrollArea, m_ui->effectContainer);
    readSoundEffect();

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->poolConnect(className(), MusicPlayer::className());
}

MusicSoundEffectsWidget::~MusicSoundEffectsWidget()
{
    G_CONNECTION_PTR->removeValue(className());
    writeSoundEffect();
    qDeleteAll(m_items);
    delete m_ui;
}

void MusicSoundEffectsWidget::setInputModule(QObject *object)
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
    Q_EMIT setEqualizerConfig();
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
    QVBoxLayout *layout = new QVBoxLayout(m_ui->effectContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    m_ui->effectContainer->setLayout(layout);

    const QString &value = G_SETTING_PTR->value(MusicSettingManager::EnhancedEffectValue).toString();
#if TTK_QT_VERSION_CHECK(5,15,0)
    const QStringList &effects = value.split(";", Qt::SkipEmptyParts);
#else
    const QStringList &effects = value.split(";", QString::SkipEmptyParts);
#endif
    for(const MusicPluginProperty &property : MusicUtils::QMMP::effectPlugins())
    {
        MusicSoundEffectsItemWidget *item = new MusicSoundEffectsItemWidget(property, this);
        m_items.push_back(item);
        layout->addWidget(item);

        if(effects.contains(property.m_type))
        {
            item->setPluginEnabled(true);
        }
    }
}

void MusicSoundEffectsWidget::writeSoundEffect()
{
    QString value;
    for(const MusicSoundEffectsItemWidget *item : qAsConst(m_items))
    {
        if(item->pluginEnabled())
        {
            value.append(item->type() + ";");
        }
    }
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedEffectValue, value);
}
