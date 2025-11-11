#include "musicsoundeffectswidget.h"
#include "ui_musicsoundeffectswidget.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicwidgetheaders.h"
#include "musicplayer.h"

MusicSoundEffectsItemWidget::MusicSoundEffectsItemWidget(const MusicPluginProperty &property, QWidget *parent)
    : QWidget(parent),
      m_enabled(false),
      m_property(property)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);

    m_textLabel = new QLabel(TTK_SPACE + property.m_name, this);
    m_textLabel->setObjectName("Background");
    m_textLabel->setStyleSheet(QString("#%1{ %2 }").arg(m_textLabel->objectName(), TTK::UI::BackgroundStyle05) +
                               TTK::UI::SpinBoxStyle01 +
                               TTK::UI::SliderStyle06 +
                               TTK::UI::CheckBoxStyle01 +
                               TTK::UI::ComboBoxStyle01 +
                               TTK::UI::PushButtonStyle12);

    QWidget *func = new QWidget(this);
    func->setStyleSheet(TTK::UI::BackgroundStyle04);
    QHBoxLayout *funcLayout = new QHBoxLayout(func);
    funcLayout->setContentsMargins(0, 5, 5, 0);
    funcLayout->setSpacing(0);

    m_settingButton = new QPushButton(func);
    m_settingButton->setStyleSheet(TTK::UI::PushButtonStyle01);
    m_settingButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_settingButton->setFixedWidth(40);
    m_settingButton->setText(tr("Sets"));
    m_settingButton->setEnabled(false);
    connect(m_settingButton, SIGNAL(clicked()), SLOT(soundEffectValueChanged()));

    m_openButton = new QPushButton(func);
    m_openButton->setStyleSheet(TTK::UI::PushButtonStyle01);
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
    iconLabel->setStyleSheet(TTK::UI::BackgroundStyle01);
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

void MusicSoundEffectsItemWidget::setPluginEnabled(bool enabled)
{
    m_enabled = !enabled;
    setPluginEnabled();
}

bool MusicSoundEffectsItemWidget::pluginEnabled() const
{
    return m_enabled;
}

void MusicSoundEffectsItemWidget::setPluginEnabled()
{
    if(!m_enabled)
    {
        m_enabled = true;
        m_openButton->setIcon(QIcon(":/tiny/btn_effect_off"));
        TTK::TTKQmmp::setEffectEnabled(m_property.m_type, true);

        m_settingButton->setEnabled(m_property.m_hasSettings);
        m_openButton->setToolTip(tr("Off"));
    }
    else
    {
        m_enabled = false;
        m_openButton->setIcon(QIcon(":/tiny/btn_effect_on"));
        TTK::TTKQmmp::setEffectEnabled(m_property.m_type, false);
        m_settingButton->setEnabled(false);
        m_openButton->setToolTip(tr("On"));
    }
}

void MusicSoundEffectsItemWidget::soundEffectValueChanged()
{
    TTK::TTKQmmp::showEffectSetting(m_property.m_type);
}



MusicSoundEffectsWidget::MusicSoundEffectsWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSoundEffectsWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    TTK::Widget::generateComboBoxStyle(m_ui->stateComboBox);
    m_ui->stateComboBox->addItems({tr("OperatorAll"), tr("All Off")});
    connect(m_ui->stateComboBox, SIGNAL(currentIndexChanged(int)), SLOT(stateComboBoxChanged(int)));

    m_ui->eqButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->eqButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->eqEffectButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->eqEffectButton->setStyleSheet(TTK::UI::PushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->eqButton->setFocusPolicy(Qt::NoFocus);
    m_ui->eqEffectButton->setFocusPolicy(Qt::NoFocus);
#endif

    TTK::Widget::generateVScrollAreaStyle(m_ui->scrollArea, m_ui->effectContainer);
    initialize();

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->connect(className(), MusicPlayer::className());
}

MusicSoundEffectsWidget::~MusicSoundEffectsWidget()
{
    G_CONNECTION_PTR->removeValue(this);
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
    connect(m_ui->eqEffectButton, SIGNAL(clicked()), object, SLOT(showEqualizerWidget()));
}

void MusicSoundEffectsWidget::updateConfig(bool v)
{
    if(v)
    {
        const QString &value = G_SETTING_PTR->value(MusicSettingManager::EnhancedEffectValue).toString();
        const QStringList &effects = value.split(";", QtSkipEmptyParts);
        for(const QString &effect : qAsConst(effects))
        {
            TTK::TTKQmmp::setEffectEnabled(effect, true);
        }
    }
    else
    {
        QString value;
        for(const MusicPluginProperty &property : TTK::TTKQmmp::effectModules())
        {
            if(TTK::TTKQmmp::isEffectEnabled(property.m_type))
            {
                value.append(property.m_type + ";");
            }
        }

        G_SETTING_PTR->setValue(MusicSettingManager::EnhancedEffectValue, value);
    }
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
        for(MusicSoundEffectsItemWidget *item : qAsConst(m_items))
        {
            item->setPluginEnabled(false);
        }
    }
}

void MusicSoundEffectsWidget::initialize()
{
    QVBoxLayout *layout = new QVBoxLayout(m_ui->effectContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    m_ui->effectContainer->setLayout(layout);

    for(const MusicPluginProperty &property : TTK::TTKQmmp::effectModules())
    {
        MusicSoundEffectsItemWidget *item = new MusicSoundEffectsItemWidget(property, this);
        m_items.push_back(item);
        item->setPluginEnabled(TTK::TTKQmmp::isEffectEnabled(property.m_type));
        layout->addWidget(item);
    }
}
