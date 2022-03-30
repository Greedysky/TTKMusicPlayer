#include "matrix.h"

#include <QFile>
#include <QPainter>

Matrix::Matrix(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    qsrand(QDateTime::currentMSecsSinceEpoch());

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

    loadFile();
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
    if(m_word_list.empty())
    {
        return;
    }

    m_word.clear();
    for(int i = 0; i < 10; ++i)
    {
        const int index = qrand() % m_word_list.count();
        m_word.append(m_word_list[index]);
    }

    update();
}

void Matrix::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);

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

void Matrix::loadFile()
{
    QFile file(":/data/binary");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug("Matrix: Open binary data failed");
        return;
    }

    while(!file.atEnd())
    {
        m_word_list << file.readLine().trimmed();
    }
}
