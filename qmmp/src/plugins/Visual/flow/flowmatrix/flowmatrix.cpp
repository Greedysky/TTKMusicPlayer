#include "flowmatrix.h"
#include "matrix.h"

#include <QMenu>
#include <QPainter>
#include <qmmp/qmmp.h>

#define MATRIX_SIZE     5

FlowMatrix::FlowMatrix(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Flow Matrix Widget"));
    srand(QDateTime::currentMSecsSinceEpoch());

    for(int i = 0; i < 50; ++i)
    {
        Matrix *matrix = new Matrix(this);
        matrix->resize(20, 200);
        matrix->move(i * 20, 0);
        m_matrixs << matrix;
    }

    m_pixmap.load(":/data/back");
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
    painter.setPen(QColor(0x60, 0x60, 0x60));
    painter.setFont(font);
    painter.drawText(0, height() / 4, width(), height() / 4, Qt::AlignCenter, "TTK Music Player");

    font.setPixelSize(font.pixelSize() * 3 / 4);
    painter.setFont(font);
    painter.drawText(0, height() / 2, width(), height() / 4, Qt::AlignCenter, "Greedysky");
    painter.setOpacity(0.8);

    const int w = m_pixmap.width();
    const int h = m_pixmap.height();

    for(int i = 0; i < width() / w + 1; ++i)
    {
        for(int j = 0; j < height() / h + 1; ++j)
        {
            painter.drawPixmap(i * w, j * h, m_pixmap);
        }
    }
}

void FlowMatrix::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.exec(QCursor::pos());
}

void FlowMatrix::processData(float *left, float *)
{
    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        delete[] m_visualData;
        m_visualData = new int[m_cols * 2]{0};
    }

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols * 2; ++i)
    {
        pos += step;
        m_visualData[i] = int(left[pos >> 8] * m_rows / 2);
        m_visualData[i] = qBound(-m_rows / 2, m_visualData[i], m_rows / 2);
        m_visualData[m_cols * 2 - i - 1] = m_visualData[i];
    }

    int max = 0;
    for(int i = 0; i < m_rows * 2; ++i)
    {
        const int value = std::abs(m_visualData[i]);
        if(max < value)
        {
            max = value;
        }
    }

    max = (max < MATRIX_SIZE) ? MATRIX_SIZE * 2 : max / MATRIX_SIZE;
    for(Matrix *matrix : qAsConst(m_matrixs))
    {
        if(matrix->isRunning())
        {
            m_x = matrix->x();
            m_y = matrix->y();
            m_y += 1;

            if(m_y > height())
            {
                m_y = 0;
            }

            matrix->move(m_x, m_y + rand() % max);
        }
        else
        {
            matrix->start();
        }
    }
}

void FlowMatrix::processState(bool v)
{
    if(v)
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
