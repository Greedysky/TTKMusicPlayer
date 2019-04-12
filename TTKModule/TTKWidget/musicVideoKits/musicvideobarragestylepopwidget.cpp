#include "musicvideobarragestylepopwidget.h"
#include "musicvideouiobject.h"
#include "musicuiobject.h"
#include "musicwidgetheaders.h"

#include <QButtonGroup>

MusicVideoBarrageStylePopWidget::MusicVideoBarrageStylePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();
    m_barrageSize = 15;
    m_barrageColor = QColor(255, 255, 255);
}

MusicVideoBarrageStylePopWidget::~MusicVideoBarrageStylePopWidget()
{
    delete m_sizeGroup;
    delete m_colorGroup;
}

int MusicVideoBarrageStylePopWidget::getBarrageSize() const
{
    return m_barrageSize;
}

QColor MusicVideoBarrageStylePopWidget::getBarrageColor() const
{
    return m_barrageColor;
}

void MusicVideoBarrageStylePopWidget::barrageSizeButtonClicked(int index)
{
    QList<QAbstractButton*> buttons = m_sizeGroup->buttons();
    if(buttons.count() >= 3)
    {
        buttons[0]->setStyleSheet(MusicUIObject::MKGVideoBtnSmall);
        buttons[1]->setStyleSheet(MusicUIObject::MKGVideoBtnMiddle);
        buttons[2]->setStyleSheet(MusicUIObject::MKGVideoBtnBig);
    }

    switch(index)
    {
        case 0: m_barrageSize = 15;
                buttons[0]->setStyleSheet(MusicUIObject::MKGVideoBtnSmallOn); break;
        case 1: m_barrageSize = 20;
                buttons[1]->setStyleSheet(MusicUIObject::MKGVideoBtnMiddleOn); break;
        case 2: m_barrageSize = 30;
                buttons[2]->setStyleSheet(MusicUIObject::MKGVideoBtnBigOn); break;
        default: break;
    }
}

void MusicVideoBarrageStylePopWidget::barrageColorButtonClicked(int index)
{
    QList<QAbstractButton*> buttons = m_colorGroup->buttons();
    if(buttons.count() >= 7)
    {
        buttons[0]->setStyleSheet(MusicUIObject::MKGVideoBtnWhite);
        buttons[1]->setStyleSheet(MusicUIObject::MKGVideoBtnRed);
        buttons[2]->setStyleSheet(MusicUIObject::MKGVideoBtnOrange);
        buttons[3]->setStyleSheet(MusicUIObject::MKGVideoBtnYellow);
        buttons[4]->setStyleSheet(MusicUIObject::MKGVideoBtnGreen);
        buttons[5]->setStyleSheet(MusicUIObject::MKGVideoBtnBlue);
        buttons[6]->setStyleSheet(MusicUIObject::MKGVideoBtnPurple);
    }

    switch(index)
    {
        case 0: m_barrageColor = QColor(255, 255, 255);
                buttons[0]->setStyleSheet(MusicUIObject::MKGVideoBtnWhiteOn); break;
        case 1: m_barrageColor = QColor(255, 0, 0);
                buttons[1]->setStyleSheet(MusicUIObject::MKGVideoBtnRedOn); break;
        case 2: m_barrageColor = QColor(255, 165, 0);
                buttons[2]->setStyleSheet(MusicUIObject::MKGVideoBtnOrangeOn); break;
        case 3: m_barrageColor = QColor(255, 255, 0);
                buttons[3]->setStyleSheet(MusicUIObject::MKGVideoBtnYellowOn); break;
        case 4: m_barrageColor = QColor(0, 255, 0);
                buttons[4]->setStyleSheet(MusicUIObject::MKGVideoBtnGreenOn); break;
        case 5: m_barrageColor = QColor(0, 0, 255);
                buttons[5]->setStyleSheet(MusicUIObject::MKGVideoBtnBlueOn); break;
        case 6: m_barrageColor = QColor(160, 32, 240);
                buttons[6]->setStyleSheet(MusicUIObject::MKGVideoBtnPurpleOn); break;
        default: break;
    }
}

void MusicVideoBarrageStylePopWidget::initWidget()
{
    setTranslucentBackground();
    m_containWidget->setFixedSize(200, 85);
    QVBoxLayout *containLayout = new QVBoxLayout(m_containWidget);
    //
    QWidget *sizeWidget = new QWidget(m_containWidget);
    QHBoxLayout *sizeLayout = new QHBoxLayout(sizeWidget);
    sizeLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *sizeLabel= new QLabel(tr("size"), sizeWidget);
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

    smallButton->setStyleSheet(MusicUIObject::MKGVideoBtnSmallOn);
    middleButton->setStyleSheet(MusicUIObject::MKGVideoBtnMiddle);
    bigButton->setStyleSheet(MusicUIObject::MKGVideoBtnBig);

    m_sizeGroup = new QButtonGroup(sizeWidget);
    m_sizeGroup->addButton(smallButton, 0);
    m_sizeGroup->addButton(middleButton, 1);
    m_sizeGroup->addButton(bigButton, 2);
    connect(m_sizeGroup, SIGNAL(buttonClicked(int)), SLOT(barrageSizeButtonClicked(int)));

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
    QLabel *colorLabel= new QLabel(tr("color"), colorWidget);
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

    whiteButton->setStyleSheet(MusicUIObject::MKGVideoBtnWhiteOn);
    redButton->setStyleSheet(MusicUIObject::MKGVideoBtnRed);
    orangeButton->setStyleSheet(MusicUIObject::MKGVideoBtnOrange);
    yellowButton->setStyleSheet(MusicUIObject::MKGVideoBtnYellow);
    greenButton->setStyleSheet(MusicUIObject::MKGVideoBtnGreen);
    blueButton->setStyleSheet(MusicUIObject::MKGVideoBtnBlue);
    purpleButton->setStyleSheet(MusicUIObject::MKGVideoBtnPurple);

    m_colorGroup = new QButtonGroup(this);
    m_colorGroup->addButton(whiteButton, 0);
    m_colorGroup->addButton(redButton, 1);
    m_colorGroup->addButton(orangeButton, 2);
    m_colorGroup->addButton(yellowButton, 3);
    m_colorGroup->addButton(greenButton, 4);
    m_colorGroup->addButton(blueButton, 5);
    m_colorGroup->addButton(purpleButton, 6);
    connect(m_colorGroup, SIGNAL(buttonClicked(int)), SLOT(barrageColorButtonClicked(int)));

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
