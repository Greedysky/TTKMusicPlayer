#include "ttkglobalhelper.h"

#include <QWidget>
#include <QApplication>
#include <QFontDatabase>

#define WIDTH  4
#define HEIGHT 4

void TTK::setApplicationFont(QApplication* app)
{
    QFont font = app->font();
    for(const QString &family : QFontDatabase().families())
    {
        if(family == "微软雅黑" ||
           family == QString::fromUtf8("微软雅黑") ||
           family == QString::fromLocal8Bit("微软雅黑") ||
           family == "Microsoft YaHei")
        {
            font.setFamily(family);
            app->setFont(font);
            TTK_INFO_STREAM("TTK application use 'Microsoft YaHei' font");
            break;
        }
    }
}

void TTK::setBorderShadow(QWidget *widget, QPainter *painter)
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
