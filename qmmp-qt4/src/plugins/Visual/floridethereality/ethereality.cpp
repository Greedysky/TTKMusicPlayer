#include "ethereality.h"

#include <ctime>
#include <GL/glu.h>

static GLfloat colors[12][3] = {
    {1.0, 0.5, 0.5}, {1.0, 0.75, 0.5}, {1.0, 1.0, 0.5}, {0.75, 1.0, 0.5},
    {0.5, 1.0, 0.5}, {0.5, 1.0, 0.75}, {0.5, 1.0, 1.0}, {0.5, 0.75, 1.0},
    {0.5, 0.5, 1.0}, {0.75, 0.5, 1.0}, {1.0, 0.5, 1.0}, {1.0, 0.5, 0.75}
};

Ethereality::Ethereality(QWidget *parent)
    : QGLWidget(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(20);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));

    m_col = 0;
    m_border = 60;
}

void Ethereality::process(GLint input)
{
    m_border = input;
}

void Ethereality::start()
{
    m_timer->start();
}

void Ethereality::stop()
{
    m_timer->stop();
}

void Ethereality::loadGLTextures()
{
    QImage tex, buf(":/images/Particle.bmp");
    tex = convertToGLFormat(buf);

    glGenTextures(1, &m_texture[0]);

    glBindTexture(GL_TEXTURE_2D, m_texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
}

void Ethereality::initializeGL()
{
    loadGLTextures();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.5);
    glClearDepth(1.0);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    srand(time(nullptr));
    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        m_particles[i].active = true;
        m_particles[i].life = 1.0;
        m_particles[i].fade = (rand() % 100 ) / 1000.0 + 0.003;
        m_particles[i].r = colors[int(i * (12.0 / MAX_PARTICLES))][0];
        m_particles[i].g = colors[int(i * (12.0 / MAX_PARTICLES))][1];
        m_particles[i].b = colors[int(i * (12.0 / MAX_PARTICLES))][2];
        m_particles[i].xi = (rand() % 50 - 25) * 10.0;
        m_particles[i].yi = (rand() % 50 - 25) * 10.0;
        m_particles[i].zi = (rand() % 50 - 25) * 10.0;
        m_particles[i].xg = 0.0;
        m_particles[i].yg = -0.8;
        m_particles[i].zg = 0.0;
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }
}

void Ethereality::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glScalef(1.0, 1.0, 1.0);
    glRotatef(0.0, 1.0, 0.0, 0.0);
    glRotatef(0.0, 0.0, 1.0, 0.0);
    glRotatef(0.0, 0.0, 0.0, 0.0);

    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        if(m_particles[i].active)
        {
            float x = m_particles[i].x;
            float y = m_particles[i].y;
            float z = m_particles[i].z - 40.0;

            glColor4f(m_particles[i].r, m_particles[i].g, m_particles[i].b, m_particles[i].life);

            glBegin(GL_TRIANGLE_STRIP);
            {
                glTexCoord2d(1, 1); glVertex3f(x + 0.5, y + 0.5, z);
                glTexCoord2d(0, 1); glVertex3f(x - 0.5, y + 0.5, z);
                glTexCoord2d(1, 0); glVertex3f(x + 0.5, y - 0.5, z);
                glTexCoord2d(0, 0); glVertex3f(x - 0.5, y - 0.5, z);
            }
            glEnd();

            m_particles[i].x += m_particles[i].xi / (2.0 * 1000);
            m_particles[i].y += m_particles[i].yi / (2.0 * 1000);
            m_particles[i].z += m_particles[i].zi / (2.0 * 1000);

            m_particles[i].xi += m_particles[i].xg;
            m_particles[i].yi += m_particles[i].yg;
            m_particles[i].zi += m_particles[i].zg;

            m_particles[i].life -= m_particles[i].fade;

            if(m_particles[i].life < 0.0)
            {
                m_particles[i].life = 1.0;
                m_particles[i].fade = (rand() % 100) / 1000.0 + 0.003;
                m_particles[i].x = 0.0;
                m_particles[i].y = 0.0;
                m_particles[i].z = 0.0;
                m_particles[i].xi = rand() % m_border - m_border / 2;
                m_particles[i].yi = rand() % m_border - m_border / 2;
                m_particles[i].zi = 0;
                m_particles[i].r = colors[m_col][0];
                m_particles[i].g = colors[m_col][1];
                m_particles[i].b = colors[m_col][2];
            }
        }
    }
}

void Ethereality::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const bool revert = width >= height;
    gluPerspective(45.0, revert ? ((GLdouble)width / (GLdouble)height) : ((GLdouble)height / (GLdouble)width), 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Ethereality::timeOut()
{
    m_col = (m_col + 1) % 12;
    updateGL();
}
