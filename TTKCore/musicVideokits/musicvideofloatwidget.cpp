#include "musicvideofloatwidget.h"

#include <QPushButton>

MusicVideoFloatWidget::MusicVideoFloatWidget(QWidget *parent)
    : MusicFloatAbstractWidget(parent)
{
    setStyleSheet(MusicUIObject::MCustomStyle28);

    resizeWindow(0, 0);
    m_search = new QPushButton(tr(" Search"), this);
    m_fresh = new QPushButton(tr(" Fresh"), this);
    m_fullscreen = new QPushButton(tr(" Fullscreen"), this);
    m_download = new QPushButton(tr(" Download"), this);
    m_share = new QPushButton(tr(" Share"), this);

    m_search->setIcon(QIcon(":/video/search"));
    m_fresh->setIcon(QIcon(":/video/fresh"));
    m_fullscreen->setIcon(QIcon(":/video/fullscreen"));
    m_download->setIcon(QIcon(":/video/download"));
    m_share->setIcon(QIcon(":/image/songShare"));

    m_search->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_fresh->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_fullscreen->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_download->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_share->setStyleSheet( MusicUIObject::MPushButtonStyle15 );

    m_search->move(15, 10);
    m_fresh->move(15, 50);
    m_fullscreen->move(15, 90);
    m_download->move(15, 130);
    m_share->move(15, 170);

    m_search->setCursor(QCursor(Qt::PointingHandCursor));
    m_fresh->setCursor(QCursor(Qt::PointingHandCursor));
    m_fullscreen->setCursor(QCursor(Qt::PointingHandCursor));
    m_download->setCursor(QCursor(Qt::PointingHandCursor));
    m_share->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_search, SIGNAL(clicked()), SIGNAL(searchButtonClicked()));
    connect(m_fresh, SIGNAL(clicked()), SIGNAL(freshButtonClicked()));
    connect(m_fullscreen, SIGNAL(clicked()), SIGNAL(fullscreenButtonClicked()));
    connect(m_download, SIGNAL(clicked()), SIGNAL(downloadButtonClicked()));
    connect(m_share, SIGNAL(clicked()), SIGNAL(shareButtonClicked()));

}

MusicVideoFloatWidget::~MusicVideoFloatWidget()
{
    delete m_search;
    delete m_fresh;
    delete m_fullscreen;
    delete m_download;
    delete m_share;
}

void MusicVideoFloatWidget::resizeWindow(int width, int height)
{
    m_rectIn = QRect(508 + width, 120 + height, 125, 210);
    m_rectOut = QRect(628 + width, 171 + height, 125, 105);
    setGeometry( m_rectOut );
}

void MusicVideoFloatWidget::setText(Type type, const QString &text)
{
    switch(type)
    {
        case SearchType: m_search->setText(text); break;
        case FreshType: m_fresh->setText(text); break;
        case FullscreenType: m_fullscreen->setText(text); break;
        case DownloadType: m_download->setText(text); break;
        case ShareType: m_share->setText(text); break;
        default: break;
    }
}

QString MusicVideoFloatWidget::getText(Type type) const
{
    switch(type)
    {
        case SearchType: return m_search->text().trimmed();
        case FreshType: return m_fresh->text().trimmed();
        case FullscreenType: return m_fullscreen->text().trimmed();
        case DownloadType: return m_download->text().trimmed();
        case ShareType: return m_share->text();
        default: return QString();
    }
}
