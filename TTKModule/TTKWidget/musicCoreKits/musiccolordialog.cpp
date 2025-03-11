#include "musiccolordialog.h"
#include "ui_musiccolordialog.h"

#include <QToolTip>
#include <QButtonGroup>
#include <QPainterPath>

MusicHlPalette::MusicHlPalette(QWidget *parent)
    : QWidget(parent),
      m_dblSaturation(1.0)
{
    setMouseTracking(true);
    setMinimumSize(QSize(360, 120));
}

QColor MusicHlPalette::color() const noexcept
{
    return m_color;
}

void MusicHlPalette::setColor(const QColor &color)
{
    m_color = color;
#if TTK_QT_VERSION_CHECK(6,0,0)
    float vh, vs, vl = -100.0f;
#else
    qreal vh, vs, vl = -100.0f;
#endif
    m_color.getHslF(&vh, &vs, &vl);
    m_dblSaturation = vs;
    m_ptfVernierPercentPos.setX(vh);
    m_ptfVernierPercentPos.setY(1 - vl);
    Q_EMIT colorChanged(m_color);
}

void MusicHlPalette::initialize()
{
    m_ptVernierPos = rect().center();
    calculateColor();
    update();
}

void MusicHlPalette::setSaturation(double dblsaturation)
{
    m_dblSaturation = dblsaturation;
    calculateColor();
    update();
}

void MusicHlPalette::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::NoBrush);

    const int ntRight = rect().right();
    const int ntBottm = rect().bottom();
    QColor colorStart, colorDatum, colorFinal;

    for(int it = 0; it < ntRight + 1; ++it)
    {
        colorStart.setHslF(it / double(ntRight), m_dblSaturation, 1);
        colorDatum.setHslF(it / double(ntRight), m_dblSaturation, 0.5);
        colorFinal.setHslF(it / double(ntRight), m_dblSaturation, 0);

        QLinearGradient linearGradient;
        linearGradient.setStart(QPointF(it, 0));
        linearGradient.setFinalStop(QPointF(it, ntBottm));
        linearGradient.setColorAt(0.0, colorStart);
        linearGradient.setColorAt(0.5, colorDatum);
        linearGradient.setColorAt(1.0, colorFinal);

        painter.setPen(QPen(linearGradient, 1));
        painter.drawLine(QPointF(it, 0), QPointF(it, ntBottm));
    }

    painter.setPen(QPen(Qt::black, 2));
    painter.drawEllipse(m_ptVernierPos, 5, 5);

    painter.setPen(QPen(Qt::white, 2));
    painter.drawEllipse(m_ptVernierPos, 7, 7);
}

void MusicHlPalette::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    m_ptVernierPos.setX(rect().right() * m_ptfVernierPercentPos.rx());
    m_ptVernierPos.setY(rect().bottom() * m_ptfVernierPercentPos.ry());
    update();
}

void MusicHlPalette::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_ptVernierPos = event->pos();
        calculateColor();
        update();
    }
}

void MusicHlPalette::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton && !(event->buttons() & Qt::RightButton))
    {
        m_ptVernierPos = event->pos();
        if(rect().contains(m_ptVernierPos))
        {
            calculateColor();
            update();
        }
    }
    else
    {
        QPainterPath path;
        path.addEllipse(m_ptVernierPos, 7, 7);

        if(path.contains(event->pos()))
        {
            QToolTip::showText(mapToGlobal(event->pos()) + QPoint(0, 5), tr("Adjust hue and brightness"), this, QRect(m_ptVernierPos - QPoint(8, 8), QSize(16, 16)));
        }
    }
}

void MusicHlPalette::calculateColor()
{
    m_ptfVernierPercentPos.setX(m_ptVernierPos.x() / TTKStaticCast(double, rect().right()));
    m_ptfVernierPercentPos.setY(m_ptVernierPos.y() / TTKStaticCast(double, rect().bottom()));
    m_color.setHslF(m_ptfVernierPercentPos.rx(), m_dblSaturation, 1 - m_ptfVernierPercentPos.ry());
    Q_EMIT colorChanged(m_color);
}



MusicHlSaturationPalette::MusicHlSaturationPalette(QWidget *parent)
    : QWidget(parent),
      m_color(Qt::red),
      m_dblVernierX(0),
      m_dblVernierPercentX(0),
      m_dblSaturation(0)
{
    setMouseTracking(true);
    setMinimumWidth(360);
    setMinimumHeight(16);
    setMaximumHeight(24);
}

double MusicHlSaturationPalette::saturation() const noexcept
{
    return m_dblSaturation;
}

void MusicHlSaturationPalette::setSaturation(double dblsaturation)
{
    m_dblSaturation = dblsaturation;
    m_dblVernierPercentX = 1 - m_dblSaturation;
    m_dblVernierX = m_dblVernierPercentX * rect().right();
}

void MusicHlSaturationPalette::setBaseColor(const QColor &color)
{
    m_color = color;
    update();
}

void MusicHlSaturationPalette::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::NoBrush);

    const int ntRight = rect().right();
    const int ntBottm = rect().bottom();

#if TTK_QT_VERSION_CHECK(6,0,0)
    float vh, vs, vl = -100.0f;
#else
    qreal vh, vs, vl = -100.0f;
#endif
    m_color.getHslF(&vh, &vs, &vl);

    QColor colorCenter, colorStart, colorFinal;
    colorCenter.setHslF(vh, 0.5, vl);
    colorStart.setHslF(vh, 1, vl);
    colorFinal.setHslF(vh, 0, vl);

    QLinearGradient linearGradient;
    linearGradient.setStart(QPointF(0, 0));
    linearGradient.setFinalStop(QPointF(ntRight, 0));
    linearGradient.setColorAt(0.0, colorStart);
    linearGradient.setColorAt(1.0, colorFinal);

    painter.fillRect(rect(), linearGradient);

    const QPointF ptfCenter(m_dblVernierX, ntBottm / 2.0);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawEllipse(ptfCenter, 5, 5);

    painter.setPen(QPen(Qt::white, 2));
    painter.drawEllipse(ptfCenter, 7, 7);
}

void MusicHlSaturationPalette::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    m_dblVernierX = rect().right() * m_dblVernierPercentX;
    update();
}

void MusicHlSaturationPalette::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_dblVernierX = event->pos().x();
        calculateSuration();
        update();
    }
}

void MusicHlSaturationPalette::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton && !(event->buttons() & Qt::RightButton))
    {
        m_dblVernierX = event->pos().x();
        if(rect().contains(event->pos()))
        {
            calculateSuration();
            update();
        }
    }
    else
    {
        QPainterPath path;
        path.addEllipse(QPointF(m_dblVernierX, rect().bottom() / 2.0), 7, 7);

        if(path.contains(event->pos()))
        {
            QToolTip::showText(mapToGlobal(event->pos()) + QPoint(0, 5), tr("Adjust hue and brightness"), this, QRect(event->pos() - QPoint(8, 8), QSize(16, 16)));
        }
    }
}

void MusicHlSaturationPalette::calculateSuration()
{
    m_dblVernierPercentX = m_dblVernierX / rect().right();
    m_dblSaturation = 1 - m_dblVernierPercentX;
    m_color.setHslF(m_color.hslHueF(), m_dblSaturation, m_color.lightnessF());
    Q_EMIT saturationChanged(m_dblSaturation);
}



MusicColorDialog::MusicColorDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicColorDialog)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->confirmButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->cancelButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->wgtPalette, SIGNAL(colorChanged(QColor)), m_ui->wgtSaturationIndicator, SLOT(setBaseColor(QColor)));
    connect(m_ui->wgtPalette, SIGNAL(colorChanged(QColor)), SLOT(colorChanged(QColor)));
    connect(m_ui->wgtSaturationIndicator, SIGNAL(saturationChanged(double)), m_ui->wgtPalette, SLOT(setSaturation(double)));

    m_ui->wgtPalette->initialize();

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_ui->topTitleCloseButton, 0);
    buttonGroup->addButton(m_ui->confirmButton, 1);
    buttonGroup->addButton(m_ui->cancelButton, 2);
    QtButtonGroupConnect(buttonGroup, this, buttonClicked, TTK_SLOT);
}

MusicColorDialog::MusicColorDialog(const QColor &color, QWidget *parent)
    : MusicColorDialog(parent)
{
    if(color.isValid())
    {
        setColor(color);
    }
}

MusicColorDialog::~MusicColorDialog()
{
    delete m_ui;
}

QColor MusicColorDialog::popup(QWidget *parent, const QColor &color)
{
    MusicColorDialog dialog(color, parent);
    return dialog.exec() ? dialog.color() : QColor();
}

QColor MusicColorDialog::color() const noexcept
{
    return m_color;
}

void MusicColorDialog::setColor(const QColor &color)
{
    m_color = color;
    m_ui->wgtPalette->setColor(color);
    m_ui->wgtSaturationIndicator->setSaturation(color.hslSaturationF());
}

void MusicColorDialog::buttonClicked(int index)
{
    switch(index)
    {
        case 0:
        case 2: reject(); break;
        case 1: accept(); break;
        default: break;
    }
}

void MusicColorDialog::colorChanged(const QColor &color)
{
    m_color = color;
}
