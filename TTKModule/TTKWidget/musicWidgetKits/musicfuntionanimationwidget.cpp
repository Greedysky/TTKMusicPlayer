#include "musicfuntionanimationwidget.h"
#include "musicleftitemlistuiobject.h"
#include "musicfunctionlistuiobject.h"
#include "musicwidgetutils.h"
#include "musicuiobject.h"
#include "musicwidgetheaders.h"

#include "qmath.h"

#include <QPainter>
#include <QButtonGroup>
#include <QPropertyAnimation>

MusicBackgroundWidget::MusicBackgroundWidget(QWidget *parent)
    : QWidget(parent)
{

}

void MusicBackgroundWidget::setTransparent(int alpha)
{
    m_backgroundAlpha = MusicUtils::Widget::reRenderValue<int>(0xFF, 0x1F, alpha);
    update();
}

void MusicBackgroundWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(), QColor(255, 255, 255, m_backgroundAlpha));
}



MusicLineBackgroundWidget::MusicLineBackgroundWidget(QWidget *parent)
    : QWidget(parent)
{
    m_transparent = false;
}

void MusicLineBackgroundWidget::transparent(bool state)
{
    m_transparent = state;
    update();
}

void MusicLineBackgroundWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 0.1, Qt::SolidLine));

    if(!m_transparent)
    {
        painter.fillRect(rect(), Qt::white);
        painter.drawLine(0, height(), width(), height());
    }
}



MusicBaseAnimationWidget::MusicBaseAnimationWidget(QWidget *parent)
    : QWidget(parent)
{
    m_pix = QPixmap(":/toolSets/btn_arrow_normal");

    m_curIndex = 0;
    m_preIndex = 0;
    m_x = 0;
    m_perWidth = 0.0f;
    m_totalWidth = 0.0f;
    m_isAnimation = true;
    m_showState = true;
    m_showLine = true;

    m_animation = new QPropertyAnimation(this, "");
    m_animation->setDuration(100);

    connect(m_animation, SIGNAL(valueChanged(QVariant)), SLOT(animationChanged(QVariant)));
    connect(m_animation, SIGNAL(finished()), SLOT(finished()));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);

    m_group = new QButtonGroup(this);
    connect(m_group, SIGNAL(buttonClicked(int)), SLOT(switchToSelectedItemStyle(int)));
}

MusicBaseAnimationWidget::~MusicBaseAnimationWidget()
{
    qDeleteAll(m_container);
    delete m_animation;
    delete m_group;
}

void MusicBaseAnimationWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if(m_showState)
    {
        m_perWidth = m_container[0]->width() + m_container[0]->x();

        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 0.1, Qt::SolidLine));

        int offset =  m_perWidth - (m_container[0]->width() + m_pix.width()) / 2;
            offset = m_isAnimation ? (offset + m_x) : (offset + m_curIndex * m_perWidth);
        if(m_showLine)
        {
            painter.drawLine(0, height(), offset, height());
            painter.drawLine(offset + m_pix.width(), height(), m_totalWidth, height());
        }
        painter.drawPixmap(offset, height() - m_pix.height(), m_pix);
    }
}

void MusicBaseAnimationWidget::switchToSelectedItemStyle(int index)
{
    m_isAnimation = true;
    m_preIndex = m_curIndex;
    m_curIndex = index;
    m_animation->setStartValue(m_preIndex*m_perWidth);
    m_animation->setEndValue(index*m_perWidth);
    m_animation->start();

    emit buttonClicked(index);
}

void MusicBaseAnimationWidget::animationChanged(const QVariant &value)
{
    m_x = value.toInt();
    update();
}

void MusicBaseAnimationWidget::finished()
{
    m_isAnimation = false;
}



MusicFuntionAnimationWidget::MusicFuntionAnimationWidget(QWidget *parent)
    : MusicBaseAnimationWidget(parent)
{
    QHBoxLayout *ly = MStatic_cast(QHBoxLayout*, layout());

    QStringList names;
    names << tr("musicPlaylist") << tr("musicCloud") << tr("musicRadio") << tr("musicMobile") << tr("musicMydownl");
    for(int i=0; i<names.count(); ++i)
    {
        QToolButton *btn = new QToolButton(this);
        btn->setToolTip(names[i]);
        btn->setFixedSize(20, 20);
        btn->setCursor(Qt::PointingHandCursor);
        ly->addWidget(btn);
        m_group->addButton(btn, i);
        m_container << btn;
    }

    switchToSelectedItemStyle(0);
}

void MusicFuntionAnimationWidget::paintEvent(QPaintEvent *event)
{
    m_totalWidth = width();
    MusicBaseAnimationWidget::paintEvent(event);
}

void MusicFuntionAnimationWidget::switchToSelectedItemStyle(int index)
{
    m_container[0]->setStyleSheet(MusicUIObject::MKGItemMusic);
    m_container[1]->setStyleSheet(MusicUIObject::MKGItemFavourite);
    m_container[2]->setStyleSheet(MusicUIObject::MKGItemRadio);
    m_container[3]->setStyleSheet(MusicUIObject::MKGItemMobile);
    m_container[4]->setStyleSheet(MusicUIObject::MKGItemDownload);

    switch(index)
    {
        case 0: m_container[0]->setStyleSheet(MusicUIObject::MKGItemMusicClicked); break;
        case 1: m_container[1]->setStyleSheet(MusicUIObject::MKGItemFavouriteClicked); break;
        case 2: m_container[2]->setStyleSheet(MusicUIObject::MKGItemRadioClicked); break;
        case 3: m_container[3]->setStyleSheet(MusicUIObject::MKGItemMobileClicked); break;
        case 4: m_container[4]->setStyleSheet(MusicUIObject::MKGItemDownloadClicked); break;
        default: break;
    }

    MusicBaseAnimationWidget::switchToSelectedItemStyle(index);
}



MusicOptionAnimationWidget::MusicOptionAnimationWidget(QWidget *parent)
    : MusicBaseAnimationWidget(parent)
{
    m_pix = QPixmap(54, 2);
    m_pix.fill(QColor(0x80, 0xB7, 0xF1));
    m_showLine = false;

    QHBoxLayout *ly = MStatic_cast(QHBoxLayout*, layout());

    for(int i=0; i<6; ++i)
    {
        QToolButton *btn = new QToolButton(this);
        btn->setFixedSize(54, 23);
        btn->setCursor(Qt::PointingHandCursor);
        ly->addWidget(btn);
        m_group->addButton(btn, i);
        m_container << btn;
    }

    switchToSelectedItemStyle(0);
}

void MusicOptionAnimationWidget::musicButtonStyleClear(bool fore)
{
    m_container[0]->setStyleSheet(fore ? MusicUIObject::MKGFuncSongFore : MusicUIObject::MKGFuncSongBack);
    m_container[1]->setStyleSheet(fore ? MusicUIObject::MKGFuncRadioFore : MusicUIObject::MKGFuncRadioBack);
    m_container[2]->setStyleSheet(fore ? MusicUIObject::MKGFuncListFore : MusicUIObject::MKGFuncListBack);
    m_container[3]->setStyleSheet(fore ? MusicUIObject::MKGFuncMVFore : MusicUIObject::MKGFuncMVBack);
    m_container[4]->setStyleSheet(fore ? MusicUIObject::MKGFuncLiveFore : MusicUIObject::MKGFuncLiveBack);
    m_container[5]->setStyleSheet(MusicUIObject::MKGFuncLrcFore);
}

void MusicOptionAnimationWidget::musicButtonStyle(int index)
{
    switch(index)
    {
        case 0: m_container[0]->setStyleSheet(MusicUIObject::MKGFuncSongForeClicked); break;
        case 1: m_container[1]->setStyleSheet(MusicUIObject::MKGFuncRadioForeClicked); break;
        case 2: m_container[2]->setStyleSheet(MusicUIObject::MKGFuncListForeClicked); break;
        case 3: m_container[3]->setStyleSheet(MusicUIObject::MKGFuncMVForeClicked); break;
        case 4: m_container[4]->setStyleSheet(MusicUIObject::MKGFuncLiveForeClicked); break;
        case 5: m_container[5]->setStyleSheet(MusicUIObject::MKGFuncLrcForeClicked); break;
        default: break;
    }
}

void MusicOptionAnimationWidget::paintEvent(QPaintEvent *event)
{
    m_totalWidth = width();
    MusicBaseAnimationWidget::paintEvent(event);
}

void MusicOptionAnimationWidget::switchToSelectedItemStyle(int index)
{
    MusicBaseAnimationWidget::switchToSelectedItemStyle(index);
    m_showState = (index != 5);
    update();
}



MusicSkinAnimationWidget::MusicSkinAnimationWidget(QWidget *parent)
    : MusicBaseAnimationWidget(parent)
{
    QHBoxLayout *ly = MStatic_cast(QHBoxLayout*, layout());

    QStringList names;
    names << tr("Re") << tr("My") << tr("daily") << tr("Online");
    for(int i=0; i<names.count(); ++i)
    {
        QToolButton *btn = new QToolButton(this);
        btn->setText(names[i]);
        btn->setFixedSize(80, 30);
        btn->setCursor(Qt::PointingHandCursor);
        ly->addWidget(btn);
        m_group->addButton(btn, i);
        m_container << btn;
    }
    ly->addStretch(1);

    switchToSelectedItemStyle(0);
}

void MusicSkinAnimationWidget::paintEvent(QPaintEvent *event)
{
    m_totalWidth = width();
    MusicBaseAnimationWidget::paintEvent(event);
}

void MusicSkinAnimationWidget::switchToSelectedItemStyle(int index)
{
    m_container[0]->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MBackgroundStyle01);
    m_container[1]->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MBackgroundStyle01);
    m_container[2]->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MBackgroundStyle01);
    m_container[3]->setStyleSheet(MusicUIObject::MColorStyle03 + MusicUIObject::MBackgroundStyle01);

    switch(index)
    {
        case 0: m_container[0]->setStyleSheet(MusicUIObject::MColorStyle08 + MusicUIObject::MBackgroundStyle01); break;
        case 1: m_container[1]->setStyleSheet(MusicUIObject::MColorStyle08 + MusicUIObject::MBackgroundStyle01); break;
        case 2: m_container[2]->setStyleSheet(MusicUIObject::MColorStyle08 + MusicUIObject::MBackgroundStyle01); break;
        case 3: m_container[3]->setStyleSheet(MusicUIObject::MColorStyle08 + MusicUIObject::MBackgroundStyle01); break;
        default: break;
    }

    MusicBaseAnimationWidget::switchToSelectedItemStyle(index);
}
