#include "musicsongdailywidget.h"
#include "musiccibarequest.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"
#include "musicimageutils.h"

MusicSongDailyWidget::MusicSongDailyWidget(QWidget *parent)
    : QFrame(parent)
{
    setObjectName(MusicSongDailyWidget::metaObject()->className());
    setStyleSheet(QString("#%1{ %2 }").arg(objectName(), TTK::UI::BackgroundStyle10));

    m_container = new QLabel(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_container);
    setLayout(layout);

    m_note = new QLabel(m_container);
    m_content = new QLabel(m_container);

    m_networkRequest = new MusicCiBaRequest(this);
    connect(m_networkRequest, SIGNAL(downLoadRawDataChanged(QByteArray)), this, SLOT(downLoadImageFinished(QByteArray)));

    m_networkRequest->startToRequest();
}

MusicSongDailyWidget::~MusicSongDailyWidget()
{
    delete m_note;
    delete m_content;
    delete m_container;
    delete m_networkRequest;
}

void MusicSongDailyWidget::resizeWidget()
{
    if(m_image.isNull())
    {
        return;
    }

    const double scale = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() * 1.0 / WINDOW_WIDTH_MIN;

    QFont font = m_note->font();
    font.setPixelSize(20.0 * scale);
    const int noteHeight = TTK::Widget::fontTextHeight(font);

    m_note->setFont(font);
    m_note->setGeometry(50, 50, TTK::Widget::fontTextWidth(font, m_note->text()), noteHeight);

    font = m_content->font();
    font.setPixelSize(18.0 * scale);
    const int contentHeight = TTK::Widget::fontTextHeight(font);

    m_content->setFont(font);
    m_content->setGeometry(50, 50 + noteHeight, TTK::Widget::fontTextWidth(font, m_content->text()), contentHeight);

    m_container->setPixmap(QPixmap::fromImage(m_image).scaled(m_container->size(), Qt::KeepAspectRatioByExpanding));
}

void MusicSongDailyWidget::downLoadImageFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    m_image.loadFromData(bytes);
    if(m_image.isNull())
    {
        return;
    }

    QRgb r = 0, g = 0, b = 0;
    for(int w = 0; w < m_image.width(); ++w)
    {
        for(int h = 0; h < m_image.height(); ++h)
        {
            const QRgb rgb = m_image.pixel(w, h);
            r += qRed(rgb);
            g += qGreen(rgb);
            b += qBlue(rgb);
        }
    }

    const int size = m_image.width() * m_image.height();
    r /= size;
    g /= size;
    b /= size;

    const QRgb rgb = TTK::Image::colorContrast(qRgb(r, g, b));
    const QString &style = QString("color:rgb(%1, %2, %3)").arg(qRed(rgb)).arg(qGreen(rgb)).arg(qBlue(rgb));

    m_note->setStyleSheet(style);
    m_content->setStyleSheet(style);

    m_note->setText(m_networkRequest->note());
    m_content->setText(m_networkRequest->content());

    resizeWidget();
}

void MusicSongDailyWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWidget();
}
