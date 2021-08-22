#include "musicwidgetutils.h"
#include "musicwidgetheaders.h"
#include "musicimageutils.h"
#include "ttkdesktopwrapper.h"

#include <QBitmap>
#include <QPainter>

#define WIDTH  4
#define HEIGHT 4

void MusicUtils::Widget::setBorderShadow(QWidget *widget, QPainter *painter)
{
    painter->drawPixmap(0, 0, WIDTH, HEIGHT, QPixmap(":/shadow/lb_left_top"));
    painter->drawPixmap(widget->width() - WIDTH, 0, WIDTH, HEIGHT, QPixmap(":/shadow/lb_right_top"));
    painter->drawPixmap(0, widget->height() - HEIGHT, WIDTH, HEIGHT, QPixmap(":/shadow/lb_left_bottom"));
    painter->drawPixmap(widget->width() - WIDTH, widget->height() - HEIGHT, WIDTH, HEIGHT, QPixmap(":/shadow/lb_right_bottom"));

    painter->drawPixmap(0, WIDTH, HEIGHT, widget->height() - 2 * WIDTH, QPixmap(":/shadow/lb_left").scaled(WIDTH, widget->height() - 2 * HEIGHT));
    painter->drawPixmap(widget->width() - WIDTH, WIDTH, HEIGHT, widget->height() - 2 * HEIGHT, QPixmap(":/shadow/lb_right").scaled(WIDTH, widget->height() - 2 * HEIGHT));
    painter->drawPixmap(HEIGHT, 0, widget->width() - 2 * WIDTH, HEIGHT, QPixmap(":/shadow/lb_top").scaled(widget->width() - 2 * WIDTH, HEIGHT));
    painter->drawPixmap(WIDTH, widget->height() - HEIGHT, widget->width() - 2 * WIDTH, HEIGHT, QPixmap(":/shadow/lb_bottom").scaled(widget->width() - 2 * WIDTH, HEIGHT));

}

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

QString MusicUtils::Widget::elidedText(const QFont &font, const QString &text, Qt::TextElideMode mode, int width)
{
    QFontMetrics ft(font);
    return ft.elidedText(text, mode, width);
}

int MusicUtils::Widget::fontTextWidth(const QFont &font, const QString &text)
{
    QFontMetrics ft(font);
#if TTK_QT_VERSION_CHECK(5,11,0)
    return ft.horizontalAdvance(text);
#else
    return ft.width(text);
#endif
}

int MusicUtils::Widget::fontTextHeight(const QFont &font)
{
    QFontMetrics ft(font);
    return ft.height();
}

void MusicUtils::Widget::adjustMenuPosition(QMenu *menu)
{
#if TTK_QT_VERSION_CHECK(5,12,0)
    const QList<QAction*> actions(menu->actions());
    if(!actions.empty())
    {
        QAction* action(actions.first());
        if(action->icon().isNull())
        {
            action->setIcon(QIcon(":/contextMenu/btn_transparent"));
        }
    }
#else
    Q_UNUSED(menu);
#endif
}

void MusicUtils::Widget::positionInCenter(QWidget *widget)
{
    const QRect &rect = TTKDesktopWrapper::screenGeometry();
    widget->move((rect.width() - widget->width()) / 2, (rect.height() - widget->height()) / 2);
}

void MusicUtils::Widget::setTransparent(QWidget *widget, int alpha)
{
    QPalette pal = widget->palette();
    pal.setBrush(QPalette::Base, QBrush(QColor(0xFF, 0xFF, 0xFF, alpha)));
    widget->setPalette(pal);
}

void MusicUtils::Widget::setComboBoxText(QComboBox *object, const QString &text)
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
    w->setMask(MusicUtils::Image::getBitmapMask(w->rect(), ratioX, ratioY));
}
