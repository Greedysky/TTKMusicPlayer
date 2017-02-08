#include "musiclrcfloatsettingwidget.h"
#include "musiclrccontainerforinline.h"
#include "musicinlinefloatuiobject.h"

#include <QTimer>
#include <QPushButton>
#include <QButtonGroup>

MusicLrcFloatSettingWidget::MusicLrcFloatSettingWidget(QWidget *parent)
    : MusicFloatAbstractWidget(parent)
{
    setObjectName("MusicLrcFloatSettingWidget");
    setStyleSheet(QString("#MusicLrcFloatSettingWidget{%1}").arg(MusicUIObject::MBackgroundStyle08));

    QLabel *colorLabel = new QLabel(tr("Color"), this);
    QLabel *sizeLabel = new QLabel(tr("Size"), this);
    QLabel *bgLabel = new QLabel(tr("Background"), this);

    const QString labelStyle = MusicUIObject::MColorStyle01 + MusicUIObject::MCustomStyle02 + "\
                               image:url(:/lrc/lb_shadow);";
    colorLabel->setStyleSheet(labelStyle);
    sizeLabel->setStyleSheet(labelStyle);
    bgLabel->setStyleSheet(labelStyle);
    colorLabel->setAlignment(Qt::AlignCenter);
    sizeLabel->setAlignment(Qt::AlignCenter);
    bgLabel->setAlignment(Qt::AlignCenter);

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
    sizeBigerButton->setStyleSheet(MusicUIObject::MKGInlineLrcBigger);
    sizeSmallerButton->setStyleSheet(MusicUIObject::MKGInlineLrcSmaller);
    sizeBigerButton->setGeometry(85, 80, 24, 24);
    sizeSmallerButton->setGeometry(125, 80, 24, 24);
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
    musicBgButton->setStyleSheet(MusicUIObject::MKGInlineFloatSetting + MusicUIObject::MPushButtonStyle08);
    artBgButton->setStyleSheet(MusicUIObject::MKGInlineFloatSetting + MusicUIObject::MPushButtonStyle08);
    musicBgButton->setGeometry(15, 155, 60, 22);
    artBgButton->setGeometry(90, 155, 60, 22);

    QPushButton *settingButton = new QPushButton(tr("More"),this);
    settingButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(settingButton, SIGNAL(clicked()), parent, SIGNAL(changeCurrentLrcColorSetting()));
    settingButton->setStyleSheet(MusicUIObject::MKGInlineFloatSetting + MusicUIObject::MPushButtonStyle08);
    settingButton->setGeometry(15, 180, 60, 22);
}

QString MusicLrcFloatSettingWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcFloatSettingWidget::resizeWindow(int width, int height)
{
    m_rectIn = QRect(495 + width, 120 + height, 165, 210);
    m_rectOut = QRect(655 + width, 171 + height, 165, 105);
    setGeometry(m_rectOut);
}

QPushButton *MusicLrcFloatSettingWidget::createPushButton(int index)
{
    QPushButton *button = new QPushButton(this);
    switch(index)
    {
        case 0: button->setIcon(QIcon(":/color/lb_origin")); break;
        case 1: button->setIcon(QIcon(":/color/lb_red")); break;
        case 2: button->setIcon(QIcon(":/color/lb_orange")); break;
        case 3: button->setIcon(QIcon(":/color/lb_yellow")); break;
        case 4: button->setIcon(QIcon(":/color/lb_green")); break;
        case 5: button->setIcon(QIcon(":/color/lb_blue")); break;
        case 6: button->setIcon(QIcon(":/color/lb_indigo")); break;
        case 7: button->setIcon(QIcon(":/color/lb_purple")); break;
        case 8: button->setIcon(QIcon(":/color/lb_white")); break;
        case 9: button->setIcon(QIcon(":/color/lb_black")); break;
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
    MusicLrcContainerForInline* line = MStatic_cast(MusicLrcContainerForInline*, parent());
    line->setLrcSize(MStatic_cast(MusicLRCManager::LrcSizeTable, line->getLrcSize() + 1));
}

void MusicLrcFloatSettingWidget::lrcSizeLowChanged()
{
    MusicLrcContainerForInline* line = MStatic_cast(MusicLrcContainerForInline*, parent());
    line->setLrcSize(MStatic_cast(MusicLRCManager::LrcSizeTable, line->getLrcSize() - 1));
}

void MusicLrcFloatSettingWidget::lrcMusicBackgroundChanged()
{
    MusicLrcContainerForInline* line = MStatic_cast(MusicLrcContainerForInline*, parent());
    if(line->artBackgroundIsShow())
    {
        line->theArtBgChanged();
    }
}

void MusicLrcFloatSettingWidget::lrcArtBackgroundChanged()
{
    MusicLrcContainerForInline* line = MStatic_cast(MusicLrcContainerForInline*, parent());
    if(!line->artBackgroundIsShow())
    {
        line->theArtBgChanged();
    }
}

void MusicLrcFloatSettingWidget::show()
{
    MusicFloatAbstractWidget::show();
    animationIn();
}

void MusicLrcFloatSettingWidget::leaveEvent(QEvent *)
{
    animationOut();
    QTimer::singleShot(m_animation->duration(), this, SIGNAL(widgetClose()));
}
