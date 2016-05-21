#include "kugouwindow.h"

#include <QBoxLayout>
#include <QWebView>
#include <QWebFrame>

KugouWindow::KugouWindow(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWebView *w = new QWebView(this);
    w->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    w->setUrl(QUrl("http://www2.kugou.kugou.com/yueku/v9/html/home.html"));

    layout->addWidget(w);
    setLayout(layout);
}

KugouWindow::~KugouWindow()
{

}
