#include "musicwidgetutils.h"

#include <QComboBox>
#include <QBitmap>
#include <QPainter>

void MusicUtils::Widget::setLabelFontSize(QWidget *widget, int size)
{
    QFont font = widget->font();
    font.setPointSize(size);
    widget->setFont(font);
}

void MusicUtils::Widget::setLabelFontStyle(QWidget *widget, MusicObject::FontStyleMode type)
{
    QFont font = widget->font();
    switch(type)
    {
        case MusicObject::FT_Bold : font.setBold(true); break;
        case MusicObject::FT_Italic : font.setItalic(true); break;
        case MusicObject::FT_Underline : font.setUnderline(true); break;
        case MusicObject::FT_Overline : font.setOverline(true); break;
        case MusicObject::FT_StrikeOut : font.setStrikeOut(true); break;
        case MusicObject::FT_FixedPitch : font.setFixedPitch(true); break;
        case MusicObject::FT_Kerningt : font.setKerning(true); break;
        default: break;
    }
    widget->setFont(font);
}

QString MusicUtils::Widget::elidedText(const QFont &font, const QString &text,
                                       Qt::TextElideMode mode, int width)
{
    QFontMetrics ft(font);
    return ft.elidedText(text, mode, width);
}

void MusicUtils::Widget::setTransparent(QWidget *widget, int alpha)
{
    QPalette pal = widget->palette();
    pal.setBrush(QPalette::Base, QBrush(QColor(255, 255, 255, alpha)));
    widget->setPalette(pal);
}

void MusicUtils::Widget::setComboboxText(QComboBox *object, const QString &text)
{
    if(object->isEditable())
    {
        object->setEditText(text);
    }
    else
    {
        const int i = object->findText(text);
        if(i > -1)
        {
            object->setCurrentIndex(i);
        }
    }
}

void MusicUtils::Widget::widgetToRound(QWidget *w, int ratioX, int ratioY)
{
    w->setMask( getBitmapMask(w->rect(), ratioX, ratioY) );
}

void MusicUtils::Widget::fusionPixmap(QPixmap &bg, const QPixmap &fg, const QPoint &pt)
{
    QPainter painter(&bg);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(pt.x(), pt.y(), fg);
    painter.end();
}

QPixmap MusicUtils::Widget::pixmapToRound(const QPixmap &src, const QSize &size, int ratioX, int ratioY)
{
    return pixmapToRound(src, QRect(QPoint(0, 0), size), ratioX, ratioY);
}

QPixmap MusicUtils::Widget::pixmapToRound(const QPixmap &src, const QRect &rect, int ratioX, int ratioY)
{
    if(src.isNull())
    {
        return QPixmap();
    }

    QPixmap image = src.scaled(rect.size());
    image.setMask( getBitmapMask(rect, ratioX, ratioY) );
    return image;
}

QPixmap MusicUtils::Widget::pixmapToRound(const QPixmap &src, const QPixmap &mask, const QSize &size)
{
    if(src.isNull())
    {
        return QPixmap();
    }

    QPixmap image(mask);
    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawPixmap(0, 0, src.scaled(size));
    painter.end();

    return image;
}

QBitmap MusicUtils::Widget::getBitmapMask(const QRect &rect, int ratioX, int ratioY)
{
    QBitmap mask(rect.size());
    QPainter painter(&mask);
    painter.fillRect(rect, Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawRoundedRect(rect, ratioX, ratioY);
    return mask;
}

void MusicUtils::Widget::reRenderImage(int delta, const QImage *input, QImage *output)
{
    for(int w=0; w<input->width(); w++)
    {
        for(int h=0; h<input->height(); h++)
        {
            QRgb rgb = input->pixel(w, h);
            uint resultR = colorBurnTransform(qRed(rgb), delta);
            uint resultG = colorBurnTransform(qGreen(rgb), delta);
            uint resultB = colorBurnTransform(qBlue(rgb), delta);
            uint newRgb = ((resultR & 255)<<16 | (resultG & 255)<<8 | (resultB & 255));
            output->setPixel(w, h, newRgb);
        }
    }
}

uint MusicUtils::Widget::colorBurnTransform(int c, int delta)
{
    Q_ASSERT(0 <= delta && delta < 255);

    int result = (c - (uint)(c*delta)/(255 - delta));
    if(result > 255)
    {
        result = 255;
    }else if(result < 0)
    {
        result = 0;
    }
    return result;
}
