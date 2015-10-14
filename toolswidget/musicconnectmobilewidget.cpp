#include "musicconnectmobilewidget.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>

MusicConnectMobileWidget::MusicConnectMobileWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);
    QLabel *textLabel = new QLabel(tr("Please choose the way"), this);
    QPushButton *firButton = new QPushButton(this);
    QPushButton *secButton = new QPushButton(this);
    firButton->setFixedSize(200, 90);
    secButton->setFixedSize(200, 90);

    vBox->addStretch(1);
    vBox->addWidget(textLabel, 0 ,Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(firButton, 0 ,Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(secButton, 0 ,Qt::AlignCenter);
    vBox->addStretch(1);

    setLayout(vBox);

}
