#include "musicemojilabelwidget.h"
#include "musictinyuiobject.h"
#include "musicuiobject.h"
#include "musicwidgetheaders.h"
#include "ttkclickedgroup.h"
#include "ttkclickedlabel.h"

#include <QFile>
#include <QButtonGroup>

MusicEMOJILabelWidget::MusicEMOJILabelWidget(QWidget *parent)
    : QLabel(parent),
      m_currentPage(-1)
{
    setFixedSize(260, 175);
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet(TTK::UI::BackgroundStyle10);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    QToolButton *closeButton = new QToolButton(this);
    closeButton->setFixedSize(16, 16);
    closeButton->setStyleSheet(TTK::UI::TinyBtnClose);
    closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QWidget *labelWidget = new QWidget(this);
    labelWidget->setFixedHeight(110);
    //
    QGridLayout *labelIconLayout = new QGridLayout(labelWidget);
    labelIconLayout->setContentsMargins(0, 0, 0, 0);
    labelIconLayout->setSpacing(3);

    TTKClickedGroup *clickedGroup = new TTKClickedGroup(this);
    connect(clickedGroup, SIGNAL(clicked(int)), SLOT(labelClicked(int)));

    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 7; ++j)
        {
            TTKClickedLabel *label = new TTKClickedLabel(labelWidget);
            label->setAlignment(Qt::AlignCenter);
            label->setStyleSheet(QString("QLabel{ %1 } QLabel:hover{ %2 }").arg(TTK::UI::BorderStyle02, TTK::UI::BorderStyle03));
            label->setFixedSize(32, 32);
            labelIconLayout->addWidget(label, i, j);

            m_labelItems << label;
            clickedGroup->mapped(label);
        }
    }
    labelWidget->setLayout(labelIconLayout);
    //
    QWidget *buttonWidget = new QWidget(this);
    QHBoxLayout *buttonWidgetLayout = new QHBoxLayout(buttonWidget);
    buttonWidgetLayout->setContentsMargins(0, 0, 5, 0);
    buttonWidgetLayout->addStretch(1);

    QButtonGroup *buttonGroup = new QButtonGroup(buttonWidget);
    QtButtonGroupConnect(buttonGroup, this, buttonClicked, TTK_SLOT);

    for(int i = 0; i < 5; ++i)
    {
        QToolButton *button = new QToolButton(buttonWidget);
        button->setCursor(Qt::PointingHandCursor);
        button->setText(QString::number(i + 1));
        button->setFixedSize(15, 15);
        buttonWidgetLayout->addWidget(button, 0 , Qt::AlignRight);
        buttonWidget->setLayout(buttonWidgetLayout);
        buttonGroup->addButton(button, i);
        m_buttonItems << button;
    }
    buttonClicked(0);
    //
    layout->addWidget(closeButton, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(labelWidget);
    layout->addWidget(buttonWidget);
    setLayout(layout);

    QFile file(":/emoji/db");
    if(file.open(QIODevice::ReadOnly))
    {
       m_datas = QString(file.readAll()).split(TTK_LINEFEED);
       file.close();
    }
}

MusicEMOJILabelWidget::~MusicEMOJILabelWidget()
{
    qDeleteAll(m_labelItems);
    qDeleteAll(m_buttonItems);
}

void MusicEMOJILabelWidget::buttonClicked(int index)
{
    m_currentPage = index;
    for(QToolButton *button : qAsConst(m_buttonItems))
    {
        button->setStyleSheet(TTK::UI::BackgroundStyle01 + TTK::UI::BorderStyle01);
    }
    m_buttonItems[index]->setStyleSheet(m_buttonItems[index]->styleSheet() + TTK::UI::BorderStyle02);

    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 7; ++j)
        {
            const int in = i * 7 + j;
            m_labelItems[in]->setPixmap(QPixmap(QString(":/emoji/%1").arg(21 * index + in + 1)));
        }
    }
}

void MusicEMOJILabelWidget::labelClicked(int index)
{
    const int offset = index + m_currentPage * 21;
    if(offset < 0 || offset >= m_datas.count())
    {
        return;
    }

    Q_EMIT dataChanged(m_datas[offset]);
}

void MusicEMOJILabelWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    close();
}
