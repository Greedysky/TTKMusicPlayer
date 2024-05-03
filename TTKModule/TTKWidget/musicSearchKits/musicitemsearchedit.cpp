#include "musicitemsearchedit.h"
#include "musicwidgetheaders.h"

MusicItemSearchEdit::MusicItemSearchEdit(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 2, 5, 0);

    QLabel *icon = new QLabel(this);
    icon->setFixedSize(16, 16);
    icon->setStyleSheet(TTK::UI::BackgroundStyle01 + "border-image:url(:/tiny/btn_search_main_hover);");

    m_searchEdit = new MusicSearchEdit(this);
    layout->addWidget(icon);
    layout->addWidget(m_searchEdit);
    setLayout(layout);
}

MusicItemSearchEdit::~MusicItemSearchEdit()
{
    delete m_searchEdit;
}

void MusicItemSearchEdit::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setPen(QPen(QColor(0x66, 0x66, 0x66)));
    painter.drawLine(0, 0, width() - 1, 0);
    painter.drawLine(0, 0, 0, height() - 1);
    painter.drawLine(0, height() - 1, width() - 1, height() - 1);
    painter.drawLine(width() - 1, 0, width() - 1, height() - 1);
}


MusicItemQueryEdit::MusicItemQueryEdit(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(TTK::UI::BackgroundStyle01);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 2, 0, 0);

    QPushButton *button = new QPushButton(this);
    button->setIcon(QIcon(":/tiny/btn_search_main_hover"));
    button->setCursor(QCursor(Qt::PointingHandCursor));
    button->setIconSize(QSize(25, 25));
#ifdef Q_OS_UNIX
    button->setFocusPolicy(Qt::NoFocus);
#endif
    m_searchEdit = new MusicSearchEdit(this);
    layout->addWidget(m_searchEdit);
    layout->addWidget(button);
    setLayout(layout);

    connect(button, SIGNAL(clicked()), SLOT(enterFinished()));
    connect(m_searchEdit, SIGNAL(enterFinished(QString)), SLOT(enterFinished()));
}

MusicItemQueryEdit::~MusicItemQueryEdit()
{
    delete m_searchEdit;
}

void MusicItemQueryEdit::enterFinished()
{
    Q_EMIT trigger(m_searchEdit->text());
}

void MusicItemQueryEdit::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(), QColor(255, 255, 255, 180));

    painter.setPen(QPen(QColor(0x66, 0x66, 0x66)));
    painter.drawLine(0, 0, width() - 1, 0);
    painter.drawLine(0, 0, 0, height() - 1);
    painter.drawLine(0, height() - 1, width() - 1, height() - 1);
    painter.drawLine(width() - 1, 0, width() - 1, height() - 1);
}
