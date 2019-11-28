/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#ifndef ETHEREALITY_H
#define ETHEREALITY_H

#include <QtOpenGL>

#define MAX_PARTICLES 1000

struct Particle
{
    bool active;
    float life;
    float fade;
    float r, g, b;
    float x, y, z;
    float xi, yi, zi;
    float xg, yg, zg;
};

class Ethereality : public QGLWidget
{
    Q_OBJECT
public:
    explicit Ethereality(QWidget *parent = nullptr);

    void process(GLint input);
    void start();
    void stop();

protected:
    void loadGLTextures();
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int width, int height) override;

protected slots:
    void timeOut();

private:
    QTimer *m_timer;
    GLint m_border;
    GLuint m_col;
    Particle m_particles[MAX_PARTICLES];
    GLuint m_texture[1];
};

#endif


