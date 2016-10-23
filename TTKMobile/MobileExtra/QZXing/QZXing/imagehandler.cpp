#include "imagehandler.h"
#include <QGraphicsObject>
#include <QImage>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QQuickItem>
#include <QQuickWindow>

ImageHandler::ImageHandler(QObject *parent) :
    QObject(parent)
{ }

QImage ImageHandler::extractQImage(QObject *imageObj,
                                   const double offsetX, const double offsetY,
                                   const double width, const double height)
{
#if QT_VERSION >= 0x050000
    QQuickItem *item = qobject_cast<QQuickItem*>(imageObj);
#else
    QGraphicsObject *item = qobject_cast<QGraphicsObject*>(imageObj);
#endif

    if (!item) {
        qDebug() << "Item is NULL";
        return QImage();
    }

#if QT_VERSION >= 0x050000
    QQuickWindow *window = item->window();
    QImage img = window->grabWindow();
    img = img.copy(((img.width() > img.height()) ? ((img.width() - img.height()) / 2) : (0)),
                   ((img.height() > img.width()) ? ((img.height() - img.width()) / 2) : (0)),
                   ((img.width() > img.height()) ? (img.height()) : (img.width())),
                   ((img.width() > img.height()) ? (img.height()) : (img.width())));
#else
    QImage img(item->boundingRect().size().toSize(), QImage::Format_RGB32);
    img.fill(QColor(255, 255, 255).rgb());
    QPainter painter(&img);
    QStyleOptionGraphicsItem styleOption;
    item->paint(&painter, &styleOption);
#endif

    if(offsetX == 0 && offsetY == 0 && width == 0 && height == 0)
    {
        return img;
    }
    else
    {
        return img.copy(offsetX, offsetY, width, height);
    }
}

void ImageHandler::save(QObject *imageObj, const QString &path,
                        const double offsetX, const double offsetY,
                        const double width, const double height)
{
    QImage img = extractQImage(imageObj, offsetX, offsetY, width, height);
    img.save(path);
}
