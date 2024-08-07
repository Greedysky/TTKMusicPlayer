#include "musicenhancedpopwidget.h"
#include "musicsettingmanager.h"
#include "musicfunctionuiobject.h"
#include "musicmagicwidgetuiobject.h"
#include "musicapplicationmodule.h"
#include "musicrightareawidget.h"
#include "musicqmmputils.h"

#include <QLabel>
#include <QButtonGroup>

static constexpr int LABEL_ANIMAT_WIDGET = 156;
static constexpr int LABEL_BUTTON_WIDGET = 116;
static constexpr int LABEL_BUTTON_HEIGHT = 111;

MusicEnhancedToolButton::MusicEnhancedToolButton(QWidget *parent)
    : QToolButton(parent),
      m_state(false)
{
    m_animationLabel = new QLabel(this);
    m_animationLabel->setGeometry(-LABEL_ANIMAT_WIDGET, 0, LABEL_ANIMAT_WIDGET, LABEL_BUTTON_HEIGHT);
    m_animationLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    m_foreLabel = new QLabel(this);
    m_foreLabel->setObjectName("ForeLabel");
    m_foreLabel->resize(LABEL_ANIMAT_WIDGET, LABEL_BUTTON_HEIGHT);

    m_animation = new QPropertyAnimation(m_animationLabel, "pos", this);
    m_animation->setDuration(TTK_DN_S2MS / 2);
    m_animation->setStartValue(QPoint(-LABEL_ANIMAT_WIDGET, 0));
    m_animation->setEndValue(QPoint(LABEL_ANIMAT_WIDGET, 0));
    connect(m_animation, SIGNAL(finished()), SLOT(finished()));
}

MusicEnhancedToolButton::~MusicEnhancedToolButton()
{
    delete m_foreLabel;
    delete m_animationLabel;
    delete m_animation;
}

void MusicEnhancedToolButton::setStyleSheet(const QString &styleSheet, bool state)
{
    m_state = state;
    if(m_state)
    {
        m_animation->setDuration(2 * TTK_DN_S2MS);
        m_foreLabel->setStyleSheet(QString("#%1{ %2 }").arg(m_foreLabel->objectName(), styleSheet));
        m_animationLabel->setStyleSheet("background-image:url(':/enhance/lb_selected')");
        QToolButton::setStyleSheet(QString("QToolButton{ background-image:url(':/enhance/lb_blue'); }"));
    }
    else
    {
        m_animation->setDuration(TTK_DN_S2MS / 2);
        m_foreLabel->setStyleSheet({});
        m_animationLabel->setStyleSheet("background-image:url(':/enhance/lb_enter')");
        QToolButton::setStyleSheet(QString("QToolButton{ %1 }").arg(styleSheet));
    }
}

void MusicEnhancedToolButton::start()
{
    m_animation->start();
}

void MusicEnhancedToolButton::stop()
{
    m_animation->stop();
}

void MusicEnhancedToolButton::finished()
{
    if(m_state)
    {
        start();
    }
}

void MusicEnhancedToolButton::enterEvent(QtEnterEvent *event)
{
    QToolButton::enterEvent(event);
    if(!m_state)
    {
        start();
    }
}


MusicEnhancedPopWidget::MusicEnhancedPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    setToolTip(tr("Magic Music"));

    initialize();

    connect(MusicApplicationModule::instance(), SIGNAL(enhancedSongChanged(int)), SLOT(setEnhancedSongConfig(int)));
    connect(m_menu, SIGNAL(windowStateChanged(bool)), SLOT(buttonAnimationChanged(bool)));
}

MusicEnhancedPopWidget::~MusicEnhancedPopWidget()
{
    delete m_caseButton;
    qDeleteAll(m_buttons);
}

void MusicEnhancedPopWidget::setEnhancedSongConfig(int type)
{
    setObjectName(className());
    QString style = TTK::UI::BtnMagic;
    switch(type)
    {
        case 0: style += "#%1{ margin-left: 0px; }"; break;
        case 1: style += "#%1{ margin-left: -48px; }"; break;
        case 2: style += "#%1{ margin-left: -192px; }"; break;
        case 3: style += "#%1{ margin-left: -96px; }"; break;
        case 4: style += "#%1{ margin-left: -144px; }"; break;
        default: break;
    }
    setStyleSheet(style.arg(objectName()));

    const QString &prfix = QString("background-image:url(':/enhance/lb_%1')");
    m_caseButton->setStyleSheet(type ? TTK::UI::EnhanceOn : TTK::UI::EnhanceOff);
    m_buttons[0]->setStyleSheet(prfix.arg(type == 1 ? "3d_on" : "3d_off"), type == 1);
    m_buttons[1]->setStyleSheet(prfix.arg(type == 2 ? "nicam_on" : "nicam_off"), type == 2);
    m_buttons[2]->setStyleSheet(prfix.arg(type == 3 ? "subwoofer_on" : "subwoofer_off"), type == 3);
    m_buttons[3]->setStyleSheet(prfix.arg(type == 4 ? "vocal_on" : "vocal_off"), type == 4);

    m_lastSelectedIndex = (type == 0) ? m_lastSelectedIndex : type;
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedMusicIndex, type);

    if(type != 0)
    {
        G_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnable, 0);
    }

    TTK::TTKQmmp::enabledEffectPlugin(false);
    Q_EMIT enhancedSongChanged(type);
    m_menu->close();
}

void MusicEnhancedPopWidget::switchButtonState()
{
    setEnhancedSongConfig(m_caseButton->styleSheet().contains(":/enhance/btn_magic_off_normal") ? m_lastSelectedIndex : 0);
}

void MusicEnhancedPopWidget::buttonAnimationChanged(bool state)
{
    const int index = G_SETTING_PTR->value(MusicSettingManager::EnhancedMusicIndex).toInt();
    if(index < 1 || index > m_buttons.count())
    {
        return;
    }

    state ? m_buttons[index - 1]->start() : m_buttons[index - 1]->stop();
}

void MusicEnhancedPopWidget::helpButtonClicked()
{
    MusicRightAreaWidget::instance()->functionClicked(MusicRightAreaWidget::KuiSheWidget);
    m_menu->close();
}

void MusicEnhancedPopWidget::initialize()
{
    setTranslucentBackground();

    m_menu->setStyleSheet(TTK::UI::MenuStyle05);
    m_containWidget->setFixedSize(272, 370);
    m_containWidget->setObjectName("ContainWidget");
    m_containWidget->setStyleSheet(QString("#%1{ %2 %3 }").arg(m_containWidget->objectName(), TTK::UI::BorderStyle01, "background:url(':/enhance/lb_background')"));

    QToolButton *labelButton = new QToolButton(m_containWidget);
    labelButton->setGeometry(80, 20, 126, 40);
    labelButton->setStyleSheet(TTK::UI::EnhanceTitle);
    labelButton->setCursor(Qt::PointingHandCursor);

    QToolButton *helpButton = new QToolButton(m_containWidget);
    helpButton->setGeometry(205, 3, 24, 24);
    helpButton->setStyleSheet(TTK::UI::EnhanceHelp);
    helpButton->setCursor(Qt::PointingHandCursor);
    connect(helpButton, SIGNAL(clicked()), SLOT(helpButtonClicked()));

    QToolButton *shareButton = new QToolButton(m_containWidget);
    shareButton->setGeometry(230, 3, 24, 24);
    shareButton->setStyleSheet(TTK::UI::EnhanceShare);
    shareButton->setCursor(Qt::PointingHandCursor);

    QToolButton *closeButton = new QToolButton(m_containWidget);
    closeButton->setGeometry(255, 8, 16, 16);
    closeButton->setStyleSheet(TTK::UI::EnhanceClose);
    closeButton->setCursor(Qt::PointingHandCursor);
    connect(closeButton, SIGNAL(clicked()), m_menu, SLOT(close()));

    m_caseButton = new QToolButton(m_containWidget);
    m_caseButton->setGeometry(200, 70, 62, 38);
    m_caseButton->setStyleSheet(TTK::UI::EnhanceOn);
    m_caseButton->setCursor(Qt::PointingHandCursor);

    MusicEnhancedToolButton *button1 = new MusicEnhancedToolButton(m_containWidget);
    button1->setGeometry(15, 115, LABEL_BUTTON_WIDGET, LABEL_BUTTON_HEIGHT);
    button1->setStyleSheet("background-image:url(':/enhance/lb_3d_off')");
    button1->setCursor(Qt::PointingHandCursor);

    MusicEnhancedToolButton *button2 = new MusicEnhancedToolButton(m_containWidget);
    button2->setGeometry(145, 115, LABEL_BUTTON_WIDGET, LABEL_BUTTON_HEIGHT);
    button2->setStyleSheet("background-image:url(':/enhance/lb_nicam_off')");
    button2->setCursor(Qt::PointingHandCursor);

    MusicEnhancedToolButton *button3 = new MusicEnhancedToolButton(m_containWidget);
    button3->setGeometry(15, 240, LABEL_BUTTON_WIDGET, LABEL_BUTTON_HEIGHT);
    button3->setStyleSheet("background-image:url(':/enhance/lb_subwoofer_off')");
    button3->setCursor(Qt::PointingHandCursor);

    MusicEnhancedToolButton *button4 = new MusicEnhancedToolButton(m_containWidget);
    button4->setGeometry(145, 240, LABEL_BUTTON_WIDGET, LABEL_BUTTON_HEIGHT);
    button4->setStyleSheet("background-image:url(':/enhance/lb_vocal_off')");
    button4->setCursor(Qt::PointingHandCursor);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(button1, 1);
    buttonGroup->addButton(button2, 2);
    buttonGroup->addButton(button3, 3);
    buttonGroup->addButton(button4, 4);
    QtButtonGroupConnect(buttonGroup, this, setEnhancedSongConfig, TTK_SLOT);
    m_buttons << button1 << button2 << button3 << button4;

    m_lastSelectedIndex = G_SETTING_PTR->value(MusicSettingManager::EnhancedMusicIndex).toInt();
    connect(m_caseButton, SIGNAL(clicked()), SLOT(switchButtonState()));
}
