#include "musicvideofloatwidget.h"
#include "musicvideofloatuiobject.h"
#include "musicwidgetheaders.h"

MusicVideoFloatWidget::MusicVideoFloatWidget(QWidget *parent)
    : MusicAbstractFloatWidget(parent)
{
    setStyleSheet(MusicUIObject::MQSSBackgroundStyle06);

    resizeWindow(0, 0);
    m_search = new QPushButton(tr(" Search"), this);
    m_popup = new QPushButton(tr(" Popup"), this);
    m_fullscreen = new QPushButton(tr(" Fullscreen"), this);
    m_download = new QPushButton(tr(" Download"), this);
    m_share = new QPushButton(tr(" Share"), this);

#ifdef Q_OS_UNIX
    m_search->setFocusPolicy(Qt::NoFocus);
    m_popup->setFocusPolicy(Qt::NoFocus);
    m_fullscreen->setFocusPolicy(Qt::NoFocus);
    m_download->setFocusPolicy(Qt::NoFocus);
    m_share->setFocusPolicy(Qt::NoFocus);
#endif

    m_search->setGeometry(15, 10, 80, 20);
    m_popup->setGeometry(15, 50, 80, 20);
    m_fullscreen->setGeometry(15, 90, 80, 20);
    m_download->setGeometry(15, 130, 80, 20);
    m_share->setGeometry(15, 170, 80, 20);

    m_search->setStyleSheet(MusicUIObject::MQSSVideoFloatSearch + MusicUIObject::MQSSPushButtonStyle07);
    m_popup->setStyleSheet(MusicUIObject::MQSSVideoFloatFresh + MusicUIObject::MQSSPushButtonStyle07);
    m_fullscreen->setStyleSheet(MusicUIObject::MQSSVideoFloatFullscreen + MusicUIObject::MQSSPushButtonStyle07);
    m_download->setStyleSheet(MusicUIObject::MQSSVideoFloatDownload + MusicUIObject::MQSSPushButtonStyle07);
    m_share->setStyleSheet(MusicUIObject::MQSSVideoFloatShare + MusicUIObject::MQSSPushButtonStyle07);

    m_search->setCursor(QCursor(Qt::PointingHandCursor));
    m_popup->setCursor(QCursor(Qt::PointingHandCursor));
    m_fullscreen->setCursor(QCursor(Qt::PointingHandCursor));
    m_download->setCursor(QCursor(Qt::PointingHandCursor));
    m_share->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_search, SIGNAL(clicked()), SIGNAL(searchButtonClicked()));
    connect(m_popup, SIGNAL(clicked()), SIGNAL(popupButtonClicked()));
    connect(m_fullscreen, SIGNAL(clicked()), SIGNAL(fullscreenButtonClicked()));
    connect(m_download, SIGNAL(clicked()), SIGNAL(downloadButtonClicked()));
    connect(m_share, SIGNAL(clicked()), SIGNAL(shareButtonClicked()));
}

MusicVideoFloatWidget::~MusicVideoFloatWidget()
{
    delete m_search;
    delete m_popup;
    delete m_fullscreen;
    delete m_download;
    delete m_share;
}

void MusicVideoFloatWidget::resizeWindow(int width, int height)
{
    m_rectIn = QRect(566 + width, 138 + height / 2, 125, 210);
    m_rectOut = QRect(676 + width, 189 + height / 2, 125, 105);
    setGeometry(m_rectOut);
}

bool MusicVideoFloatWidget::isPopupMode() const
{
    return m_popup->text() != tr(" Popup");
}

void MusicVideoFloatWidget::popupMode(bool popup)
{
    m_popup->setText(popup ? tr(" Inline") : tr(" Popup"));
}

bool MusicVideoFloatWidget::isFullscreenMode() const
{
    return m_fullscreen->text() != tr(" Fullscreen");
}

void MusicVideoFloatWidget::fullscreenMode(bool full)
{
    m_fullscreen->setText(full ? tr(" Normal") : tr(" Fullscreen"));
}
