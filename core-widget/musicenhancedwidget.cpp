#include "musicenhancedwidget.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicconnectionpool.h"

#include <QMenu>
#include <QWidgetAction>
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
    delete m_caseButton;
    delete m_Button1;
    delete m_Button2;
    delete m_Button3;
    delete m_Button4;
}

void MusicEnhancedWidget::initWidget()
{
    m_menu = new QMenu(this);
    QWidgetAction *actionWidget = new QWidgetAction(m_menu);

    QWidget *containWidget = new QWidget(this);
    containWidget->setFixedSize(270, 358);
    containWidget->setObjectName("containWidget");
    containWidget->setStyleSheet("#containWidget{background:url(':/enhance/background')}");

    m_caseButton = new QToolButton(containWidget);
    m_caseButton->setGeometry(200, 73, 54, 24);
    m_caseButton->setStyleSheet("background-image:url(':/enhance/on')");
    m_caseButton->setCursor(Qt::PointingHandCursor);

    m_Button1 = new QToolButton(containWidget);
    m_Button1->setGeometry(15, 115, 112, 107);
    m_Button1->setStyleSheet("background-image:url(':/enhance/3dOff')");
    m_Button1->setCursor(Qt::PointingHandCursor);

    m_Button2 = new QToolButton(containWidget);
    m_Button2->setGeometry(145, 115, 112, 107);
    m_Button2->setStyleSheet("background-image:url(':/enhance/vocalOff')");
    m_Button2->setCursor(Qt::PointingHandCursor);

    m_Button3 = new QToolButton(containWidget);
    m_Button3->setGeometry(15, 240, 112, 107);
    m_Button3->setStyleSheet("background-image:url(':/enhance/NICAMOff')");
    m_Button3->setCursor(Qt::PointingHandCursor);

    m_Button4 = new QToolButton(containWidget);
    m_Button4->setGeometry(145, 240, 112, 107);
    m_Button4->setStyleSheet("background-image:url(':/enhance/subwooferOff')");
    m_Button4->setCursor(Qt::PointingHandCursor);

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(m_caseButton, 0);
    group->addButton(m_Button1, 1);
    group->addButton(m_Button2, 2);
    group->addButton(m_Button3, 3);
    group->addButton(m_Button4, 4);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(setEnhancedMusicConfig(int)));

    actionWidget->setDefaultWidget(containWidget);

    m_menu->addAction(actionWidget);
    setMenu(m_menu);
    setPopupMode(QToolButton::InstantPopup);
}

void MusicEnhancedWidget::setEnhancedMusicConfig(int type)
{
    QString prfix = "QToolButton{background-image:url(':/enhance/";
    switch(type)
    {
        case 0: prfix.append("iconOff');}"); break;
        case 1: prfix.append("icon3D');}"); break;
        case 2: prfix.append("iconVocal');}"); break;
        case 3: prfix.append("iconNICAM');}"); break;
        case 4: prfix.append("iconSubwoofer');}"); break;
    }
    setStyleSheet("QToolButton::menu-indicator{image:None;}" + prfix + "QWidget{border:none;}");

    prfix = QString("background-image:url(':/enhance/%1')");
    m_caseButton->setStyleSheet(prfix.arg(type ? "on" : "off"));
    m_Button1->setStyleSheet(prfix.arg(type == 1 ? "3dOn" : "3dOff"));
    m_Button2->setStyleSheet(prfix.arg(type == 2 ? "vocalOn" : "vocalOff"));
    m_Button3->setStyleSheet(prfix.arg(type == 3 ? "NICAMOn" : "NICAMOff"));
    m_Button4->setStyleSheet(prfix.arg(type == 4 ? "subwooferOn" : "subwooferOff"));

    M_SETTING->setValue(MusicSettingManager::EqualizerEnableChoiced, 0);
    M_SETTING->setValue(MusicSettingManager::EnhancedMusicChoiced, type);
    emit enhancedMusicChanged(type);
}
