#include "musicvideofloatwidget.h"
#include "musicvideofloatuiobject.h"
#include "musicwidgetheaders.h"

MusicVideoFloatWidget::MusicVideoFloatWidget(QWidget *parent)
    : MusicFloatAbstractWidget(parent)
{
    setStyleSheet(MusicUIObject::MBackgroundStyle06);

    resizeWindow(0, 0);
    m_search = new QPushButton(tr(" Search"), this);
    m_fresh = new QPushButton(tr(" Fresh"), this);
    m_fullscreen = new QPushButton(tr(" Fullscreen"), this);
    m_download = new QPushButton(tr(" Download"), this);
    m_share = new QPushButton(tr(" Share"), this);

#ifdef Q_OS_UNIX
    m_search->setFocusPolicy(Qt::NoFocus);
    m_fresh->setFocusPolicy(Qt::NoFocus);
    m_fullscreen->setFocusPolicy(Qt::NoFocus);
    m_download->setFocusPolicy(Qt::NoFocus);
    m_share->setFocusPolicy(Qt::NoFocus);
#endif

    m_search->setGeometry(15, 10, 80, 20);
    m_fresh->setGeometry(15, 50, 80, 20);
    m_fullscreen->setGeometry(15, 90, 80, 20);
    m_download->setGeometry(15, 130, 80, 20);
    m_share->setGeometry(15, 170, 80, 20);

    m_search->setStyleSheet(MusicUIObject::MKGVideoFloatSearch + MusicUIObject::MPushButtonStyle09);
    m_fresh->setStyleSheet(MusicUIObject::MKGVideoFloatFresh + MusicUIObject::MPushButtonStyle09);
    m_fullscreen->setStyleSheet(MusicUIObject::MKGVideoFloatFullscreen + MusicUIObject::MPushButtonStyle09);
    m_download->setStyleSheet(MusicUIObject::MKGVideoFloatDownload + MusicUIObject::MPushButtonStyle09);
    m_share->setStyleSheet(MusicUIObject::MKGVideoFloatShare + MusicUIObject::MPushButtonStyle09);

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
    m_rectIn = QRect(566 + width, 138 + height/2, 125, 210);
    m_rectOut = QRect(676 + width, 189 + height/2, 125, 105);
    setGeometry( m_rectOut );
}

void MusicVideoFloatWidget::setText(Type type, const QString &text)
{
    switch(type)
    {
        case SearchType: m_search->setText(text); break;
        case FreshType: m_fresh->setText(text); break;
        case FullscreenType: m_fullscreen->setText(text); break;
        case DownloadType : m_download->setText(text); break;
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
        case DownloadType : return m_download->text().trimmed();
        case ShareType: return m_share->text();
        default: return QString();
    }
}
