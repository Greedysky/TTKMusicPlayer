#include "kugouwindow.h"

#ifdef MUSIC_WEBKIT
# ifdef MUSIC_GREATER_NEW
#  include <QtWebKitWidgets/QWebView>
#  include <QtWebKitWidgets/QWebFrame>
# else
#  include <QWebView>
#  include <QWebFrame>
# endif
#endif
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

const QString radioStyle = "QPushButton{ border:none; color:rgb(135, 135, 135);} \
                            QPushButton:hover{ color:rgb(104, 169, 236);} \
                            QPushButton:checked{ color:rgb(40, 143, 231);}";

KugouWindow::KugouWindow(KuGouType type, QWidget *parent)
    : QWidget(parent)
{
    m_buttonGroup = nullptr;
    m_topWidget = nullptr;
    m_webView = nullptr;
    switch(type)
    {
        case KuGouSong:
                createKugouSongWidget();
                break;
        case KuGouRadio:
                createKugouRadioWidget();
                break;
        case KuGouList:
                createKugouListWidget();
                break;
        case KugouMv:
                createKugouMVWidget();
                break;
        case KuGouLive: break;
        case KuGouLrc: break;
    }
}

KugouWindow::~KugouWindow()
{
    delete m_buttonGroup;
    delete m_topWidget;
    delete m_webView;
}

QString KugouWindow::getClassName()
{
    return staticMetaObject.className();
}

void KugouWindow::kugouSongIndexChanged(int index)
{
    changeClickedButtonStyle(index);
    QString url = KugouUrl::getSongRecommendUrl();
    switch(index)
    {
        case 0: url = KugouUrl::getSongRecommendUrl(); break;
        case 1: url = KugouUrl::getSongRankUrl(); break;
        case 2: url = KugouUrl::getSongSingerUrl(); break;
        case 3: url = KugouUrl::getSongCategoryUrl(); break;
        case 4: url = KugouUrl::getSongShowUrl(); break;
        case 5: url = KugouUrl::getSongHeroesUrl(); break;
    }
#ifdef MUSIC_WEBKIT
    static_cast<QWebView*>(m_webView)->setUrl(QUrl( url ));
#endif
}

void KugouWindow::kugouRadioIndexChanged(int index)
{
    changeClickedButtonStyle(index);
    QString url = KugouUrl::getRadioPublicUrl();
    switch(index)
    {
        case 0: url = KugouUrl::getRadioPublicUrl(); break;
        case 1: url = KugouUrl::getRadioHighFmUrl(); break;
        case 2: url = KugouUrl::getRadioFxUrl(); break;
        case 3: url = KugouUrl::getRadioHomeUrl(); break;
    }
#ifdef MUSIC_WEBKIT
    static_cast<QWebView*>(m_webView)->setUrl(QUrl( url ));
#endif
}

void KugouWindow::kugouMVIndexChanged(int index)
{
    changeClickedButtonStyle(index);
    QString url = KugouUrl::getMVRadioUrl();
    switch(index)
    {
        case 0: url = KugouUrl::getMVRadioUrl(); break;
        case 1: url = KugouUrl::getMVRecommendUrl(); break;
        case 2: url = KugouUrl::getMVFanxingUrl(); break;
        case 3: url = KugouUrl::getMVStarMusicUrl(); break;
    }
#ifdef MUSIC_WEBKIT
    static_cast<QWebView*>(m_webView)->setUrl(QUrl( url ));
#endif
}

void KugouWindow::createKugouSongWidget()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#ifdef MUSIC_WEBKIT
    m_topWidget = new QWidget(this);
    m_topWidget->setFixedHeight(25);
    m_topWidget->setStyleSheet(radioStyle + "QWidget{background:white;}");
    QHBoxLayout *topLayout = new QHBoxLayout(m_topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(25);

    m_buttonGroup = new QButtonGroup(this);
    QPushButton *bt = new QPushButton(tr(" SongRecommend "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 0);
    bt = new QPushButton(tr(" SongRank "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 1);
    bt = new QPushButton(tr(" SongSinger "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 2);
    bt = new QPushButton(tr(" SongCategory "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 3);
    bt = new QPushButton(tr(" SongShow "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 4);
    bt = new QPushButton(tr(" SongHeroes "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 5);
    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(kugouSongIndexChanged(int)));

    topLayout->addStretch(1);
    topLayout->addWidget(m_buttonGroup->button(0));
    topLayout->addWidget(m_buttonGroup->button(1));
    topLayout->addWidget(m_buttonGroup->button(2));
    topLayout->addWidget(m_buttonGroup->button(3));
    topLayout->addWidget(m_buttonGroup->button(4));
    topLayout->addWidget(m_buttonGroup->button(5));
    topLayout->addStretch(1);

    QWebView *view = new QWebView(this);
    view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    layout->addWidget(m_topWidget);
    layout->addWidget(m_webView = view);

    kugouSongIndexChanged(0);
#else
    QLabel *pix = new QLabel(this);
    pix->setPixmap(QPixmap(":/image/lb_no_webkit_song"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void KugouWindow::createKugouRadioWidget()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#ifdef MUSIC_WEBKIT
    m_topWidget = new QWidget(this);
    m_topWidget->setFixedHeight(25);
    m_topWidget->setStyleSheet(radioStyle + "QWidget{background:white;}");
    QHBoxLayout *topLayout = new QHBoxLayout(m_topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(50);

    m_buttonGroup = new QButtonGroup(this);
    QPushButton *bt = new QPushButton(tr(" RadioPublic "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 0);
    bt = new QPushButton(tr(" RadioHigh "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 1);
    bt = new QPushButton(tr(" RadioFx "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 2);
    bt = new QPushButton(tr(" RadioHome "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 3);
    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(kugouRadioIndexChanged(int)));

    topLayout->addStretch(1);
    topLayout->addWidget(m_buttonGroup->button(0));
    topLayout->addWidget(m_buttonGroup->button(1));
    topLayout->addWidget(m_buttonGroup->button(2));
    topLayout->addWidget(m_buttonGroup->button(3));
    topLayout->addStretch(1);

    QWebView *view = new QWebView(this);
    view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    layout->addWidget(m_topWidget);
    layout->addWidget(m_webView = view);

    kugouRadioIndexChanged(0);
#else
    QLabel *pix = new QLabel(this);
    pix->setPixmap(QPixmap(":/image/lb_no_webkit_radio"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void KugouWindow::createKugouListWidget()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#ifdef MUSIC_WEBKIT
    QWebView *view = new QWebView(this);
    view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    view->setUrl(QUrl( KugouUrl::getListUrl() ));
    layout->addWidget(m_webView = view);
#else
    QLabel *pix = new QLabel(this);
    pix->setPixmap(QPixmap(":/image/lb_no_webkit_list"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void KugouWindow::createKugouMVWidget()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#ifdef MUSIC_WEBKIT
    m_topWidget = new QWidget(this);
    m_topWidget->setFixedHeight(25);
    m_topWidget->setStyleSheet(radioStyle + "QWidget{background:white;}");
    QHBoxLayout *topLayout = new QHBoxLayout(m_topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(50);

    m_buttonGroup = new QButtonGroup(this);
    QPushButton *bt = new QPushButton(tr(" MVRadio "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 0);
    bt = new QPushButton(tr(" MVRecommend "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 1);
    bt = new QPushButton(tr(" MVFanxing "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 2);
    bt = new QPushButton(tr(" MVStarMusic "), m_topWidget);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonGroup->addButton(bt, 3);
    connect(m_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(kugouMVIndexChanged(int)));

    topLayout->addStretch(1);
    topLayout->addWidget(m_buttonGroup->button(0));
    topLayout->addWidget(m_buttonGroup->button(1));
    topLayout->addWidget(m_buttonGroup->button(2));
    topLayout->addWidget(m_buttonGroup->button(3));
    topLayout->addStretch(1);

    QWebView *view = new QWebView(this);
    view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    layout->addWidget(m_topWidget);
    layout->addWidget(m_webView = view);

    kugouMVIndexChanged(0);
#else
    QLabel *pix = new QLabel(this);
    pix->setPixmap(QPixmap(":/image/lb_no_webkit_mv"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

void KugouWindow::changeClickedButtonStyle(int index)
{
    for(int i=0; i<m_buttonGroup->buttons().count(); ++i)
    {
        m_buttonGroup->button(i)->setStyleSheet(radioStyle);
    }
    m_buttonGroup->button(index)->setStyleSheet("QPushButton{ color:rgb(104, 169, 236);}");
}
