#include "musicvideofloatwidget.h"

#include <QPushButton>

MusicVideoFloatWidget::MusicVideoFloatWidget(QWidget *parent)
    : MusicFloatAbstractWidget(parent)
{
    setStyleSheet("background:rgba(41,41,41);");

    resizeWidth(0);
    m_update = new QPushButton(tr(" Update"), this);
    m_search = new QPushButton(tr(" Search"), this);
    m_more = new QPushButton(tr(" More"), this);
    m_wallp = new QPushButton(tr(" Wallp"), this);
    m_photo = new QPushButton(tr(" Photo"), this);

    m_update->setIcon(QIcon(":/lrc/update"));
    m_search->setIcon(QIcon(":/lrc/search"));
    m_more->setIcon(QIcon(":/lrc/more"));
    m_wallp->setIcon(QIcon(":/lrc/wallpaper"));
    m_photo->setIcon(QIcon(":/share/showMV2"));

    m_update->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_search->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_more->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_wallp->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_photo->setStyleSheet( MusicUIObject::MPushButtonStyle15 );

    m_update->setGeometry(15, 10, m_update->width(), m_update->height());
    m_search->setGeometry(15, 50, m_search->width(), m_search->height());
    m_more->setGeometry(15, 90, m_more->width(), m_more->height());
    m_wallp->setGeometry(15, 130, m_wallp->width(), m_wallp->height());
    m_photo->setGeometry(15, 170, m_photo->width(), m_photo->height());

    m_update->setCursor(QCursor(Qt::PointingHandCursor));
    m_search->setCursor(QCursor(Qt::PointingHandCursor));
    m_more->setCursor(QCursor(Qt::PointingHandCursor));
    m_wallp->setCursor(QCursor(Qt::PointingHandCursor));
    m_photo->setCursor(QCursor(Qt::PointingHandCursor));

}

MusicVideoFloatWidget::~MusicVideoFloatWidget()
{
    delete m_more;
    delete m_update;
    delete m_search;
    delete m_wallp;
    delete m_photo;
}

void MusicVideoFloatWidget::resizeWidth(int width)
{
    m_rectIn = QRect(518 + width, 120, 115, 210);
    m_rectOut = QRect(628 + width, 171, 115, 105);
    setGeometry( m_rectOut );
}
