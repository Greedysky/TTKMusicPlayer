#include "matrix.h"

#include <QPainter>

Matrix::Matrix(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    srand(QDateTime::currentMSecsSinceEpoch());

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void Matrix::start()
{
    if(!m_timer->isActive())
    {
        m_timer->start();
    }
}

void Matrix::stop()
{
    m_timer->stop();
}

bool Matrix::isRunning() const
{
    return m_timer->isActive();
}

void Matrix::timeout()
{
    m_word.clear();
    for(int i = 0; i < 10; ++i)
    {
        m_word.append(rand() % 2 ? "0" : "1");
    }

    update();
}

void Matrix::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);

    for(int i = 0; i < m_word.count(); ++i)
    {
        painter.setPen(QColor(0, 0x8b, 0x8b, 23 * i));
        QFont font("Microsoft YaHei", 10);
        font.setBold(true);

        const QFontMetrics fm(font);
        painter.setFont(font);
        painter.drawText(0, fm.height() * (i + 1), m_word.at(i));
    }
}
