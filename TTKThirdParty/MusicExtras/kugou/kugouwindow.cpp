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

KugouWindow::KugouWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
#ifdef MUSIC_WEBKIT
    const QString radioStyle = "QPushButton{ border:none; color:rgb(135, 135, 135);} \
                                QPushButton:hover{ color:rgb(104, 169, 236);} \
                                QPushButton:checked{ color:rgb(40, 143, 231);} \
                                QWidget{background: white;}";
    QWidget *top = new QWidget(this);
    top->setFixedHeight(25);
    top->setStyleSheet( radioStyle );
    QHBoxLayout *topLayout = new QHBoxLayout(top);
    topLayout->setContentsMargins(0, 0, 0, 0);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    QPushButton *bt = new QPushButton(tr(" Recommend "), top);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(bt, 0);
    bt = new QPushButton(tr(" Radio "), top);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(bt, 1);
    bt = new QPushButton(tr(" Rank "), top);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(bt, 2);
    bt = new QPushButton(tr(" Singer "), top);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(bt, 3);
    bt = new QPushButton(tr(" Category "), top);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(bt, 4);
    bt = new QPushButton(tr(" Show "), top);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(bt, 5);
    bt = new QPushButton(tr(" CCTV "), top);
    bt->setCursor(QCursor(Qt::PointingHandCursor));
    buttonGroup->addButton(bt, 6);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(differButtonIndexChanged(int)));

    topLayout->addStretch(1);
    topLayout->addWidget(buttonGroup->button(0));
    topLayout->addWidget(buttonGroup->button(1));
    topLayout->addWidget(buttonGroup->button(2));
    topLayout->addWidget(buttonGroup->button(3));
    topLayout->addWidget(buttonGroup->button(4));
    topLayout->addWidget(buttonGroup->button(5));
    topLayout->addWidget(buttonGroup->button(6));
    topLayout->addStretch(1);

    QWebView *view = new QWebView(this);
    view->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    view->setUrl(QUrl( KugouUrl::getRecommendUrl() ));

    layout->addWidget(top);
    layout->addWidget(m_webView = view);
#else
    m_webView = nullptr;
    QLabel *pix = new QLabel(this);
    pix->setPixmap(QPixmap(":/image/nowebkit"));
    layout->addWidget(pix);
#endif
    setLayout(layout);
}

KugouWindow::~KugouWindow()
{
    delete m_webView;
}

QString KugouWindow::getClassName()
{
    return staticMetaObject.className();
}

void KugouWindow::differButtonIndexChanged(int index)
{
    QString url = KugouUrl::getRecommendUrl();
    switch(index)
    {
        case 0: url = KugouUrl::getRecommendUrl(); break;
        case 1: url = KugouUrl::getRadioUrl(); break;
        case 2: url = KugouUrl::getRankUrl(); break;
        case 3: url = KugouUrl::getSingerUrl(); break;
        case 4: url = KugouUrl::getCategoryUrl(); break;
        case 5: url = KugouUrl::getShowUrl(); break;
        case 6: url = KugouUrl::getCCTVUrl(); break;
    }
#ifdef MUSIC_WEBKIT
    static_cast<QWebView*>(m_webView)->setUrl(QUrl( url ));
#endif
}
