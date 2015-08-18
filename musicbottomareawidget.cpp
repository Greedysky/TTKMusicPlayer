#include "musicbottomareawidget.h"
#include "ui_musicapplication.h"
#include "musicuiobject.h"

MusicBottomAreaWidget::MusicBottomAreaWidget(QWidget *parent)
    : QWidget(parent)
{

}

void MusicBottomAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    ui->menuSetting->setIcon(QIcon(QString::fromUtf8(":/image/menu")));
    ui->menuSetting->setIconSize(QSize(50,50));
    ui->menuSetting->setStyleSheet(MusicUIObject::QToolButtonMenuPopStyle);
    ui->menuSetting->setCursor(QCursor(Qt::PointingHandCursor));
    ui->menuSetting->setToolTip(tr("Menu"));
    ui->menuSetting->setMenu(&m_toolPopupMenu);
    
    ui->musicImport->setIcon(QIcon(QString::fromUtf8(":/image/import")));
    ui->musicImport->setIconSize(QSize(40,40));
    ui->musicImport->setStyleSheet(MusicUIObject::QToolButtonStyle);
    ui->musicImport->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicImport->setToolTip(tr("Import"));
    connect(ui->musicImport,SIGNAL(clicked()), parent(), SLOT(musicImportSongs()));
    
    ui->musicSetting->setIcon(QIcon(QString::fromUtf8(":/image/setting")));
    ui->musicSetting->setIconSize(QSize(40,40));
    ui->musicSetting->setStyleSheet(MusicUIObject::QToolButtonStyle);
    ui->musicSetting->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSetting->setToolTip(tr("Setting"));
    connect(ui->musicSetting,SIGNAL(clicked()), parent(), SLOT(musicSetting()));
    
    ui->musicSearch->setIcon(QIcon(QString::fromUtf8(":/image/search")));
    ui->musicSearch->setIconSize(QSize(40,40));
    ui->musicSearch->setStyleSheet(MusicUIObject::QToolButtonStyle);
    ui->musicSearch->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearch->setToolTip(tr("musicSearch"));
    connect(ui->musicSearch,SIGNAL(clicked()), parent(), SLOT(musicSearch()));
    
    ui->musicCurrentLocation->setIcon(QIcon(QString::fromUtf8(":/image/location")));
    ui->musicCurrentLocation->setIconSize(QSize(40,40));
    ui->musicCurrentLocation->setStyleSheet(MusicUIObject::QToolButtonStyle);
    ui->musicCurrentLocation->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicCurrentLocation->setToolTip(tr("musicLocation"));
    connect(ui->musicCurrentLocation,SIGNAL(clicked()), parent(), SLOT(musicCurrentPlayLocation()));

    createToolPopupMenu();
}

void MusicBottomAreaWidget::createToolPopupMenu()
{
    m_toolPopupMenu.setStyleSheet(MusicUIObject::MusicSystemTrayMenu);
    m_toolPopupMenu.addAction(m_ui->action_ImportSongs);
    m_toolPopupMenu.addAction(m_ui->action_Setting);

    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(m_ui->action_Privious);
    m_toolPopupMenu.addAction(m_ui->action_Play);
    m_toolPopupMenu.addAction(m_ui->action_Next);
    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(m_ui->action_OrderPlay);
    m_toolPopupMenu.addAction(m_ui->action_RandomPlay);
    m_toolPopupMenu.addAction(m_ui->action_SingleCycle);
    m_toolPopupMenu.addAction(m_ui->action_ListCycle);
    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(m_ui->action_VolumeSub);
    m_toolPopupMenu.addAction(m_ui->action_VolumePlus);
    m_toolPopupMenu.addSeparator();
    m_toolPopupMenu.addAction(m_ui->action_About);
    m_toolPopupMenu.addAction(m_ui->action_Quit);
}
