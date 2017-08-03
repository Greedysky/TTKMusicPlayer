#include "musicbottomareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musicuiobject.h"
#include "musicsystemtraymenu.h"
#include "musicwindowextras.h"
#include "musiclocalsongsearchdialog.h"
#include "musicfunctionuiobject.h"

MusicBottomAreaWidget *MusicBottomAreaWidget::m_instance = nullptr;

MusicBottomAreaWidget::MusicBottomAreaWidget(QWidget *parent)
    : QWidget(parent), m_musicSongSearchWidget(nullptr)
{
    m_instance = this;
    m_systemCloseConfig = false;

    createSystemTrayIcon();

    m_musicWindowExtras = new MusicWindowExtras(parent);
}

MusicBottomAreaWidget::~MusicBottomAreaWidget()
{
    delete m_systemTrayMenu;
    delete m_systemTray;
    delete m_musicWindowExtras;
    delete m_musicSongSearchWidget;
}

QString MusicBottomAreaWidget::getClassName()
{
    return staticMetaObject.className();
}

MusicBottomAreaWidget *MusicBottomAreaWidget::instance()
{
    return m_instance;
}

void MusicBottomAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    ui->showCurrentSong->setEffectOnResize(true);
    connect(ui->musicDesktopLrc, SIGNAL(clicked()), m_systemTrayMenu, SLOT(showDesktopLrc()));
}

void MusicBottomAreaWidget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::DoubleClick:
            break;
        case QSystemTrayIcon::Trigger:
            if(!MusicApplication::instance()->isVisible())
            {
                MusicApplication::instance()->show();
                MusicApplication::instance()->activateWindow();
            }
            break;
        default:
            break;
    }
}

void MusicBottomAreaWidget::createSystemTrayIcon()
{
    m_systemTray = new QSystemTrayIcon(MusicApplication::instance());
    m_systemTray->setIcon(QIcon(QString::fromUtf8(":/image/lb_player_logo")));
    m_systemTray->setToolTip(tr("TTKMusicPlayer"));

    m_systemTrayMenu = new MusicSystemTrayMenu(MusicApplication::instance());

    m_systemTray->setContextMenu(m_systemTrayMenu);
    m_systemTray->show();
    connect(m_systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                          SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MusicBottomAreaWidget::setDestopLrcVisible(bool status) const
{
    m_systemTrayMenu->showDesktopLrc(status);
}

void MusicBottomAreaWidget::showPlayStatus(bool status) const
{
    m_systemTrayMenu->showPlayStatus(status);
#if defined Q_OS_WIN && defined MUSIC_WINEXTRAS
    m_musicWindowExtras->showPlayStatus(status);
#endif
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

#if defined MUSIC_DEBUG && defined Q_OS_WIN && defined MUSIC_WINEXTRAS
void MusicBottomAreaWidget::setValue(int value) const
{
    m_musicWindowExtras->setValue(value);
}

void MusicBottomAreaWidget::setRange(int min, int max) const
{
    m_musicWindowExtras->setRange(min, max);
}
#endif

void MusicBottomAreaWidget::setWindowConcise()
{
    bool con = m_musicWindowExtras->isDisableBlurBehindWindow();
    M_SETTING_PTR->setValue(MusicSettingManager::WindowConciseChoiced, con);

    m_ui->topRightWidget->setVisible(!con);
    m_ui->centerRightWidget->setVisible(!con);
    m_ui->bottomCenterWidget->setVisible(!con);
    m_ui->bottomRightWidget->setVisible(!con);
    m_ui->bottomLeftWidget->setMinimumWidth(con ? 370 : 220);

    m_ui->musicWindowConcise->setParent(con ? m_ui->background : m_ui->topRightWidget);
    m_ui->musicWindowConcise->setStyleSheet(con ? MusicUIObject::MKGBtnConciseOut : MusicUIObject::MKGBtnConciseIn);
    m_ui->minimization->setParent(con ? m_ui->background : m_ui->topRightWidget);
    m_ui->windowClose->setParent(con ? m_ui->background : m_ui->topRightWidget);

    m_ui->musicBestLove->setParent(con ? m_ui->background : m_ui->bottomRightWidget);
    m_ui->musicDownload->setParent(con ? m_ui->background : m_ui->bottomRightWidget);
    m_ui->musicMoreFunction->setParent(con ? m_ui->background : m_ui->bottomRightWidget);
    m_ui->musicSound->setParent(con ? m_ui->background : m_ui->bottomRightWidget);
    m_ui->musicDesktopLrc->setParent(con ? m_ui->background : m_ui->bottomRightWidget);

    if(con)
    {
        MusicApplication::instance()->setMinimumSize(370, WINDOW_HEIGHT_MIN);
        MusicApplication::instance()->setMaximumSize(370, WINDOW_HEIGHT_MIN);

        m_ui->musicWindowConcise->move(295, 20);
        m_ui->musicWindowConcise->show();
        m_ui->minimization->move(320, 20);
        m_ui->minimization->show();
        m_ui->windowClose->move(345, 20);
        m_ui->windowClose->show();

        m_ui->bottomLeftWidgetLayout->addWidget(m_ui->musicBestLove);
        m_ui->bottomLeftWidgetLayout->addWidget(m_ui->musicDownload);
        m_ui->bottomLeftWidgetLayout->addWidget(m_ui->musicMoreFunction);
        m_ui->bottomLeftWidgetLayout->addWidget(m_ui->musicSound);
        m_ui->bottomLeftWidgetLayout->addWidget(m_ui->musicDesktopLrc);
    }
    else
    {
        QSize size = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
        MusicApplication::instance()->setMinimumSize(WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
        MusicApplication::instance()->setMaximumSize(size.width(), size.height());

        m_ui->topRightWidgetLayout->insertWidget(6, m_ui->musicWindowConcise);
        m_ui->topRightWidgetLayout->addWidget(m_ui->minimization);
        m_ui->topRightWidgetLayout->addWidget(m_ui->windowClose);

        m_ui->bottomRightWidgetLayout->insertWidget(0, m_ui->musicBestLove);
        m_ui->bottomRightWidgetLayout->insertWidget(1, m_ui->musicDownload);
        m_ui->bottomRightWidgetLayout->insertWidget(2, m_ui->musicMoreFunction);
        m_ui->bottomRightWidgetLayout->insertWidget(4, m_ui->musicSound);
        m_ui->bottomRightWidgetLayout->insertWidget(6, m_ui->musicDesktopLrc);
    }

    m_musicWindowExtras->disableBlurBehindWindow( !con );
}

QString MusicBottomAreaWidget::getSearchedText() const
{
    return m_musicSongSearchWidget->getSearchedText();
}

void MusicBottomAreaWidget::resizeWindow()
{
    int h = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().height() - WINDOW_HEIGHT_MIN;
    if(m_musicSongSearchWidget)
    {
        m_musicSongSearchWidget->move(51, 554 + h);
    }
    m_ui->musicSongSearchLine->resizeWindow();

    h = m_ui->musiclrccontainerforinline->size().height() - m_ui->lrcDisplayAllButton->height();
    m_ui->lrcDisplayAllButton->move(m_ui->lrcDisplayAllButton->x(), h/2);
}

void MusicBottomAreaWidget::clearSearchedText()
{
    m_musicSongSearchWidget->close();
}

void MusicBottomAreaWidget::musicSearch()
{
    if(m_musicSongSearchWidget == nullptr)
    {
        m_musicSongSearchWidget = new MusicLocalSongSearchDialog(MusicApplication::instance());
        resizeWindow();
    }
    m_musicSongSearchWidget->setVisible(!m_musicSongSearchWidget->isVisible());
}

void MusicBottomAreaWidget::lockDesktopLrc(bool lock)
{
    m_systemTrayMenu->lockDesktopLrc(lock);
}

void MusicBottomAreaWidget::desktopLrcClosed()
{
    m_ui->musicDesktopLrc->setChecked(false);
    m_systemTrayMenu->showDesktopLrc(false);
    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrcChoiced, false);
}
