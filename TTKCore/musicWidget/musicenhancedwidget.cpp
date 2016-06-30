#include "musicenhancedwidget.h"
#include "musicsettingmanager.h"
#include "musicconnectionpool.h"
#include "musicuiobject.h"

#include <QLabel>
#include <QMovie>
#include <QMenu>
#include <QWidgetAction>
#include <QButtonGroup>

#define LABEL_BUTTON_WIDGET 112
#define LABEL_BUTTON_HEIGHT 107

MusicEnhancedToolButton::MusicEnhancedToolButton(QWidget *parent)
    : QToolButton(parent)
{
    m_label = new QLabel(this);
    m_label->resize(LABEL_BUTTON_WIDGET, LABEL_BUTTON_HEIGHT);

    m_movie = new QMovie(":/enhance/enter", QByteArray(), this);
    m_label->setMovie(m_movie);
}

MusicEnhancedToolButton::~MusicEnhancedToolButton()
{
    delete m_movie;
    delete m_label;
}

QString MusicEnhancedToolButton::getClassName()
{
    return staticMetaObject.className();
}

void MusicEnhancedToolButton::enterEvent(QEvent *event)
{
    QToolButton::enterEvent(event);
    m_movie->start();
}


MusicEnhancedWidget::MusicEnhancedWidget(QWidget *parent)
    : QToolButton(parent)
{
    setCursor(Qt::PointingHandCursor);
    setToolTip(tr("magic music"));

    initWidget();
    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicEnhancedWidget::~MusicEnhancedWidget()
{
    M_CONNECTION_PTR->poolDisConnect(getClassName());
    delete m_caseButton;
    delete m_Button1;
    delete m_Button2;
    delete m_Button3;
    delete m_Button4;
}

QString MusicEnhancedWidget::getClassName()
{
    return staticMetaObject.className();
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

    m_Button1 = new MusicEnhancedToolButton(containWidget);
    m_Button1->setGeometry(15, 115, LABEL_BUTTON_WIDGET, LABEL_BUTTON_HEIGHT);
    m_Button1->setStyleSheet("background-image:url(':/enhance/3dOff')");
    m_Button1->setCursor(Qt::PointingHandCursor);

    m_Button2 = new MusicEnhancedToolButton(containWidget);
    m_Button2->setGeometry(145, 115, LABEL_BUTTON_WIDGET, LABEL_BUTTON_HEIGHT);
    m_Button2->setStyleSheet("background-image:url(':/enhance/vocalOff')");
    m_Button2->setCursor(Qt::PointingHandCursor);

    m_Button3 = new MusicEnhancedToolButton(containWidget);
    m_Button3->setGeometry(15, 240, LABEL_BUTTON_WIDGET, LABEL_BUTTON_HEIGHT);
    m_Button3->setStyleSheet("background-image:url(':/enhance/NICAMOff')");
    m_Button3->setCursor(Qt::PointingHandCursor);

    m_Button4 = new MusicEnhancedToolButton(containWidget);
    m_Button4->setGeometry(145, 240, LABEL_BUTTON_WIDGET, LABEL_BUTTON_HEIGHT);
    m_Button4->setStyleSheet("background-image:url(':/enhance/subwooferOff')");
    m_Button4->setCursor(Qt::PointingHandCursor);

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(m_Button1, 1);
    group->addButton(m_Button2, 2);
    group->addButton(m_Button3, 3);
    group->addButton(m_Button4, 4);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(setEnhancedMusicConfig(int)));

    actionWidget->setDefaultWidget(containWidget);

    m_menu->addAction(actionWidget);
    setMenu(m_menu);
    setPopupMode(QToolButton::InstantPopup);

    m_lastSelectedIndex = M_SETTING_PTR->value(MusicSettingManager::EnhancedMusicChoiced).toInt();
    connect(m_caseButton, SIGNAL(clicked()), SLOT(caseButtonOnAndOff()));
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

    m_lastSelectedIndex = (type == 0) ? m_lastSelectedIndex : type;
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnableChoiced, 0);
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedMusicChoiced, type);
    emit enhancedMusicChanged(type);
}

void MusicEnhancedWidget::caseButtonOnAndOff()
{
    setEnhancedMusicConfig( m_caseButton->styleSheet().contains(":/enhance/off") ?
                            m_lastSelectedIndex : 0);
}
