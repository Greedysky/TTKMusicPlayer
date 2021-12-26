#include "musicsongsearchedit.h"
#include "musicwidgetheaders.h"

MusicSongSearchEdit::MusicSongSearchEdit(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 2, 5, 0);

    QLabel *showIcon = new QLabel(this);
    showIcon->setFixedSize(16, 16);
    showIcon->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01 + "border-image: url(:/tiny/btn_search_main_hover);");

    m_searchEdit = new MusicSearchEdit(this);
    layout->addWidget(showIcon);
    layout->addWidget(m_searchEdit);
    setLayout(layout);
}

MusicSongSearchEdit::~MusicSongSearchEdit()
{
    delete m_searchEdit;
}

void MusicSongSearchEdit::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setPen(QPen("#666666"));
    painter.drawLine(0, 0, width() - 1, 0);
    painter.drawLine(0, 0, 0, height() - 1);
    painter.drawLine(0, height() - 1, width() - 1, height() - 1);
    painter.drawLine(width() - 1, 0, width() - 1, height() - 1);
}
