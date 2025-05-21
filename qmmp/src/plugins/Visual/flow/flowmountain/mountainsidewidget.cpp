#include "mountainsidewidget.h"

#include <math.h>

static constexpr const int ranges[] = {0, 1, 2, 3, 5, 7, 10, 14, 20, 28, 40, 54, 74, 101, 137, 187, 255};

MountainSideWidget::MountainSideWidget(QWidget *parent)
    : MountainWidget(parent)
{

}

void MountainSideWidget::addBuffer(float *left)
{
    for(int i = NUM_BANDS - 1; i > 0; --i)
    {
        for(int j = 0; j < NUM_BANDS; ++j)
        {
            m_bars[i][j] = m_bars[i - 1][j];
        }
    }

    for(int i = 0; i < NUM_BANDS; ++i)
    {
        float value = 0;
        for(int j = ranges[i]; j < ranges[i + 1]; ++j)
        {
            if(left[j] > value)
            {
                value = left[j];
            }
        }

        m_bars[0][i] = value * 10;
    }

    MountainWidget::addBuffer(left);

    m_angleX = fmod(20.0f + 0.0f * m_angle, 360.0f);
    m_angleY = fmod(45.0f + 360.0f / 60.0f * m_angle, 360.0f);
    m_angleZ = fmod(0.0f + 0.0f * m_angle, 360.0f);

    update();
}

void MountainSideWidget::initializeGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1.5, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    for(int i = 0; i < NUM_BANDS; ++i)
    {
        for(int j = 0; j < NUM_BANDS; ++j)
        {
            m_bars[i][j] = 0.0f;
        }
    }
}

void MountainSideWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MountainSideWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    drawBars();
}

void MountainSideWidget::drawBars()
{
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, -5.0f);
    glRotatef(m_angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(m_angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(m_angleZ, 0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLES);

    for(int i = 0; i < NUM_BANDS; ++i)
    {
        const float z = -1.6f + (NUM_BANDS - i) * BAR_SPACING;
        const float b = i * (1.0 / NUM_BANDS);
        const float r = 1.0 - b;

        for(int j = 0; j < NUM_BANDS; ++j)
        {
            drawBar(-1.6 + (j * BAR_SPACING), z, m_bars[i][j] * BAR_SPACING, r - (j * (r / NUM_BANDS)), j * (1.0 / NUM_BANDS), b);
        }
    }

    glEnd();
    glPopMatrix();
}

void MountainSideWidget::drawBar(float x, float z, float h, float r, float g, float b)
{
    constexpr float w = 0.1f;

    glColor3f(r, g, b);
    drawRectangle(x, h, z, x + w, h, z + 0.1);
    drawRectangle(x, 0, z, x + w, 0, z + 0.1);

    glColor3f(0.5f * r, 0.5f * g, 0.5f * b);
    drawRectangle(x, 0.0, z + 0.1, x + w, h, z + 0.1);
    drawRectangle(x, 0.0, z, x + w, h, z );

    glColor3f(0.25f * r, 0.25f * g, 0.25f * b);
    drawRectangle(x, 0.0, z, x, h, z + 0.1);
    drawRectangle(x + w, 0.0, z, x + w, h, z + 0.1);
}

void MountainSideWidget::drawRectangle(float x1, float y1, float z1, float x2, float y2, float z2)
{
    if(y1 == y2)
    {
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y1, z1);
        glVertex3f(x2, y2, z2);

        glVertex3f(x2, y2, z2);
        glVertex3f(x1, y2, z2);
        glVertex3f(x1, y1, z1);
    }
    else
    {
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y1, z2);
        glVertex3f(x2, y2, z2);

        glVertex3f(x2, y2, z2);
        glVertex3f(x1, y2, z1);
        glVertex3f(x1, y1, z1);
    }
}
