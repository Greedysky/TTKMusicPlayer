#include "qrcodewidget.h"

#include <QPainter>
#include <QPaintEvent>

QRCodeQWidget::QRCodeQWidget(const QByteArray &text, const QSize &size, QWidget *parent)
    : QWidget(parent)
{
    if(text.isEmpty())
    {
        m_text = QByteArray("https://github.com/Greedysky/TTKMusicplayer");
    }
    else
    {
        m_text = text;
    }
    setFixedSize(size);

    m_margin = 10;
    m_foreground = QColor("black");
    m_background = QColor("white");
    m_casesen = true;
    m_mode = QR_MODE_8;
    m_level = QR_ECLEVEL_Q;
    m_percent = 0.23;
}

QString QRCodeQWidget::getClassName()
{
    return staticMetaObject.className();
}

void QRCodeQWidget::setMargin(const int margin)
{
    m_margin = margin;
    repaint();
}

int QRCodeQWidget::getMargin() const
{
    return m_margin;
}

void QRCodeQWidget::setIcon(const QString &path, qreal percent)
{
    setIconPercent(percent);
    m_iconPath = path;
    repaint();
}

QString QRCodeQWidget::getIcon() const
{
    return m_iconPath;
}

void QRCodeQWidget::setIconPercent(qreal percent)
{
    m_percent = percent < 0.5 ? percent : 0.3;
}

qreal QRCodeQWidget::getIconPercent() const
{
    return m_percent;
}

void QRCodeQWidget::setCaseSensitive(bool flag)
{
    m_casesen = flag;
    repaint();
}

bool QRCodeQWidget::caseSensitive() const
{
    return m_casesen;
}

void QRCodeQWidget::setText(const QByteArray &text)
{
    m_text = text;
}

QByteArray QRCodeQWidget::getText() const
{
    return m_text;
}

void QRCodeQWidget::setForegroundColor(const QColor &color)
{
    m_foreground = color;
}

QColor QRCodeQWidget::getForegroundColor() const
{
    return m_foreground;
}

void QRCodeQWidget::setBackgroundColor(const QColor &color)
{
    m_background = color;
}

QColor QRCodeQWidget::getBackgroundColor() const
{
    return m_background;
}

void QRCodeQWidget::setMode(QRencodeMode mode)
{
    m_mode = mode;
}

QRencodeMode QRCodeQWidget::getMode() const
{
    return m_mode;
}

void QRCodeQWidget::setLevel(QRecLevel level)
{
    m_level = level;
}

QRecLevel QRCodeQWidget::getLevel() const
{
    return m_level;
}

void QRCodeQWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent (event);
    QPainter painter(this);

    QRcode *qrcode = QRcode_encodeString(m_text.constData(), 7, m_level, m_mode, m_casesen);
    if(qrcode != nullptr)
    {
        unsigned char *point = qrcode->data;
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_background);
        painter.drawRect(0, 0, width(), height());
        double scale = (width () - 2.0 * m_margin) / qrcode->width;
        painter.setBrush(m_foreground);

        for(int x=0; x<qrcode->width; ++x)
        {
            for(int y =0; y<qrcode->width; ++y)
            {
                if(*point & 1)
                {
                    QRectF r(m_margin + y * scale, m_margin + x * scale, scale, scale);
                    painter.drawRects(&r, 1);
                }
                ++point;
            }
        }
        point = nullptr;
        QRcode_free(qrcode);

        /// draw icon
        painter.setBrush(m_background);
        double icon_width = (width () - 2.0 * m_margin) * m_percent;
        double icon_height = icon_width;
        double wrap_x = (width () - icon_width) / 2.0;
        double wrap_y = (width () - icon_height) / 2.0;
        QRectF wrap(wrap_x - 5, wrap_y - 5, icon_width + 10, icon_height + 10);
        painter.drawRoundRect(wrap, 50, 50);
        QPixmap image(m_iconPath);
        QRectF target(wrap_x, wrap_y, icon_width, icon_height);
        QRectF source(0, 0, image.width (), image.height ());
        painter.drawPixmap(target, image, source);
    }
    qrcode = nullptr;
    event->accept();
}
