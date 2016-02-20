#include "musicnetworktestwidget.h"
#include "ui_musicnetworktestwidget.h"

MusicNetworkTestWidget::MusicNetworkTestWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MusicNetworkTestWidget)
{
    ui->setupUi(this);
}

MusicNetworkTestWidget::~MusicNetworkTestWidget()
{
    delete ui;
}
