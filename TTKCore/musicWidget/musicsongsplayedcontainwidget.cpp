#include "musicsongsplayedcontainwidget.h"
#include "musicsongsplayedlistwidget.h"
#include "musictinyuiobject.h"
#include "musicuiobject.h"

#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include <QToolButton>

MusicSongsPlayedContainWidget::MusicSongsPlayedContainWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QWidget *topWidget = new QWidget(this);
    topWidget->setFixedHeight(35);
    topWidget->setStyleSheet(MusicUIObject::MBackgroundStyle20);

    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setSpacing(15);
    QLabel *label = new QLabel(tr("playedList"), topWidget);
    label->setStyleSheet(MusicUIObject::MColorStyle02 + MusicUIObject::MFontStyle01 +
                         MusicUIObject::MFontStyle03);

    QPushButton *shareButton = new QPushButton(this);
    shareButton->setFixedSize(16, 16);
    shareButton->setToolTip(tr("shareList"));
    shareButton->setCursor(QCursor(Qt::PointingHandCursor));
    shareButton->setStyleSheet(MusicUIObject::MKGTinyBtnShare);

    QPushButton *deleteButton = new QPushButton(this);
    deleteButton->setFixedSize(16, 16);
    deleteButton->setToolTip(tr("clearList"));
    deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    deleteButton->setStyleSheet(MusicUIObject::MKGTinyBtnDelete);

    QToolButton *closeButton = new QToolButton(this);
    closeButton->setFixedSize(16, 16);
    closeButton->setToolTip(tr("closeList"));
    closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    closeButton->setStyleSheet(MusicUIObject::MKGTinyBtnClose);
    connect(closeButton, SIGNAL(clicked()), parent, SLOT(close()));

    topWidgetLayout->addWidget(label);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(shareButton);
    topWidgetLayout->addWidget(deleteButton);
    topWidgetLayout->addWidget(closeButton);
    topWidget->setLayout(topWidgetLayout);

    m_playedListWidget = new MusicSongsPlayedListWidget(this);
    layout->addWidget(topWidget);
    layout->addWidget(m_playedListWidget);

    setLayout(layout);
}

MusicSongsPlayedContainWidget::~MusicSongsPlayedContainWidget()
{
    delete m_playedListWidget;
}

QString MusicSongsPlayedContainWidget::getClassName()
{
    return staticMetaObject.className();
}
