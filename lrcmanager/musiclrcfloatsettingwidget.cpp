#include "musiclrcfloatsettingwidget.h"
#include "musiclrccontainerforinline.h"

#include <QTimer>
#include <QPushButton>
#include <QButtonGroup>

MusicLrcFloatSettingWidget::MusicLrcFloatSettingWidget(QWidget *parent)
    : MusicLrcFloatAbstractWidget(parent)
{
    m_rectIn = QRect(355, 120, 165, 210);
    m_rectOut = QRect(515, 171, 165, 105);
    setGeometry(m_rectOut);
    setObjectName("MusicLrcFloatSettingWidget");
    setStyleSheet("#MusicLrcFloatSettingWidget{" + MusicUIObject::MCustomStyle02 + "}" +
                  MusicUIObject::MPushButtonStyle11);

    QLabel *colorLabel = new QLabel(tr("Color"), this);
    QLabel *sizeLabel = new QLabel(tr("Size"), this);
    QLabel *bgLabel = new QLabel(tr("Background"), this);
    colorLabel->setStyleSheet( MusicUIObject::MCustomStyle23 );
    sizeLabel->setStyleSheet( MusicUIObject::MCustomStyle23 );
    bgLabel->setStyleSheet( MusicUIObject::MCustomStyle23 );
    colorLabel->setAlignment(Qt::AlignCenter);
    sizeLabel->setAlignment(Qt::AlignCenter);
    bgLabel->setAlignment(Qt::AlignCenter);
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setGeometry(10, 170, 150, 5);
    colorLabel->setGeometry(10, 30, 70, 20);
    sizeLabel->setGeometry(10, 80, 70, 20);
    bgLabel->setGeometry(10, 120, 70, 20);

    QPushButton *originButton = new QPushButton(this);
    QPushButton *redButton = new QPushButton(this);
    QPushButton *orangeButton = new QPushButton(this);
    QPushButton *yellowButton = new QPushButton(this);
    QPushButton *greenButton = new QPushButton(this);
    QPushButton *blueButton = new QPushButton(this);
    QPushButton *indigoButton = new QPushButton(this);
    QPushButton *purpleButton = new QPushButton(this);
    QPushButton *whiteButton = new QPushButton(this);
    QPushButton *blackButton = new QPushButton(this);
    originButton->setIcon(QIcon(":/color/origin"));
    redButton->setIcon(QIcon(":/color/red"));
    orangeButton->setIcon(QIcon(":/color/orange"));
    yellowButton->setIcon(QIcon(":/color/yellow"));
    greenButton->setIcon(QIcon(":/color/green"));
    blueButton->setIcon(QIcon(":/color/blue"));
    indigoButton->setIcon(QIcon(":/color/indigo"));
    purpleButton->setIcon(QIcon(":/color/purple"));
    whiteButton->setIcon(QIcon(":/color/white"));
    blackButton->setIcon(QIcon(":/color/black"));
    originButton->setGeometry(80, 10, 16, 16);
    redButton->setGeometry(100, 10, 16, 16);
    orangeButton->setGeometry(120, 10, 16, 16);
    yellowButton->setGeometry(140, 10, 16, 16);
    greenButton->setGeometry(80, 30, 16, 16);
    blueButton->setGeometry(100, 30, 16, 16);
    indigoButton->setGeometry(120, 30, 16, 16);
    purpleButton->setGeometry(140, 30, 16, 16);
    whiteButton->setGeometry(80, 50, 16, 16);
    blackButton->setGeometry(100, 50, 16, 16);
    originButton->setCursor(QCursor(Qt::PointingHandCursor));
    redButton->setCursor(QCursor(Qt::PointingHandCursor));
    orangeButton->setCursor(QCursor(Qt::PointingHandCursor));
    yellowButton->setCursor(QCursor(Qt::PointingHandCursor));
    greenButton->setCursor(QCursor(Qt::PointingHandCursor));
    blueButton->setCursor(QCursor(Qt::PointingHandCursor));
    indigoButton->setCursor(QCursor(Qt::PointingHandCursor));
    purpleButton->setCursor(QCursor(Qt::PointingHandCursor));
    whiteButton->setCursor(QCursor(Qt::PointingHandCursor));
    blackButton->setCursor(QCursor(Qt::PointingHandCursor));

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(originButton, 0);
    group->addButton(redButton, 1);
    group->addButton(orangeButton, 2);
    group->addButton(yellowButton, 3);
    group->addButton(greenButton, 4);
    group->addButton(blueButton, 5);
    group->addButton(indigoButton, 6);
    group->addButton(purpleButton, 7);
    group->addButton(whiteButton, 8);
    group->addButton(blackButton, 9);
    connect(group, SIGNAL(buttonClicked(int)), parent, SLOT(changeCurrentLrcColor(int)));

    QPushButton *sizeBigerButton = new QPushButton(this);
    QPushButton *sizeSmallerButton = new QPushButton(this);
    sizeBigerButton->setIcon(QIcon(":/desktopTool/lrcsizeUp"));
    sizeSmallerButton->setIcon(QIcon(":/desktopTool/lrcsizeDown"));
    sizeBigerButton->setIconSize(QSize(25,25));
    sizeSmallerButton->setIconSize(QSize(25,25));
    sizeBigerButton->setGeometry(85, 80, 25, 25);
    sizeSmallerButton->setGeometry(125, 80, 25, 25);
    sizeBigerButton->setCursor(QCursor(Qt::PointingHandCursor));
    sizeSmallerButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(sizeBigerButton,SIGNAL(clicked()),SLOT(lrcSizeUpChanged()));
    connect(sizeSmallerButton,SIGNAL(clicked()),SLOT(lrcSizeLowChanged()));

    QPushButton *musicBgButton = new QPushButton(tr("MusicBg"),this);
    musicBgButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(musicBgButton,SIGNAL(clicked()), SLOT(lrcMusicBackgroundChanged()));
    QPushButton *artBgButton = new QPushButton(tr("ArtBg"),this);
    artBgButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(artBgButton,SIGNAL(clicked()), SLOT(lrcArtBackgroundChanged()));
    musicBgButton->setStyleSheet(MusicUIObject::MPushButtonStyle02);
    artBgButton->setStyleSheet(MusicUIObject::MPushButtonStyle02);
    musicBgButton->setGeometry(10, 145, 70, 20);
    artBgButton->setGeometry(90, 145, 70, 20);

    QPushButton *settingButton = new QPushButton(tr("More"),this);
    settingButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(settingButton,SIGNAL(clicked()), parent, SIGNAL(changeCurrentLrcColorSetting()));
    settingButton->setStyleSheet(MusicUIObject::MPushButtonStyle02);
    settingButton->setGeometry(10, 180, 150, 20);
}

void MusicLrcFloatSettingWidget::lrcSizeUpChanged()
{
    MusicLrcContainerForInline* line = static_cast<MusicLrcContainerForInline*>( parent() );
    line->setLrcSize(static_cast<LrcSizeTable>(line->getLrcSize() + 1));
}

void MusicLrcFloatSettingWidget::lrcSizeLowChanged()
{
    MusicLrcContainerForInline* line = static_cast<MusicLrcContainerForInline*>( parent() );
    line->setLrcSize(static_cast<LrcSizeTable>(line->getLrcSize() - 1));
}

void MusicLrcFloatSettingWidget::lrcMusicBackgroundChanged()
{
    MusicLrcContainerForInline* line = static_cast<MusicLrcContainerForInline*>( parent() );
    if(line->artBackgroundIsShow())
    {
        line->theArtBgChanged();
    }
}

void MusicLrcFloatSettingWidget::lrcArtBackgroundChanged()
{
    MusicLrcContainerForInline* line = static_cast<MusicLrcContainerForInline*>( parent() );
    if(!line->artBackgroundIsShow())
    {
        line->theArtBgChanged();
    }
}

void MusicLrcFloatSettingWidget::show()
{
    MusicLrcFloatAbstractWidget::show();
    animationIn();
}

void MusicLrcFloatSettingWidget::leaveEvent(QEvent *)
{
    animationOut();
    QTimer::singleShot(m_animation->duration(), this, SIGNAL(widgetClose()));
}
