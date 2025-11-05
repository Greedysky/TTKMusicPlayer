#include "mountainmidwidget.h"

#include <cmath>

MountainMidWidget::MountainMidWidget(QWidget *parent)
    : MountainWidget(parent)
{
    memset(m_bars, 0, sizeof(m_bars));
}

void MountainMidWidget::addBuffer(float *left)
{
    generateLogGraph(left, m_bars[m_offset]);
    m_offset = (m_offset + 1) % NUM_BANDS;

    MountainWidget::addBuffer(left);
}

void MountainMidWidget::initializeGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.1, 1, -1.5, 1, 2, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(int i = 0; i <= NUM_BANDS; ++i)
    {
        m_logScale[i] = std::pow(256, (float)i / NUM_BANDS) - 0.5f;
    }

    for(int y = 0; y < NUM_BANDS; ++y)
    {
        const float yf = (float)y / (NUM_BANDS - 1);
        for(int x = 0; x < NUM_BANDS; ++x)
        {
            const float xf = (float)x / (NUM_BANDS - 1);
            m_colors[x][y][0] = (1 - xf) * (1 - yf);
            m_colors[x][y][1] = xf;
            m_colors[x][y][2] = yf;
        }
    }
}

void MountainMidWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MountainMidWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    drawBars();
}

/* stolen from the skins plugin */
/* convert linear frequency graph to logarithmic one */
void MountainMidWidget::generateLogGraph(const float *freq, float *graph)
{
    for(int i = 0; i < NUM_BANDS; ++i)
    {
        /* sum up values in freq array between m_logScale[i] and m_logScale[i + 1],
           including fractional parts */
        int a = std::ceil(m_logScale[i]);
        int b = std::floor(m_logScale[i + 1]);
        float sum = 0;

        if(b < a)
        {
            sum += freq[b] *(m_logScale[i + 1] - m_logScale[i]);
        }
        else
        {
            if(a > 0)
            {
                sum += freq[a - 1] *(a - m_logScale[i]);
            }

            for(; a < b; ++a)
            {
                sum += freq[a];
            }

            if(b < 256)
            {
                sum += freq[b] *(m_logScale[i + 1] - b);
            }
        }

        /* fudge factor to make the graph have the same overall height as a
           12-band one no matter how many bands there are */
        sum *=(float) NUM_BANDS / 12;

        /* convert to dB */
        float val = 20 * std::log10(sum);

        /* scale(-DB_RANGE, 0.0) to(0.0, 1.0) */
        val = 1 + val / DB_RANGE;

        graph[i] = qBound(0.0f, val, 1.0f);
    }
}

void MountainMidWidget::drawBars()
{
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, -5.0f);
    glRotatef(38.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(m_angle + 180.0f, 0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for(int i = 0; i < NUM_BANDS; ++i)
    {
        const float z = -1.6f +(NUM_BANDS - i) * BAR_SPACING;
        for(int j = 0; j < NUM_BANDS; ++j)
        {
            drawBar(1.6f - BAR_SPACING * j, z, m_bars[(m_offset + i) % NUM_BANDS][j] * 1.6, m_colors[i][j][0], m_colors[i][j][1], m_colors[i][j][2]);
        }
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}

void MountainMidWidget::drawBar(float x, float z, float h, float r, float g, float b)
{
    drawRectangle(x, 0, z, x + BAR_WIDTH, h, z + BAR_WIDTH, r * (0.2f + 0.8f * h), g * (0.2f + 0.8f * h), b * (0.2f + 0.8f * h));
}

void MountainMidWidget::drawRectangle(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b)
{
    glColor3f(r, g, b);

    glBegin(GL_POLYGON);
    glVertex3f(x1, y2, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    glEnd();

    glColor3f(0.65f * r, 0.65f * g, 0.65f * b);

    glBegin(GL_POLYGON);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y1, z2);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glEnd();

    glColor3f(0.8f * r, 0.8f * g, 0.8f * b);

    glBegin(GL_POLYGON);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1);
    glEnd();
}
