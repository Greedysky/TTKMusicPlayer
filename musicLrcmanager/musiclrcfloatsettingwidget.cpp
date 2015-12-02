#include "musiclrcfloatsettingwidget.h"
#include "musiclrccontainerforinline.h"

#include <QTimer>
#include <QPushButton>
#include <QButtonGroup>

MusicLrcFloatSettingWidget::MusicLrcFloatSettingWidget(QWidget *parent)
    : MusicLrcFloatAbstractWidget(parent)
{
    setObjectName("MusicLrcFloatSettingWidget");
    setStyleSheet("#MusicLrcFloatSettingWidget{" + MusicUIObject::MCustomStyle03 + "}" +
                  MusicUIObject::MPushButtonStyle14);

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

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(createPushButton(0), 0);
    group->addButton(createPushButton(1), 1);
    group->addButton(createPushButton(2), 2);
    group->addButton(createPushButton(3), 3);
    group->addButton(createPushButton(4), 4);
    group->addButton(createPushButton(5), 5);
    group->addButton(createPushButton(6), 6);
    group->addButton(createPushButton(7), 7);
    group->addButton(createPushButton(8), 8);
    group->addButton(createPushButton(9), 9);
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
    connect(sizeBigerButton, SIGNAL(clicked()), SLOT(lrcSizeUpChanged()));
    connect(sizeSmallerButton, SIGNAL(clicked()), SLOT(lrcSizeLowChanged()));

    QPushButton *musicBgButton = new QPushButton(tr("MusicBg"), this);
    musicBgButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(musicBgButton,SIGNAL(clicked()), SLOT(lrcMusicBackgroundChanged()));
    QPushButton *artBgButton = new QPushButton(tr("ArtBg"), this);
    artBgButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(artBgButton, SIGNAL(clicked()), SLOT(lrcArtBackgroundChanged()));
    musicBgButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    artBgButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    musicBgButton->setGeometry(10, 145, 70, 20);
    artBgButton->setGeometry(90, 145, 70, 20);

    QPushButton *settingButton = new QPushButton(tr("More"),this);
    settingButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(settingButton, SIGNAL(clicked()), parent, SIGNAL(changeCurrentLrcColorSetting()));
    settingButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    settingButton->setGeometry(10, 180, 150, 20);
}

void MusicLrcFloatSettingWidget::resizeWidth(int width)
{
    m_rectIn = QRect(355 + width, 120, 165, 210);
    m_rectOut = QRect(515 + width, 171, 165, 105);
    setGeometry(m_rectOut);
}

QPushButton *MusicLrcFloatSettingWidget::createPushButton(int index)
{
    QPushButton *button = new QPushButton(this);
    switch(index)
    {
        case 0: button->setIcon(QIcon(":/color/origin")); break;
        case 1: button->setIcon(QIcon(":/color/red")); break;
        case 2: button->setIcon(QIcon(":/color/orange")); break;
        case 3: button->setIcon(QIcon(":/color/yellow")); break;
        case 4: button->setIcon(QIcon(":/color/green")); break;
        case 5: button->setIcon(QIcon(":/color/blue")); break;
        case 6: button->setIcon(QIcon(":/color/indigo")); break;
        case 7: button->setIcon(QIcon(":/color/purple")); break;
        case 8: button->setIcon(QIcon(":/color/white")); break;
        case 9: button->setIcon(QIcon(":/color/black")); break;
    }
    if(index < 4)
        button->setGeometry(80 + index*20, 10, 16, 16);
    else if(4 <= index && index < 8)
        button->setGeometry(index*20, 30, 16, 16);
    else
        button->setGeometry(index*20 - 80, 50, 16, 16);
    button->setCursor(QCursor(Qt::PointingHandCursor));
    return button;
}

void MusicLrcFloatSettingWidget::lrcSizeUpChanged()
{
    MusicLrcContainerForInline* line = static_cast<MusicLrcContainerForInline*>( parent() );
    line->setLrcSize(static_cast<MusicLRCManager::LrcSizeTable>(line->getLrcSize() + 1));
}

void MusicLrcFloatSettingWidget::lrcSizeLowChanged()
{
    MusicLrcContainerForInline* line = static_cast<MusicLrcContainerForInline*>( parent() );
    line->setLrcSize(static_cast<MusicLRCManager::LrcSizeTable>(line->getLrcSize() - 1));
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
