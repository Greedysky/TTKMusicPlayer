#include "musiclrcfloatsettingwidget.h"
#include "musiclrccontainerforinline.h"
#include "musicinlinefloatuiobject.h"
#include "musiclrcdefines.h"

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

    colorLabel->setGeometry(10, 23, 70, 20);
    sizeLabel->setGeometry(10, 70, 70, 20);
    bgLabel->setGeometry(10, 115, 70, 20);

    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(createPushButton(0), 0);
    group->addButton(createPushButton(1), 8);
    group->addButton(createPushButton(2), 1);
    group->addButton(createPushButton(3), 7);
    connect(group, SIGNAL(buttonClicked(int)), parent, SLOT(changeCurrentLrcColor(int)));

    QPushButton *sizeBigerButton = new QPushButton(this);
    QPushButton *sizeSmallerButton = new QPushButton(this);
    sizeBigerButton->setStyleSheet(MusicUIObject::MKGInlineLrcBigger);
    sizeSmallerButton->setStyleSheet(MusicUIObject::MKGInlineLrcSmaller);
    sizeBigerButton->setGeometry(85, 70, 24, 24);
    sizeSmallerButton->setGeometry(125, 70, 24, 24);
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
    musicBgButton->setGeometry(15, 150, 60, 22);
    artBgButton->setGeometry(90, 150, 60, 22);

    QPushButton *settingButton = new QPushButton(tr("More"),this);
    settingButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(settingButton, SIGNAL(clicked()), parent, SIGNAL(changeCurrentLrcColorSetting()));
    settingButton->setStyleSheet(MusicUIObject::MKGInlineFloatSetting + MusicUIObject::MPushButtonStyle08);
    settingButton->setGeometry(15, 175, 60, 22);

#ifdef Q_OS_UNIX
    sizeBigerButton->setFocusPolicy(Qt::NoFocus);
    sizeSmallerButton->setFocusPolicy(Qt::NoFocus);
    musicBgButton->setFocusPolicy(Qt::NoFocus);
    artBgButton->setFocusPolicy(Qt::NoFocus);
    settingButton->setFocusPolicy(Qt::NoFocus);
#endif
}

QString MusicLrcFloatSettingWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcFloatSettingWidget::resizeWindow(int width, int height)
{
    m_rectIn = QRect(495 + width, 120 + height/2, 165, 210);
    m_rectOut = QRect(655 + width, 171 + height/2, 165, 105);
    setGeometry(m_rectOut);
}

QPushButton *MusicLrcFloatSettingWidget::createPushButton(int index)
{
    QPushButton *button = new QPushButton(this);
    switch(index)
    {
        case 0: button->setIcon(QIcon(":/color/lb_yellow")); break;
        case 1: button->setIcon(QIcon(":/color/lb_indigo")); break;
        case 2: button->setIcon(QIcon(":/color/lb_blue")); break;
        case 3: button->setIcon(QIcon(":/color/lb_orange")); break;
    }
    button->setGeometry(80 + index*20, 25, 16, 16);
    button->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    button->setFocusPolicy(Qt::NoFocus);
#endif
    return button;
}

void MusicLrcFloatSettingWidget::lrcSizeUpChanged()
{
    MusicLrcContainerForInline* line = MStatic_cast(MusicLrcContainerForInline*, parent());
    MusicLrcDefines lrc;
    int v = lrc.findInlineLrcIndex(line->getLrcSize());
    v = lrc.findInlineNextSize(v);
    line->setLrcSize(v);
}

void MusicLrcFloatSettingWidget::lrcSizeLowChanged()
{
    MusicLrcContainerForInline* line = MStatic_cast(MusicLrcContainerForInline*, parent());
    MusicLrcDefines lrc;
    int v = lrc.findInlineLrcIndex(line->getLrcSize());
    v = lrc.findInlinePreSize(v);
    line->setLrcSize(v);
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
