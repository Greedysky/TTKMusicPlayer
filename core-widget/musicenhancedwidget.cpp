#include "musicenhancedwidget.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicconnectionpool.h"

#include <QMenu>
#include <QWidgetAction>
#include <QGridLayout>
#include <QButtonGroup>

MusicEnhancedWidget::MusicEnhancedWidget(QWidget *parent)
    : QToolButton(parent)
{
    initWidget();
    M_Connection->setValue("MusicEnhancedWidget", this);
}

MusicEnhancedWidget::~MusicEnhancedWidget()
{
    M_Connection->disConnect("MusicEnhancedWidget");
}

void MusicEnhancedWidget::initWidget()
{
    m_menu = new QMenu(this);
    m_menu->setStyleSheet(MusicUIObject::MMenuStyle02);
    QWidgetAction *actionWidget = new QWidgetAction(m_menu);

    QWidget *containWidget = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(containWidget);

    QToolButton *caseButton = new QToolButton(this);
    QToolButton *tButton1 = new QToolButton(this);
    QToolButton *tButton2 = new QToolButton(this);
    QToolButton *tButton3 = new QToolButton(this);
    QToolButton *tButton4 = new QToolButton(this);
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(caseButton, 0);
    buttonGroup->addButton(tButton1, 1);
    buttonGroup->addButton(tButton2, 2);
    buttonGroup->addButton(tButton3, 3);
    buttonGroup->addButton(tButton4, 4);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(setEnhancedMusicConfig(int)));

    gridLayout->addWidget(caseButton, 0, 1);
    gridLayout->addWidget(tButton1, 1, 0);
    gridLayout->addWidget(tButton2, 1, 1);
    gridLayout->addWidget(tButton3, 2, 0);
    gridLayout->addWidget(tButton4, 2, 1);
    containWidget->setLayout(gridLayout);
    actionWidget->setDefaultWidget(containWidget);

    m_menu->addAction(actionWidget);
    setMenu(m_menu);
    setPopupMode(QToolButton::InstantPopup);
}

void MusicEnhancedWidget::setEnhancedMusicConfig(int type)
{
    M_SETTING->setValue(MusicSettingManager::EqualizerEnableChoiced, 0);
    M_SETTING->setValue(MusicSettingManager::EnhancedMusicChoiced, type);
    emit enhancedMusicChanged(type);
}
