#include "musicfuntionanimationwidget.h"
#include "musicleftitemlistuiobject.h"
#include "musicwidgetutils.h"
#include "qmath.h"

#include <QPainter>
#include <QBoxLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QPropertyAnimation>

#define BUTTON_SZIE_WIDTH       20

MusicBackgroundWidget::MusicBackgroundWidget(QWidget *parent)
    : QWidget(parent)
{

}

QString MusicBackgroundWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicBackgroundWidget::setTransparent(int alpha)
{
    m_backgroundAlpha = MusicUtils::Widget::reRenderValue<int>(0xff, 0x1f, alpha);;
    update();
}

void MusicBackgroundWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), QColor(255, 255, 255, m_backgroundAlpha));
    painter.end();

    QWidget::paintEvent(event);
}



MusicFuntionAnimationWidget::MusicFuntionAnimationWidget(QWidget *parent)
    : QWidget(parent)
{
    m_pix = QPixmap(":/toolSets/btn_arrow_normal");
    m_curIndex = 0;
    m_preIndex = 0;
    m_x = 0;
    m_perWidth = 0.0f;
    m_isAnimation = true;
    m_animation = new QPropertyAnimation(this, "");
    m_animation->setDuration(500);

    connect(m_animation, SIGNAL(valueChanged(QVariant)), SLOT(animationChanged(QVariant)));
    connect(m_animation, SIGNAL(finished()), SLOT(finished()));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    QButtonGroup *group = new QButtonGroup(this);

    QStringList names;
    names << tr("musicPlaylist") << tr("musicCloud") << tr("musicRadio")
          << tr("musicMobile") << tr("musicMydownl");
    for(int i=0; i<names.count(); ++i)
    {
        QToolButton *btn = new QToolButton(this);
        btn->setToolTip(names[i]);
        btn->setFixedSize(20, 20);
        layout->addWidget(btn);
        group->addButton(btn, i);
        m_container << btn;
    }
    setLayout(layout);

    connect(group, SIGNAL(buttonClicked(int)), SLOT(switchToSelectedItemStyle(int)));

    switchToSelectedItemStyle(0);
}

MusicFuntionAnimationWidget::~MusicFuntionAnimationWidget()
{
    qDeleteAll(m_container);
}

QString MusicFuntionAnimationWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicFuntionAnimationWidget::paintEvent(QPaintEvent *event)
{
    m_perWidth = m_container[0]->width() + m_container[0]->x();

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 0.1, Qt::SolidLine));

    int offset = 0;
    if(m_isAnimation)
    {
        offset = m_x + m_perWidth - (BUTTON_SZIE_WIDTH + m_pix.width()) / 2;
        painter.drawLine(0, height(), offset, height());
        painter.drawLine(offset + m_pix.width(), height(), width(), height());
        painter.drawPixmap(offset, height() - m_pix.height(), m_pix);
    }
    else
    {
        offset = m_curIndex * m_perWidth + m_perWidth - (BUTTON_SZIE_WIDTH + m_pix.width()) / 2;
        painter.drawLine(0, height(), offset, height());
        painter.drawLine(offset + m_pix.width(), height(), width(), height());
        painter.drawPixmap(offset, height() - m_pix.height(), m_pix);
    }

    QWidget::paintEvent(event);
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

    m_isAnimation = true;
    m_preIndex = m_curIndex;
    m_curIndex = index;
    m_animation->setStartValue(m_preIndex*m_perWidth);
    m_animation->setEndValue(index*m_perWidth);
    m_animation->start();

    emit buttonClicked(index);
}

void MusicFuntionAnimationWidget::animationChanged(const QVariant &value)
{
    m_x = value.toInt();
    update();
}

void MusicFuntionAnimationWidget::finished()
{
    m_isAnimation = false;
}
