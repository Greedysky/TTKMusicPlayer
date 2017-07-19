#ifndef SPEKRULER_H
#define SPEKRULER_H

#include <QObject>
#include <QPainter>

class SpekRuler
{
public:
    enum Position
    {
        TOP,
        RIGHT,
        BOTTOM,
        LEFT
    };

    typedef QString (*formatter_cb)(int unit);

    SpekRuler(
        int x, int y, Position pos, QString sample_label,
        int *factors, int min_units, int max_units, double spacing,
        double scale, double offset, formatter_cb formatter
    );

    void draw(QPainter &dc);

protected:
    void draw_tick(QPainter &dc, int tick);

    int x;
    int y;
    Position pos;
    QString sample_label;
    int *factors;
    int min_units;
    int max_units;
    double spacing;
    double scale;
    double offset;
    formatter_cb formatter;
};

#endif
