#include "musicwebradioview.h"
#include "musicwebmusicradiolistview.h"
#include "musicuiobject.h"
#include "musicrightareawidget.h"

#include <QBoxLayout>
#include <QPushButton>

MusicWebRadioView::MusicWebRadioView(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_musicRadio = new MusicWebMusicRadioListView(this);

    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(40);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setSpacing(0);
    bottomLayout->setContentsMargins(5, 0, 0, 0);
    QPushButton *button = new QPushButton(this);
    button->setIcon(QIcon(":/tiny/btn_wallpaper_off_hover"));
    button->setCursor(QCursor(Qt::PointingHandCursor));
    button->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    button->setFixedWidth(40);
#ifdef Q_OS_UNIX
    button->setFocusPolicy(Qt::NoFocus);
#endif

    bottomLayout->addStretch(1);
    bottomLayout->addWidget(button);
    bottomWidget->setLayout(bottomLayout);

    layout->addWidget(m_musicRadio);
    layout->addWidget(bottomWidget);
    setLayout(layout);

    connect(button, SIGNAL(clicked()), SLOT(openDJRadioWindow()));
}

MusicWebRadioView::~MusicWebRadioView()
{
    delete m_musicRadio;
}

QString MusicWebRadioView::getClassName()
{
    return staticMetaObject.className();
}

void MusicWebRadioView::init()
{
    m_musicRadio->initListItems();
}

void MusicWebRadioView::openDJRadioWindow()
{
    MusicRightAreaWidget::instance()->musicFunctionClicked(MusicRightAreaWidget::WebDJRadio);
}

void MusicWebRadioView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
