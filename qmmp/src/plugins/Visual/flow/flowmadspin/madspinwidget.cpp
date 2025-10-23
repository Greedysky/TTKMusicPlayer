#include "madspinwidget.h"

#include <math.h>
#include <QFile>
#include <QDateTime>

namespace bitmap
{
#define BITMAP_RGB	0
#define BITMAP_RLE8	1
#define BITMAP_RLE4	2

struct VisImage
{
    int width;
    int	height;
    int depth;
    int bpp;
    int pitch;
    uint8_t* buffer;
};

static int uncompressed(FILE *fp, VisImage *image, int depth)
{
    const int pad = (4 - (image->pitch & 3)) & 3;
    uint8_t *data = image->buffer + (image->height * image->pitch);

    switch(depth)
    {
        case 24:
        case 8:
            while(data > image->buffer)
            {
                data -= image->pitch;

                if(fread(data, image->pitch, 1, fp) != 1)
                {
                    goto err;
                }

                if(pad)
                {
                    fseek(fp, pad, SEEK_CUR);
                }
            }
            break;
        case 4:
            while(data > image->buffer)
            {
                /* Unpack 4 bpp pixels aka 2 pixels per byte */
                uint8_t *col = data - image->pitch;
                uint8_t *end = (uint8_t *)((uintptr_t)data & ~1);
                data = col;

                while(col < end)
                {
                    uint8_t p = fgetc(fp);
                    *col++ = p >> 4;
                    *col++ = p & 0xf;
                }

                if(image->pitch & 1)
                {
                    *col++ = fgetc(fp) >> 4;
                }

                if(pad)
                {
                    fseek(fp, pad, SEEK_CUR);
                }
            }
            break;
        case 1:
            while(data > image->buffer)
            {
                /* Unpack 1 bpp pixels aka 8 pixels per byte */
                uint8_t *col = data - image->pitch;
                uint8_t *end = (uint8_t *)((uintptr_t)data & ~7);
                data = col;

                while(col < end)
                {
                    uint8_t p = fgetc(fp);
                    for(int i = 0; i < 8; ++i)
                    {
                        *col++ = p >> 7;
                        p <<= 1;
                    }
                }

                if(image->pitch & 7)
                {
                    uint8_t p = fgetc(fp);
                    uint8_t count = image->pitch & 7;
                    for(int i = 0; i < count; ++i)
                    {
                        *col++ = p >> 7;
                        p <<= 1;
                    }
                }

                if(pad)
                {
                    fseek(fp, pad, SEEK_CUR);
                }
            }
            break;
    }
    return 0;
err:
    qDebug("Bitmap data is not complete");
    return -1;
}

static int rle(FILE *fp, VisImage *image, int mode)
{
    int c, pad;
    int processing = 1;
    int y = image->height - 1;

    uint8_t *end = image->buffer + (image->height * image->pitch);
    uint8_t *col = end - image->pitch;

    do
    {
        if((c = fgetc(fp)) == EOF)
        {
            goto err;
        }

        if(c)
        {
            if(y < 0)
            {
                goto err;
            }

            /* Encoded mode */
            uint8_t p = fgetc(fp); /* Color */
            if(mode == BITMAP_RLE8)
            {
                while(c-- && col < end)
                {
                    *col++ = p;
                }
            }
            else
            {
                int k = c >> 1; /* Even count */
                while(k-- && col < end - 1)
                {
                    *col++ = p >> 4;
                    *col++ = p & 0xf;
                }

                if(c & 1 && col < end)
                {
                    *col++ = p >> 4;
                }
            }
            continue;
        }

        /* Escape sequence */
        c = fgetc(fp);
        switch(c)
        {
            case EOF:
                goto err;
            case 0: /* End of line */
                y--;
                col = image->buffer + image->pitch * y;

                /* Normally we would error here if y < 0.
                 * However, some encoders apparently emit an
                 * End-Of-Line sequence at the very end of a bitmap.
                 */
                break;
            case 1: /* End of bitmap */
                processing = 0;
                break;
            case 2: /* Delta */
                /* X Delta */
                col += (uint8_t) fgetc(fp);

                /* Y Delta */
                c = (uint8_t) fgetc(fp);
                col -= c * image->pitch;
                y -= c;

                if(col < image->buffer)
                {
                    goto err;
                }
                break;
            default: /* Absolute mode: 3 - 255 */
                if(mode == BITMAP_RLE8)
                {
                    pad = c & 1;
                    while(c-- && col < end)
                    {
                        *col++ = fgetc(fp);
                    }
                }
                else
                {
                    pad = ((c + 1) >> 1) & 1;
                    int k = c >> 1; /* Even count */
                    while(k-- && col < end - 1)
                    {
                        uint8_t p = fgetc(fp);
                        *col++ = p >> 4;
                        *col++ = p & 0xf;
                    }

                    if(c & 1 && col < end)
                    {
                        *col++ = fgetc(fp) >> 4;
                    }
                }

                if(pad)
                {
                    fgetc(fp);
                }
                break;
        }
    }
    while(processing);
    return 0;
err:
    qDebug("Bitmap data is not complete");
    return -1;
}

static int loadData(VisImage *image, const char *name)
{
    /* File read vars */
    FILE * fp = fopen(name, "rb");
    if(fp == NULL)
    {
        return 1;
    }

    /* The win32 BMP header */
    char magic[2];
    /* Read the magic string */
    if(fread(magic, 2, 1, fp) != 1)
    {
        return 2;
    }

    if(strncmp(magic, "BM", 2) != 0)
    {
        fclose(fp);
        return 3;
    }

    /* Skip past the reserved bits */
    fseek(fp, 4 + 4 , SEEK_CUR);

    uint32_t bits = 0;
    /* Read the offset bits */
    if(fread(&bits, 4, 1, fp) != 1)
    {
        return 4;
    }

    fseek(fp, 4, SEEK_CUR);

    /* And read the width, height */
    if(fread(&image->width, 4, 1, fp) != 1)
    {
        return 5;
    }

    if(fread(&image->height, 4, 1, fp) != 1)
    {
        return 6;
    }

    /* Skip over the planet */
    fseek(fp, 2, SEEK_CUR);

    int16_t bitCount = 0;
    /* Read the bits per pixel */
    if(fread(&bitCount, 2, 1, fp) != 1)
    {
        return 7;
    }

    uint32_t compression = 0;
    /* Read the compression flag */
    if(fread(&compression, 4, 1, fp) != 1)
    {
        return 8;
    }

    /* Skip over the nonsense we don't want to know */
    fseek(fp, 8 + 4 + 4 + 4, SEEK_CUR);

    /* Check if we can handle it */
    if(bitCount != 1 && bitCount != 4 && bitCount != 8 && bitCount != 24)
    {
        fclose(fp);
        return 9;
    }

    if(compression > 3)
    {
        fclose(fp);
        return 10;
    }

    image->pitch = image->width * image->bpp;
    image->buffer = (uint8_t*)malloc(image->pitch * image->height * sizeof(uint8_t));

    /* Set to the beginning of image data, note that MickeySoft likes stuff upside down .. */
    fseek(fp, bits, SEEK_SET);

    int32_t error = 0;
    /* Load image data */
    switch(compression)
    {
        case BITMAP_RGB:
            error = uncompressed(fp, image, bitCount);
            break;
        case BITMAP_RLE4:
            error = rle(fp, image, BITMAP_RLE4);
            break;
        case BITMAP_RLE8:
            error = rle(fp, image, BITMAP_RLE8);
            break;
    }

    fclose(fp);
    return error ? -1 : 0;
}

static void freeData(VisImage *image)
{
    if(image->buffer)
    {
        free(image->buffer);
        image->buffer = NULL;
    }
}

static void loadTextrue(VisImage *image, const QString &bmp, GLuint texture)
{
    QString tmp("color.raw");
    QFile::copy(":/data/" + bmp, tmp);
    QFile::setPermissions(tmp, QFile::ReadOwner | QFile::WriteOwner);

    if(bitmap::loadData(image, qPrintable(tmp)) == 0)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->buffer);
        qDebug("Load %s bitmap resource success", qPrintable(bmp));
    }

    bitmap::freeData(image);
    QFile::remove(tmp);
}
}


MadSpinWidget::MadSpinWidget(QWidget *parent)
    : QtOpenGLWidget(parent)
{
    srand(QDateTime::currentMSecsSinceEpoch());
}

void MadSpinWidget::addBuffer(float *left)
{
    for(int i = 0; i < 256; ++i)
    {
        m_buffer[i] = left[i] * 2000.0;
        if(m_buffer[i] > 1.0)
        {
            m_buffer[i] = 1.0;
        }

        if(i > 80)
        {
            m_buffer[i] = m_buffer[i] * (2 + (i / 256.0));
        }
    }

    update();
}

void MadSpinWidget::initializeGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-4.0, 4.0, -4.0, 4.0, -18.0, 18.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glEnable(GL_TEXTURE_2D);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glGenTextures(2, m_texture);

    bitmap::VisImage image;
    image.bpp = 3;
    image.depth = 24;
    image.buffer = NULL;

    bitmap::loadTextrue(&image, "star", m_texture[0]);
    bitmap::loadTextrue(&image, "line", m_texture[1]);
}

void MadSpinWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MadSpinWidget::paintGL()
{
    constexpr int trail = 40;
    constexpr int ampl = 200;
    constexpr int maxlines = 1;

    for(int i = 1; i < 50; ++i)
    {
        m_total += m_buffer[i];
    }

    if(m_total > 2.5)
    {
        m_total = 2.5;
    }

    m_total /= 2.5f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    for(int line = maxlines; line > 0; --line)
    {
        for(int point = 0; point <= m_stars; ++point)
        {
            const float b = 1.5f + point / 33.33333f;
            const float aa = trail - line * 1.0f;
            const float a = (aa + m_frame) / 33.33333f;

            const double csab = cos(a / b);
            const double csapb = cos(a + b);
            const double snab = sin(a / b);
            const double snapb = sin(a + b);

            const double sa = sin(a * .1f);
            const double ca = cos(a * .1f);
            const double s5 = sin(a * .05f);

            double x = sa * snapb * ampl * csab + ca * (csapb * ampl * b * csab + s5 * (ampl * (csab + 2 * snapb)));
            double y = sa * csapb * ampl * csab + ca * (csapb * ampl * b * snab + s5 * (ampl * (csab + 2 * csapb)));
            double z = ca * csapb * ampl * csab + ca * (snapb * ampl * b * snab + s5 * (ampl * (snab + 2 * snapb)));

            x /= 255.0f;
            y /= 255.0f;
            z /= 255.0f;

            glPushMatrix();
            glTranslatef((float)x, (float)y, (float)z);
            glBindTexture(GL_TEXTURE_2D, m_texture[0]);

            const double s1r = ((point * 1.0f) / m_stars);
            const double s1g = (m_stars - point) / (m_stars * 1.0f);
            const double s1b = ((point * 1.0f) / m_stars) * 0.5f;
            const double s1a = ((m_buffer[(int) (point / m_stars * 220)] + (m_total / 200.0f)) / 4.0f);

            const double s2r = sin(m_frame / 400.0f);
            const double s2g = cos(m_frame / 200.0f);
            const double s2b = cos(m_frame / 300.0f);
            const double s2a = (m_buffer[(int)(point / m_stars * 220)] / 2.0f);

            if(s1a > 0.008f)
            {
                glBegin(GL_TRIANGLE_STRIP);
                glColor4f((float)s1r, (float)s1g, (float)s1b, (float)s1a);

                m_texSize = (((m_buffer[(int) (point / m_stars * 220)])) /
                            (2048.01f - (point * 4.0f))) *
                            (((point - m_stars) / (-m_stars)) * 18.0f) + 0.15f;

                /* Top Right */
                glTexCoord2d(1, 1);
                glVertex3f(m_texSize, m_texSize, (float)z);
                /* Top Left */
                glTexCoord2d(0, 1);
                glVertex3f(-m_texSize, m_texSize, (float)z);
                /* Bottom Right */
                glTexCoord2d(1, 0);
                glVertex3f(m_texSize, -m_texSize, (float)z);
                /* Bottom Left */
                glTexCoord2d(0, 0);
                glVertex3f(-m_texSize, -m_texSize, (float)z);
                glEnd();
            }

            glBindTexture(GL_TEXTURE_2D, m_texture[1]);
            glRotatef(m_frame + point, 0.0f, 0.0f, 1.0f);

            if(s2a > 0.005f)
            {
                glBegin(GL_TRIANGLE_STRIP);
                glColor4f((float)s2r, (float)s2g, (float)s2b, (float)s2a);

                m_texSize = (((m_buffer[(int) (point / m_stars * 220)])) /
                            (2048.01f - (point * 4.0f))) *
                            (((point - m_stars) / (-m_stars)) * 18.0f) + 0.35f;

                m_texSize *= ((rand() % 100) / 100.0f) * 2.0f;

                /* Top Right */
                glTexCoord2d(1, 1);
                glVertex3f(m_texSize, m_texSize, (float)z);
                /* Top Left */
                glTexCoord2d(0, 1);
                glVertex3f(-m_texSize, m_texSize, (float)z);
                /* Bottom Right */
                glTexCoord2d(1, 0);
                glVertex3f(m_texSize, -m_texSize, (float)z);
                /* Bottom Left */
                glTexCoord2d(0, 0);
                glVertex3f(-m_texSize, -m_texSize, (float)z);
                glEnd();
            }
            /* Move back to main position */
            glPopMatrix();
        }
    }

    glLoadIdentity();
    m_frame += 0.001 * m_speed;
}
