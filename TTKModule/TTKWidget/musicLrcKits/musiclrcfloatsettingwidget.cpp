#include "musiclrcfloatsettingwidget.h"
#include "musiclrccontainerforinterior.h"
#include "musicinteriorfloatuiobject.h"
#include "musiclrchelper.h"

#include <QButtonGroup>

MusicLrcFloatSettingWidget::MusicLrcFloatSettingWidget(QWidget *parent)
    : MusicAbstractFloatWidget(parent)
{
    setObjectName(MusicLrcFloatSettingWidget::metaObject()->className());
    setStyleSheet(QString("#%1{ %2 }").arg(objectName(), TTK::UI::BackgroundStyle05));

    QLabel *colorLabel = new QLabel(tr("Color"), this);
    QLabel *sizeLabel = new QLabel(tr("Size"), this);
    QLabel *backLabel = new QLabel(tr("Background"), this);

    const QString &labelStyle = TTK::UI::ColorStyle06 + TTK::UI::CustomStyle02 + "image:url(:/lrc/lb_shadow);";

    colorLabel->setStyleSheet(labelStyle);
    sizeLabel->setStyleSheet(labelStyle);
    backLabel->setStyleSheet(labelStyle);

    colorLabel->setAlignment(Qt::AlignCenter);
    sizeLabel->setAlignment(Qt::AlignCenter);
    backLabel->setAlignment(Qt::AlignCenter);

    colorLabel->setGeometry(10, 23, 70, 20);
    sizeLabel->setGeometry(10, 70, 70, 20);
    backLabel->setGeometry(10, 115, 70, 20);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(createPushButton(0), 0);
    buttonGroup->addButton(createPushButton(1), 8);
    buttonGroup->addButton(createPushButton(2), 1);
    buttonGroup->addButton(createPushButton(3), 7);
    QtButtonGroupConnect(buttonGroup, parent, changeCurrentLrcColor, TTK_SLOT);

    QPushButton *sizeBigerButton = new QPushButton(this);
    QPushButton *sizeSmallerButton = new QPushButton(this);
    sizeBigerButton->setStyleSheet(TTK::UI::InteriorLrcBigger);
    sizeSmallerButton->setStyleSheet(TTK::UI::InteriorLrcSmaller);
    sizeBigerButton->setGeometry(85, 70, 24, 24);
    sizeSmallerButton->setGeometry(125, 70, 24, 24);
    sizeBigerButton->setCursor(QCursor(Qt::PointingHandCursor));
    sizeSmallerButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(sizeBigerButton, SIGNAL(clicked()), SLOT(lrcSizeUpChanged()));
    connect(sizeSmallerButton, SIGNAL(clicked()), SLOT(lrcSizeLowChanged()));

    QPushButton *musicBackButton = new QPushButton(tr("Defualt"), this);
    musicBackButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(musicBackButton,SIGNAL(clicked()), SLOT(lrcMusicBackgroundChanged()));
    QPushButton *artBackButton = new QPushButton(tr("Art"), this);
    artBackButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(artBackButton, SIGNAL(clicked()), SLOT(lrcArtBackgroundChanged()));
    musicBackButton->setStyleSheet(TTK::UI::InteriorFloatSetting + TTK::UI::PushButtonStyle06);
    artBackButton->setStyleSheet(TTK::UI::InteriorFloatSetting + TTK::UI::PushButtonStyle06);
    musicBackButton->setGeometry(15, 150, 60, 22);
    artBackButton->setGeometry(90, 150, 60, 22);

    QPushButton *settingButton = new QPushButton(tr("More"), this);
    settingButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(settingButton, SIGNAL(clicked()), parent, SIGNAL(showCurrentLrcSetting()));
    settingButton->setStyleSheet(TTK::UI::InteriorFloatSetting + TTK::UI::PushButtonStyle06);
    settingButton->setGeometry(15, 175, 60, 22);

#ifdef Q_OS_UNIX
    sizeBigerButton->setFocusPolicy(Qt::NoFocus);
    sizeSmallerButton->setFocusPolicy(Qt::NoFocus);
    musicBackButton->setFocusPolicy(Qt::NoFocus);
    artBackButton->setFocusPolicy(Qt::NoFocus);
    settingButton->setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicLrcFloatSettingWidget::resizeGeometry(int width, int height)
{
    m_rectEnter = QRect(513 + width, 120 + height / 2, 165, 210);
    m_rectLeave = QRect(673 + width, 171 + height / 2, 165, 105);
    setGeometry(m_rectLeave);
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
        default: break;
    }

    button->setGeometry(80 + index * 20, 25, 16, 16);
    button->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    button->setFocusPolicy(Qt::NoFocus);
#endif
    return button;
}

void MusicLrcFloatSettingWidget::lrcSizeUpChanged()
{
    MusicLrcContainerForInterior* line = TTKObjectCast(MusicLrcContainerForInterior*, parent());
    const MusicLrcHelper lrc;
    int v = lrc.findInteriorLrcIndex(line->lrcSize());
        v = lrc.findInteriorNextSize(v);
    line->setLrcSize(v);
}

void MusicLrcFloatSettingWidget::lrcSizeLowChanged()
{
    MusicLrcContainerForInterior* line = TTKObjectCast(MusicLrcContainerForInterior*, parent());
    const MusicLrcHelper lrc;
    int v = lrc.findInteriorLrcIndex(line->lrcSize());
        v = lrc.findInteriorPreSize(v);
    line->setLrcSize(v);
}

void MusicLrcFloatSettingWidget::lrcMusicBackgroundChanged()
{
    MusicLrcContainerForInterior* line = TTKObjectCast(MusicLrcContainerForInterior*, parent());
    if(line->isShowArtistBackground())
    {
        line->artistBackgroundChanged();
    }
}

void MusicLrcFloatSettingWidget::lrcArtBackgroundChanged()
{
    MusicLrcContainerForInterior* line = TTKObjectCast(MusicLrcContainerForInterior*, parent());
    if(!line->isShowArtistBackground())
    {
        line->artistBackgroundChanged();
    }
}

void MusicLrcFloatSettingWidget::show()
{
    MusicAbstractFloatWidget::show();
    animationEnter();
}

void MusicLrcFloatSettingWidget::leaveEvent(QEvent *)
{
    animationLeave();
    TTK_SIGNLE_SHOT(m_animation->duration(), this, widgetClose, TTK_SIGNAL);
}
