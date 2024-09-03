#include "mountainwidget.h"

#include <math.h>

MountainWidget::MountainWidget(QWidget *parent)
    : QtOpenGLWidget(parent)
{
    setMinimumSize(580, 320);

    memset(m_bars, 0, sizeof(m_bars));
}

void MountainWidget::addBuffer(float *left)
{
    generateLogGraph(left, m_bars[m_offset]);
    m_offset =(m_offset + 1) % NUM_BANDS;

    m_angle += m_angleSpeed;
    if(m_angle > 45 || m_angle < -45)
    {
        m_angleSpeed = -m_angleSpeed;
    }

    update();
}

void MountainWidget::initializeGL()
{
    for(int i = 0; i <= NUM_BANDS; ++i)
    {
        m_logScale[i] = powf(256,(float) i / NUM_BANDS) - 0.5f;
    }

    for(int y = 0; y < NUM_BANDS; ++y)
    {
        const float yf = (float)y / (NUM_BANDS - 1);
        for(int x = 0; x < NUM_BANDS; ++x)
        {
            const float xf = (float)x / (NUM_BANDS - 1);
            m_colors[x][y][0] = (1 - xf) *(1 - yf);
            m_colors[x][y][1] = xf;
            m_colors[x][y][2] = yf;
        }
    }
}

void MountainWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MountainWidget::paintGL()
{
    glDisable(GL_BLEND);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glFrustum(-1.1f, 1, -1.5f, 1, 2, 10);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    drawBars();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

/* stolen from the skins plugin */
/* convert linear frequency graph to logarithmic one */
void MountainWidget::generateLogGraph(const float * freq, float * graph)
{
    for(int i = 0; i < NUM_BANDS; ++i)
    {
        /* sum up values in freq array between m_logScale[i] and m_logScale[i + 1],
           including fractional parts */
        int a = ceilf(m_logScale[i]);
        int b = floorf(m_logScale[i + 1]);
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
        float val = 20 * log10f(sum);

        /* scale(-DB_RANGE, 0.0) to(0.0, 1.0) */
        val = 1 + val / DB_RANGE;

        graph[i] = qBound(0.0f, val, 1.0f);
    }
}

void MountainWidget::drawRectangle(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b)
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

void MountainWidget::drawBar(float x, float z, float h, float r, float g, float b)
{
    drawRectangle(x, 0, z, x + BAR_WIDTH, h, z + BAR_WIDTH, r * (0.2f + 0.8f * h), g * (0.2f + 0.8f * h), b * (0.2f + 0.8f * h));
}

void MountainWidget::drawBars()
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
