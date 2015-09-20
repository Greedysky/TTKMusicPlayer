#include "musicbackgroundremotewidget.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QListWidget>

MusicBackgroundRemoteWidget::MusicBackgroundRemoteWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(new QPushButton(this));
    hbox->addWidget(new QPushButton(this));
    hbox->addWidget(new QPushButton(this));
    vbox->addLayout(hbox);

    QListWidget *listWidget = new QListWidget(this);
    vbox->addWidget(listWidget);

    setLayout(vbox);
}

MusicBackgroundRemoteWidget::~MusicBackgroundRemoteWidget()
{

}
