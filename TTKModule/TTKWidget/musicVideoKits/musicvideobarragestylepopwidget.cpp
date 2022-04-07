#include "musicvideobarragestylepopwidget.h"
#include "musicvideouiobject.h"
#include "musicwidgetheaders.h"

#include <QButtonGroup>

MusicVideoBarrageStylePopWidget::MusicVideoBarrageStylePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();
    m_barrageSize = 15;
    m_barrageColor = QColor(255, 255, 255);
}

MusicVideoBarrageStylePopWidget::~MusicVideoBarrageStylePopWidget()
{
    delete m_sizeGroup;
    delete m_colorGroup;
}

int MusicVideoBarrageStylePopWidget::barrageSize() const
{
    return m_barrageSize;
}

QColor MusicVideoBarrageStylePopWidget::barrageColor() const
{
    return m_barrageColor;
}

void MusicVideoBarrageStylePopWidget::barrageSizeButtonClicked(int index)
{
    QList<QAbstractButton*> buttons = m_sizeGroup->buttons();
    if(buttons.count() >= 3)
    {
        buttons[0]->setStyleSheet(MusicUIObject::MQSSVideoBtnSmall);
        buttons[1]->setStyleSheet(MusicUIObject::MQSSVideoBtnMiddle);
        buttons[2]->setStyleSheet(MusicUIObject::MQSSVideoBtnBig);
    }

    switch(index)
    {
        case 0:
        {
            m_barrageSize = 15;
            buttons[0]->setStyleSheet(MusicUIObject::MQSSVideoBtnSmallOn);
            break;
        }
        case 1:
        {
            m_barrageSize = 20;
            buttons[1]->setStyleSheet(MusicUIObject::MQSSVideoBtnMiddleOn);
            break;
        }
        case 2:
        {
            m_barrageSize = 30;
            buttons[2]->setStyleSheet(MusicUIObject::MQSSVideoBtnBigOn);
            break;
        }
        default: break;
    }
}

void MusicVideoBarrageStylePopWidget::barrageColorButtonClicked(int index)
{
    QList<QAbstractButton*> buttons = m_colorGroup->buttons();
    if(buttons.count() >= 7)
    {
        buttons[0]->setStyleSheet(MusicUIObject::MQSSVideoBtnWhite);
        buttons[1]->setStyleSheet(MusicUIObject::MQSSVideoBtnRed);
        buttons[2]->setStyleSheet(MusicUIObject::MQSSVideoBtnOrange);
        buttons[3]->setStyleSheet(MusicUIObject::MQSSVideoBtnYellow);
        buttons[4]->setStyleSheet(MusicUIObject::MQSSVideoBtnGreen);
        buttons[5]->setStyleSheet(MusicUIObject::MQSSVideoBtnBlue);
        buttons[6]->setStyleSheet(MusicUIObject::MQSSVideoBtnPurple);
    }

    switch(index)
    {
        case 0:
        {
            m_barrageColor = QColor(255, 255, 255);
            buttons[0]->setStyleSheet(MusicUIObject::MQSSVideoBtnWhiteOn);
            break;
        }
        case 1:
        {
            m_barrageColor = QColor(255, 0, 0);
            buttons[1]->setStyleSheet(MusicUIObject::MQSSVideoBtnRedOn);
            break;
        }
        case 2:
        {
            m_barrageColor = QColor(255, 165, 0);
            buttons[2]->setStyleSheet(MusicUIObject::MQSSVideoBtnOrangeOn);
            break;
        }
        case 3:
        {
            m_barrageColor = QColor(255, 255, 0);
            buttons[3]->setStyleSheet(MusicUIObject::MQSSVideoBtnYellowOn);
            break;
        }
        case 4:
        {
            m_barrageColor = QColor(0, 255, 0);
            buttons[4]->setStyleSheet(MusicUIObject::MQSSVideoBtnGreenOn);
            break;
        }
        case 5:
        {
            m_barrageColor = QColor(0, 0, 255);
            buttons[5]->setStyleSheet(MusicUIObject::MQSSVideoBtnBlueOn);
            break;
        }
        case 6:
        {
            m_barrageColor = QColor(160, 32, 240);
            buttons[6]->setStyleSheet(MusicUIObject::MQSSVideoBtnPurpleOn);
            break;
        }
        default: break;
    }
}

void MusicVideoBarrageStylePopWidget::initialize()
{
    setTranslucentBackground();
    m_containWidget->setFixedSize(200, 85);
    QVBoxLayout *containLayout = new QVBoxLayout(m_containWidget);
    //
    QWidget *sizeWidget = new QWidget(m_containWidget);
    QHBoxLayout *sizeLayout = new QHBoxLayout(sizeWidget);
    sizeLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *sizeLabel= new QLabel(tr("Size"), sizeWidget);
    QPushButton *smallButton = new QPushButton(sizeWidget);
    QPushButton *middleButton = new QPushButton(sizeWidget);
    QPushButton *bigButton = new QPushButton(sizeWidget);

#ifdef Q_OS_UNIX
    smallButton->setFocusPolicy(Qt::NoFocus);
    middleButton->setFocusPolicy(Qt::NoFocus);
    bigButton->setFocusPolicy(Qt::NoFocus);
#endif

    smallButton->setFixedSize(16, 16);
    middleButton->setFixedSize(16, 16);
    bigButton->setFixedSize(16, 16);

    smallButton->setStyleSheet(MusicUIObject::MQSSVideoBtnSmallOn);
    middleButton->setStyleSheet(MusicUIObject::MQSSVideoBtnMiddle);
    bigButton->setStyleSheet(MusicUIObject::MQSSVideoBtnBig);

    m_sizeGroup = new QButtonGroup(sizeWidget);
    m_sizeGroup->addButton(smallButton, 0);
    m_sizeGroup->addButton(middleButton, 1);
    m_sizeGroup->addButton(bigButton, 2);
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_sizeGroup, SIGNAL(idClicked(int)), SLOT(barrageSizeButtonClicked(int)));
#else
    connect(m_sizeGroup, SIGNAL(buttonClicked(int)), SLOT(barrageSizeButtonClicked(int)));
#endif

    sizeLayout->addWidget(sizeLabel, 1, Qt::AlignCenter);
    sizeLayout->addWidget(smallButton);
    sizeLayout->addStretch(1);
    sizeLayout->addWidget(middleButton);
    sizeLayout->addStretch(1);
    sizeLayout->addWidget(bigButton);
    sizeLayout->addStretch(1);
    sizeWidget->setLayout(sizeLayout);
    //
    QWidget *colorWidget = new QWidget(m_containWidget);
    QHBoxLayout *colorLayout = new QHBoxLayout(colorWidget);
    colorLayout->setContentsMargins(0, 0, 0, 0);
    colorLayout->setSpacing(2);
    QLabel *colorLabel= new QLabel(tr("Color"), colorWidget);
    QPushButton *whiteButton = new QPushButton(colorWidget);
    QPushButton *redButton = new QPushButton(colorWidget);
    QPushButton *orangeButton = new QPushButton(colorWidget);
    QPushButton *yellowButton = new QPushButton(colorWidget);
    QPushButton *greenButton = new QPushButton(colorWidget);
    QPushButton *blueButton = new QPushButton(colorWidget);
    QPushButton *purpleButton = new QPushButton(colorWidget);
#ifdef Q_OS_UNIX
    whiteButton->setFocusPolicy(Qt::NoFocus);
    redButton->setFocusPolicy(Qt::NoFocus);
    orangeButton->setFocusPolicy(Qt::NoFocus);
    yellowButton->setFocusPolicy(Qt::NoFocus);
    greenButton->setFocusPolicy(Qt::NoFocus);
    blueButton->setFocusPolicy(Qt::NoFocus);
    purpleButton->setFocusPolicy(Qt::NoFocus);
#endif

    colorLabel->setFixedSize(40, 15);
    whiteButton->setFixedSize(19, 26);
    redButton->setFixedSize(19, 26);
    orangeButton->setFixedSize(19, 26);
    yellowButton->setFixedSize(19, 26);
    greenButton->setFixedSize(19, 26);
    blueButton->setFixedSize(19, 26);
    purpleButton->setFixedSize(19, 26);

    whiteButton->setStyleSheet(MusicUIObject::MQSSVideoBtnWhiteOn);
    redButton->setStyleSheet(MusicUIObject::MQSSVideoBtnRed);
    orangeButton->setStyleSheet(MusicUIObject::MQSSVideoBtnOrange);
    yellowButton->setStyleSheet(MusicUIObject::MQSSVideoBtnYellow);
    greenButton->setStyleSheet(MusicUIObject::MQSSVideoBtnGreen);
    blueButton->setStyleSheet(MusicUIObject::MQSSVideoBtnBlue);
    purpleButton->setStyleSheet(MusicUIObject::MQSSVideoBtnPurple);

    m_colorGroup = new QButtonGroup(this);
    m_colorGroup->addButton(whiteButton, 0);
    m_colorGroup->addButton(redButton, 1);
    m_colorGroup->addButton(orangeButton, 2);
    m_colorGroup->addButton(yellowButton, 3);
    m_colorGroup->addButton(greenButton, 4);
    m_colorGroup->addButton(blueButton, 5);
    m_colorGroup->addButton(purpleButton, 6);
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_colorGroup, SIGNAL(idClicked(int)), SLOT(barrageColorButtonClicked(int)));
#else
    connect(m_colorGroup, SIGNAL(buttonClicked(int)), SLOT(barrageColorButtonClicked(int)));
#endif

    colorLayout->addWidget(colorLabel, 0, Qt::AlignCenter);
    colorLayout->addWidget(whiteButton);
    colorLayout->addWidget(redButton);
    colorLayout->addWidget(orangeButton);
    colorLayout->addWidget(yellowButton);
    colorLayout->addWidget(greenButton);
    colorLayout->addWidget(blueButton);
    colorLayout->addWidget(purpleButton);
    colorWidget->setLayout(colorLayout);
    //
    containLayout->addWidget(sizeWidget);
    containLayout->addWidget(colorWidget);
    m_containWidget->setLayout(containLayout);
}
