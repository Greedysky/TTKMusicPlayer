#include "musicvideobarragestylepopwidget.h"
#include "musicvideouiobject.h"
#include "musicwidgetheaders.h"

#include <QButtonGroup>

MusicVideoBarrageStylePopWidget::MusicVideoBarrageStylePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent),
      m_barrageSize(15),
      m_barrageColor(255, 255, 255)
{
    initialize();
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
    const QList<QAbstractButton*> &buttons = m_sizeGroup->buttons();
    if(buttons.count() >= 3)
    {
        buttons[0]->setStyleSheet(TTK::UI::VideoBtnSmall);
        buttons[1]->setStyleSheet(TTK::UI::VideoBtnMiddle);
        buttons[2]->setStyleSheet(TTK::UI::VideoBtnBig);
    }

    switch(index)
    {
        case 0:
        {
            m_barrageSize = 15;
            buttons[0]->setStyleSheet(TTK::UI::VideoBtnSmallOn);
            break;
        }
        case 1:
        {
            m_barrageSize = 20;
            buttons[1]->setStyleSheet(TTK::UI::VideoBtnMiddleOn);
            break;
        }
        case 2:
        {
            m_barrageSize = 30;
            buttons[2]->setStyleSheet(TTK::UI::VideoBtnBigOn);
            break;
        }
        default: break;
    }
}

void MusicVideoBarrageStylePopWidget::barrageColorButtonClicked(int index)
{
    const QList<QAbstractButton*> &buttons = m_colorGroup->buttons();
    if(buttons.count() >= 7)
    {
        buttons[0]->setStyleSheet(TTK::UI::VideoBtnWhite);
        buttons[1]->setStyleSheet(TTK::UI::VideoBtnRed);
        buttons[2]->setStyleSheet(TTK::UI::VideoBtnOrange);
        buttons[3]->setStyleSheet(TTK::UI::VideoBtnYellow);
        buttons[4]->setStyleSheet(TTK::UI::VideoBtnGreen);
        buttons[5]->setStyleSheet(TTK::UI::VideoBtnBlue);
        buttons[6]->setStyleSheet(TTK::UI::VideoBtnPurple);
    }

    switch(index)
    {
        case 0:
        {
            m_barrageColor = QColor(255, 255, 255);
            buttons[0]->setStyleSheet(TTK::UI::VideoBtnWhiteOn);
            break;
        }
        case 1:
        {
            m_barrageColor = QColor(255, 0, 0);
            buttons[1]->setStyleSheet(TTK::UI::VideoBtnRedOn);
            break;
        }
        case 2:
        {
            m_barrageColor = QColor(255, 165, 0);
            buttons[2]->setStyleSheet(TTK::UI::VideoBtnOrangeOn);
            break;
        }
        case 3:
        {
            m_barrageColor = QColor(255, 255, 0);
            buttons[3]->setStyleSheet(TTK::UI::VideoBtnYellowOn);
            break;
        }
        case 4:
        {
            m_barrageColor = QColor(0, 255, 0);
            buttons[4]->setStyleSheet(TTK::UI::VideoBtnGreenOn);
            break;
        }
        case 5:
        {
            m_barrageColor = QColor(0, 0, 255);
            buttons[5]->setStyleSheet(TTK::UI::VideoBtnBlueOn);
            break;
        }
        case 6:
        {
            m_barrageColor = QColor(160, 32, 240);
            buttons[6]->setStyleSheet(TTK::UI::VideoBtnPurpleOn);
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

    smallButton->setStyleSheet(TTK::UI::VideoBtnSmallOn);
    middleButton->setStyleSheet(TTK::UI::VideoBtnMiddle);
    bigButton->setStyleSheet(TTK::UI::VideoBtnBig);

    m_sizeGroup = new QButtonGroup(sizeWidget);
    m_sizeGroup->addButton(smallButton, 0);
    m_sizeGroup->addButton(middleButton, 1);
    m_sizeGroup->addButton(bigButton, 2);
    QtButtonGroupConnect(m_sizeGroup, this, barrageSizeButtonClicked, TTK_SLOT);

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

    whiteButton->setStyleSheet(TTK::UI::VideoBtnWhiteOn);
    redButton->setStyleSheet(TTK::UI::VideoBtnRed);
    orangeButton->setStyleSheet(TTK::UI::VideoBtnOrange);
    yellowButton->setStyleSheet(TTK::UI::VideoBtnYellow);
    greenButton->setStyleSheet(TTK::UI::VideoBtnGreen);
    blueButton->setStyleSheet(TTK::UI::VideoBtnBlue);
    purpleButton->setStyleSheet(TTK::UI::VideoBtnPurple);

    m_colorGroup = new QButtonGroup(this);
    m_colorGroup->addButton(whiteButton, 0);
    m_colorGroup->addButton(redButton, 1);
    m_colorGroup->addButton(orangeButton, 2);
    m_colorGroup->addButton(yellowButton, 3);
    m_colorGroup->addButton(greenButton, 4);
    m_colorGroup->addButton(blueButton, 5);
    m_colorGroup->addButton(purpleButton, 6);
    QtButtonGroupConnect(m_colorGroup, this, barrageColorButtonClicked, TTK_SLOT);

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
