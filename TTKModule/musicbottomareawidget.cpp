#include "musicbottomareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musicsystemtraymenu.h"
#include "musicplatformextras.h"
#include "musicfunctionuiobject.h"
#include "musictinyuiobject.h"
#include "musicrightareawidget.h"
#include "musicblurspecturmmodule.h"

MusicBottomAreaWidget *MusicBottomAreaWidget::m_instance = nullptr;

MusicBottomAreaWidget::MusicBottomAreaWidget(QWidget *parent)
    : QWidget(parent),
      m_isAvailable(false),
      m_systemCloseConfig(false),
      m_lrcWidgetShowFullScreen(true)
{
    m_instance = this;

    createSystemTrayIcon();

    m_platformExtras = new MusicPlatformExtras(this);
    m_blurModule = new MusicBlurSpecturmModule(this);
}

MusicBottomAreaWidget::~MusicBottomAreaWidget()
{
    delete m_systemTrayMenu;
    delete m_systemTray;
    delete m_platformExtras;
    delete m_blurModule;
}

MusicBottomAreaWidget *MusicBottomAreaWidget::instance()
{
    return m_instance;
}

void MusicBottomAreaWidget::setupUi(Ui::MusicApplication *ui)
{
    m_ui = ui;
    //
    m_blurModule->initialize(ui->backgroundLayout, ui->bottomWidget);

    ui->resizeLabelWidget->setPixmap(QPixmap(":/tiny/lb_resize_normal"));
    ui->songTitleLabel->setEffectOnResize(true);
    connect(ui->desktopLrcButton, SIGNAL(clicked()), m_systemTrayMenu, SLOT(showDesktopLrc()));
}

void MusicBottomAreaWidget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::DoubleClick: break;
        case QSystemTrayIcon::Trigger:
        {
            MusicApplication *w = MusicApplication::instance();
            if(w->isMinimized() || w->isHidden())
            {
                w->showNormal();
                w->activateWindow();
            }
            break;
        }
        default: break;
    }
}

void MusicBottomAreaWidget::createSystemTrayIcon()
{
    m_isAvailable = QSystemTrayIcon::isSystemTrayAvailable();
    m_systemTray = new QSystemTrayIcon(MusicApplication::instance());
    m_systemTray->setIcon(QIcon(":/image/lb_app_logo"));
    m_systemTray->setToolTip(tr("TTKMusicPlayer"));

    m_systemTrayMenu = new MusicSystemTrayMenu(MusicApplication::instance());

    if(m_isAvailable)
    {
        m_systemTray->setContextMenu(m_systemTrayMenu);
        m_systemTray->show();
    }
    else
    {
        TTK_INFO_STREAM("Current no systemtray available");
    }

    connect(m_systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MusicBottomAreaWidget::setDestopLrcVisible(bool status) const
{
    m_systemTrayMenu->showDesktopLrc(status);
}

void MusicBottomAreaWidget::setCurrentPlayState(bool state) const
{
    m_systemTrayMenu->setCurrentPlayState(state);
    m_platformExtras->setCurrentPlayState(state);
}

void MusicBottomAreaWidget::setVolumeValue(int value) const
{
    m_systemTrayMenu->setVolumeValue(value);
}

void MusicBottomAreaWidget::setLabelText(const QString &name) const
{
    m_systemTrayMenu->setLabelText(name);
    m_systemTray->setToolTip(name);
}

void MusicBottomAreaWidget::showMessage(const QString &title, const QString &text)
{
    m_systemTray->showMessage(title, text);
}

bool MusicBottomAreaWidget::setWindowConcise()
{
    const bool concise = !G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toBool();

    m_ui->topRightWidget->setVisible(!concise);
    m_ui->centerRightWidget->setVisible(!concise);
    m_ui->bottomCenterWidget->setVisible(!concise);
    m_ui->bottomRightWidget->setVisible(!concise);
    m_ui->bottomLeftContainWidget->setMinimumWidth(concise ? CONCISE_WIDTH_MIN : 220);

    m_ui->windowConcise->setParent(concise ? m_ui->background : m_ui->topRightWidget);
    m_ui->windowConcise->setStyleSheet(concise ? TTK::UI::BtnConciseOut : TTK::UI::BtnConciseIn);
    m_ui->minimization->setParent(concise ? m_ui->background : m_ui->topRightWidget);
    m_ui->windowClose->setParent(concise ? m_ui->background : m_ui->topRightWidget);

    m_ui->bestLoveButton->setParent(concise ? m_ui->background : m_ui->bottomRightWidget);
    m_ui->downloadButton->setParent(concise ? m_ui->background : m_ui->bottomRightWidget);
    m_ui->moreFunctionButton->setParent(concise ? m_ui->background : m_ui->bottomRightWidget);
    m_ui->soundButton->setParent(concise ? m_ui->background : m_ui->bottomRightWidget);
    m_ui->desktopLrcButton->setParent(concise ? m_ui->background : m_ui->bottomRightWidget);

    m_ui->timeSliderWidget->setParent(concise ? m_ui->background : m_ui->bottomCenterWidget);

    MusicApplication *app = MusicApplication::instance();
    if(concise)
    {
        app->setMinimumSize(CONCISE_WIDTH_MIN, WINDOW_HEIGHT_MIN);
        app->setMaximumSize(CONCISE_WIDTH_MIN, WINDOW_HEIGHT_MIN);

        m_ui->windowConcise->move(245, 20);
        m_ui->windowConcise->show();
        m_ui->minimization->move(270, 20);
        m_ui->minimization->show();
        m_ui->windowClose->move(295, 20);
        m_ui->windowClose->show();

        m_ui->previousButton->setStyleSheet(TTK::UI::TinyBtnPrevious);
        m_ui->playButton->setStyleSheet(app->isPlaying() ? TTK::UI::TinyBtnPause : TTK::UI::TinyBtnPlay);
        m_ui->nextButton->setStyleSheet(TTK::UI::TinyBtnNext);
        m_ui->previousButton->setFixedSize(28, 28);
        m_ui->playButton->setFixedSize(28, 28);
        m_ui->nextButton->setFixedSize(28, 28);

        m_ui->bottomLeftWidgetLayout->addWidget(m_ui->bestLoveButton);
        m_ui->bottomLeftWidgetLayout->addWidget(m_ui->downloadButton);
        m_ui->bottomLeftWidgetLayout->addWidget(m_ui->moreFunctionButton);
        m_ui->bottomLeftWidgetLayout->addWidget(m_ui->soundButton);
        m_ui->bottomLeftWidgetLayout->addWidget(m_ui->desktopLrcButton);

        m_ui->bottomLeftContainWidgetLayout->addWidget(m_ui->timeSliderWidget);
    }
    else
    {
        const QSize &size = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
        app->setMinimumSize(WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
        app->setMaximumSize(size.width(), size.height());

        m_ui->previousButton->setStyleSheet(TTK::UI::BtnPrevious);
        m_ui->playButton->setStyleSheet(app->isPlaying() ? TTK::UI::BtnPause : TTK::UI::BtnPlay);
        m_ui->nextButton->setStyleSheet(TTK::UI::BtnNext);
        m_ui->previousButton->setFixedSize(44, 44);
        m_ui->playButton->setFixedSize(44, 44);
        m_ui->nextButton->setFixedSize(44, 44);

        m_ui->topRightWidgetLayout->insertWidget(11, m_ui->windowConcise);
        m_ui->topRightWidgetLayout->addWidget(m_ui->minimization);
        m_ui->topRightWidgetLayout->addWidget(m_ui->windowClose);

        m_ui->bottomRightWidgetLayout->insertWidget(0, m_ui->bestLoveButton);
        m_ui->bottomRightWidgetLayout->insertWidget(1, m_ui->downloadButton);
        m_ui->bottomRightWidgetLayout->insertWidget(2, m_ui->moreFunctionButton);
        m_ui->bottomRightWidgetLayout->insertWidget(4, m_ui->soundButton);
        m_ui->bottomRightWidgetLayout->insertWidget(6, m_ui->desktopLrcButton);

        m_ui->bottomCenterWidgetLayout->addWidget(m_ui->timeSliderWidget, 3, 0, 1, 6);
    }

    m_platformExtras->setAction();
    G_SETTING_PTR->setValue(MusicSettingManager::WindowConciseMode, concise);
    return concise;
}

bool MusicBottomAreaWidget::isLrcWidgetShowFullScreen() const
{
    return !m_lrcWidgetShowFullScreen;
}

void MusicBottomAreaWidget::resizeWindow()
{
    const int height = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().height() - 155 - m_ui->lrcDisplayAllButton->height() - 40;
    m_ui->lrcDisplayAllButton->move(m_ui->lrcDisplayAllButton->x(), height / 2);
}

void MusicBottomAreaWidget::applyParameter()
{
    setSystemCloseConfig(G_SETTING_PTR->value(MusicSettingManager::CloseEventMode).toBool());
    setDestopLrcVisible(G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toBool());

    if(G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumEnable).toBool())
    {
        m_blurModule->update(!G_SETTING_PTR->value(MusicSettingManager::RippleLowPowerMode).toBool());
    }
    else
    {
        m_blurModule->update(false);
    }
}

void MusicBottomAreaWidget::lrcWidgetShowFullScreen()
{
    if(G_SETTING_PTR->value(MusicSettingManager::OtherSideByInMode).toBool())
    {
        return;
    }

    if(m_ui->lrcForInterior->lrcDisplayExpand())
    {
        MusicRightAreaWidget::instance()->lrcDisplayAllClicked();
    }

    m_lrcWidgetShowFullScreen = !m_lrcWidgetShowFullScreen;
    m_blurModule->setVisible(m_lrcWidgetShowFullScreen);

    m_ui->topWidget->setVisible(m_lrcWidgetShowFullScreen);
    m_ui->bottomWidget->setVisible(m_lrcWidgetShowFullScreen);
    m_ui->centerLeftWidget->setVisible(m_lrcWidgetShowFullScreen);
    m_ui->songsContainer->setVisible(m_lrcWidgetShowFullScreen);
    m_ui->stackedFunctionWidget->setVisible(m_lrcWidgetShowFullScreen);
    m_ui->lrcDisplayAllButton->setVisible(m_lrcWidgetShowFullScreen);

    m_ui->lrcForInterior->createFloatPlayWidget();
    m_lrcWidgetShowFullScreen ? MusicApplication::instance()->showNormal() : MusicApplication::instance()->showFullScreen();
    m_ui->lrcForInterior->lrcWidgetShowFullScreen();
}

void MusicBottomAreaWidget::lockDesktopLrc(bool lock)
{
    m_systemTrayMenu->lockDesktopLrc(lock);
}

void MusicBottomAreaWidget::desktopLrcClosed()
{
    m_ui->desktopLrcButton->setChecked(false);
    m_systemTrayMenu->showDesktopLrc(false);
    G_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc, false);
}
