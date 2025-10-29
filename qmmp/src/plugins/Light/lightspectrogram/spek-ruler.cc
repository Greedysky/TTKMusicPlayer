#include "spek-ruler.h"
#include "spek.h"

#include <cmath>
#include <QFontMetrics>

SpekRuler::SpekRuler(
    int x, int y, Position pos, QString sample_label,
    int *factors, int min_units, int max_units, double spacing,
    double scale, double offset, formatter_cb formatter)
    : m_x(x),
      m_y(y),
      m_pos(pos),
      m_sample_label(sample_label),
      m_factors(factors),
      m_min_units(min_units),
      m_max_units(max_units),
      m_spacing(spacing),
      m_scale(scale),
      m_offset(offset),
      m_formatter(formatter)
{

}

void SpekRuler::draw(QPainter &dc)
{
    const QFontMetrics ftm(dc.font());
    const int w = QtFontWidth(ftm, m_sample_label);
    const int h = ftm.height();
    const int len = m_pos == TOP || m_pos == BOTTOM ? w : h;

    // Select the factor to use, we want some space between the labels.
    int factor = 0;
    for (int i = 0; m_factors[i]; ++i) {
        if (std::fabs(m_scale * m_factors[i]) >= m_spacing * len) {
            factor = m_factors[i];
            break;
        }
    }

    // Draw the ticks.
    draw_tick(dc, m_min_units);
    draw_tick(dc, m_max_units);

    if (factor > 0) {
        for (int tick = m_min_units + factor; tick < m_max_units; tick += factor) {
            if (std::fabs(m_scale * (m_max_units - tick)) < len * 1.2) {
                break;
            }
            draw_tick(dc, tick);
        }
    }
}

void SpekRuler::draw_tick(QPainter &dc, int tick)
{
    constexpr int GAP = 10;
    constexpr int TICK_LEN = 4;

    const QString &label = m_formatter(tick);
    const int value = m_pos == TOP || m_pos == BOTTOM ? tick : m_max_units + m_min_units - tick;
    const double p = m_offset + m_scale * (value - m_min_units);

    const QFontMetrics ftm(dc.font());
    const int w = QtFontWidth(ftm, m_sample_label);
    const int h = -ftm.height() / 2;

    if (m_pos == TOP) {
        dc.drawText(m_x + p - w / 2, m_y - GAP - h, label);
    } else if (m_pos == RIGHT){
        dc.drawText(m_x + GAP, m_y + p - h / 2, label);
    } else if (m_pos == BOTTOM) {
        dc.drawText(m_x + p - w / 2, m_y + GAP*2, label);
    } else if (m_pos == LEFT){
        dc.drawText(m_x - w - GAP, m_y + p - h / 2, label);
    }

    if (m_pos == TOP) {
        dc.drawLine(m_x + p, m_y, m_x + p, m_y - TICK_LEN);
    } else if (m_pos == RIGHT) {
        dc.drawLine(m_x, m_y + p, m_x + TICK_LEN, m_y + p);
    } else if (m_pos == BOTTOM) {
        dc.drawLine(m_x + p, m_y, m_x + p, m_y + TICK_LEN);
    } else if (m_pos == LEFT) {
        dc.drawLine(m_x, m_y + p, m_x - TICK_LEN, m_y + p);
    }
}
