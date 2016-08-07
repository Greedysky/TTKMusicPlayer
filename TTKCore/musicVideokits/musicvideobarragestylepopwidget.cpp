#include "musicvideobarragestylepopwidget.h"
#include "musicttkuiobject.h"
#include "musicuiobject.h"

#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>

MusicVideoBarrageStylePopWidget::MusicVideoBarrageStylePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();
}

QString MusicVideoBarrageStylePopWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicVideoBarrageStylePopWidget::initWidget()
{
    m_menu->setWindowFlags(m_menu->windowFlags() | Qt::FramelessWindowHint);
    m_menu->setAttribute(Qt::WA_TranslucentBackground);
    m_menu->setStyleSheet(MusicUIObject::MMenuStyle03);

    m_containWidget->setFixedSize(220, 80);
    QVBoxLayout *containLayout = new QVBoxLayout(m_containWidget);
    //////////////////////////////////////////////////////////////
    QWidget *sizeWidget = new QWidget(m_containWidget);
    QHBoxLayout *sizeLayout = new QHBoxLayout(sizeWidget);
    QLabel *sizeLabel= new QLabel(tr("size"), sizeWidget);
    QPushButton *smallButton = new QPushButton(sizeWidget);
    QPushButton *middleButton = new QPushButton(sizeWidget);
    QPushButton *bigButton = new QPushButton(sizeWidget);

    smallButton->setFixedSize(16, 16);
    middleButton->setFixedSize(16, 16);
    bigButton->setFixedSize(16, 16);

    smallButton->setStyleSheet(MusicUIObject::MKGVideoBtnSmall);
    middleButton->setStyleSheet(MusicUIObject::MKGVideoBtnMiddle);
    bigButton->setStyleSheet(MusicUIObject::MKGVideoBtnBig);

    sizeLayout->addWidget(sizeLabel);
    sizeLayout->addStretch(1);
    sizeLayout->addWidget(smallButton);
    sizeLayout->addStretch(1);
    sizeLayout->addWidget(middleButton);
    sizeLayout->addStretch(1);
    sizeLayout->addWidget(bigButton);
    sizeWidget->setLayout(sizeLayout);
    //////////////////////////////////////////////////////////////
    QWidget *colorWidget = new QWidget(m_containWidget);
    QHBoxLayout *colorLayout = new QHBoxLayout(colorWidget);
    colorLayout->setSpacing(2);
    QLabel *colorLabel= new QLabel(tr("color"), colorWidget);
    QPushButton *whiteButton = new QPushButton(colorWidget);
    QPushButton *redButton = new QPushButton(colorWidget);
    QPushButton *orangeButton = new QPushButton(colorWidget);
    QPushButton *yellowButton = new QPushButton(colorWidget);
    QPushButton *greenButton = new QPushButton(colorWidget);
    QPushButton *blueButton = new QPushButton(colorWidget);
    QPushButton *purpleButton = new QPushButton(colorWidget);

    colorLabel->setFixedSize(40, 15);
    whiteButton->setFixedSize(19, 26);
    redButton->setFixedSize(19, 26);
    orangeButton->setFixedSize(19, 26);
    yellowButton->setFixedSize(19, 26);
    greenButton->setFixedSize(19, 26);
    blueButton->setFixedSize(19, 26);
    purpleButton->setFixedSize(19, 26);

    whiteButton->setStyleSheet(MusicUIObject::MKGVideoBtnWhite);
    redButton->setStyleSheet(MusicUIObject::MKGVideoBtnRed);
    orangeButton->setStyleSheet(MusicUIObject::MKGVideoBtnOrange);
    yellowButton->setStyleSheet(MusicUIObject::MKGVideoBtnYellow);
    greenButton->setStyleSheet(MusicUIObject::MKGVideoBtnGreen);
    blueButton->setStyleSheet(MusicUIObject::MKGVideoBtnBlue);
    purpleButton->setStyleSheet(MusicUIObject::MKGVideoBtnPurple);

    colorLayout->addWidget(colorLabel, 0, Qt::AlignCenter);
    colorLayout->addWidget(whiteButton);
    colorLayout->addWidget(redButton);
    colorLayout->addWidget(orangeButton);
    colorLayout->addWidget(yellowButton);
    colorLayout->addWidget(greenButton);
    colorLayout->addWidget(blueButton);
    colorLayout->addWidget(purpleButton);
    colorWidget->setLayout(colorLayout);
    //////////////////////////////////////////////////////////////
    containLayout->addWidget(sizeWidget);
    containLayout->addWidget(colorWidget);
    m_containWidget->setLayout(containLayout);
}
