#include "flowmatrix.h"
#include "matrix.h"
#include "inlines.h"

#include <QMenu>
#include <QPainter>
#include <qmmp/qmmp.h>

FlowMatrix::FlowMatrix(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Flow Matrix Widget"));

    qsrand(QDateTime::currentMSecsSinceEpoch());

    for(int i = 0; i < 50; ++i)
    {
        Matrix *matrix = new Matrix(this);
        matrix->resize(20, 200);
        matrix->move(i * 20, 0);
        m_matrixs << matrix;
    }
}

FlowMatrix::~FlowMatrix()
{
    qDeleteAll(m_matrixs);
}

void FlowMatrix::resizeEvent(QResizeEvent *)
{
    const int w = width() / 50;
    for(int i = 0; i < m_matrixs.count(); ++i)
    {
        Matrix *matrix = m_matrixs[i];
        matrix->resize(w, height());
        matrix->move(i * w, 0);
    }
}

void FlowMatrix::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QFont font = painter.font();
    font.setBold(true);
    font.setPixelSize(qMin(height() / 4, width() / 12));
    painter.setFont(font);
    painter.drawText(0, height() / 4, width(), height() / 4, Qt::AlignCenter, "TTK Music Player");

    font.setPixelSize(font.pixelSize() * 3 / 4);
    painter.setFont(font);
    painter.drawText(0, height() / 2, width(), height() / 4, Qt::AlignCenter, "Greedysky");
    painter.setOpacity(0.8);

    QPixmap pix(":/data/back");
    const int w = pix.width();
    const int h = pix.height();

    for(int i = 0; i < width() / w + 1; ++i)
    {
        for(int j = 0; j < height() / h + 1; ++j)
        {
            painter.drawPixmap(i * w, j * h, pix);
        }
    }
}

void FlowMatrix::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.exec(QCursor::pos());
}

void FlowMatrix::process(float *left, float *)
{
    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }

        m_intern_vis_data = new int[m_cols * 2]{0};
    }

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols * 2; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(left[pos >> 8] * m_rows / 2);
        m_intern_vis_data[i] = qBound(-m_rows / 2, m_intern_vis_data[i], m_rows / 2);
        m_intern_vis_data[m_cols * 2 - i - 1] = m_intern_vis_data[i];
    }

    int max = 0;
    for(int i = 0; i < m_rows * 2; ++i)
    {
        const int value = std::abs(m_intern_vis_data[i]);
        if(max < value)
        {
            max = value;
        }
    }

    max = (max <= 3) ? 10 : max / 5;
    for(Matrix *matrix : qAsConst(m_matrixs))
    {
        if(matrix->isRunning())
        {
            m_pos_x = matrix->pos().x();
            m_pos_y = matrix->pos().y();
            m_pos_y += 1;

            if(m_pos_y > height())
            {
                m_pos_y = 0;
            }
            matrix->move(m_pos_x, m_pos_y + qrand() % max);
        }
        else
        {
            matrix->start();
        }
    }
}

void FlowMatrix::process(bool state)
{
    Visual::process(state);

    if(state)
    {
        for(Matrix *matrix : qAsConst(m_matrixs))
        {
            matrix->start();
        }
    }
    else
    {
        for(Matrix *matrix : qAsConst(m_matrixs))
        {
            matrix->stop();
        }
    }
}
